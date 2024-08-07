#include "UdpChannel.h"
#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // For close()
#endif

bool UdpChannel::init(std::string myIp, int myPort, std::string remoteIp, int remotePort)
{
    m_remoteIp = remoteIp;
    m_remotePort = remotePort;

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed" << std::endl;
        return false;
    }
#endif

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

#ifdef _WIN32
    InetPton(AF_INET, myIp.c_str(), &myaddr.sin_addr);
#else
    inet_pton(AF_INET, myIp.c_str(), &myaddr.sin_addr);
#endif

    if (bind(m_sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
#ifdef _WIN32
        closesocket(m_sockfd);
        WSACleanup();
#else
        close(m_sockfd);
#endif
        return false;
    }

    return true;
}

bool UdpChannel::sendData(uint8_t *data, int sizeOfData)
{
    struct sockaddr_in remaddr;
    remaddr.sin_family = AF_INET;
    remaddr.sin_port = htons(m_remotePort);

#ifdef _WIN32
    InetPton(AF_INET, m_remoteIp.c_str(), &remaddr.sin_addr);
#else
    inet_pton(AF_INET, m_remoteIp.c_str(), &remaddr.sin_addr);
#endif

    int sentBytes = sendto(m_sockfd, reinterpret_cast<char *>(data), sizeOfData, 0, (struct sockaddr *)&remaddr, sizeof(remaddr));
    return sentBytes == sizeOfData;
}

int UdpChannel::receiveData(uint8_t *data, int sizeOfBuffer)
{
    struct sockaddr_in remaddr; // Remote address
    socklen_t addr_len = sizeof(remaddr);
    int recvBytes = recvfrom(m_sockfd, reinterpret_cast<char *>(data), sizeOfBuffer, 0, (struct sockaddr *)&remaddr, &addr_len);

    return recvBytes;
}

#ifdef _WIN32
UdpChannel::~UdpChannel()
{
    closesocket(m_sockfd);
    WSACleanup();
}
#else
UdpChannel::~UdpChannel()
{
    close(m_sockfd);
}
#endif
