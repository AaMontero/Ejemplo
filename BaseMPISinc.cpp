//Encontrar los numeros perfectos, es decir los numeros que la suma
//de sus divisores sea igual al numero 

#include <iostream>  
#include <mpi.h>  
int main(int argc, char ** argv){  
    int rank; 
    int size; 
    int data[1000]; 
    MPI_Init(&argc,&argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD,&size); 
    std::printf("Hola, soy el rank %d, de un total de %d procesos \n", rank, size); 
    if(rank ==0){
        // Se llena el array
        for(int i = 0; i<1000; i++){
            data[i] = i ;
            //std::printf("valor %d", i); 
        }
        //Se necesita enviar los datos a todos los ranks 
        for(int i = 1; i<size;i++){
            std::printf("RANK0 enviando... desde: %d hasta %d \n", data[i* (1000/size) ], (i+1) *data[1000/size] -1 ); 
            MPI_Send(&data[i * (1000/size)],1000/size, MPI_INT,i,0,MPI_COMM_WORLD);    
        }         
    }else{
        int dataPar [1000/size]; 
        MPI_Recv(dataPar,1000/size,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
        std::printf("RANK%d recibiendo .... inicial = %d , final = %d\n", rank, dataPar[0], dataPar[249]);
    }
    MPI_Finalize(); 
    return 0;   
}