#include <iostream>
#include <cmath>
#include <omp.h>
#include <cstdlib>
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


const int N = 1000;
int main()
{
    double A[N][N], B[N][N], C[N][N];

    // Inicializar las matrices A y B con valores aleatorios
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }
    }

    // Inicializar la matriz C con ceros
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0.0;
        }
    }

    // Multiplicar las matrices A y B utilizando OpenMP
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                 #pragma omp atomic
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Imprimir los resultados
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << C[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}





