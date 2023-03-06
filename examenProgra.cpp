#include <iostream> 
#include <omp.h>
#include <mpi.h>  
#include <vector> 
#include <cmath>
#include <algorithm>
#include <fstream>
#include <ctime>
using namespace std;

std::vector<int> llenarArray(){
    std::vector<int> valoresLectura; 
    std::ifstream archivo;
    archivo.open("datos.txt");
    string tmp;
    // Obtener línea de archivo, y almacenar contenido en "linea"
    while (std::getline(archivo, tmp)) {
    // Lo vamos imprimiendo
    int valor = std::stoi(tmp); 
    valoresLectura.push_back(valor); 
}
return valoresLectura; 
}
//Método Lineal 
void remove(std::vector<int> &v){
    auto end = v.end();
    for (auto it = v.begin(); it != end; ++it) {
        end = std::remove(it + 1, end, *it);
        }
        v.erase(end, v.end());
        }

void calcularConteoLineal (std::vector<int> vectorEnteros){
std::sort(vectorEnteros.begin(), vectorEnteros.end()); 
std::vector<int> copia = vectorEnteros; 
remove(vectorEnteros);
std::vector<int> repetidos(vectorEnteros.size());
for(int i = 0; i < vectorEnteros.size(); i++){
    int aux = 0;
    for(int j = 0; j < copia.size(); j++){
        if(vectorEnteros[i] == copia[j]){
            aux++;
            }
        }
        repetidos[i] = aux;
    } 
        for(int i = 0; i<repetidos.size(); i++){
             printf("[%d], Etiqueta: [%d], Repeticiones: [%d]\n",i, vectorEnteros[i], repetidos[i]);
        }
}

std::vector<int> calcularRepeticionesLineal (std::vector<int> vectorEnteros){
std::sort(vectorEnteros.begin(), vectorEnteros.end()); 
std::vector<int> copia = vectorEnteros; 
remove(vectorEnteros);
std::vector<int> repetidos(vectorEnteros.size());
for(int i = 0; i < vectorEnteros.size(); i++){
    int aux = 0;
    for(int j = 0; j < copia.size(); j++){
        if(vectorEnteros[i] == copia[j]){
            aux++;
            }
        }
        repetidos[i] = aux;
    } 
    return repetidos; 
}
std::vector<int> calcularEtiquetasLineal (std::vector<int> vectorEnteros){
std::sort(vectorEnteros.begin(), vectorEnteros.end()); 
std::vector<int> copia = vectorEnteros; 
remove(vectorEnteros);
std::vector<int> repetidos(vectorEnteros.size());
for(int i = 0; i < vectorEnteros.size(); i++){
    int aux = 0;
    for(int j = 0; j < copia.size(); j++){
        if(vectorEnteros[i] == copia[j]){
            aux++;
            }
        }
        repetidos[i] = aux;
    } 
    return vectorEnteros; 
}



void calcularFrecuencia(std::vector<int> vectorEnteros){
    std::sort(vectorEnteros.begin(), vectorEnteros.end()); 
    int numero_hilos; 
    std::vector<int> copia = vectorEnteros; 
    remove(vectorEnteros);
    std::vector<int> repetidos(vectorEnteros.size());
    #pragma omp parallel
    {   
        #pragma opm master//le pone en hilo cero
            numero_hilos= omp_get_num_threads();
    };
    repetidos[numero_hilos]; 
    #pragma omp parallel shared (repetidos)
    {
        int thread_id= omp_get_thread_num();
        repetidos[thread_id]=0;
        
        #pragma omp for
        for(int i = 0; i < vectorEnteros.size(); i++){
            int aux = 0;
            for(int j = 0; j < copia.size(); j++){
                if(vectorEnteros[i] == copia[j]){
                    aux++;
                }
            }
            repetidos[i] = aux;
            }           
        }
        for(int i = 0; i<repetidos.size(); i++){
             printf("[%d], Etiqueta: [%d], Repeticiones: [%d]\n",i, vectorEnteros[i], repetidos[i]);
        }
    }




int main(int argc, char ** argv){  
int rank;  
int size;  
MPI_Init(&argc,&argv); 
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
vector<int> lista; 
lista = llenarArray();
/*double start_time = MPI_Wtime(); 
std::printf("Modo Serial"); 
calcularConteoLineal(lista);
double main_time = MPI_Wtime()-start_time;
std::printf("El tiempo de trabajo es: %lf ", main_time);
*/
double start_time = MPI_Wtime(); 
std::printf("OMP"); 
calcularFrecuencia(lista); 
double main_time = MPI_Wtime()-start_time;
std::printf("El tiempo de trabajo es: %lf ", main_time);
/*double start_time = MPI_Wtime(); 
std::printf("MPI"); 
if(rank==0 ){
    //lista = {6, 8, 7, 3, 2, 8, 9, 3 ,6, 8, 7, 3, 2, 8, 9, 3,6, 8, 7, 3, 2, 8, 9, 3,6, 8, 7, 3, 2, 8, 9, 3,6, 8, 7, 3, 2, 8, 9, 3,6, 8, 7, 3, 2, 8, 9, 3}; 
    lista = llenarArray(); 
    int tamanioLista = lista.size(); 
    int tamanioenviar = std::ceil(tamanioLista/(float)size); 
    std::vector <int> vector0; 
    int valorInicial = tamanioLista  - (tamanioenviar *(size -1)); 
    for(int i =1 ; i<size ; i++){
        MPI_Send(&tamanioLista, 1, MPI_INT, i , 0 , MPI_COMM_WORLD);         
        MPI_Send(&lista[valorInicial*i], tamanioenviar, MPI_INT, i , 0 , MPI_COMM_WORLD); 
    }
    std::vector<int> lista0; 
    for(int i = 0; i< valorInicial-1; i++){
        lista0.push_back(lista[i]);
    }


    std::vector<int> lista0Etiquetas = calcularEtiquetasLineal(lista0);
    std::vector<int> lista0Repeticiones = calcularRepeticionesLineal(lista0); 


    int longitudArreglo1; 
    int longitudArreglo2; 
    int longitudArreglo3; 


    MPI_Recv(&longitudArreglo1,1,MPI_INT, 1,MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::vector<int>listaEtiq1(longitudArreglo1); 
    MPI_Recv(listaEtiq1.data(), longitudArreglo1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    std::vector<int>listaRep1(longitudArreglo1); 
    MPI_Recv(listaRep1.data(), longitudArreglo1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    

    MPI_Recv(&longitudArreglo2,1,MPI_INT, 2,MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::vector<int>listaEtiq2(longitudArreglo2); 
    MPI_Recv(listaEtiq2.data(), longitudArreglo2, MPI_INT, 2, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    std::vector<int>listaRep2(longitudArreglo2);
    MPI_Recv(listaRep2.data(), longitudArreglo2, MPI_INT, 2, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    

    MPI_Recv(&longitudArreglo3,1,MPI_INT, 3,MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  
    std::vector<int>listaEtiq3(longitudArreglo3); 
    MPI_Recv(listaEtiq3.data(), longitudArreglo3, MPI_INT, 3, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    std::vector<int>listaRep3(longitudArreglo3);
    MPI_Recv(listaRep3.data(), longitudArreglo3, MPI_INT, 3, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    
    std::vector<int> listaRepeticionesFinal; 
    listaRepeticionesFinal.insert(listaRepeticionesFinal.end(), lista0Repeticiones.begin(), lista0Repeticiones.end());
    listaRepeticionesFinal.insert(listaRepeticionesFinal.end(), listaRep1.begin(), listaRep1.end());
    listaRepeticionesFinal.insert(listaRepeticionesFinal.end(), listaRep2.begin(), listaRep2.end());
    listaRepeticionesFinal.insert(listaRepeticionesFinal.end(), listaRep3.begin(), listaRep3.end());
    std::vector<int> listaEtiqueasFinal; 
    listaEtiqueasFinal.insert(listaEtiqueasFinal.end(),lista0Etiquetas.begin(), lista0Etiquetas.end());
    listaEtiqueasFinal.insert(listaEtiqueasFinal.end(),listaEtiq1.begin(), listaEtiq1.end());
    listaEtiqueasFinal.insert(listaEtiqueasFinal.end(),listaEtiq2.begin(), listaEtiq2.end());
    listaEtiqueasFinal.insert(listaEtiqueasFinal.end(),listaEtiq3.begin(), listaEtiq3.end());
    std::printf("\n Realizacion MPI \n");
    
    std::vector<int> listEtiquetas (100); 
    std::vector<int> listRepeticiones (100); 
    for(int i = 0; i < 101; i++){
        listEtiquetas[i] =i; 
        listRepeticiones[i] = listaRepeticionesFinal[i]+listaRepeticionesFinal[i+101]+listaRepeticionesFinal[i+202]+listaRepeticionesFinal[i+303];
    }
    for(int i = 0; i <= listEtiquetas.size(); i++){
          printf("[%d], Etiqueta: [%d], Repeticiones: [%d]\n",i, listEtiquetas[i], listRepeticiones[i]);
    }
    double main_time = MPI_Wtime()-start_time;
    std::printf("El tiempo de trabajo es: %lf ", main_time);
}else{
    int tamanioR; 
    MPI_Recv(&tamanioR, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    std::vector<int> listaR(tamanioR);
    MPI_Recv(listaR.data(), tamanioR, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    std::vector<int> etiquetas = calcularEtiquetasLineal(listaR); 
    std::vector<int> repeticiones = calcularRepeticionesLineal(listaR); 
    int longitudArregloR = etiquetas.size(); 
    MPI_Send(&longitudArregloR, 1, MPI_INT, 0 , 0 , MPI_COMM_WORLD); 
    MPI_Send(etiquetas.data(), etiquetas.size(), MPI_INT, 0 , 0 , MPI_COMM_WORLD); 
    MPI_Send(repeticiones.data(), repeticiones.size(), MPI_INT, 0 , 0 , MPI_COMM_WORLD); 

}
*/

MPI_Finalize(); 

return 0;   
} 