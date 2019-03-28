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

semaphore Tobacco_and_paper(0);
semaphore Paper_and_match(0);
semaphore Match_and_Tobacco(0);
semaphore smoke(1);

void *agent(void *lul)
{
  while (1) {
    srand(time(NULL));
    smoke.P();
    int r = rand() % 3;
    switch( r ) {
      case 0: Tobacco_and_paper.V();
        break;
      case 1: Paper_and_match.V();
        break;
      case 2: Match_and_Tobacco.V();
        break;
    }
  }
}

void *Match(void *x)
{
  while(1) {
    Tobacco_and_paper.P();
    cout << "Person with matches is smoking!" << endl;
    sleep(1);
    smoke.V();
  }
}

void *Tobacco(void *x)
{
  while(1) {
    Paper_and_match.P();
    cout << "Person with tobacco is smoking!" << endl;
    sleep(1);
    smoke.V();
  }
}

void *Paper(void *x)
{
  while(1) {
    Match_and_Tobacco.P();
    cout << "Person with paper is smoking!" << endl;
    sleep(1);
    smoke.V();
  }
}


int main()
{
  pthread_t idx[4];
  pthread_create(&idx[0], NULL, Match, NULL);
  pthread_create(&idx[1], NULL, Tobacco, NULL);
  pthread_create(&idx[2], NULL, Paper, NULL);
  pthread_create(&idx[3], NULL, agent, NULL);

  for (int i = 0; i < 4; ++i)
  {
    /* code */
    pthread_join(idx[i],NULL);
  }
}
