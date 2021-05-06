#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

void* compute(void *arg)
{
    int **data = (int *)arg;
    int *c11=data[0];
    int c1=*c11;
    int *c22=data[6];
    int c2=*c22;
    int (*matrix1)[c1];
    int (*matrix2)[c2];
    int (*matrix3)[c2];
    int *ii=data[4];
    int *jj=data[5];
    matrix1=data[1];
    matrix2=data[2];
    matrix3=data[3];
    int n = c1;
    int i=(int)ii;
    int j=(int)jj;
    int x,result;
    result=0;
    for(x=0;x<n;x++)
        result+= matrix1[i][x]*matrix2[x][j];
    matrix3[i][j]=result;
    free(data);
}
void* computeRow(void *arg)
{
    int **data = (int *)arg;
    int *c11=data[0];
    int c1=*c11;
    int *c22=data[5];
    int c2=*c22;
    int (*matrix1)[c1];
    int (*matrix2)[c2];
    int (*matrix3)[c2];
    matrix1=data[1];
    matrix2=data[2];
    matrix3=data[3];
    int *r = data[4];
    int rownumber=(int)r;
    int result=0;
    int l2,l1;
    for(l1=0; l1<c2;l1++)
    {
        result =0;
        for(l2=0;l2<c1;l2++)
            {
                result+=matrix1[rownumber][l2]*matrix2[l2][l1];
            }
        matrix3[rownumber][l1]=result;
    }

}

int main()
{
    clock_t start_t, end_t, total_t;
    int rows,columns;
    int rows2,columns2;
    FILE *input=fopen("input.txt","r");
    fscanf(input,"%d", &rows);
    fscanf(input,"%d", &columns);
    int i,j;
    int matrix1[rows][columns];
    for(i=0;i<rows;i++)
    {
        for(j=0;j<columns;j++)
        {
            fscanf(input,"%d",&matrix1[i][j]);
        }
    }
    fscanf(input,"%d", &rows2);
    fscanf(input,"%d", &columns2);
    int matrix2[rows2][columns2];
    for(i=0;i<rows2;i++)
    {
        for(j=0;j<columns2;j++)
        {
            fscanf(input,"%d",&matrix2[i][j]);
        }
    }
    fclose(input);
    int matrix3[rows][columns2];
    int size=rows*columns2;
    pthread_t threads[size];
    int z=0;
    start_t = clock();
    for(i=0;i<rows;i++)
    {
        for(j=0;j<columns2;j++)
        {
            int ii, jj;
            ii = i;
            jj = j;
            int **data=(int **)malloc(sizeof(int*)*7);
            int (*p1)[columns];
            p1=matrix1;
            int (*p2)[columns2];
            p2=matrix2;
            int (*p3)[columns2];
            p3=matrix3;
            data[0]=&columns;
            data[1]=p1;
            data[2]=p2;
            data[3]=p3;
            data[6]=&columns2;
            data[4]=(int *)ii;
            data[5]=(int *)jj;
            pthread_create(&threads[z],NULL,compute,(void *)data);
            z++;
        }
    }

    for(int t=0;t<size;t++)
    {
        int dummy;
        pthread_join(threads[t],&dummy);
    }
    end_t=clock();
    FILE *output=fopen("output.txt","w");
    fprintf(output,"%s","Result matrix entries\n");
    total_t=(end_t-start_t);
    for(int p=0;p<rows;p++)
    {
        for(int w=0;w<columns2;w++)
            fprintf(output,"%d\t ",matrix3[p][w]);
            fprintf(output,"%s","\n");
    }
    fprintf(output,"%s","END1:");
    fprintf(output,"%d",total_t);
    fprintf(output,"%s","\n");
    fprintf(output,"%s","result matrix\n");
    z=0;
    pthread_t threads2[rows];
    int q;
    start_t = clock();
    for(q=0;q<rows;q++)
    {
        int **data2=(int **)malloc(sizeof(int*)*6);
        int ii;
        ii = q;
        int (*p1)[columns];
        p1=matrix1;
        int (*p2)[columns2];
        p2=matrix2;
        int (*p3)[columns2];
        p3=matrix3;
        data2[1]=p1;
        data2[2]=p2;
        data2[3]=p3;
        data2[0]=&columns;
        data2[4]=(int *)ii;
        data2[5]=&columns2;
        pthread_create(&threads2[z],NULL,computeRow,(void *)data2);
        z++;
    }
    for(int t=0;t<rows;t++)
    {
        int dummy;
        pthread_join(threads2[t],&dummy);
    }
    end_t=clock();
    total_t=(end_t-start_t);
    //printf("OUTPUT MATRIX\n");
    for(int p=0;p<rows;p++)
    {
        for(int w=0;w<columns2;w++)
            fprintf(output,"%d\t ",matrix3[p][w]);
            fprintf(output,"%s","\n");
    }
    fprintf(output,"%s","END2:");
    fprintf(output,"%d",total_t);
    fprintf(output,"%s","\n");
    fprintf(output,"%s","WE NOTICE THAT COMPUTING EACH ROW IN A THREAD IS FASTER THAN COMPUTING EACH ELEMENT IN A THREAD\n");
    fclose(output);
    return 0;
}
