#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <sys/time.h>

#define NBUCKET 5
#define NENTRY 1000000
#define NKEYS 100000

struct entry {
  int key;
  int value;
  int inuse;
};

struct entry table[NBUCKET][NENTRY];
int keys[NKEYS];

int nthread = 1;
volatile int done;

pthread_mutex_t bucket_locks[NBUCKET];

double now() {
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

static void print(void) {
  int b, i;
  for (b = 0; b < NBUCKET; b++) {
    printf("%d: ", b);
    for (i = 0; i < NENTRY; i++) {
      if (table[b][i].inuse)
        printf("(%d, %d)", table[b][i].key, table[b][i].value);
    }
    printf("\n");
  }
}

static void put(int key, int value) {
  int b = key % NBUCKET;
  int i;

  assert(pthread_mutex_lock(&bucket_locks[b]) == 0);
  for (i = 0; i < NENTRY; i++) {
    if (!table[b][i].inuse) {
      table[b][i].key = key;
      table[b][i].value = value;
      table[b][i].inuse = 1;
      assert(pthread_mutex_unlock(&bucket_locks[b]) == 0);
      return;
    }
  }
  assert(pthread_mutex_unlock(&bucket_locks[b]) == 0);
  assert(0);
}

static int get(int key) {
  int b = key % NBUCKET;
  int i;
  int v = -1;
  for (i = 0; i < NENTRY; i++) {
    if (table[b][i].inuse && table[b][i].key == key) {
      v = table[b][i].value;
      break;
    }
  }
  return v;
}

static void *put_thread(void *xa) {
  long n = (long) xa;
  int i;
  int b = NKEYS / nthread;

  for (i = 0; i < b; i++) {
    put(keys[b * n + i], n);
  }
  return NULL;
}

static void *get_thread(void *xa) {
  long n = (long) xa;
  int i;
  int k = 0;
  int b = NKEYS / nthread;

  for (i = 0; i < b; i++) {
    int v = get(keys[b * n + i]);
    if (v == -1) k++;
  }
  printf("%ld: %d keys missing\n", n, k);
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t *tha;
  void *value;
  long i;
  int b;
  double t1, t0;

  if (argc < 2) {
    fprintf(stderr, "%s: %s nthread\n", argv[0], argv[0]);
    exit(-1);
  }
  nthread = atoi(argv[1]);

  for (b = 0; b < NBUCKET; b++) {
    assert(pthread_mutex_init(&bucket_locks[b], NULL) == 0);
  }

  tha = malloc(sizeof(pthread_t) * nthread);
  assert(tha != NULL);

  srandom(0);
  assert(NKEYS % nthread == 0);
  for (i = 0; i < NKEYS; i++) {
    keys[i] = random();
    assert(keys[i] > 0);
  }

  t0 = now();
  for (i = 0; i < nthread; i++) {
    assert(pthread_create(&tha[i], NULL, put_thread, (void *) i) == 0);
  }
  for (i = 0; i < nthread; i++) {
    assert(pthread_join(tha[i], &value) == 0);
  }
  t1 = now();

  printf("completion time for put phase = %f\n", t1 - t0);

  t0 = now();
  for (i = 0; i < nthread; i++) {
    assert(pthread_create(&tha[i], NULL, get_thread, (void *) i) == 0);
  }
  for (i = 0; i < nthread; i++) {
    assert(pthread_join(tha[i], &value) == 0);
  }
  t1 = now();

  printf("completion time for get phase = %f\n", t1 - t0);

  free(tha);
  for (b = 0; b < NBUCKET; b++) {
    assert(pthread_mutex_destroy(&bucket_locks[b]) == 0);
  }
  return 0;
}
