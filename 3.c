#include <stdio.h>
#include <mpi.h>

int main (int argc, char* argv[])
{
  int errCode;

  if ((errCode = MPI_Init(&argc, &argv)) != 0)
  {
    return errCode;
  }

  int arrLength = 6;
  int arr[arrLength];
  int myRank;
  int count;

  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &count);

  if ( count < 2) {
    printf("add more process");
    MPI_Finalize();
    return 0;
  }

  if (myRank == 1) {
    for (int i = 0; i < arrLength; i++) {
      arr[i] = i;
    }
    for (int i = 0; i < count; i++) {
      if (i == myRank) {
        continue;
      }
      MPI_Send(arr, arrLength, MPI_INT, i, 123, MPI_COMM_WORLD);
    }
  } else {
    int bufLength;
    MPI_Status stat;
    MPI_Probe(1, 123, MPI_COMM_WORLD, &stat);
    MPI_Get_count(&stat, MPI_INT, &bufLength);
    int buf[bufLength];

    MPI_Recv(buf, bufLength, MPI_INT, 1, 123, MPI_COMM_WORLD, &stat);

    for (int i = 0; i < bufLength; i++) {
      printf("rank %d, index %d, value %d\n", myRank, i, buf[i]);
    }

  }

  MPI_Finalize();
  return 0;
}
