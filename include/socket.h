#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class Socket{
 Protected:
  explicit Socket(int fd) : m_sockfd(fd){};
 public:
  Socket();
  virtual ~Socket();
}

class ServerSocket : public Socket{
 public:
  void Bind(int port);
  void Listen();
  int Accept();
};

#endif
