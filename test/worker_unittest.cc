#include <gtest/gtest.h>
#include <semaphore.h>

#include "connection_queue.h"
#include "worker.h"

int process_call_count;

Connection::Connection(int fd) {};
Connection::~Connection() {};
void Connection::process() { ++process_call_count; };

TEST(WorkerTest, ProcessConnection) {
  connection_queue_t connection_queue;

  pthread_mutex_t queue_request_mutex;
  pthread_mutex_init(&queue_request_mutex, NULL);

  sem_t queue_semaphore;
  sem_init(&queue_semaphore, 0, 0);

  process_call_count = 0;

  // Create a worker thread
  Worker worker1(&connection_queue, &queue_request_mutex, &queue_semaphore);
  worker1.run();

  Connection *c = new Connection(0);
  connection_queue.push_back(c);

  ASSERT_EQ(process_call_count, 0);

  sem_post(&queue_semaphore);

  ASSERT_EQ(process_call_count, 1);
}
