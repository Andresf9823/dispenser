#ifndef _TCP_CLIENT_HPP_
#define _TCP_CLIENT_HPP_

#include "../ProtocolRepository.hpp"

class TcpClient : public IpClientRepository
{
private:
    static constexpr const string tag = "TCP CLIENT";

public:
    TcpClient() = default;
    void connectToServer(IpClientConfiguration &config) override;
    ~TcpClient() = default;
};

#endif