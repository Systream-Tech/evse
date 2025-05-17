/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  @mainpage EVSE
 *  Copyright (c) 2025 by Systream Pvt Ltd. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Systream Pvt Ltd.
 *                All other rights remain with Systream Pvt Ltd.
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  main.cpp
 *        \brief  Entry point for Evse App
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdlib>

#include "evse.hpp"

/// @brief Main entry point of Evse app
/// @return Zero on Successful
auto main() -> int {
  /* Create the evse object */
  evse::Evse evse;

  /// Set the name for EVSE main thread
  pthread_setname_np(pthread_self(), "EvseMainThread");

  /// TODO: Implement reactor for handling IO requests

  /**
   * @brief Run evse application until externally/internally shutdown
   */
  evse.Run();

  /* Terminate process */
  evse.Shutdown();

  return EXIT_SUCCESS;
}
