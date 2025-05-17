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
/**        \file  evse.hpp
 *        \brief  Main class representing the Evse App
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef EVSE_HPP
#define EVSE_HPP

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <signal.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>
#include <vector>

#include "iso15118_2/sdp_server/sdp_server.hpp"

namespace evse {

constexpr uint8_t kNumOfThreads = 1; /**< Number of Threads used in Evse. */

/// TODO: Pass a configurable flag from compiler
#define USE_ETH

// ToDo: The configuration through JSON if required in future.
/// Ethernet Like SPI Interafce
///  @note The STM32MP157x use this SPI interface to communicate with CG5317 HPGP Chip
#ifdef USE_SPI
#define IF_NAME "seth0"
#endif
/// Ethernet Interface
/// @note This Ethernet interface of STM32MP157x can connect communicate with CG5317 HPGP Chip
/// over Ethernet Switch
#ifdef USE_ETH
#define IF_NAME "end0"
#endif

/**
 * \brief Main class representing the EVSE
 */
class Evse final {
 public:
  Evse();
  ~Evse();

  Evse(const Evse&) = delete;
  Evse(const Evse&&) = delete;
  Evse operator=(const Evse&) = delete;
  Evse operator=(const Evse&&) = delete;

  /**
   * \brief Lifecycle Method for Initialization.
   */
  void Initialize();

  /**
   * \brief Lifecycle Methods for Run Mode.
   */
  void Run();

  /**
   * \brief Lifecycle Methods for Shutdown.
   */
  void Shutdown();

 private:
  /**
   * \brief Entry point of the thread receiving signals from the CLI or OS
   */
  void SignalHandlerThread();

  /**
   * \brief Flag to identify whether the application was requested to terminate
   */
  std::atomic_bool exit_requested_{false};

  /**
   * \brief Vector to store all threads spawned by EVSE app.
   */
  std::vector<std::thread> threads_;

  /// Declare the object for SDP server
  evse::iso15118_2::sdp_server::SdpServer sdp_server;
};

}  // namespace evse

#endif  /// EVSE_HPP
