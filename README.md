"# Ejemplo" 
## Instalacion de OMP
sudo apt-get install libomp-dev
sudo apt-get update
sudo apt-get install -y build-essential
#Es necesario añadir lo siguiente al CMAKElist 
find_package(MPI REQUIRED)
Asi se compila
--> mpicxx -fopenmp TrabajoGrupal.cpp -o ejemplo


## Instalación de MPI en Ubuntu
sudo apt-get update 
sudo apt-get install libopenmpi-dev

Verificar la instalación
mpiexec --version

##Compilar 

mpicxx main.cpp -o ejemplo 

mpicxx ejemploPI.cpp -o ejemplo 
mpicxx EjemploNumPerfectosAsinc.cpp -o ejemplo
mpicxx TrabajoGrupal.cpp -o ejemplo 
mpicxx correccionPrueba.cpp -o ejemplo 
mpicxx correcionPBCast.cpp -o ejemplo 
mpicxx ejemploReduce.cpp -o ejemplo 
mpicxx ejemploParalelo.cpp -o ejemplo 
mpicxx multiMatrices.cpp -o ejemplo 

mpicxx -fopenmp productoPunto.cpp -o ejemplo
mpicxx -fopenmp examenProgra.cpp -o ejemplo
mpicxx -fopenmp TrabajoGrupal.cpp -o ejemplo
mpicxx -fopenmp multMatricesOMP.cpp -o ejemplo
g++ -fopenmp multMatricesOMP.cpp -o ejemplo


##Ejecutar
mpiexec -n 2 ./ejemplo
mpiexec -n 1 ./ejemplo
mpiexec -n 4 ./ejemplo