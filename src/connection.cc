#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "connection.h"

Connection::Connection(int _fd) {
  m_fd = _fd;
}

Connection::~Connection() {
  close(m_fd);
}

void Connection::process() {
  char buffer[1024];

  ssize_t bytes_read = recv(m_fd, buffer, 1024, 0);
  buffer[bytes_read] = '\0';

  // std::cout << "Read: " << buffer << std::endl;

  usleep(20000);

  std::stringstream response;

  response << "HTTP/1.0 200 OK\r\n";
  response << "Content-Length: 13\r\n";
  response << "Content-Type: text/plain\r\n";
  response << "\r\n";
  response << "Hello World\r\n";
  
  send(m_fd, response.str().c_str(), response.str().length(), 0);
  
}
