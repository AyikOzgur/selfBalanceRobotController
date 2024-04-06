#include "UdpChannel.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // For close()

bool UdpChannel::init(std::string myIp, int myPort, std::string remoteIp, int remotePort) 
{
    m_remoteIp = remoteIp;
    m_remotePort = remotePort;

    // Create socket
    if ((m_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        std::cerr << "Socket creation failed" << std::endl;
        return false;
    }

    // Bind the socket to myIp and myPort
    struct sockaddr_in myaddr;
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(myPort);
    myaddr.sin_addr.s_addr = inet_addr(myIp.c_str());

    if (bind(m_sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) 
    {
        std::cerr << "Bind failed" << std::endl;
        return false;
    }

    return true;
}

bool UdpChannel::sendData(uint8_t *data, int sizeOfData) 
{
    struct sockaddr_in remaddr;
    remaddr.sin_family = AF_INET;
    remaddr.sin_port = htons(m_remotePort);
    remaddr.sin_addr.s_addr = inet_addr(m_remoteIp.c_str());

    ssize_t sentBytes = sendto(m_sockfd, data, sizeOfData, 0, (struct sockaddr *)&remaddr, sizeof(remaddr));
    return sentBytes == sizeOfData;
}

int UdpChannel::receiveData(uint8_t *data, int sizeOfBuffer) 
{
    struct sockaddr_in remaddr; // Remote address
    socklen_t addr_len = sizeof(remaddr);
    int recvBytes = recvfrom(m_sockfd, data, sizeOfBuffer, 0, (struct sockaddr *)&remaddr, &addr_len);

    return recvBytes;
}
