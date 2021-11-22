#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define N 100

struct Point
{
  int x;
  int y;
  int z;
};

typedef struct Point Point;

int main(int argc, char ** argv)
{
  int rank, size, i, j;
  clock_t startTime, endTime;
  double elapsedTime;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  long localSum[3] = {0L,0L,0L};
  long totalSum[3] = {0L,0L,0L};
  int workToDo = N/size;
  Point * arr;
  int start = workToDo*rank;
  int end = start+workToDo;
  srand(time(0));
  
  startTime = clock();
  arr = (Point *) malloc(workToDo*sizeof(Point));
  for( i = start, j= 0; i < end; i++,j++)
  {
    arr[j].x = i+1;
    arr[j].y = i+2;
    arr[j].z = i+3;
  }

  for( i = 0; i < workToDo; i++)
  {
    localSum[0] += arr[i].x;
    localSum[1] += arr[i].y;
    localSum[2] += arr[i].z;
  }
  
  MPI_Reduce(&localSum, &totalSum, 3, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
  endTime = clock();
  elapsedTime = ((double)endTime - startTime) / CLOCKS_PER_SEC;
  printf("Waktu yang dibutuhkan: %d : %f detik\n", rank, elapsedTime);
  

  free((void *) arr);
  if(rank == 0)
    printf("The sum is x = %ld, y = %ld, z = %ld\n", totalSum[0], totalSum[1], totalSum[2]);

  MPI_Finalize();
  return 0;
}
