/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  Copyright (c) 2025 by Systream Pvt Ltd. All rights reserved.
 *
 *                This software is copyright protected and proprietary to
 * Systream Pvt Ltd. All other rights remain with Systream Pvt Ltd.
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  sdp_server.hpp
 *        \brief  The class representing the SDP Implementation
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "iso15118_2/sdp_server/sdp_server.hpp"

#include <iostream>
#include <iso15118/detail/io/socket_helper.hpp>
#include <iso15118/io/connection_plain.hpp>

namespace evse {
namespace iso15118_2 {
namespace sdp_server {
using namespace std;

SdpServer::SdpServer(string_view ifname) : interface_name(ifname) {
  const auto result_interface_check = iso15118::io::check_and_update_interface(interface_name);
  if (result_interface_check) {
    cout << "Using ethernet interface: " << interface_name.c_str() << endl;
  }
}

void SdpServer::StartSdpServer() {
  sdp_server = std::make_unique<iso15118::io::SdpServer>(interface_name);
  poll_manager.register_fd(sdp_server->get_fd(), [this]() { HandlesSdpServerRequest(); });
}

void SdpServer::HandlesSdpServerRequest() {
  cout << "SDP server handler called " << endl;
  auto request = sdp_server->get_peer_request();

  if (not request) {
    return;
  }

  auto connection = std::make_unique<iso15118::io::ConnectionPlain>(poll_manager, interface_name);

  const auto ipv6_endpoint = connection->get_public_endpoint();
  cout << "Connected endpoint %d" << ipv6_endpoint.port << endl;

  /// TODO: instantiate the session that handle incoming session request

  sdp_server->send_response(request, ipv6_endpoint);
}

}  // namespace sdp_server
}  // namespace iso15118_2
}  // namespace evse
