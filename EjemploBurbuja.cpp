//Encontrar los numeros perfectos, es decir los numeros que la suma
//de sus divisores sea igual al numero 

#include <iostream>  
#include <mpi.h>  
#include <cstdlib>
#include <ctime>
int main(int argc, char ** argv){  
    int rank; 
    int size; 
    int numDatos = 1000; 
    int data[numDatos]; 
    MPI_Init(&argc,&argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD,&size); 
    std::printf("Hola, soy el rank %d, de un total de %d procesos \n", rank, size); 
    if(rank ==0){

        srand(time(0)); // Se establece la semilla 
        // Se llena el array
        for(int i =0; i<numDatos; i++){
            data[i] = rand() % 10000;
        }
        MPI_Request myRequest;
        //Se necesita enviar los datos a todos los ranks 
        for(int i = 1; i<size;i++){
            //std::printf("RANK0 enviando... desde: %d hasta %d \n", data[i* (1000/size) ], (i+1) *data[1000/size] -1 ); 
            MPI_Isend(&data[i * (numDatos/size)],numDatos/size, MPI_INT,i,0,MPI_COMM_WORLD,&myRequest);    
        }
        int arr[numDatos/size]; 
        
        for(int i = 0; i < numDatos/size -1; i++){
            arr[i] = data[i];
        }
        for (int i = 0; i < numDatos/size - 1; i++) {
            for (int j = 0; j < numDatos/size - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    // Intercambia arr[j] y arr[j + 1] si están en el orden equivocado
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
    }   
        int arreglo2[numDatos/size]; 
        int arreglo3[numDatos/size]; 
        int arreglo4[numDatos/size]; 
        MPI_Status status; 
        MPI_Irecv(arreglo2,numDatos/size,MPI_INT,1,0,MPI_COMM_WORLD,&myRequest); 
        MPI_Irecv(arreglo3,numDatos/size,MPI_INT,2,0,MPI_COMM_WORLD,&myRequest); 
        MPI_Irecv(arreglo4,numDatos/size,MPI_INT,3,0,MPI_COMM_WORLD,&myRequest); 
        MPI_Wait(&myRequest, &status);
        /*
        
        for(int i = 0; i < numDatos/size -1; i++){
            std::printf("[%d]",arreglo4[i]); 
        }*/
        int arrayFinalOrd[numDatos]; 

        for(int i = 0; i<numDatos/size-1; i++){
            arrayFinalOrd[i] = arr[i]; 
            arrayFinalOrd[numDatos/size + i] = arreglo2[i]; 
            arrayFinalOrd[2*numDatos/size + i] = arreglo3[i]; 
            arrayFinalOrd[3*numDatos/size + i] = arreglo4[i]; 
        }
        /*for(int i = 0; i < numDatos -1; i++){
            std::printf("[%d]",arrayFinalOrd[i]); 
        }*/
        for (int i = 0; i < numDatos - 1; i++) {
            for (int j = 0; j < numDatos- i - 1; j++) {
                if (arrayFinalOrd[j] > arrayFinalOrd[j + 1]) {
                    // Intercambia arr[j] y arr[j + 1] si están en el orden equivocado
                    int temp = arrayFinalOrd[j];
                    arrayFinalOrd[j] = arrayFinalOrd[j + 1];
                    arrayFinalOrd[j + 1] = temp;
                }
            }
    }   
            printf("El array ordenado es el siguiente: \n");
            for(int i = 0; i < numDatos -1; i++){
            std::printf("[%d]",arrayFinalOrd[i]); 
        }

        //std::printf("El tamaño final del arreglo es: [%ld]",sizeof(arrayFinalOrd)); 

    }else{
        MPI_Request myRequest;
        MPI_Status status;
        int dataPar [numDatos/size]; 
        MPI_Irecv(dataPar,numDatos/size,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&myRequest); 
        MPI_Wait(&myRequest, &status);
        //std::printf("El tamaño del arreglo A es: %ld" ,sizeof(dataPar));
        //std::printf("RANK%d recibiendo .... inicial = %d , final = %d\n", rank, dataPar[0], dataPar[249]);
        for (int i = 0; i < numDatos/size - 1; i++) {
            for (int j = 0; j < numDatos/size - i - 1; j++) {
                if (dataPar[j] > dataPar[j + 1]) {
                    int temp = dataPar[j];
                    dataPar[j] = dataPar[j + 1];
                    dataPar[j + 1] = temp;
                }
            }
    } 

    MPI_Isend(dataPar,numDatos/size, MPI_INT,0,0,MPI_COMM_WORLD,&myRequest);
        
    }
    MPI_Finalize(); 
    return 0;   
}