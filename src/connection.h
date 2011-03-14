#ifndef __CONNECTION_H__
#define __CONNECTION_H__

class Connection {
 public:
  Connection(int _fd);
  ~Connection();

  void process();

 private:
  int m_fd;
};

#endif
