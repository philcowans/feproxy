#include <deque>
#include <netinet/in.h>
#include <poll.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "connection.h"
#include "connection_queue.h"
#include "worker.h"

int main(int argc, char **argv) {
  connection_queue_t connection_queue;

  pthread_mutex_t queue_request_mutex;
  pthread_mutex_init(&queue_request_mutex, NULL);

  sem_t queue_semaphore;
  sem_init(&queue_semaphore, 0, 0);

  // Create a worker thread
  Worker worker1(&connection_queue, &queue_request_mutex, &queue_semaphore);
  worker1.run();

  Worker worker2(&connection_queue, &queue_request_mutex, &queue_semaphore);
  worker2.run();

  Worker worker3(&connection_queue, &queue_request_mutex, &queue_semaphore);
  worker3.run();

  Worker worker4(&connection_queue, &queue_request_mutex, &queue_semaphore);
  worker4.run();

  // Set up the primary socket
  int primary_socket = socket(PF_INET, SOCK_STREAM, 0);
  
  in_addr bind_address = {
    INADDR_ANY
  };

  sockaddr_in socket_address = {
    AF_INET,
    htons(8081),
    bind_address 
  };
 
  if(bind(primary_socket, (sockaddr*)&socket_address, sizeof(socket_address)) < 0)
    return 1;

  listen(primary_socket, 1);

  pollfd file_descriptor = {
    primary_socket,
    POLLIN,
    0
  };

  while(true) {
    if(poll(&file_descriptor, 1, -1) < 0)
      return 1;
    
    int connection_fd = accept(primary_socket, NULL, NULL);
    
    pthread_mutex_lock(&queue_request_mutex);
    connection_queue.push_back(new Connection(connection_fd));
    pthread_mutex_unlock(&queue_request_mutex);
    
    sem_post(&queue_semaphore);
  }

  // Shutdown - close all open connections and the primary socket

  for(connection_queue_t::iterator it = connection_queue.begin(); it != connection_queue.end(); ++it)
    delete *it;

  close(primary_socket);
  pthread_mutex_destroy(&queue_request_mutex);

  return 0;
}
