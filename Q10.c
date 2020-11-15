
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, np;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    int a[n], i, j;

    for (j = 0; j < np; j++)
    {
        if (rank == j)
        {
            printf("Rank:%d\n", rank);
            for (int i = 0; i < n; i++)
            {
                a[i] = rank;
                printf("%d ", a[i]);
            }
            printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    int cur = 0, val;
    for (int i = 0; i < m; i++)
    {
        if (cur == rank)
            val = rand() % np;
        MPI_Bcast(&val, 1, MPI_INT, cur, MPI_COMM_WORLD);
        if (val == cur)
            continue;
        if (cur == rank)
            MPI_Send(&a[0], n, MPI_INT, val, 1, MPI_COMM_WORLD);
        if (rank == val)
            MPI_Recv(&a[0], n, MPI_INT, cur, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cur = val;
        MPI_Barrier(MPI_COMM_WORLD);
    }

    for (int j = 0; j < np; j++)
    {
        if (rank == j)
        {
            printf("Rank:%d\n", rank);
            for (int i = 0; i < n; i++)
                printf("%d ", a[i]);
            printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}