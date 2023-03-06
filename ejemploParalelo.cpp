/*Generar  40 vectores y  debe devolver la posición del vector 
que sea paralelo con  otro vector que le metas por parámetro o quemado*/
#include <iostream>
#include <mpi.h>  
#include <vector>
#include <cmath> 
#include <stdlib.h>
#define numVectores 40

bool calcularPerteneceRecta(int x, int y, int z, int a1, int a2, int a3){
    float t; 
    if (x/(float)a1==y/(float)a2 && y/(float)a2==z/(float)a3){
        return true; 
    }else{
        return false; 
    }
}

int main(int argc, char ** argv){  
int rank; 
int size; 
MPI_Init(&argc,&argv);  
MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
MPI_Comm_size(MPI_COMM_WORLD, &size); 
srand(time(NULL));
std::vector<int> vectorI(numVectores*3); 
std::vector<int> vectorFijo(3); 
std::vector<int> vectorR(numVectores*3/size);
if(rank ==0){
    //Se rellena el vector con los puntos (40*3)
    for(int i = 0; i< numVectores*3;i++){
        vectorI[i] = (std::rand()%11);
    }
}
MPI_Scatter(vectorI.data(), numVectores*3/size, MPI_INT, vectorR.data(), numVectores*3/size, MPI_INT, 0, MPI_COMM_WORLD);


if(rank==0){
    
    std::vector<int> indices; 
    for(int i = 0; i < 3; i++){
        vectorFijo[i] = (std::rand()%11+1);
    }
    MPI_Bcast(&vectorFijo[0], 3, MPI_INT, 0, MPI_COMM_WORLD); //Se envia el centro a todos
    //std::printf("Rank 0"); 
    for(int i =0 ; i<numVectores/size; i++){
        //std::printf("%d,%d,%d,%d,%d,%d \n",vectorR[3*i],vectorR[3*i+1],vectorR[3*i+2],vectorFijo[0],vectorFijo[1],vectorFijo[2]);
        if(calcularPerteneceRecta(vectorR[3*i],vectorR[3*i+1],vectorR[3*i+2],vectorFijo[0],vectorFijo[1],vectorFijo[2])){
            indices.push_back(i); 
            //std::printf("Esta en la recta ");
        }
    }
    
    for(int i = 1 ; i<size; i++){
      int indiceAux; 
        MPI_Recv(&indiceAux, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::vector<int> auxV (indiceAux) ; 
        MPI_Recv(auxV.data(), indiceAux, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        indices.insert(indices.end(), auxV.begin(), auxV.end()); 
    }
    std::printf("El centro es: [%d][%d][%d]\n", vectorFijo[0], vectorFijo[1], vectorFijo[2]); 
    std::printf("El número de elementos que cumplen es: %ld \n", indices.size()); 
    for(int i = 0; i < indices.size(); i++){

        std::printf("El indide del valor que esta en la recta es: %d \n", indices[i]);
        std::printf("Es el punto [%d],[%d],[%d]\n", vectorI[indices[i]*3],vectorI[indices[i]*3+1],vectorI[indices[i]*3+2]);
    }
    
    

}else{
    std::vector<int> punto(3); 
    std::vector<int> indices(0);  
    MPI_Bcast(punto.data(),3, MPI_INT, 0, MPI_COMM_WORLD);
    for(int i = 0; i<numVectores/size; i++){
        if(calcularPerteneceRecta(vectorR[3*i],vectorR[3*i+1],vectorR[3*i+2],punto[0],punto[1],punto[2])){
            indices.push_back(rank* numVectores/size + i); 
            printf("En el rank %d ,el valor que añade es: %d \n",rank, rank* numVectores/size + i);
        }
    }
    int auxIndice = indices.size() ; 
    //std::printf("Indice del rank %d, en %d \n" , rank, auxIndice );
    MPI_Send(&auxIndice, 1, MPI_INT, 0,0,MPI_COMM_WORLD);
    MPI_Send(indices.data(), auxIndice, MPI_INT, 0 , 0 , MPI_COMM_WORLD); 
}

MPI_Finalize(); 
return 0;   
} 

