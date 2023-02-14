//Encontrar la menor distancia entre los distintos vectores 

#include <iostream>  
#include <mpi.h>  
#include <thread>
#include <random>
#include <cmath>
int main(int argc, char ** argv){  
    int rank; 
    int size; 
    int numVectores = 10; 
    int vectores[numVectores][3] ; 
    int numDatos = 30; 
    int vectorTotal[30]; 
    int vectorCentros[] = {1,3,6,5,2,8,1,6,9}; 
    MPI_Init(&argc,&argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD,&size); 
    std::printf("Hola, soy el rank %d, de un total de %d procesos \n", rank, size);
    
    for(int i = 0; i<numVectores; i++){
            for(int j=0;j<3;j++){
                std::random_device r;
                std::default_random_engine e1(r());
                std::uniform_int_distribution<int> uniform_dist(1, 30); //Generador de numeros Aleat 1-30
                int aleat = uniform_dist(e1); 
                vectores[i][j] = aleat; 
            }
        }

    if(rank ==0){
        for(int i = 0; i<numDatos; i++){
            std::random_device r;
            std::default_random_engine e1(r());
            std::uniform_int_distribution<int> uniform_dist(1, 30); //Generador de numeros Aleat 1-30
            int aleat = uniform_dist(e1); 
            vectorTotal[i] = aleat;        
        }

        MPI_Send(&vectorTotal[0],6, MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Send(&vectorTotal[6],6, MPI_INT,2,0,MPI_COMM_WORLD);
        MPI_Send(&vectorTotal[12],6, MPI_INT,3,0,MPI_COMM_WORLD);

        int indiceMenor0[4]; 
        int datosParc0[12];
        for(int i = 0 ; i <12 ; i++){
            datosParc0[i] = vectorTotal[18+i]; 
        }
       /*     std::printf("DATOS DEL RANK 0");
        for(int i = 0 ; i <12 ; i++){
            std::printf("[%d]", datosParc0[i]);
        }*/


        double distMenor[4] = {1000000000,1000000000,1000000000,100000000}; 
        int indiceDistanciaMenor[4] = {10,10,10,10}; 
        int distanciasMenores[4]; 
        for(int i = 0 ; i < 4; i++){
            for(int j =0 ; j<3 ; j++){
                double dist = sqrt(pow(datosParc0[i]-vectorCentros[j],2)+pow (datosParc0[i+1]-vectorCentros[j+1],2)+pow(datosParc0[i+2]-vectorCentros[j+2],2));
                //std::printf("La distancia es de %d a C%d es: %f \n" ,i,j, dist); 
                if (dist<distMenor[i]){
                    distMenor[i] = dist; 
                    indiceDistanciaMenor[i] = j; 
                } 
            }
            
        }

        int indiceMenorRank1[2]; 
        int indiceMenorRank2[2]; 
        int indiceMenorRank3[2]; 

        MPI_Recv(&indiceMenorRank1,2,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&indiceMenorRank2,2,MPI_INT,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&indiceMenorRank3,2,MPI_INT,3,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        int indiceJuntosMatrices[10];

        std::printf("La menor distancia para el vector 1 [%d] [%d] [%d] se encuentra con el centro # %d \n", vectores[0][0], vectores[0][1], vectores[0][2], indiceMenorRank1[0]);
        std::printf("La menor distancia para el vector 2 [%d] [%d] [%d] se encuentra con el centro # %d \n", vectores[1][0], vectores[1][1], vectores[1][2], indiceMenorRank1[1]);
        std::printf("La menor distancia para el vector 3 [%d] [%d] [%d] se encuentra con el centro # %d \n", vectores[2][0], vectores[2][1], vectores[2][2], indiceMenorRank2[0]);
        std::printf("La menor distancia para el vector 4 [%d] [%d] [%d] se encuentra con el centro # %d \n", vectores[3][0], vectores[3][1], vectores[3][2],  indiceMenorRank2[1]);
        std::printf("La menor distancia para el vector 5 [%d] [%d] [%d] se encuentra con el centro # %d \n", vectores[4][0], vectores[4][1], vectores[4][2],  indiceMenorRank3[0]);
        std::printf("La menor distancia para el vector 6 [%d] [%d] [%d] se encuentra con el centro # %d \n", vectores[5][0], vectores[5][1], vectores[5][2],  indiceMenorRank3[1]);
        std::printf("La menor distancia para el vector 7 [%d] [%d] [%d] se encuentra con el centro # %d \n", vectores[6][0], vectores[6][1], vectores[6][2],  indiceDistanciaMenor[0]);
        std::printf("La menor distancia para el vector 8 [%d] [%d] [%d] se encuentra con el centro # %d \n", vectores[7][0], vectores[7][1], vectores[7][2],  indiceDistanciaMenor[1]);
        std::printf("La menor distancia para el vector 9 [%d] [%d] [%d] se encuentra con el centro # %d \n", vectores[8][0], vectores[8][1], vectores[8][2],  indiceDistanciaMenor[2]);
        std::printf("La menor distancia para el vector 10 [%d] [%d] [%d] se encuentra con el centro # %d \n", vectores[9][0], vectores[9][1], vectores[9][2],  indiceDistanciaMenor[3]);
    
       
    }else{
        int datosParc[6]; 
       MPI_Recv(&datosParc,6,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
       /*std::printf("\n Datos del rank %d" , rank );
       for(int i = 0 ; i <6;i++){
        std::printf("[%d] ",datosParc[i]);
       }*/
        double distMenor[2] = {1000000000,1000000000}; 
        int indiceDistanciaMenor[2] = {10,10}; 
        int distanciasMenores[2]; 
        for(int i = 0 ; i < 2; i++){
            for(int j =0 ; j<3 ; j++){
                double dist = sqrt(pow(datosParc[i]-vectorCentros[j],2)+pow (datosParc[i+1]-vectorCentros[j+1],2)+pow(datosParc[i+2]-vectorCentros[j+2],2));
                //std::printf("La distancia de %d a C%d es: %f \n" ,i,j, dist); 
                if (dist<distMenor[i]){
                    distMenor[i] = dist; 
                    indiceDistanciaMenor[i] = j; 
                    //distMenor[i] = distMenor;
                } 
            }
            
        }
        //std::printf ("El indice de la distancia menor es: %d ",indiceDistanciaMenor) ; 
        MPI_Send(&indiceDistanciaMenor,2, MPI_INT,0,0,MPI_COMM_WORLD);
       
    }
        
    MPI_Finalize(); 
    return 0;   
}
//  mpicxx prueba.cpp -o ejemplo
//  mpiexec -n 4 ./ejemplo
