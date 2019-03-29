#include <bits/stdc++.h>
#include <pthread.h>
#include <queue>
#include <atomic>
#include <unistd.h>

using namespace std;

class semaphore {

  private:
    atomic<int> cnt;
    mutex mut;


  public:

    semaphore(int init=1){
      cnt=init;
    }
    void V() {
      cnt++;
    }
    void P() 
    {
      while (1) {
        while (cnt <= 0);
        mut.lock();
        if (cnt <= 0) {
          mut.unlock();
          continue;
        }
        cnt--;
        mut.unlock();
        break;
      } 
    }
};

mutex MUTEX;
semaphore writer_sem(1);
queue<int> buffer_queue;
int ind = 0;
atomic<int> read_count{0};

void *Write(void *B)
{
  while (1) 
  {
    writer_sem.P();
    int it = ind++;
    printf("Buffer write: %d\n", it);
    fflush(stdout);
    usleep(35000);
    buffer_queue.push(it);
    sleep(1);
    writer_sem.V();
  }
}

int ReadBuffer() {
  int it = buffer_queue.front();
  printf("Buffer read: %d\n",it);
  fflush(stdout);
  usleep(20000);
  buffer_queue.pop();
  return it;
}

void *Read(void *B) 
{
  while (1) 
  {
    MUTEX.lock();
    read_count++;
    if (read_count == 1){
      writer_sem.P();
    }
    MUTEX.unlock();
    ReadBuffer();
    sleep(1);
    MUTEX.lock();
    read_count--; 
    if (read_count == 0){
      writer_sem.V();
    }
    MUTEX.unlock();
  }
}

int32_t main()
{
  srand(time(NULL));

  pthread_t Reader;
  pthread_t Writer;
  
  pthread_create(&Writer, NULL, Write, NULL);
  pthread_create(&Reader, NULL, Read, NULL);

  pthread_exit(NULL);
}
