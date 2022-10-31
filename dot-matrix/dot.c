#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

const int SIZE = 5;
const int THREADS = 3;

typedef struct matrix
{
  int rows;
  int cols;
  int **data;
} matrix;

typedef struct process
{
  int offset;
  int limit;
  matrix *matrix1;
  matrix *matrix2;
  matrix *matrix3;
} process;

void print(matrix *m)
{
  int i, j;
  printf("--------------\n");

  for (i = 0; i < m->rows; i++)
  {
    for (j = 0; j < m->cols; j++)
    {
      printf("  %d  ", m->data[i][j]);
    }
    printf("\n");
  }
  printf("--------------\n");
  printf("\n");
}

void fill(matrix *m)
{
  int i, j;

  m->rows = SIZE;
  m->cols = SIZE;
  m->data = malloc(sizeof(int *) * m->rows);

  for (i = 0; i < m->rows; i++)
  {
    m->data[i] = malloc(sizeof(int) * m->cols);

    for (j = 0; j < m->cols; j++)
    {
      m->data[i][j] = 1;
    }
  }
}

void multiply(matrix **m1, matrix **m2, matrix **m3, int offset, int limit)
{
  int i, j, k;
  for (i = offset; i < offset + limit; i++)
  {
    for (j = 0; j < (*m1)->cols; j++)
    {
      (*m3)->data[i][j] = 0;
      for (k = 0; k < (*m2)->rows; k++)
      {
        (*m3)->data[i][j] += (*m1)->data[i][k] * (*m2)->data[k][j];
      }
    }
  }
}

void *thread_main(void *arg)
{
  process p = *((process *)arg);

  multiply(&p.matrix1, &p.matrix2, &p.matrix3, p.offset, p.limit);

  pthread_exit(&arg);
}

int main()
{
  matrix a;
  matrix b;
  matrix m;
  process p[THREADS];
  pthread_t th[THREADS];
  int i, j, limit, offset;

  limit = ceil( (float)SIZE / THREADS );

  fill(&a);
  fill(&b);
  fill(&m);

  // print(&a);
  // print(&b);

  for (i = 0; i < THREADS; i++)
  {
    offset = i * limit;
    if (offset + limit > SIZE){
      limit = SIZE - offset;
    }

    printf("Offset: %d\n", offset);
    printf("Limit: %d\n", limit);
    printf("\n");

    p[i].offset = offset; 
    p[i].limit = limit;
    p[i].matrix1 = &a;
    p[i].matrix2 = &b;
    p[i].matrix3 = &m;

    pthread_create(&th[i], NULL, thread_main, &p[i]);
  }

  for (i = 0; i < THREADS; i++)
  {
    pthread_join(th[i], NULL);
  }

  print(&m);
  return 0;
}
