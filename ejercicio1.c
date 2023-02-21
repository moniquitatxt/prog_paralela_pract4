#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int i, j, n, m, x, count = 0, count_local = 0;
    int my_rank, p;
    int *A, *A_local;
    int n_local, m_local;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank == 0)
    {
        n = 1000;
        m = 1000;
        A = (int *)malloc(n * m * sizeof(int));
        srand(time(NULL));
        for (i = 0; i < n * m; i++)
        {
            A[i] = rand() % 10;
        }
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    n_local = n / p;
    m_local = m / p;
    A_local = (int *)malloc(n_local * m_local * sizeof(int));

    MPI_Scatter(A, n_local * m_local, MPI_INT, A_local, n_local * m_local, MPI_INT, 0, MPI_COMM_WORLD);

    x = 9;

    for (i = 0; i < n_local * m_local; i++)
    {
        if (A_local[i] == x)
            count_local++;
    }

    if (my_rank != 0)
    {
        MPI_Send(&count_local, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        for (i = 0; i < p - 1; i++)
        {
            MPI_Recv(&count_local, 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD, &status);
            count += count_local;
        }
        printf("The number of times %d appears in the matrix is %d\n", x, count);
    }


    MPI_Finalize();

    return 0;
}