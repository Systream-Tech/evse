// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 - 2022 Pionix GmbH and Contributors to EVerest
#ifndef SLAC_IO_HPP
#define SLAC_IO_HPP

#include <condition_variable>
#include <functional>
#include <mutex>
#include <slac/channel.hpp>
#include <string>
#include <thread>

class SlacIO {
 public:
  using InputHandlerFnType = void(slac::messages::HomeplugMessage&);
  explicit SlacIO(const std::string& if_name);

  void release_input();
  void run(std::function<InputHandlerFnType> callback);
  void send(slac::messages::HomeplugMessage& msg);
  void quit();

 private:
  void loop();
  slac::Channel slac_channel;
  slac::messages::HomeplugMessage incoming_msg;
  std::function<InputHandlerFnType> input_handler;
  std::thread loop_thread;

  bool running{false};
};

#endif  // SLAC_IO_HPP
