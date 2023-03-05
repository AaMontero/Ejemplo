#include <iostream>
#include <mpi.h>  
#include <vector>

#define SIZE 4 
int main(int argc, char ** argv){  

    int rank, size;
    int sendbuf[SIZE];
    int recvbuf[SIZE]; 
    int max;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    for (int i = 0; i < SIZE; i++) {
        sendbuf[i] =  i+1;
        std::printf("El valor que entra es: %d \n", i+1); 
    }
    MPI_Reduce(&sendbuf,recvbuf , SIZE, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);   
    if (rank == 0) {
        printf("Proceso %d recibió el máximo valor: %d \n", rank, recvbuf[1]);
    }
    MPI_Finalize();
    return 0;
/*int rank; 
int size;  
MPI_Init(&argc,&argv);  
MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
MPI_Comm_size(MPI_COMM_WORLD, &size); 

    int sum = rank;
    int total_sum;
    MPI_Reduce(&sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    //std::printf("El valor en el rank %d es %d", rank, sum); 
    if (rank == 0) {
        std::cout << "The total sum is " << total_sum << std::endl;
    }
*/
MPI_Finalize(); 

return 0;   

}