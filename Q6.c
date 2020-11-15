#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char** argv)
{
    srand(time(NULL));

    int n= atoi(argv[1]);

    int a[n];

    for(int i = 0 ; i < n ; ++i)
        a[i] = rand() % 100;
    for(int i = 0 ; i < n ; ++i)
        printf("%d ", a[i]);
    printf("\n");
    int sum = 0;
    #pragma omp parallel
    for(int i = 0; i < n ; ++i)
        sum += a[i];
    printf("SUM : %d\n", sum);
    return 0;
}