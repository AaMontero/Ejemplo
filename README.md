"# Ejemplo" 
## Instalación de MPI en Ubuntu

sudo apt-get update 
sudo apt-get install libopenmpi-dec

Verificar la instalación
mpiexec --version

##Compilar 

mpicxx main.cpp -o ejemplo 


##Ejecutar
mpiexec -n 2 ./ejemplo
