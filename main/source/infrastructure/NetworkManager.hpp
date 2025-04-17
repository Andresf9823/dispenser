

#ifndef _NETWORK_MANAGER_HPP_
#define _NETWORK_MANAGER_HPP_

#include <domain/ports/INetworkRepository.h>

class NetworkManager : public INetworkRepository
{
public:
    NetworkManager(&INetworkRepository networkRepo);
    ~NetworkManager();
};





#endif
