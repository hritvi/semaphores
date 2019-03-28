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

vector<semaphore> PHILOSOPHER_DINE(5);
mutex MUTEX;

int LIST[5] = {0, 1, 2, 3, 4}; 

void* eat(void* num) 
{ 

  int* ind = (int *)num; 
  while (1) {
    srand(time(NULL));
    MUTEX.lock();
    PHILOSOPHER_DINE[*ind].P();
    PHILOSOPHER_DINE[(*ind + 1) % 5].P();
    MUTEX.unlock();

    cout << "Philosopher " << *ind << " eating." << endl ;
    sleep(2);
    PHILOSOPHER_DINE[*ind].V();
    PHILOSOPHER_DINE[(*ind + 1) % 5].V();
    usleep((rand() % 100) * 100);
  } 
}

int main() 
{
  pthread_t thread_list[3]; 
  for (int i = 1; i < 6; i++) { 
    pthread_create(&thread_list[i], NULL, eat, &LIST[i]); 
  }
  for (int i = 1; i < 6; i++) {
    pthread_join(thread_list[i], NULL);
  }  
}