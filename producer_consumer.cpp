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

    semaphore(int init){
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

semaphore empt_cnt(10), fill_cnt(0);

mutex M_BUFFER;
queue<int> buffer_queue;

void *produce(void *id) 
{
  while (1) 
  {
    empt_cnt.P();
    M_BUFFER.lock();

    srand(time(NULL));
    int addData = rand()%100;
    cout << "Produced: " << addData << endl;
    usleep(300000);
    buffer_queue.push(addData);
    sleep(1);

    M_BUFFER.unlock();
    fill_cnt.V();
  }
}

void *consume(void *id) 
{
  while (1) 
  {
    fill_cnt.P();
    M_BUFFER.lock();

    cout << "Consumed: " << buffer_queue.front() << " Buffer Size: " << buffer_queue.size() << " " << endl;
    usleep(200000);
    buffer_queue.pop();
    sleep(1);

    M_BUFFER.unlock();    
    empt_cnt.V();
  }
}

int32_t main()
{
  srand(time(NULL));
  int producer_cnt=5;
  int consumer_cnt=3;
  pthread_t producer[producer_cnt];
  pthread_t consumer[consumer_cnt];
  for (int i = 0; i < producer_cnt; ++i)
  {
    /* code */
    pthread_create(&producer[i], NULL, produce, NULL);
  }
  for (int i = 0; i < consumer_cnt; ++i)
  {
    /* code */
    pthread_create(&consumer[i], NULL, consume, NULL);
  }

  pthread_exit(NULL);
}