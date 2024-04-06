#pragma once
#include <string>

class UdpChannel
{
public:

    bool init(std::string myIp, int myPort, std::string remoteIp, int remotePort);

    bool sendData(uint8_t *data, int sizeOfData);

    int receiveData(uint8_t *data, int sizeOfBuffer);

private:

    // Private methods and variables.
    int m_sockfd; // Socket file descriptor
    std::string m_remoteIp;
    int m_remotePort;
};