# ИДЗ4,  Демьяненко Виктор Николевич БПИ217 

## Вариант 27.
## Задание:
Задача о Винни-Пухе - 3 или мстительные пчелы. Неправильные пчелы, подсчитав в конце месяца убытки от наличия в лесу Винни-Пуха, решили
разыскать его и наказать в назидание всем другим любителям сладкого. Для
поисков медведя они поделили лес на участки, каждый из которых прочесывает одна стая неправильных пчел. В случае нахождения медведя на своем
участке стая проводит показательное наказание и возвращается в улей. Если
участок прочесан, а Винни-Пух на нем не обнаружен, стая также возвращается в улей. Там она получает информацию об еще неисследованных участках
и снова улетает. Требуется создать многопоточное приложение, моделирующее действия пчел. При решении использовать парадигму портфеля
задач.

## Входные данные:
Входные данные: n m

n - строка в которой прячется Винни Пух
m - столбец в котором прячется Винни Пух

## Модель параллельных вычислений:
В этой задаче я буду использовать парадигму портфеля задач, то есть у нас будет двумерный массив. Задачи - это строки массива. Я буду раздавать эти задачи потоком, чтобы какой то из потоков нашёл медведя.

## Отчет:
- 4 балла:

1) Приведено условие задачи.
2) Описана модель параллельных вычислений, используемая при разработке многопоточной программы.
3) Описаны входные данные программы, включающие вариативные
диапазоны, возможные при многократных запусках.
4) Реализовано консольное приложение, решающее поставленную задачу с использованием одного варианта синхропримитивов
5) Ввод данных в приложение реализован с консоли.
6) Результаты работы приведены в отчете.

- 5 баллов:
1) В программу добавлены комментарии, поясняющие выполняемые
действия и описание используемых переменных.
2) В отчете должен быть приведен сценарий, описывающий одновре-
менное поведение представленных в условии задания сущностей в
терминах предметной области. То есть, описано поведение объектов
разрабатываемой программы как взаимодействующих субъектов, а
не то, как это будет реализовано в программе.

- 6 - 8 баллов
1) Был реализован метод, который запускается при создании потока
2) Были реализованы ввод из строки (рандом) и чтение из файла.
По умолчанию запускается ввод с консоли.

- 9 баллов

На 9 баллов расписано в самом низу.

## ВАЖНО!
Прошу обратить внимание на код:
```Cpp
for (int j = 0; j < 5000000; ++j) {
          line++;
          line--;
    }
```
Предположим, что пчелы нашли Винни-Пуха, однако гонялись за ним некое кол-во времени. Так что я делаю бесполезные вычисления в цикле, чтобы замедлить программу.
## Код на C++
Комментировать начал по привычке с ассмеблера, так что первые комментарии идут после строчек.

```Cpp
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
    pthread_mutex_lock(&myMutex); // lock main thread
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
    pthread_mutex_unlock(&myMutex); // unlock thread
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

int main(int argc, char *argv[])
{
  // position of bear
  int nBear, mBear;
  std::cout << "Enter N and M (0 <= (N and M) <= 19):" << '\n';
  std::cin >> nBear >> mBear;
  while (!isCorrect(nBear, mBear)) {
    std::cout << "Enter N and M (0 <= (N and M) <= 19):" << '\n';
    std::cin >> nBear >> mBear;
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
}
```

По итогу, мы создаем потоки, которые идут по нашему лесу и запускаем каждый. Дальше у каждого потока своя строка по которой он ищет медведя. 

## Выходные данные:
```
Enter N and M (0 <= (N and M) <= 19):
15 3

Thread 0 was started
Thread 1 was started
Thread 2 was started
Thread 3 was started
Thread 4 was started
Thread 5 was started
Thread 6 was started
Thread 7 was started
Thread 8 was started
Thread 9 was started
Thread 10 was started
Thread 11 was started
Thread 12 was started
Thread 13 was started
Thread 14 was started
Thread 15 was started
Thread num 16 found that! 
Thread 16 was started
Thread 17 was started
Thread 18 was started
Thread 19 was started
Thread 0 has finished execution!
Thread 1 has finished execution!
Thread 2 has finished execution!
Thread 3 has finished execution!
Thread 4 has finished execution!
Thread 5 has finished execution!
Thread 6 has finished execution!
Thread 7 has finished execution!
Thread 8 has finished execution!
Thread 9 has finished execution!
Thread 10 has finished execution!
Thread 11 has finished execution!
Thread 12 has finished execution!
Thread 13 has finished execution!
Thread 14 has finished execution!
Bees found bear in row 15 and column 3
Thread 15 has finished execution!
Thread 16 has finished execution!
Thread 17 has finished execution!
Thread 18 has finished execution!
Thread 19 has finished execution!
```

Как мы видим, поток 16 нашёл медведя, однако догнал его после того, как поток 14 завершился. Это показывает то, что все потоки работают параллельно. Мне не очень понравилась именно эта задача, потому что тут лишь в одном случае находится медведь, а для других потоков информация одна и та же, так что они достаточно быстро выполняют свою работу.

## Дополненный код C++

```Cpp
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
    pthread_mutex_lock(&myMutex); // lock main thread
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
    pthread_mutex_unlock(&myMutex); // unlock thread
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
```

Тут добавлена возможность чтения из файла и рандомная генерация с заданныи сидом.


## На оценку 9

Из кода убрал синхопримитивы и можно заметить, что потоки попытались записать информацию одновременно:

```
Enter N and M (0 <= (N and M) <= 19):
0 0

Thread Thread num 0 was started
0 found that!
Thread 1 was started
Thread 2 was started
Thread 3 was started
Thread 4 was started
Thread 5 was started
Thread 6 was started
Thread 7 was started
Thread 8 was started
Thread 9 was started
Thread 10 was started
Thread 11 was started
Thread 12 was started
Thread 13 was started
Thread 14 was started
Thread 15 was started
Thread 16 was started
Thread 17 was started
Thread 18 was started
Thread 19 was started
Bees found bear in row 0 and column 0
Thread 0 has finished execution!
Thread 1 has finished execution!
Thread 2 has finished execution!
Thread 3 has finished execution!
Thread 4 has finished execution!
Thread 5 has finished execution!
Thread 6 has finished execution!
Thread 7 has finished execution!
Thread 8 has finished execution!
Thread 9 has finished execution!
Thread 10 has finished execution!
Thread 11 has finished execution!
Thread 12 has finished execution!
Thread 13 has finished execution!
Thread 14 has finished execution!
Thread 15 has finished execution!
Thread 16 has finished execution!
Thread 17 has finished execution!
Thread 18 has finished execution!
Thread 19 has finished execution!
```

(Да, тут одновременно потоки постарались вписать информацию.)

Это произошло из-за того, что два потока попытались одновременно вписать информацию.