#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>

class Socket{
 protected:
  int m_sockfd;
 public:
  explicit Socket(int fd) : m_sockfd(fd){};
  Socket();
  virtual ~Socket();
  int get_fd() const { return m_sockfd; }
};

class ServerSocket : public Socket{
 public:
  void Bind(int port);
  void Listen();
  std::unique_ptr<Socket> Accept();
};

#endif
