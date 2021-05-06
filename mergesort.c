#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
void merge(int arr[], int s, int m, int e)
{
    int i, j, k;
    int index1 = m - s + 1;
    int index2 = e - m;
    int L[index1], R[index2];

    for (i = 0; i < index1; i++)
        L[i] = arr[s + i];
    for (j = 0; j < index2; j++)
        R[j] = arr[m + 1 + j];
    i = 0;
    j = 0;
    k = s;
    while (i < index1 && j < index2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }


    while (i < index1) {
        arr[k] = L[i];
        i++;
        k++;
    }


    while (j < index2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}


void* mergeSort(void *arg)
{
    int **data = (int *)arg;
    int *ss=data[0];
    int start=*ss;
    int *ee=data[1];
    int end=*ee;
    int *arr=data[2];
    if (start < end) {
        int **data1=(int **)malloc(sizeof(int*)*3);
        int **data2=(int **)malloc(sizeof(int*)*3);
        pthread_t id1;
        pthread_t id2;
        int midpoint = start + (end - start) / 2;
        data1[0]=&start;
        data1[1]=&midpoint;
        data1[2]=arr;
        int val=midpoint+1;
        data2[0]=&val;
        data2[1]=&end;
        data2[2]=arr;
        int dummy;
        pthread_create(&id1,NULL,mergeSort,(void *)data1);
        pthread_create(&id2,NULL,mergeSort,(void *)data2);
        pthread_join(id1,&dummy);
        pthread_join(id2,&dummy);
        merge(arr,start,midpoint,end);
    }
}
int main()
{
    FILE *input=fopen("input.txt","r");
    clock_t start_t, end_t, total_t;
    int n;
    fscanf(input,"%d",&n);
    printf("%d\n",n);
    int array[n];
    for(int i=0;i<n;i++)
    {
        fscanf(input,"%d",&array[i]);
    }
    fclose(input);
    int **data=(int **)malloc(sizeof(int*)*3);
    int start=0;
    int end=n-1;
    data[0]=&start;
    data[1]=&end;
    data[2]=array;
    pthread_t threadid;
    int dummy;
    start_t = clock();
    //pthread_create(&threadid,NULL,mergeSort,data);
    //pthread_join(threadid,dummy);
    mergeSort(data);
    end_t = clock();
    total_t= end_t-start_t;
    for(int i=0;i<n;i++)
        printf("%d\t",array[i]);
    printf("\nElapsed Time:%d",total_t);
    return 0;
}
