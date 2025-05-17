/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  Copyright (c) 2025 by Systream Pvt Ltd. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Systream Pvt Ltd.
 *                All other rights remain with Systream Pvt Ltd.
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  evse.cpp
 *        \brief  Main class representing the Evse App
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "evse.hpp"

#include <signal.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <csignal>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "slac_process.cpp"

namespace evse {
using namespace std;

Evse::Evse() : sdp_server(IF_NAME) { Initialize(); }

Evse::~Evse() { Shutdown(); }

void Evse::SignalHandlerThread() {
  sigset_t signal_set;
  sigemptyset(&signal_set);          /* empty the set of signals */
  ::sigaddset(&signal_set, SIGTERM); /* add SIGTERM to set */
  ::sigaddset(&signal_set, SIGINT);  /* add SIGINT to set */

  cout << "Runtime::" << __func__ << ": signal handler thread running" << endl;

  int signal = -1;
  // run loop until the signals SIGTERM or SIGINT are received.
  while (not exit_requested_) {
    sigwait(&signal_set, &signal);
    switch (signal) {
      case SIGINT:
        cout << "Evse::" << __func__ << ": SIGINT received";
        exit_requested_ = true;
        break;
      case SIGTERM:
        cout << "Evse::" << __func__ << ": SIGTERM received";
        exit_requested_ = true;
        break;
      default: /* ignore other signals*/
        break;
    }
  }

  cout << "Runtime::" << __func__ << ": signal handler thread stopped";
}

void Evse::Initialize() {
  cout << "Evse::" << __func__ << " called" << endl;

  /* Block all signals for this thread. Signal mask will be inherited by subsequent threads. */
  sigset_t signals;
  sigfillset(&signals);
  pthread_sigmask(SIG_SETMASK, &signals, NULL);

  /* reserve space in std::vector */
  threads_.reserve(kNumOfThreads);
  /* spawn a new signal handler thread*/
  threads_.emplace_back(&Evse::SignalHandlerThread, this);
}

void Evse::Run() {
  cout << "Evse::" << __func__ << " called" << endl;

  std::thread slac_thread([] { SlacProcess(IF_NAME); });

  /// Start SDP server
  sdp_server.StartSdpServer();

  /* store the current point of time */
  std::chrono::steady_clock::time_point next_run(std::chrono::steady_clock::now());

  /* define a time interval of 200 milliseconds */
  std::chrono::steady_clock::duration duration(std::chrono::milliseconds(200));
  while (!exit_requested_) {
    /* calculate the next point of time to be rescheduled */
    next_run += duration;
    /* Blocks the execution of the current thread until specified point of time */
    std::this_thread::sleep_until(next_run);
  }

  slac_thread.join();
  cout << "Evse::" << __func__ << ": execution stopped (exit requested)" << endl;
}

void Evse::Shutdown() {
  cout << "Evse::" << __func__ << " called" << endl;

  exit_requested_ = true;

  /* do application clean-up in here */
  /* wait till other threads have joined */
  for (std::thread& thread : threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}
}  // namespace evse
