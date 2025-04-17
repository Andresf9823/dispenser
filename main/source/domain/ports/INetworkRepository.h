#ifndef INETWORKREPOSITORY_H
#define INETWORKREPOSITORY_H

#include <IpNetwork.h>

// This is an interface that defines the contract for network repository implementations
// It inherits from IRepository<ipNetwork> and provides methods for saving and retrieving
// network configuration data
class INetworkRepository : public ipNetwork
{
public:
    // Virtual destructor to allow proper cleanup of derived classes
    virtual ~INetworkRepository() = default;

    // Saves network configuration to persistent storage
    virtual void saveIpNetwork(ipNetwork *network) = 0;

    // Retrieves network configuration from persistent storage
    virtual ipNetwork *getIpNetwork() = 0;
};

#endif
    
