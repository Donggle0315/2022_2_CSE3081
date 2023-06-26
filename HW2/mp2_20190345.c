/*
sort a list int a increasing order by 4 method
algorithm 1 : insertion sort
algorithm 2 : quick sort
algorithm 3 : merge sort
algorithm 4 : optimizational sort(quick+insertion)

./mp2_20190345 "input file name" "algorithm index"
input file : unsorted list of integers in a single line
output file : 
    1st line : input file name
    2nd line : algorithm index
    3rd line : input size(number of elements)
    4th line : running time in seconds
    5th line : sorted list
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RUN 32

void init(char*);
/**
input : filename
output : x
파일 오픈, 배열 길이 저장, 배열 동적 할당, 배열 저장
*/
void finish(void);
/**
input : x
output : x
동적 해제
*/
void runSortAlgorithm(int);
/**
input : algorithm Number
output : x
메인 입력으로 들어온 정수에 따른 정렬 방법 실행
*/
void makeResultFile(char*,int,int,double,int*);
/**
input : 파일이름, 알고리즘 번호, 배열 사이즈, 러닝 타임, 정렬된 배열
output : x
결과 파일 생성, 매개변수 파일 출력
*/
void insertionSort(int*,int,int);
/**
input : unsorted list, size of list
output : x
sort by using insertion sort
*/
int quickSortPartition(int*,int,int,int);
void quickSort(int*,int,int);
/**
input : unsorted list, left index, right index
output : x
sort by using quick sort
*/
void merge(int*,int,int,int);
void mergeSort(int*,int,int);
/**
input : unsorted list, left index, right index
output : x
sort by using merge sort
*/
int opti_partition(int*,int,int);
void optimizationSort(int*,int,int);
/**
input : unsorted list, size of list
output : x
sort by using optimizational sort
*/

int* list;
int n;

int main(int argc,char* argv[]){
    clock_t start,end;
    double runtime;
    init(argv[1]);
    start=clock();
    runSortAlgorithm(argv[2][0]-'0');
    end=clock();
    runtime=(double)(end-start)/CLOCKS_PER_SEC;
    makeResultFile(argv[1],argv[2][0]-'0',n,runtime,list);
    finish();

    return 0;
}

 void init(char* filename){
    FILE* fp=fopen(filename,"rt");
    if(fp==NULL){
        printf("file open error\n");
        exit(1);
    }
    fscanf(fp,"%d",&n);
    list=(int*)malloc(n*sizeof(int));
    for(int i=0;i<n;i++){
        fscanf(fp,"%d",&list[i]);
    }
    fclose(fp);
 }

 void finish(void){
    free(list);
 }

 void runSortAlgorithm(int algoNum){
    switch(algoNum){
        case 1:
            insertionSort(list,0,n-1);
            break;
        case 2:
            quickSort(list,0,n-1);
            break;
        case 3:
            mergeSort(list,0,n-1);
            break;
        case 4:
            optimizationSort(list,0,n-1);
            break;
    }
 }

 void makeResultFile(char* filename,int algoNum,int arrSize, double runtime, int* sorted_list){
    char* str=(char*)malloc((strlen(filename)+15)*sizeof(char));
    strcpy(str,"result_");
    if(algoNum==1) strcat(str,"1_");
    else if(algoNum==2) strcat(str,"2_");
    else if(algoNum==3) strcat(str,"3_");
    else if(algoNum==4) strcat(str,"4_");
    strcat(str,filename);
    FILE* fp=fopen(str,"wt");
    if(fp==NULL){
        printf("file open error\n");
        exit(1);
    }
    fprintf(fp,"%s\n",filename);
    fprintf(fp,"%d\n",algoNum);
    fprintf(fp,"%d\n",arrSize);
    fprintf(fp,"%lf\n",runtime);
    for(int i=0;i<arrSize;i++){
        fprintf(fp,"%d ",sorted_list[i]);
    }
    fprintf(fp,"\n");
    fclose(fp);
 }

 void insertionSort(int* arr,int start,int end){
    for(int i=start+1;i<=end;i++){
        int key=arr[i];
        int j=i-1;
        while(j>=0&&arr[j]>key){
            arr[j+1]=arr[j];
            j--;
        }
        arr[j+1]=key;
    }
 }

 void quickSort(int* arr,int left, int right){
    if(left<right){
        int pivot=quickSortPartition(arr,left,right,right);
        quickSort(arr,left,pivot-1);
        quickSort(arr,pivot+1,right);
    }
 }

 int quickSortPartition(int* arr,int left, int right,int pivot){
    int i=left-1;
    int tmp=arr[pivot];
    arr[pivot]=arr[right];
    arr[right]=tmp;
    int pivot_dat=arr[right];
    for(int j=left;j<right;j++){
        if(arr[j]<=pivot_dat){
            i++;
            tmp=arr[i];
            arr[i]=arr[j];
            arr[j]=tmp;
        }
    }
    tmp=arr[i+1];
    arr[i+1]=arr[right];
    arr[right]=tmp;
    return i+1;
 }

void mergeSort(int* arr,int left, int right){
    if(left>=right) return;
    int pivot=(left+right)/2;
    mergeSort(arr,left,pivot);
    mergeSort(arr,pivot+1,right);
    merge(arr,left,pivot,right);
}

void merge(int* arr,int left, int pivot, int right){
    int nl=pivot-left+1;
    int nr=right-pivot;
    int i=0,j=0,k=left;
    int* larr=(int*)malloc(nl*sizeof(int));
    int* rarr=(int*)malloc(nr*sizeof(int));
    for(int idx=0;idx<nl;idx++){
        larr[idx]=arr[left+idx];
    }
    for(int idx=0;idx<nr;idx++){
        rarr[idx]=arr[pivot+idx+1];
    }
    while(i<nl&&j<nr){
        if(larr[i]<=rarr[j]) arr[k++]=larr[i++];
        else arr[k++]=rarr[j++];
    }
    while(i<nl){
        arr[k++]=larr[i++];
    }
    while(j<nr){
        arr[k++]=rarr[j++];
    }
    free(larr);
    free(rarr);
}


void optimizationSort(int* arr,int left,int right){
    if(right-left<=RUN){
        insertionSort(arr,left,right);
    }
    else{
        int pivot=opti_partition(arr,left,right);
        optimizationSort(arr,left,pivot-1);
        optimizationSort(arr,pivot+1,right);
    }
}

int opti_partition(int* arr,int left, int right){
    int pivot_list[5]={left,left+(right-left)/4,left+(right-left)/2,left+(3*(right-left))/4,right};
    for(int i=0;i<3;i++){
        int max=0;
        for(int j=1;j<5-i;j++){
            if(arr[pivot_list[j]]>arr[pivot_list[max]]){
                max=j;
            }
        }
        int tmp=pivot_list[max];
        pivot_list[max]=pivot_list[4-i];
        pivot_list[4-i]=tmp;
    }
    return quickSortPartition(arr,left,right,pivot_list[2]);
}
