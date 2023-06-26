//
//  mp1_20190345.c
//  testProject
//
//  Created by 김동현 on 2022/09/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void init(char*);
/*
 input : 메인 함수를 통해 들어온 파일 이름
 output : x
 파일을 열고, row,col값을 저장 후, 배열 동적 할당 및 값 저장
 */

void finish(void);
/*
 input : x
 output : x
 동적 할당된 메모리를 해제
 */

void makeResultFile(char*,int,int,int,int,double);
/*
 input : 파일 이름, 알고리즘 넘버, 행, 열, MSS 값, running time
 output : x
 결과 파일을 생성한 후, 함수 매개변수의 값을 파일로 출력
 */

int mostSumSubarray(int);
/*
 input : algorithm num
 output : result of mss
 알고리즘 번호에 맞는 알고리즘으로 mms값을 구함
 */

int MSS_6(void);
/*
 input : x
 output : MSS의 값
 시간복잡도 O(n^6)을 가진 알고리즘으로  MMS를 구한다
 */

int MSS_4(void);
/*
 input : x
 output : MSS의 값
 시간복잡도 O(n^4)을 가진 알고리즘으로  MMS를 구한다
 */

int MSS_3(void);
/*
 input : x
 output : MSS의 값
 시간복잡도 O(n^3)을 가진 알고리즘으로  MMS를 구한다
 */



int row, col;
int** arr;


int main(int argc,char* argv[]){
    clock_t start,end;
    int result;
    double runtime;
    init(argv[1]);
    start=clock();
    result =mostSumSubarray(argv[2][0]-'0');
    end=clock();
    runtime=(double)(end-start)/CLOCKS_PER_SEC;
    makeResultFile(argv[1], argv[2][0]-'0', row, col, result, runtime);
    finish();
}

void init(char* filename){
    FILE*  fp=fopen(filename,"rt");
    if(fp==NULL){
        printf("file open error\n");
        exit(1);
    }
    fscanf(fp,"%d %d",&row,&col);
    arr=(int**)malloc(row*sizeof(int*));
    for(int i=0;i<row;i++){
        arr[i]=(int*)malloc(col*sizeof(int));
        for(int j=0;j<col;j++){
            fscanf(fp,"%d",&arr[i][j]);
        }
    }
    fclose(fp);
}

void finish(void){
    for(int i=0;i<row;i++){
        free(arr[i]);
    }
    free(arr);
}

void makeResultFile(char* filename,int algoNum,int r,int c ,int result,double runTime){
    char* str=(char*)malloc((strlen(filename)+15)*sizeof(char));
    strcpy(str,"result_");
    strcat(str,filename);
    FILE*  fp=fopen(str,"wt");
    
    fprintf(fp,"%s\n",filename);
    fprintf(fp,"%d\n",algoNum);
    fprintf(fp,"%d\n",r);
    fprintf(fp,"%d\n",c);
    fprintf(fp,"%d\n",result);
    fprintf(fp,"%lf\n",runTime);
    
    fclose(fp);
}

int mostSumSubarray(int algoNum){
    int result;
    
    switch(algoNum){
        case 1:
            result=MSS_6();
            break;
        case 2:
            result=MSS_4();
            break;
        case 3:
            result=MSS_3();
            break;
        default :
            result=0;
    }
    
    return result;
}

int MSS_6(void){
    int maxSum=arr[0][0];
    for(int i=0;i<row;i++){
        int startRow=i;
        for(int j=0;j<col;j++){
            int startCol=j;
            for(int k=i;k<row;k++){
                int finishRow=k;
                for(int l=0;l<col;l++){
                    if(startRow==finishRow&&startCol>l){
                        continue;
                    }
                    int finishCol=l;
                    
                    int sum=0;
                    for(int p=startRow;p<=finishRow;p++){
                        for(int q=startCol;q<=finishCol;q++){
                            sum+=arr[p][q];
                        }
                    }
                    if(sum>maxSum){
                        maxSum=sum;
                    }
                }
            }
        }
    }
    return maxSum;
}

int MSS_4(void){
    int maxSum=arr[0][0];
    int** new_arr=NULL;
    new_arr=(int**)malloc(row*sizeof(int*));
    for(int i=0;i<row;i++){
        new_arr[i]=(int*)malloc(col*sizeof(int));
    }
    
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            if(i==0&&j==0){
                new_arr[i][j]=arr[i][j];
            }
            else if(i==0){
                new_arr[i][j]=new_arr[i][j-1]+arr[i][j];
            }
            else if(j==0){
                new_arr[i][j]=new_arr[i-1][j]+arr[i][j];
            }
            else{
                new_arr[i][j]=new_arr[i][j-1]+new_arr[i-1][j]+arr[i][j]-new_arr[i-1][j-1];
            }
        }
    }
    
    
    for(int i=0;i<row;i++){
        int startRow=i;
        for(int j=0;j<col;j++){
            int startCol=j;
            for(int k=startRow;k<row;k++){
                int finishRow=k;
                for(int l=0;l<col;l++){
                    if(startRow==finishRow&&startCol>l){
                        continue;
                    }
                    int finishCol=l;
                    
                    
                    
                    // 우하향 - 좌하향 - 우상향 + 좌상향
                    int thisSum;
                    if(startRow==0&&startCol==0){
                        thisSum=new_arr[finishRow][finishCol];
                    }
                    else if(startRow==0){
                        thisSum=new_arr[finishRow][finishCol]-new_arr[finishRow][startCol-1];
                    }
                    else if(startCol==0){
                        thisSum=new_arr[finishRow][finishCol]-new_arr[startRow-1][finishCol];
                    }
                    else{
                        thisSum=new_arr[finishRow][finishCol]-new_arr[startRow-1][finishCol]-new_arr[finishRow][startCol-1]+new_arr[startRow-1][startCol-1];
                    }
                    if(thisSum>maxSum){
                        maxSum=thisSum;
                    }
                }
            }
        }
    }
    
    for(int i=0;i<row;i++){
        free(new_arr[i]);
    }
    free(new_arr);
    
    return maxSum;
}

int MSS_3(void){
    int maxSum=arr[0][0];
    int** new_arr = NULL;
    new_arr=(int**)malloc(row*sizeof(int*));
    for(int i=0;i<row;i++){
        new_arr[i]=(int*)malloc(col*sizeof(int));
        for(int j=0;j<col;j++){
            if(j==0){
                new_arr[i][j]=arr[i][j];
            }
            else{
                new_arr[i][j]=new_arr[i][j-1]+arr[i][j];
            }
        }
    }
    
    for(int left=0;left<col;left++){
        for(int right=left;right<col;right++){
            int thisSum=0;
            for(int height=0;height<row;height++){
                int p=(left==0)?0:new_arr[height][left-1];
                int q=new_arr[height][right];
                thisSum+=(q-p);
                if(thisSum>maxSum){
                    maxSum=thisSum;
                }
                else if(thisSum<0){
                    thisSum=0;
                }
            }
        }
    }
    for(int i=0;i<row;i++){
        free(new_arr[i]);
    }
    free(new_arr);
        
    return maxSum;
}
