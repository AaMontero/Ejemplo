//Realizar una multiplicacion de matrices cuadradas
#include <iostream> 
#include <mpi.h>  
#include <vector> 
#define dimension 4

std::vector<int> vectorResp(std::vector<int> vectores, std::vector<int> matriz){
    std::vector<int> respParcial; 
    for(int i =0; i < dimension; i++){
        int aux=0; 
        for(int j = 0; j < dimension; j++){
            aux += vectores[j] * matriz[i+dimension*j]; //0,0 -> 1 * 0 + 4 // 1,0 2
            //std::printf("Multiplicando [%d]*[%d]\n",vectores[j],matriz[i+dimension*j]);
        }
        //std::printf("%d",aux); 
        respParcial.push_back(aux); 
    }
    return respParcial; 
}


int main(int argc, char ** argv){  
int rank;  
int size;  
MPI_Init(&argc,&argv);  
MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
MPI_Comm_size(MPI_COMM_WORLD, &size); 
int totalElem = dimension*dimension; 
std::vector<int> matrizA (totalElem); 
std::vector<int> matrizB (totalElem);
std::vector <int> matrizPasa (totalElem); 
srand(time(NULL));
if(rank ==0){
    for(int i = 0; i< totalElem; i++){
        matrizA[i] =  (std::rand()%10+1);
    }
    for(int i = 0; i< totalElem; i++){
        matrizB[i] =  (std::rand()%10+1);
    }
   std::printf("Matriz A");          
    for(int i = 0; i<totalElem; i++){
        if(i % dimension==0){
            std::printf("\n");
        }
        std::printf("[%d]", matrizA[i]);
   }
    std::printf("\nMatriz B"); 
    for(int i = 0; i<totalElem; i++){
        if(i%dimension==0){
            std::printf("\n");
        }
        std::printf("[%d]", matrizB[i]);
    }
    MPI_Bcast(&matrizB[0], totalElem, MPI_INT, 0, MPI_COMM_WORLD);
        
}else{  
    MPI_Bcast(matrizPasa.data(),totalElem, MPI_INT, 0, MPI_COMM_WORLD);
}
std::vector<int> vectorFilas (totalElem/size);
MPI_Scatter(matrizA.data(), totalElem/size, MPI_INT, vectorFilas.data(), totalElem/size, MPI_INT, 0, MPI_COMM_WORLD);
if(rank ==0){   
    std::vector<int> parcial = vectorResp(vectorFilas, matrizB);
    std::vector<int> matrizFinal; 
    matrizFinal.insert(matrizFinal.end(), parcial.begin(), parcial.end());
    for(int i = 1; i<size; i++){
        std::vector<int> auxV(dimension); 
        MPI_Recv(auxV.data(), dimension, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        matrizFinal.insert(matrizFinal.end(), auxV.begin(), auxV.end());
    }
    std::printf("\nMatriz Resultante");       
    for(int i = 0; i<totalElem; i++){
        if(i % dimension==0){
            std::printf("\n");
        }
        std::printf("[%d]", matrizFinal[i]);
    } 
}else{
    std::vector<int> parcial = vectorResp(vectorFilas,matrizPasa);
    MPI_Send(parcial.data(), dimension, MPI_INT, 0 , 0 , MPI_COMM_WORLD);
}

MPI_Finalize(); 
return 0;   

 

} 