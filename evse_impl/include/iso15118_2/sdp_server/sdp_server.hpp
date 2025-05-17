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
/**        \file  sdp_server.hpp
 *        \brief  The class representing the SDP Declaration
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef ISO15118_2_SDP_SERVER_HPP
#define ISO15118_2_SDP_SERVER_HPP

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <iso15118/io/poll_manager.hpp>
#include <iso15118/io/sdp_server.hpp>
#include <memory>
#include <string_view>

namespace evse {
namespace iso15118_2 {
namespace sdp_server {
using namespace std;

/**
 * \brief Implementation class for DID F190 skeleton.
 */
class SdpServer final {
 public:
  /**
   * \brief A parameterized Constructor.
   * @param takes ethernet interface name
   */
  explicit SdpServer(string_view ifname);

  /**
   * \brief Destructor.
   */
  virtual ~SdpServer() = default;

  SdpServer(const SdpServer&) = delete;

  SdpServer& operator=(const SdpServer&) = delete;

  void StartSdpServer();

 private:
  iso15118::io::PollManager poll_manager;
  std::unique_ptr<iso15118::io::SdpServer> sdp_server;

  // callbacks for sdp server
  void HandlesSdpServerRequest();

  std::string interface_name;
};
}  // namespace sdp_server
}  // namespace iso15118_2
}  // namespace evse

#endif  // ISO15118_2_SDP_SERVER_HPP
