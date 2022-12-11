#include <fstream>
#include <iostream>
#include <pthread.h>

int line = 0; // row in our forest
pthread_mutex_t myMutex; // Main thread

int threadNum = 0; // Num of thread
const int n = 20; // rows in forest
const int m = 20; // columns in forest
int forest[n][m]; // Forest
static bool found = false; // Bool that shows if we find a bear

void *mutexRun(void *param) // Finding a bear in current row
{
  if (!found) { // if bear wasn't found:
    for (int i = 0; i < m; ++i) {    // try to find a bear in row
      if (forest[line][i] == 1) {    // if bear was found, then make found = true;
        found = true;
        std::cout << "Thread num " << threadNum << " found that!\n";
        for (int j = 0; j < 5000000; ++j) {
          line++;
          line--;
        }
        std::cout << "Bees found bear in row " << line << " and column " << i << '\n';
      }
    }
    line++;                         // go to next row if bear was not found!
  }
  threadNum++;                       // set num of current thread
}

bool isCorrect(int n, int m) {      // check input data is correct
  if (n < 0 || n > 19) {
    return false;
  }
  if (m < 0 || m > 19) {
    return false;
  }
  return true;
}

int main(int argc, char** argv)
{
  int nBear, mBear;
  // Random
  if (argc == 2) {
    char* arg = argv[1];
    int seed = atoi(arg);
    srand(seed);
    nBear = rand() % 20;
    mBear = rand() % 20;
  } else if (argc == 3) { // From file
    std::ifstream ifstream(argv[1]);
    ifstream >> nBear >> mBear;
  } else {
    // position of bear
    std::cout << "Enter N and M (0 <= (N and M) <= 19):" << '\n';
    std::cin >> nBear >> mBear;
    while (!isCorrect(nBear, mBear)) {
      std::cout << "Enter N and M (0 <= (N and M) <= 19):" << '\n';
      std::cin >> nBear >> mBear;
    }
  }
  forest[nBear][mBear] = 1;
  // Thread of bees
  pthread_t bees[20];
  // Init of main thread
  pthread_mutex_init(&myMutex,0);
  for (int i = 0; i < n; ++i) {
    // Create threads
    pthread_create(&bees[i], 0, mutexRun, (void *) threadNum);
    std::cout << "Thread " << i << " was started\n";
  }
  for (int i = 0; i < n; ++i) {
    // Join threads
    pthread_join(bees[i], 0);
    std::cout << "Thread " << i << " has finished execution!\n";
  }
  // Destroy main thread
  pthread_mutex_destroy(&myMutex);
  return 0;
}