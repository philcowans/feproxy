#ifndef __WORKER_H__
#define __WORKER_H__

#include <deque>
#include <pthread.h>
#include <semaphore.h>

#include "connection_queue.h"

class Worker {
 public:
                      Worker(connection_queue_t *_connection_queue, pthread_mutex_t *_mutex, sem_t *_semaphore);
  void                run();

 private:
  connection_queue_t *m_connection_queue;
  pthread_mutex_t    *m_mutex;
  sem_t              *m_semaphore;
  pthread_t           m_thread;

  static void        *start_thread(void *obj);
  void                execute();
};

#endif
