#include <iostream> 
#include <mpi.h>  
#include <vector> 
#include <omp.h>
#include <cmath>
#define longitud 200
//Serial 
std::vector<int> llenarAleatorios(std::vector<int> vectorVacio){
    std::vector <int> vectorCopia (vectorVacio.size()); 
    srand(time(NULL)); 
    for(int i = 0; i< vectorVacio.size(); i++){
        vectorCopia[i] = rand()%(10+1); 
    }
    return vectorCopia; 
}
double calcularProductoPunto(std::vector<int> vector1, std::vector<int> vector2){
    std::vector<int> vectorLleno1 = vector1;  // llenarAleatorios(vector1)
    std::vector<int> vectorLleno2 = vector2;  // llenarAleatorios(vector2)
    std::vector<int> vectorResp; 
    for(int i = 0; i< vectorLleno1.size(); i++){
        vectorResp.push_back(vectorLleno1[i]*vectorLleno2[i]); 
    }
    double productoPunto = 0; 
    for(int i = 0 ; i < vectorResp.size(); i++){
        productoPunto += vectorResp[i]; 
    }
    return productoPunto; 
}
//MPI 
double calcularProdPuntoOMP(std::vector<int> vector1, std::vector<int> vector2){
    int numero_hilos; 
    #pragma omp parallel
    {   
        #pragma opm master//le pone en hilo cero
            numero_hilos= omp_get_num_threads();
    };
    double sumas_parciales[numero_hilos]; 
    #pragma omp parallel shared (sumas_parciales)
    {
        int thread_id= omp_get_thread_num();
        sumas_parciales[thread_id]=0; 
        #pragma omp for
        for(int i= 0 ; i <= longitud ; i++){
            sumas_parciales[thread_id] += vector1[i]* vector2[i];
            //std::printf("[%d] * [%d]",vector1[i],vector2[i]);
        }
    }
    double respuesta=0;
    //suma de todos los hilos
    for(int i=0; i<= numero_hilos; i++){
       std::printf("[%lf]\n", sumas_parciales[i]);
       respuesta= respuesta + sumas_parciales[i];
    }
    return respuesta;
}



int main(int argc, char ** argv){  
int rank;  
int size;  
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
MPI_Comm_size(MPI_COMM_WORLD, &size); 
std::vector<int> arregloAle1(longitud); 
std::vector<int> arregloAle2(longitud);
/* Serial
double respuesta = calcularProductoPunto(arregloAle1, arregloAle2);
std::printf("El producto punto es: %lf", respuesta); */
/*std::vector<int> arreglo (longitud);

arregloAle1 = llenarAleatorios(arreglo); 
arregloAle2 = llenarAleatorios(arreglo);*/
int total_puntos = std::ceil(longitud/(float)size); 
/*std::printf("\nArreglo 1 \n");
for(int i = 0; i<arregloAle1.size(); i++){
    std::printf("[%d],",arregloAle1[i]);
}
std::printf("\nArreglo 2\n");
for(int i = 0; i<arregloAle2.size(); i++){
    std::printf("[%d],",arregloAle2[i]);
}*/
/*double respuesta = calcularProductoPunto(arregloAle1, arregloAle2);
std::printf("El producto punto serial es: %lf \n ", respuesta);
double respuestaOMP = calcularProdPuntoOMP(arregloAle1, arregloAle2); 
std::printf("El valor de la respuesta OMP es: %lf", respuestaOMP );*/
if(rank ==0){
     
    std::vector<int> vector1MPI(longitud); 
    std::vector<int> vector2MPI(longitud); 
    srand(time(NULL));
    for(int i = 0; i< longitud; i++){
        vector1MPI[i] = rand()%(10+1); 
    }
    for(int i = 0; i< longitud; i++){
        vector2MPI[i] = rand()%(10+1); 
    }

    std::printf("%d", longitud/size);
    std::printf("Arreglo 1 \n");
    for(int i = 0; i<longitud/size; i++){
        std::printf("[%d],", vector1MPI[i]);
    }
    std::printf("Arreglo 2 \n");
    for(int i = 0; i<longitud/size; i++){
        std::printf("[%d],", vector2MPI[i]);
    }
    for(int i = 1; i < size; i++){
        //std::printf("Entra a enviar");
        MPI_Send(&vector1MPI[i* longitud/size], longitud/size-1, MPI_INT, i , 0 , MPI_COMM_WORLD); 
        MPI_Send(&vector2MPI[i* longitud/size], longitud/size-1, MPI_INT, i , 0 , MPI_COMM_WORLD); 
    }
    double sumaParcial1 = 0; 
    for(int i = 0; i < longitud/size; i++){
        sumaParcial1 += vector1MPI[i] * vector2MPI[i];
    }
    //std::printf("\n La suma parcial del rank 0 es: %lf", sumaParcial1);
    double sumaFinal = sumaParcial1; 
        for(int i =1 ; i <size; i++){
            double sumaParcial; 
            MPI_Recv(&sumaParcial , i , MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);    
            sumaFinal+=sumaParcial;
        }  
    std::printf("El resultado en MPI es: %lf", sumaFinal);
}else{
    std::vector<int> vectorARec (longitud/size);
    std::vector<int> vectorBrec (longitud/size);
    MPI_Recv(vectorARec.data(),longitud/size-1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(vectorBrec.data(),longitud/size-1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::vector<int> vectorSum (longitud/size); 
    for(int i = 0; i<vectorSum.size();i++){
        vectorSum[i] = vectorARec[i]*vectorBrec[i]; 
    }   
    double suma = 0; 
    for(int i = 0; i<vectorSum.size(); i++){
        suma += vectorSum[i];
    }
    //std::printf("Suma: %lf", suma);
    MPI_Send(&suma, 1, MPI_DOUBLE, 0 , 0 , MPI_COMM_WORLD); 
}





MPI_Finalize();
return 0;   
} 