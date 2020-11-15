#include <omp.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    srand(time(NULL));

    int rank, np;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    int n = atoi(argv[1]);
    int a[n];
    if (rank == 0)
    {
        for (int i = 0; i < n; i++)
        {
            a[i] = rand() % 100;
            printf("%d ", a[i]);
        }
        printf("\n");
    }

    if (rank == 0)
    {
        int i, base;
        int n1 = n / np;
        if (np > 1)
        {
            for (i = 1; i < np - 1; i++)
            {
                base = i * n1;
                MPI_Send(&n1, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&a[base], n1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            base = i * n1;
            int remain = n - base;
            MPI_Send(&remain, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&a[base], remain, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        int sum = 0;
#pragma omp parallel for reduction(+ \
                                   : sum)
        for (i = 0; i < n1; i++)
            sum += a[i];
        int part_sum;
        for (i = 1; i < np; i++)
        {
            MPI_Recv(&part_sum, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += part_sum;
        }
        printf("SUM: %d\n", sum);
    }
    else
    {
        int n1, i;
        MPI_Recv(&n1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int a1[n1];
        MPI_Recv(&a1, n1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int part_sum = 0;
#pragma omp parallel for reduction(+ \
                                   : part_sum)
        for (i = 0; i < n1; i++)
            part_sum += a1[i];
        MPI_Send(&part_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}