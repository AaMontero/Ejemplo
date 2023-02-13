#include <iostream>
#include <thread>
#include <mpi.h>
#include <stdlib.h>
#include <ctime>
//Para utilizar la sintaxis con ms 
using namespace std::chrono_literals; 

int main(int argc, char** argv){
    int rank;
    int size;
    MPI_Init(&argc,&argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD,&size); 
    std::printf("Hola, soy el rank %d, de un total de %d procesos \n", rank, size); 
    //Sincronizar todos los procesos 
    MPI_Barrier(MPI_COMM_WORLD); 

    double start_time = MPI_Wtime(); 
    double rank_time =  rank+1; 
    //Trabajo 
    
    MPI_Barrier(MPI_COMM_WORLD); 
    double main_time = MPI_Wtime()-start_time;
    if(rank == 0){
        std::printf("El tiempo de trabajo es: %lf ", main_time); 
    }
        
    MPI_Finalize(); 
    return 0; 
}