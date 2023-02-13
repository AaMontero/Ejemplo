#include <iostream>
#include <thread>
#include <mpi.h> 
#include <random>

//para utilizar la sintaxis 2000ms
using namespace std::chrono_literals;

int main(int argc, char ** argv){ 
    int rank; 
    int size; 


    MPI_Init(&argc,&argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    //Ya que no hay ninguna especificacion el proceso se realiza en todos los ranks
    std::printf("hola soy el rank: %d de un total de %d procesos\n",rank, size);
    //Creacion de una variable randomica
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(1, 15); //1 a 15s
    int mean = uniform_dist(e1); 
    std::printf("mi random es: %d seconds\n",mean);

    //sincronizar todos los procesos para que empiecen iguales
    MPI_Barrier(MPI_COMM_WORLD);

    double start_time = MPI_Wtime();
    
    //trabajo
    //tiempo aleatorio entre 1 y 15seg
    std::this_thread::sleep_for(1000ms*mean);

    double rank_m_time = MPI_Wtime()-start_time;

    std::printf("Time for rank %d is: %lf seconds\n",rank,rank_m_time);

    //dentro del barrier va el tiempo del rank que más demore

    //Espera que todos los procesos finalicen para antes de sincronizar
    MPI_Barrier(MPI_COMM_WORLD);
    double main_time = MPI_Wtime()-start_time;
    //El rank 0 se encarga de sincronizar y mostrar el tiempo total 
    if(rank==0){
        std::printf("Time for work is: %lf seconds\n",main_time);
    }

    MPI_Finalize(); 
    return 0; 
} 

//mpicxx ejemplo2Virtual.cpp -o ejemplo2Virtual
//mpiexec -n 4 ./ejemplo2Virtual