#include <mpi>
#include <stdio.h>
#include <stdlib.h>

main( int argc, char **argv){
      int myrank;
      int len;

      char name [MPI_MAX_PROCESSOR_NAME];

      MPI_Init(&argc, &argv);
      MPI_Comm_rank( MPI_COMM_WORLD, &myrank);
      MPI_Get_processor_name( name, &len );

      printf("%s: hello from %d\n", name, myrank);

      MPI_Finalize();
}
