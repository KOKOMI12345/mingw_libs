
#ifndef SOCKETEXCEPTION_H
#define SOCKETEXCEPTION_H

#include <exception>
#include <string>

class SocketException : public std::exception {
    // base class for all socket exceptions
private:
    std::string message;
public:
    SocketException(const std::string& msg) : message(msg) {}
    virtual const char* what() const throw() {
        return message.c_str();
    }
};

class StartupException : public SocketException {
    // raise when the wsl_init function fails
public:
    StartupException(const std::string& msg) : SocketException(msg) {}
};

class ConnectionException : public SocketException {
    // raise when the socket connection fails
public:
    ConnectionException(const std::string& msg) : SocketException(msg) {}
};

class SendException : public SocketException {
    // raise when the socket send fails
public:
    SendException(const std::string& msg) : SocketException(msg) {}
};

class ReceiveException : public SocketException {
    // raise when the socket receive fails
public:
    ReceiveException(const std::string& msg) : SocketException(msg) {}
};

#endif // SOCKETEXCEPTION_H