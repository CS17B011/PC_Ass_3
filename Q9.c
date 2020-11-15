#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char** argv)
{
    srand(time(NULL));

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int p = atoi(argv[3]);

    int a[m][n] , b[n][p] , c[m][p];

    for(int i = 0 ; i < m ; ++i)
    {
        for(int j = 0 ; j < n ; ++j)
            a[i][j] = rand()%10;
    }

    for(int i = 0 ; i < n ; ++i)
    {
        for(int j = 0 ; j < p ; ++j)
            b[i][j] = rand()%10;
    }

    #pragma omp parallel
    for(int i = 0; i < m ; ++i)
    {
        for (int j = 0; j < p; ++j)
        {
            int sum = 0;
            for(int k = 0; k < n; ++k)
                sum += a[i][k]*b[k][j];
            c[i][j] = sum;
        }
    }
    for (int i = 0; i < m; ++i)
    {
        for(int j = 0 ; j < p ; ++j)
            printf("%d ",c[i][j]);
        printf("\n");
    }
    return 0;
}