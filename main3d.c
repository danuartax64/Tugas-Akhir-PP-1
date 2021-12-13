#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000

struct Point
{
  int x;
  int y;
  int z;
};

typedef struct Point Point;                 #memberikan nama lain/alias kepada Point yang sudah dibuat sebelumnya

int main(int argc, char ** argv)            #deklarasi bahwa bagian ini sampai MPI_Finalize() menggunakan MPI
{
  int rank, size, i, j;
  clock_t startTime, endTime;
  double waktulewat;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);     #jumlah proses yang digunakan dari executor/server

  long jumlahlokal[3] = {0L,0L,0L};            #long integer value dengan semua nilai dijadikan 0 pada awal
  long jumlahtotal[3] = {0L,0L,0L};            #long integer value dengan semua nilai dijadikan 0 pada awal
  int tugas = N/size;
  Point * arr;
  int start = tugas*rank;
  int end = start+tugas;
  
  startTime = clock();
  arr = (Point *) malloc(tugas*sizeof(Point));
  for( i = start, j= 0; i < end; i++,j++)
  {
    arr[j].x = i+1;
    arr[j].y = i+2;
    arr[j].z = i+3;
  }

  for( i = 0; i < tugas; i++)
  {
    jumlahlokal[0] += arr[i].x;
    jumlahlokal[1] += arr[i].y;
    jumlahlokal[2] += arr[i].z;
  }
  
  MPI_Reduce(&jumlahlokal, &jumlahtotal, 3, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
  endTime = clock();
  waktulewat = ((double)endTime - startTime) / CLOCKS_PER_SEC;                       #karena ingin menjadi satuan detik maka dibagi CLOCKS_PER_SEC
  printf("Waktu yang dibutuhkan: %d : %f detik\n", rank, waktulewat);
  

  free((void *) arr);
  if(rank == 0)
    printf("The sum is x = %ld, y = %ld, z = %ld\n", jumlahtotal[0], jumlahtotal[1], jumlahtotal[2]);

  MPI_Finalize();
  return 0;
}
