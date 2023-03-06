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
    std::vector<int> sendbuf2(2, rank);
    std::vector<int> recvbuf2(size * 2);
    for (int i = 0; i < SIZE; i++) {
        sendbuf[i] =  i+1;
        //std::printf("El valor que entra es: %d \n", i+1); 
    }
    MPI_Reduce(&sendbuf,recvbuf , SIZE, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);   
    if (rank == 0) {
        printf("Proceso %d recibió en la suma el valor: %d \n", rank, recvbuf[1]);
    }

    /* Ejemplo de MPI_Gather - Todos los procesos envian datos a la raiz 
    int MPI_Gather(const void* sendbuf, int sendcount, MPI_Datatype sendtype,
               void* recvbuf, int recvcount, MPI_Datatype recvtype,
               int root, MPI_Comm comm)*/
    MPI_Gather(sendbuf2.data(), 2, MPI_INT, recvbuf2.data(), 2, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        std::cout << "MPI_Gather: ";
        for (int i = 0; i < size * 2; i++) {
            std::cout << recvbuf2[i] << " ";
        }
        std::cout << std::endl;
    }

    /* Ejemplo de MPI_Allgather - Tpdos los procesos se envian datos a todos los procesos, se omite el origen
    int MPI_Allgather(const void* sendbuf, int sendcount, MPI_Datatype sendtype,
                  void* recvbuf, int recvcount, MPI_Datatype recvtype,
                  MPI_Comm comm)*/

    MPI_Allgather(sendbuf2.data(), 2, MPI_INT, recvbuf2.data(), 2, MPI_INT, MPI_COMM_WORLD);
    if(rank == 0 ){ // Esto está en todos los ranks 
        std::cout << "MPI_Allgather: ";
    for (int i = 0; i < size * 2; i++) {
        std::cout << recvbuf2[i] << " ";
    }
    std::cout << std::endl;
    }
    

    /*  Ejemplo de MPI_Scatter - Distribuye un vector a todos los ranks (Partes iguales)
    int MPI_Scatter(const void* sendbuf, int sendcount, MPI_Datatype sendtype,
                void* recvbuf, int recvcount, MPI_Datatype recvtype,
                int root, MPI_Comm comm)*/

    std::vector<int> scatterbuf(size * 2);
    if (rank == 0) {
        for (int i = 0; i < size * 2; i++) {
            scatterbuf[i] = i;
        }
    }
    MPI_Scatter(scatterbuf.data(), 2, MPI_INT, recvbuf2.data(), 2, MPI_INT, 0, MPI_COMM_WORLD);
    std::cout << "MPI_Scatter: ";
    for (int i = 0; i < 2; i++) {
        std::cout << recvbuf2[i] << " ";
    }
    std::cout << std::endl;
    
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