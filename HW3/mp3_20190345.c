#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ASCIIcode 128
#define MAX_HUFFMAN_LENGTH 64
#define MAX_FILE_LENGHT 100000000
#define HUFFMAN_LENGTH 8

typedef struct node{
    int ASCIInum; //ASCII code number, -1 isn't directeed to ASCII num
    int freq; //frequency of character in the text
    unsigned char* huffman; // huffman code of the ASCII num
    int hlength;//length of huffman code
    struct node* parent; //parent node of PQ
    struct node* left; // left child node of PQ
    struct node* right; //right child node of PQ
}node;

void init(char*,char*);
//detemine mode(-c,-d), dynamic allocations, file input to make sentence
void countASCII();
//count the frequency of the ASCII num in the sentence
void makePriortyQueue();
//make PQ by inserting node which frequency is positive integer
void makeNode(node*,int,int,int,node*,node*,node*);
//make struct node and initialize the node
void insert(node);
//insert node into the PQ
node* delete();
//delete node from the PQ
void makeHuffmanTree();
//make huffman tree by removing two nodes and inserting the sum of two nodes
void getBinaryNum(node*,unsigned char*);
//get huffman code, parent's huffman code + parent's left(0)|right(1)
void makeCompressFile(unsigned char*);
//make compression file(.zz) by file print huffman code of the ASCII code in the text
unsigned char getCompressedCode(int*,int);
//return a word by combining 8 bits of huffman codes
void makeDecompressFile(unsigned char*);
//make decompression file(.yy) by file print ASCII code of the huffman code in the text
int isHuffman(unsigned char*,int,unsigned char*);
//determine whether the string is a huffman code or not. if the string is a huffman code, word is an ASCII code of that huffman code
void finish();
//free dynamic allocations

node* list;
node* PQ;
unsigned char* compressionCode;
int numOfData;
unsigned char* sentence;
int slength;
int goalNum;
int* binaryList;
int blength;
int listNum;
int total;
unsigned char* result;
int relen;
int mode;//0 compression, 1 decompression

int main(int argc,char* argv[]){

    init(argv[1],argv[2]);
    if(mode==0) countASCII();
    makePriortyQueue();
    makeHuffmanTree();
    getBinaryNum(delete(),"");
    if(mode==0) makeCompressFile(argv[2]);
    else makeDecompressFile(argv[2]);
    finish();

    return 0;
}

void init(char* modename,char* filename){
    if(strcmp(modename,"-c")==0) {
        mode=0;
    }
    else if(strcmp(modename,"-d")==0) {
        mode=1;
    }
    else {
        printf("ERROR INPUT\n");
        exit(0);
    }

    FILE* fp=fopen(filename,"rt");
    if(fp==NULL){
        printf("ERROR FILE\n");
        exit(0);
    }
    int word;
    binaryList=(int*)malloc(HUFFMAN_LENGTH*MAX_FILE_LENGHT*sizeof(int));
    compressionCode=(unsigned char*)malloc(HUFFMAN_LENGTH*MAX_FILE_LENGHT*sizeof(unsigned char));
    sentence=(unsigned char*)malloc(MAX_FILE_LENGHT*sizeof(unsigned char));
    list=(node*)malloc(ASCIIcode*sizeof(node));
    result=(unsigned char*)malloc(MAX_FILE_LENGHT*sizeof(unsigned char));
    PQ=(node*)malloc(2*ASCIIcode*sizeof(node));
    for(int i=0;i<2*ASCIIcode;i++){
        PQ[i].huffman=(unsigned char*)malloc(MAX_HUFFMAN_LENGTH*sizeof(unsigned char));
    }
    slength=0;
    numOfData=0;
    goalNum=0;
    blength=0;
    total=0;
    relen=0;
    
    for(int i=0;i<ASCIIcode;i++){
        makeNode(&list[i],i,0,MAX_HUFFMAN_LENGTH,NULL,NULL,NULL);
    }
    if(mode==1){//decompression
        for(int i=0;i<ASCIIcode;i++){
            int freq;
            fscanf(fp,"%d",&freq);
            list[i].freq=freq;
            total+=freq;
        }
        word=fgetc(fp);
    }
    while(1){
        word=fgetc(fp);
        if(word==EOF)break;
        sentence[slength++]=word;
    }
    fclose(fp);
}

void finish(){
    for(int i=0;i<2*ASCIIcode;i++){
        free(PQ[i].huffman);
    }
    for(int i=0;i<ASCIIcode;i++){
        free(list[i].huffman);
    }
    free(PQ);
    free(sentence);
    free(list);
    free(binaryList);
    free(compressionCode);
    free(result);
}

void countASCII(){
    for(int i=0;i<slength;i++){
        list[(int)sentence[i]].freq++;
    }
}

void makePriortyQueue(){
    for(int i=0;i<ASCIIcode;i++){
        if(list[i].freq==0){
            continue;
        }
        insert(list[i]);
        goalNum++;
    }
}

void makeHuffmanTree(){
    for(int i=0;i<goalNum-1;i++){
        node* tmp1=delete();
        node* tmp2=delete();
        node* new_node=(node*)malloc(sizeof(node));
        makeNode(new_node,-1,tmp1->freq+tmp2->freq,MAX_HUFFMAN_LENGTH,NULL,tmp1,tmp2);
        tmp1->parent=new_node;
        tmp2->parent=new_node;
        insert(*new_node);
    }
}

void makeNode(node* w,int num,int freq,int huffSize,node* parent,node* left,node*right){
    w->ASCIInum=num;
    w->freq=freq;
    w->huffman=(unsigned char*)malloc(huffSize*sizeof(unsigned char));
    w->hlength=(int)strlen(w->huffman);
    w->parent=parent;
    w->left=left;
    w->right=right;
}

void insert(node data){
    int i=++numOfData;
    while((i!=1)&&data.freq<PQ[i/2].freq){
        PQ[i].ASCIInum=PQ[i/2].ASCIInum;
        PQ[i].freq=PQ[i/2].freq;
        strcpy(PQ[i].huffman,PQ[i/2].huffman);
        PQ[i].hlength=PQ[i/2].hlength;
        PQ[i].parent=PQ[i/2].parent;
        PQ[i].left=PQ[i/2].left;
        PQ[i].right=PQ[i/2].right;
        i/=2;
    }
    PQ[i].ASCIInum=data.ASCIInum;
    PQ[i].freq=data.freq;
    strcpy(PQ[i].huffman,data.huffman);
    PQ[i].hlength=data.hlength;
    PQ[i].parent=data.parent;
    PQ[i].left=data.left;
    PQ[i].right=data.right;
}

node* delete(){
    int parent,child;
    node* data=(node*)malloc(sizeof(node));
    node* tmp=(node*)malloc(sizeof(node));
    makeNode(data,PQ[1].ASCIInum,PQ[1].freq,MAX_HUFFMAN_LENGTH,PQ[1].parent,PQ[1].left,PQ[1].right);
    makeNode(tmp,PQ[numOfData].ASCIInum,PQ[numOfData].freq,MAX_HUFFMAN_LENGTH,PQ[numOfData].parent,PQ[numOfData].left,PQ[numOfData].right);
    if(numOfData==0) return NULL;
    strcpy(data->huffman,PQ[1].huffman);
    strcpy(tmp->huffman,PQ[numOfData].huffman);
    numOfData--;
    parent=1;
    child=2;
    while(child<=numOfData){
        if((child<numOfData)&&PQ[child].freq>PQ[child+1].freq) child++;
        if(tmp->freq<=PQ[child].freq) break;
        PQ[parent].ASCIInum=PQ[child].ASCIInum;
        PQ[parent].freq=PQ[child].freq;
        strcpy(PQ[parent].huffman,PQ[child].huffman);
        PQ[parent].hlength=PQ[child].hlength;
        PQ[parent].parent=PQ[child].parent;
        PQ[parent].left=PQ[child].left;
        PQ[parent].right=PQ[child].right;
        parent=child;
        child*=2;
    }
    PQ[parent].ASCIInum=tmp->ASCIInum;
    PQ[parent].freq=tmp->freq;
    strcpy(PQ[parent].huffman,tmp->huffman);
    PQ[parent].hlength=tmp->hlength;
    PQ[parent].parent=tmp->parent;
    PQ[parent].left=tmp->left;
    PQ[parent].right=tmp->right;
    free(tmp->huffman);
    free(tmp);
    return data;
}

void getBinaryNum(node* tmp, unsigned char* childWord){
    if(tmp->parent==NULL){
        strcpy(tmp->huffman,childWord);
    }
    else{
        strcpy(tmp->huffman,tmp->parent->huffman);
        strcat(tmp->huffman,childWord);
        tmp->hlength=tmp->parent->hlength+1;

    }
    if(tmp->left!=NULL){
        tmp->left->parent=tmp;
        getBinaryNum(tmp->left,"0");
    }
    if(tmp->right!=NULL){
        tmp->right->parent=tmp;
        getBinaryNum(tmp->right,"1");
    }
    if(tmp->ASCIInum!=-1){
        strcpy(list[tmp->ASCIInum].huffman,tmp->huffman);
        list[tmp->ASCIInum].hlength=tmp->hlength;
    }
}

void makeCompressFile(unsigned char* filename){
    unsigned char* new_filename=(unsigned char*)malloc((3+strlen(filename))*sizeof(unsigned char));
    strcpy(new_filename,filename);
    strcat(new_filename,".zz");
    FILE* fp=fopen(new_filename,"wt");
    for(int i=0;i<ASCIIcode;i++){
        fprintf(fp,"%d\n",list[i].freq);
    }
    for(int i=0;i<slength;i++){
        unsigned char word=sentence[i];
        for(int j=0;j<list[word].hlength;j++){
            binaryList[blength++]=list[word].huffman[j]-'0';
        }
    }
    while(blength%HUFFMAN_LENGTH!=0){
        binaryList[blength++]=0;
    }
    for(int i=0;i<blength/HUFFMAN_LENGTH;i++){
        compressionCode[i]=getCompressedCode(binaryList,i);
        fputc(compressionCode[i],fp);
    }

    fclose(fp);
}

unsigned char getCompressedCode(int* blist, int i){
    unsigned char result=0;
    for(int j=0;j<HUFFMAN_LENGTH;j++){
        result+=(blist[HUFFMAN_LENGTH*i+j])*(int)pow(2,HUFFMAN_LENGTH-(j+1));
    }
    return result;
}

void makeDecompressFile(unsigned char* filename){
    unsigned char* new_filename=(unsigned char*)malloc((5+strlen(filename))*sizeof(unsigned char));
    strcpy(new_filename,filename);
    strcat(new_filename,".yy");
    FILE* fp=fopen(new_filename,"wt");    
    unsigned char word;
    unsigned char tmp;
    for(int i=0;i<slength;i++){
        word=sentence[i];
        if(word<0){
            tmp=word+256;
        }
        else{
            tmp=word;
        }
        for(int j=0;j<HUFFMAN_LENGTH;j++){
            binaryList[HUFFMAN_LENGTH*(i+1)-(j+1)]=tmp%2;
            tmp/=2;
        }
    }
    unsigned char str[MAX_HUFFMAN_LENGTH];
    int len_str=0;
    for(int i=0;i<slength;i++){
        for(int j=0;j<HUFFMAN_LENGTH;j++){
            str[len_str++]=binaryList[i*HUFFMAN_LENGTH+j]+48;
            int flag=isHuffman(str,len_str,&word);
            if(flag==1){
                result[relen++]=word;
                len_str=0;
            }
        }
    }

    for(int i=0;i<relen;i++){
        fputc(result[i],fp);
        total--;
        if(total==0){
            break;
        }
    }
    fclose(fp);
}


int isHuffman(unsigned char* str,int len_str,unsigned char* word){
    for(int i=0;i<ASCIIcode;i++){
        int j;
        for(j=0;j<len_str;j++){
            if(len_str!=list[i].hlength){
                break;
            }
            if(str[j]!=list[i].huffman[j]){
                break;
            }
        }
        if(j==len_str){
            *word=list[i].ASCIInum;
            return 1;
        }
    }
    return -1;
}
