#include <iostream>
#include <mpi.h>
int main(int argc, char** argv){
    int rank;
    int size;
    int data[130];
    MPI_Init(&argc,&argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD,&size); 
    std::printf("Hola, soy el rank %d, de un total de %d procesos \n", rank, size); 
    if(rank == 0 ){
        for(int i =0 ; i<130;i++){
            data[i]= i; 
        }
        std::printf("RANK0 enviando... \n"); 
        MPI_Send(&data[34],32,MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Send(&data[66],32,MPI_INT,2,0,MPI_COMM_WORLD);
        MPI_Send(&data[98],32,MPI_INT,3,0,MPI_COMM_WORLD);
        int suma_parcial = 0; 
        for(int i = 0 ; i <34;i++){
            suma_parcial = suma_parcial + data[i];
        }
        std::printf("RANK%d recibiendo .... inicial = %d , final = %d\n", rank, data[0], data[32]);
        int suma_parcial2 = 0 ; 
        int suma_parcial3 = 0 ; 
        int suma_parcial4 = 0 ; 
        MPI_Recv(&suma_parcial2,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
        MPI_Recv(&suma_parcial3,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
        MPI_Recv(&suma_parcial4,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
        int suma_total = suma_parcial + suma_parcial2 + suma_parcial3 + suma_parcial4; 
        std::printf("La suma total es: %d", suma_total);
    }else{
        std::printf("RANK%d recibiendo ...\n", rank); 
        MPI_Recv(data,33,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
        std::printf("RANK%d recibiendo .... inicial = %d , final = %d\n", rank, data[0], data[31]);
        int suma_parcial = 0; 
        for(int i = 0 ; i <32;i++){
            suma_parcial = suma_parcial + data[i];
        }
        std::printf("RANK%d suma parcial = %d\n", rank, suma_parcial);
        MPI_Send(&suma_parcial,1,MPI_INT,0,0,MPI_COMM_WORLD);
    }
}
