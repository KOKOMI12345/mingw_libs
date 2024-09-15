
/*
windows的g++编译器没有socket库，所以需要自己写socket程序。
Copyright (c) 2024 Fuxuan. All rights reserved.
*/
#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "socketError.h"

#pragma comment(lib, "ws2_32.lib") // 链接Winsock库

class Socket {
public:
    Socket() : m_sock(INVALID_SOCKET) {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (result != 0) {
            throw StartupException("Failed to start Winsock");
        }
    }

    ~Socket() {
        if (m_sock != INVALID_SOCKET) {
            closesocket(m_sock);
        }
        WSACleanup();
    }

    bool connect(const std::string& host, int port) {
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = inet_addr(host.c_str());

        int result = ::connect(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
        if (result == SOCKET_ERROR) {
            throw ConnectionException("Failed to connect to server, Error code: " + std::to_string(WSAGetLastError()));
            return false;
        }
        return true;
    }

    bool send(const std::string& message) {
        int result = ::send(m_sock, message.c_str(), message.size(), 0);
        if (result == SOCKET_ERROR) {
            throw SendException("Failed to send message, Error code: " + std::to_string(WSAGetLastError()));
            return false;
        }
        return true;
    }

    std::string receive(int bufferSize) {
        char buffer[bufferSize];
        int result = ::recv(m_sock, buffer, sizeof(buffer), 0);
        if (result > 0) {
            return std::string(buffer, result);
        } else if (result == 0) {
            std::cout << "Connection closed" << std::endl;
        } else {
            throw ReceiveException("Failed to receive message, Error code: " + std::to_string(WSAGetLastError()));
        }
        return "";
    }

private:
    SOCKET m_sock;
};

#endif // SOCKET_H