#include <assert.h>
#include <iostream>

#include "connection.h"
#include "worker.h"

Worker::Worker(connection_queue_t *_connection_queue, pthread_mutex_t *_mutex, sem_t *_semaphore) {
  m_connection_queue = _connection_queue;
  m_mutex            = _mutex;
  m_semaphore        = _semaphore;
}

void Worker::run() {
  pthread_create(&m_thread, 0, &Worker::start_thread, this);
}

void *Worker::start_thread(void *obj) {
  static_cast<Worker *>(obj)->execute();
  return NULL;
}

void Worker::execute() {
  while(true) {
    Connection *connection = NULL;

    //std::cout << "Waiting for semaphore ... " << std::flush;
    sem_wait(m_semaphore);
    //std::cout << "received." << std::endl;
      
    pthread_mutex_lock(m_mutex);
    assert(m_connection_queue->size() > 0);
    connection = m_connection_queue->back();
    m_connection_queue->pop_back();
    pthread_mutex_unlock(m_mutex);
    
    //std::cout << "Picked up connection" << std::endl;

    connection->process();
    
    //std::cout << "Processing completed" << std::endl;
    
    delete connection;
  }
}
