#ifndef IP_NETWORK_H
#define IP_NETWORK_H

class IpNetwork
{
private:
    struct ipv4 {
        unsigned char address[4];
        unsigned char subnet[4];
        unsigned char gateway[4];
        unsigned char dns[4];
    } ipv4;
    unsigned char mac[6];
public:
    ipNetwork(unsigned char *address, unsigned char *subnet, unsigned char *gateway, unsigned char *dns, unsigned char *mac) {
        memcpy(ipv4.address, address, 4);
        memcpy(ipv4.subnet, subnet, 4); 
        memcpy(ipv4.gateway, gateway, 4);
        memcpy(ipv4.dns, dns, 4);
        memcpy(this->mac, mac, 6);
    }

    ~ipNetwork() = default;
};

#endif

