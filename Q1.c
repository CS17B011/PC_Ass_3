#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<time.h>

int *a;

int main(int argc, char** argv)
{
    srand(time(NULL));

    int np,rank;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = atoi(argv[1]);
    a = (int*)malloc(n*sizeof(int));
    for(int i = 0; i < n ; ++i)
        a[i] = rand()%100;
    if(rank == 0)
    {
        printf("Elements: ");
        for (int i = 0; i < n; ++i)
            printf("%d ",a[i]);
        printf("\n");
    }

    int start_ele = (n/np)*rank;
    int end_ele;

    if(rank == np-1)
        end_ele = n;
    else
        end_ele = (n / np) * (rank + 1);

    int sum = 0;

    for(int i = start_ele; i < end_ele; ++i)
        sum += a[i];

    if(rank == 0)
    {
        int total_sum = 0;
        for(int i = 1 ; i < np ; ++i)
        {
            int osum;
            MPI_Recv(&osum , 1 , MPI_INT , i , i , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += osum;
        }

        total_sum += sum;

        printf("SUM : %d\n",total_sum);
    }
    else
        MPI_Send(&sum , 1 , MPI_INT , 0 , rank , MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}