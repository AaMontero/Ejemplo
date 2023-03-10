//Encontrar los numeros perfectos, es decir los numeros que la suma
//de sus divisores sea igual al numero 

#include <iostream>  
#include <mpi.h>  
int main(int argc, char ** argv){  
    int rank; 
    int size; 
    int valoresNum = 10000; 
    int data[valoresNum]; 
    MPI_Init(&argc,&argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD,&size); 
    std::printf("Hola, soy el rank %d, de un total de %d procesos \n", rank, size); 
    if(rank ==0){
        int arrayFinal[10];
        int contador = 0;  
        
        // Se llena el array
        for(int i = 0; i<valoresNum; i++){
            data[i] = i ;
        }
        //Se necesita enviar los datos a todos los ranks 
        for(int i = 1; i<size;i++){
            std::printf("RANK0 enviando... desde: %d hasta %d \n", data[i* (valoresNum/size) ], (i+1) *data[valoresNum/size] -1 ); 
            MPI_Send(&data[i * (valoresNum/size)],valoresNum/size, MPI_INT,i,0,MPI_COMM_WORLD);    
        }

        for (int i =0; i<valoresNum/size; i++){
            int suma =0; 
            if(i == 0){
            }else{
                int suma = 0; 
                for(int y= 1; y<i; y++){       
                    if(i % y == 0){
                        suma += y; 
                        
                        
                    }
                }
                if(suma == i){
                    arrayFinal[contador] = i; 
                    contador+=1; 
                    std::printf("Los numeros primos son: %d \n", i);
            }
            }
            
        }
        int datosARecibir = 0; 
        for(int i =1; i<size; i++){            
            MPI_Recv(&datosARecibir,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
            int vector[datosARecibir]; 
            MPI_Recv(vector,datosARecibir,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
            for(int i = 0; i<datosARecibir; i++){
                std::printf("Los numeros perfectos son: %d \n", vector[i]);
                arrayFinal[contador] = vector[i]; 
                contador+=1; 
            }

        }
        std::printf("Los valores perfectos en este rango son: ");
        for(int i=0; i < contador; i++){
            std::printf("[%d]", arrayFinal[i]); 
        }


    }else{
        int aux[10]; 
        int dataPar [valoresNum/size]; 
        MPI_Recv(dataPar,valoresNum/size,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
        std::printf("RANK%d recibiendo .... inicial = %d , final = %d\n", rank, dataPar[0], dataPar[249]);
        int contador = 0; 
        //std::printf("Soy el rank %d y El valor final es: %d \n", rank ,dataPar[valoresNum/size -1]);
        for (int i = dataPar[0] ; i<dataPar[valoresNum/size-1]; i++){
            int suma = 0; 
            if(i == 0){
            }else{
                int suma = 0; 
                for(int y= 1; y<i; y++){       
                    if(i % y == 0){
                        suma += y; 
                    }
                }
                if(suma == i){
                    //std::printf("Los numeros perfectos son: %d , %d \n", i, i);
                    aux[contador] = i; 
                    contador +=1;
                }
            }       
        }
        int arreglo[contador]; 
        MPI_Send(&contador,1, MPI_INT,0,0,MPI_COMM_WORLD);
            //std::printf("El arreglo aux en el recibe tiene %d datos, el array tiene %d datos \n", aux[0], contador); 
        for(int i = 0; i<contador; i++){
                arreglo[i] = aux[i];
            }
        MPI_Send(arreglo,contador, MPI_INT,0,0,MPI_COMM_WORLD);
    }
    MPI_Finalize(); 
    return 0;   
}