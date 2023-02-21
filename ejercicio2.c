#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size, i, j, suma1=0, suma2=0, mayor1=0, mayor2=0;
    int matriz[100][100], matriz2[100][100];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank==0)
    {

    for(i=0; i<100; i++)
    {
        for(j=0; j<100; j++)
        {
            matriz[i][j]=rand()%10;
    }

    MPI_Scatter(matriz, 100*100/size, MPI_INT, matriz2, 100*100/size, MPI_INT, 0, MPI_COMM_WORLD);

    for(i=0; i<100/size; i++)
    {
        for(j=0; j<100; j++)
        {
            if(i==j)
            {
                suma1=suma1+matriz2[i][j];
                if(matriz2[i][j]>mayor1)
                {
                    mayor1=matriz2[i][j];
                }
            }
            if(i+j==99)
            {
                suma2=suma2+matriz2[i][j];
                if(matriz2[i][j]>mayor2)
                {
                    mayor2=matriz2[i][j];
                }
            }
        }
    }

    MPI_Reduce(&suma1, &suma1, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&suma2, &suma2, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mayor1, &mayor1, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mayor2, &mayor2, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if(rank==0)
    {
        printf("La suma de la diagonal principal es: %d\n", suma1);
        printf("El mayor de la diagonal principal es: %d\n", mayor1);
        printf("La suma de la diagonal secundaria es: %d\n", suma2);
        printf("El mayor de la diagonal secundaria es: %d\n", mayor2);
        }
    }

    MPI_Finalize();

    return 0;
 }