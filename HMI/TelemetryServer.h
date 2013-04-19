//==============================================================================
// Project  : UGV1
// Module   : hmi
// File     : TelemetryServer.h
// Brief    : Server for HMI command messages
//==============================================================================

#ifndef UGV1_TELEMETRYSERVER_H
#define UGV1_TELEMETRYSERVER_H

#include "hmi_common.h"

namespace Ugv1
{

/// design - follow track
/// In Grape
/// - IpClient
/// - TcpSocket
/// - UdpSocket
/// - TcpClient
/// - UdpClient
/// - TcpServer
/// - UdpServer
/// In UGV1
/// - TelemetryServer
/// - TcpTelemetryServer
/// - UdpTelemetryServer
class UGV1HMI_DLL_API TelemetryServer
{
public:
    TelemetryServer();
};

} // Ugv1

#endif // UGV1_TELEMETRYSERVER_H
