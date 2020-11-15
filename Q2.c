#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char** argv)
{
    srand(time(NULL));

    int np,rank;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = atoi(argv[1]);
    int a[n],i,j;
    for (i = 0; i < np; ++i)
    {
        if (rank == i)
        {
            printf("Before sending data for rank:%d\n",rank);
            for (j = 0; j < n; j++)
            {
                a[j] = rank + rand()%10;
                printf("%d ", a[j]);
            }
            printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if (rank == 0)
        MPI_Sendrecv_replace(&a[0], n, MPI_DOUBLE, (rank + 1) % np, 1, (np - 1), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    else
        MPI_Sendrecv_replace(&a[0], n, MPI_DOUBLE, (rank + 1) % np, 1, (rank - 1), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (i = 0; i < np; ++i)
    {
        if (rank == i)
        {
            printf("After sending data for rank:%d\n",rank);
            for (j = 0; j < n; j++)
                printf("%d ", a[j]);
            printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }  
    MPI_Finalize();
    return 0;
}