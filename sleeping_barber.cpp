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

semaphore Active_Barber(0), Waiting(1), ActiveCustomer(0);
int TotalSeats = 10;

void *Barber(void *b) {
  while(1) {
    ActiveCustomer.P();
    Waiting.P();
    cout << "Barber cutting hairs. Empty seats: " << TotalSeats << endl;
    sleep(1);
    TotalSeats += 1;
    Active_Barber.V();
    Waiting.V();
  }
}

void *Customer(void *b) {
  while(1) {
    Waiting.P();
    if (TotalSeats > 0) {
      cout << "Customer entered. Empty seats: " << TotalSeats << endl;
      TotalSeats--;
      ActiveCustomer.V();
      Waiting.V();
      Active_Barber.P();
    }
    else {
      cout<<"All seats full in Waiting room"<<endl;
      Waiting.V();
    }
  }
}

int main() 
{ 
  int Total_seats=10;
  pthread_t idx[Total_seats];
  pthread_t barber;
  for (int i = 0; i < Total_seats; ++i)
  {
    /* code */
    pthread_create(&idx[i], NULL, Customer, NULL);
  }
  pthread_create(&barber, NULL, Barber, NULL);

  for(int i=0; i<Total_seats; i++){
    pthread_join(idx[i], NULL);
  }

  pthread_join(barber, NULL);
}