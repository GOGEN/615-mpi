#include <stdio.h>
#include <mpi.h>

int main (int argc, char* argv[])
{
  int errCode;

  if ((errCode = MPI_Init(&argc, &argv)) != 0)
  {
    return errCode;
  }

  int myRank;
  int count;
  MPI_Status stat;
  int aMatrix[8][8];
  int bMatrix[4][8];
  int cMatrix[4][8];

  MPI_Datatype rowType;
  MPI_Datatype matrixType;
  MPI_Datatype fullMatrixType;

  MPI_Type_vector(1, 8, 8, MPI_INT, &rowType);
  MPI_Type_commit(&rowType);

  MPI_Type_vector(4, 1, 2, rowType, &matrixType);
  MPI_Type_commit(&matrixType);

  MPI_Type_vector(4, 1, 1, rowType, &fullMatrixType);
  MPI_Type_commit(&fullMatrixType);

  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &count);

  if ( count < 2) {
    printf("add more process");
    MPI_Finalize();
    return 0;
  }

  if (myRank == 0) {
    printf("matrix a on proc 0\n");
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        aMatrix[i][j] = i * j;
        printf("%d ", aMatrix[i][j]);
      }
      printf("\n");
    }
    printf("\n");

    MPI_Send(&aMatrix[0][0], 1, matrixType, 1, 10, MPI_COMM_WORLD);
    MPI_Send(&aMatrix[1][0], 1, matrixType, 1, 20, MPI_COMM_WORLD);

  } else if (myRank == 1) {

    MPI_Recv(&bMatrix[0][0], 1, fullMatrixType, 0, 10, MPI_COMM_WORLD, &stat);
    MPI_Recv(&cMatrix[0][0], 1, fullMatrixType, 0, 20, MPI_COMM_WORLD, &stat);

    printf("matrix b and c on proc 1\n");
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 8; j++) {
        printf("%d ", bMatrix[i][j]);
      }
      printf("\n");
    }
    printf("\n");
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 8; j++) {
        printf("%d ", cMatrix[i][j]);
      }
      printf("\n");
    }

  }
  MPI_Finalize();
  return 0;
}
