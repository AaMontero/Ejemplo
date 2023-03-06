#include <iostream>
#include <cmath>
#include <omp.h>
#include <mpi.h> 
#include <vector> 
#include <ctime>
using namespace std;
#define dimension 4
std::vector<int> matrizA(dimension*dimension); 
std::vector<int> matrizB(dimension*dimension); 

std::vector<int> multiplicarVectMatriz(std::vector<int> vectores, std::vector<int> matriz){
    std::vector<int> respParcial; 
    for(int i =0; i < dimension; i++){
        int aux=0; 
        for(int j = i; j < dimension; j++){
            aux += vectores[j+i] * matriz[i+j]; //0,0 -> 1 * 0 + 4 // 1,0 2
            std::printf("Multiplicando [%d]*[%d]\n",vectores[j],matriz[i+dimension*j]);
        }
        //std::printf("%d",aux); 
        respParcial.push_back(aux); 
    }
    return respParcial; 
}
std::vector<int> matrizResp(std::vector<int> matrizA, std::vector<int> matrizB){
    int numero_hilos = 0; 
    #pragma omp parallel
    {   
        #pragma opm master//le pone en hilo cero
            numero_hilos= omp_get_num_threads();
            
    };
    std::vector<int> vectoresParciales[numero_hilos]; 
    #pragma omp parallel shared (vectoresParciales)
    {
        int thread_id= omp_get_thread_num();
        for(long i=thread_id;i <= dimension; i++){
            vectoresParciales[thread_id] = multiplicarVectMatriz(matrizA,matrizB);
        }

    }
    std::vector<int> matrizRespuesta; 
    for(int i=0;i<=numero_hilos;i++){
       matrizRespuesta.insert(matrizRespuesta.end(), vectoresParciales[i].begin(), vectoresParciales[i].end());
    }
    return matrizRespuesta;
}



int main(){
    srand(time(NULL));
    for(int i = 0; i< dimension*dimension; i++){
        matrizA[i] =  (std::rand()%10+1);
    }
        for(int i = 0; i< dimension*dimension; i++){
        matrizB[i] =  (std::rand()%10+1);
    }
    std::vector<int> matrizFinalResp = matrizResp(matrizA, matrizB); 
    std::printf("\nMatriz A");       
    for(int i = 0; i < dimension*dimension; i++){
        if(i % dimension==0){
            std::printf("\n");
        }
        std::printf("[%d]", matrizA[i]);
    } 
    std::printf("\nMatriz B");       
    for(int i = 0; i < dimension*dimension; i++){
        if(i % dimension==0){
            std::printf("\n");
        }
        std::printf("[%d]", matrizB[i]);
    } 
    std::printf("\nMatriz Resultante");       
    for(int i = 0; i < dimension*dimension; i++){
        if(i % dimension==0){
            std::printf("\n");
        }
        std::printf("[%d]", matrizFinalResp[i]);
    } 


    return 0; 
}