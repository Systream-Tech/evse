// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 - 2022 Pionix GmbH and Contributors to EVerest
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <csignal>
#include <fsm/specialization/sync/posix.hpp>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "iso15118_3/slac/evse_fsm.hpp"
#include "iso15118_3/slac/slac_io.hpp"

namespace evse {
using namespace std;

const uint8_t sample_nmk[] = {0x34, 0x52, 0x23, 0x54, 0x45, 0xae, 0xf2, 0xd4,
                              0x55, 0xfe, 0xff, 0x31, 0xa3, 0xb3, 0x03, 0xad};

void logging_callback(const std::string& msg) { cout << "FSM_CTRL: {}\n" << msg << endl; }

class FSMDriver {
 public:
  explicit FSMDriver(const std::string& if_name) : slac_io_handle(if_name), evse_fsm(slac_io_handle) {
    fsm_ctrl.reset(evse_fsm.sd_reset);
    running = true;
    loop_thread = std::thread(&FSMDriver::loop, this);
    slac_io_handle.run([this](slac::messages::HomeplugMessage& msg) { feed_slac_message(msg); });
  };

  void enter_bcd() {
    std::lock_guard<std::mutex> lck(drive_mutex);
    fsm_ctrl.submit_event(EventEnterBCD());
    notify();
  };

  std::future<bool> matched_future() { return matched.get_future(); }

  ~FSMDriver() {
    slac_io_handle.quit();

    running = false;
    notify();

    loop_thread.join();
  }

 private:
  void notify() {
    new_event = true;
    cv_new_event.notify_one();
  }

  void feed_slac_message(slac::messages::HomeplugMessage& msg) {
    std::lock_guard<std::mutex> lck(drive_mutex);

    if (fsm_ctrl.submit_event(EventSlacMessage(msg))) {
      notify();
    } else {
      cout << "No SLAC message handler for current state: {}\n" << fsm_ctrl.current_state()->id.name << endl;
    }
  }

  void loop() {
    std::unique_lock<std::mutex> feed_lck(drive_mutex);
    int next_timeout_in_ms;

    while (running) {
      do {
        next_timeout_in_ms = fsm_ctrl.feed();
      } while (next_timeout_in_ms == 0);

      // stipulate link establishment if appropriate
      if (fsm_ctrl.current_state()->id.id == State::WaitForSlacMatch) {
        if (evse_fsm.received_slac_match()) {
          fsm_ctrl.submit_event(EventLinkDetected());
          continue;
        }
      }

      if (fsm_ctrl.current_state()->id.id == State::Matched) {
        matched.set_value(true);
        return;
      }

      if (next_timeout_in_ms < 0) {
        cv_new_event.wait(feed_lck, [this] { return new_event; });
      } else {
        cv_new_event.wait_for(feed_lck, std::chrono::milliseconds(next_timeout_in_ms), [this] { return new_event; });
      }

      if (new_event) {
        new_event = false;
      }
    }
  }

  SlacIO slac_io_handle;

  std::promise<bool> matched;
  std::thread loop_thread;

  std::mutex drive_mutex;
  std::condition_variable cv_new_event;

  bool new_event{false};

  bool running;

  EvseFSM evse_fsm;
  fsm::sync::PosixController<EvseFSM::StateHandleType, logging_callback> fsm_ctrl;
};

void SlacProcess(std::string_view interface_name) {
  /// TODO: Implement Future & Promise to sync thread with Main Thread
  const std::string if_name{interface_name};

  // FIXME (aw): cleanup construction/destruction
  try {
    FSMDriver driver{if_name};
    std::this_thread::sleep_for(std::chrono::seconds(1));

    driver.enter_bcd();

    if (driver.matched_future().get()) {
      cout << "Made it to matching, exiting!\n" << endl;
    } else {
      cout << "Failed somewhere ...\n" << endl;
    }
  } catch (const std::runtime_error& err) {
    cerr << "I/O error: {}\n" << err.what() << endl;
  };
}
}  // namespace evse
