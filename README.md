"# Ejemplo" 
## Instalación de MPI en Ubuntu

sudo apt-get update 
sudo apt-get install libopenmpi-dev

Verificar la instalación
mpiexec --version

##Compilar 

mpicxx main.cpp -o ejemplo 

mpicxx ejemploPI.cpp -o ejemplo 
mpicxx EjemploNumPerfectosAsinc.cpp -o ejemplo


##Ejecutar
mpiexec -n 2 ./ejemplo
mpiexec -n 4 ./ejemplo
