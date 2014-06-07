//
//  main.c
//  C_DataEngineering
//
//  Created by jason9075 on 2014/3/17.
//  Copyright (c) 2014年 Jason Kuan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <getopt.h>

struct recNode {
    fpos_t id;
    fpos_t pub;
    fpos_t upd;
    fpos_t tit;
    fpos_t con;
    fpos_t aut;
    fpos_t key;
    fpos_t fav;
    fpos_t vie;
    fpos_t dur;
    fpos_t cat;
    fpos_t src;
    struct recNode *next;
};

int readFile(struct recNode [],char []);
fpos_t posGet(struct recNode [],char [],int );
// sorting function //
int compare(char [],char []);
int strcicmp(char const [], char const []);
//int quick_sort_old(struct recNode [],int ,int , char []);
void swap(struct recNode *x,struct recNode *y);
void quick_sort(struct recNode [],int,int,char []);
void merge_sort(struct recNode *list, int list_size,char *recName);
void merge_array(struct recNode *list1, int list1_size, struct recNode *list2, int list2_size,char *recName);
void Output(struct recNode [], int , char []);
void Display(struct recNode [], int , char []);
// searching function //
int min(int, int, int );
int ASM(char [], char []);

/* Flags  */
bool numCmp;
bool inc;
bool insensitive;
bool numerical;
bool quicksort;
bool key;
char name[30];
int  page = 1;

FILE *fPtr;

/* Flags End */

int main(int argc, const char *argv[]){
    int totalData;
    struct recNode Data[27000];
    struct recNode searchData[100];
    char column[100];
    
    numCmp         = false;
    inc            = false;
    insensitive    = false;
    numerical      = false;
    key            = false;
    quicksort      = true;
    
    sscanf("youtubeSampleRec.high.txt","%s",name);
//    sscanf("searchSmall.txt","%s",name);
    fPtr = fopen(name,"r");  /* fopen function  */
    totalData = readFile(Data, name);
//    printf("Content-Type: text/plain;charset=UTF-8\n");
//    printf("%d筆資料\n",totalData);
    
    printf("Content-Type: application/json; charset=utf-8\n\n");
    
    /* Command Arguments Start */
    const char *optstring="r:ik:csnp:";
    int c,index;
    struct option opts[]={
        {"rb",required_argument,NULL,'r'},
//        {"inc",no_argument,NULL,'i'},
//        {"key",required_argument,NULL,'k'},
//        {"caseinsenitive",no_argument,NULL,'c'},
//        {"stable",no_argument,NULL,'s'},
//        {"numerical",no_argument,NULL,'n'},
        {"page",required_argument,NULL,'p'},
        {0,0,0,0}
    };
    while((c=getopt_long(argc,(char **)argv,optstring,opts,&index))!=-1){
        switch(c){
            case 'r'://-r 或者 --rb 指定排列的欄位
                strcpy(column,optarg);
                break;
//            case 'i'://-i 或者 --inc 排列從小到大
//                inc = true;
//                break;
//            case 'k'://-k 或者--key,用key1和key2排列
//                key = true;
//                strcpy(column,optarg);
//                break;
//            case 'c'://-c 或者--caseinsenitive,忽略大小寫
//                insensitive    = true;
//                break;
//            case 's'://-s 或者--stable,排列是否穩定
//                quicksort      = false;
//                break;
//            case 'n'://-n 或者--numerical,用字串長度排列
//                numerical      = true;
//                break;
            case 'p'://-p 或者--page,第幾頁
                page           = atoi(optarg);
                break;
            default:
                printf("error input");
                column[0] = '\0';
                break;
        }
    }
    /* Command Arguments End */
    
    /* Sorting start  */
//    if (key == true) {
//        char *be, tempStr[100];
//        int len;
//        while (strrchr(column,',') != NULL ) {
//            be = strrchr(column,',');
//            len = (int)(strlen(column) - (be - column));
//            column[(int)(be - column)] = '\0';
//            int i;
//            for (i = 0; i<len; i++) {
//                tempStr[i] = column[(int)(be - column)+i+1];
//                column[(int)(be - column)+i] = '\0';
//            }
//            merge_sort(Data, totalData,tempStr);
//            memset(tempStr,0,strlen(tempStr));
//        }
//        merge_sort(Data, totalData,column);
//    }else{
//        if( quicksort== true ){
//            quick_sort(Data,0,totalData-1,column);
//        }
//        else{
//            merge_sort(Data, totalData,column);
//        }
//    }
    /* Sorting end  */
    
    /* Searching start  */
    char text[100];
    char ch;
    int si=0, i;
    int tolerance = ((int)strlen(column)/4);
    for (i=0; i<totalData; i++) {
        index = 0;
        fseek(fPtr, Data[i].tit, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' && index<99){
            text[index] = ch;
            index++;
        }
        text[index] = '\0';
        if (ASM(text, column)<=tolerance && si<100) {
            searchData[si++] = Data[i];
        }
    }
    /* Searching end  */
    
    
//    Output(Data, totalData, name);
//    Display(Data, totalData, name);
    Display(searchData, si, name);
    
    fclose(fPtr); /* close */
    return 0;
}

/* Read File Start */
int readFile(struct recNode *Data, char *fileName){

    fpos_t pos;
    char line[500];
    int total = 0; /* total Datas  */
    
    if (!fPtr) {
        printf("Error: File not found...\n");
        exit(1);
    }
    
    while (!feof(fPtr)) {
        fgets(line,500,fPtr);                   /* Read Line  */
        if (line[0]=='@' && line[1]=='\n') {
            while (1) {
                fgets(line,500,fPtr);
                if (line[0]=='@') {                         /* RecordData or not  */
                    fgetpos(fPtr, &pos);
                    pos -= (strlen(line)-1);
                    if (line[1]=='i' && line[2]=='d')
                        Data[total].id = pos  + strlen("id:");
                    else if (line[1]=='p' && line[2]=='u' && line[3]=='b')
                        Data[total].pub = pos + strlen("published:");
                    else if (line[1]=='u' && line[2]=='p' && line[3]=='d')
                        Data[total].upd = pos + strlen("updated:");
                    else if (line[1]=='t' && line[2]=='i' && line[3]=='t')
                        Data[total].tit = pos + strlen("title:");
                    else if (line[1]=='c' && line[2]=='o' && line[3]=='n')
                        Data[total].con = pos + strlen("content:");
                    else if (line[1]=='a' && line[2]=='u' && line[3]=='t')
                        Data[total].aut = pos + strlen("author:");
                    else if (line[1]=='k' && line[2]=='e' && line[3]=='y')
                        Data[total].key = pos + strlen("keyword:");
                    else if (line[1]=='f' && line[2]=='a' && line[3]=='v')
                        Data[total].fav = pos + strlen("favoriteCount:");
                    else if (line[1]=='v' && line[2]=='i' && line[3]=='e')
                        Data[total].vie = pos + strlen("viewCount:");
                    else if (line[1]=='d' && line[2]=='u' && line[3]=='r')
                        Data[total].dur = pos + strlen("duration:");
                    else if (line[1]=='c' && line[2]=='a' && line[3]=='t')
                        Data[total].cat = pos + strlen("category:");
                    else if (line[1]=='s' && line[2]=='r' && line[3]=='c'){
                        Data[total].src = pos + strlen("src:");
                        total++;
                        break;
                    }
                }
            }
        }
    }
    
    return total;
}
/* Read File End */

/* posGet Start */
fpos_t posGet(struct recNode *array,char *fileName,int index){
    fpos_t pos;
    
    /* which column start  */
    if (!strcmp(fileName, "id")){
        pos = array[index].id;
        numCmp = false;
    }
    else if (!strcmp(fileName, "published")){
        pos = array[index].pub;
        numCmp = false;
    }
    else if (!strcmp(fileName, "updated")){
        pos = array[index].upd;
        numCmp = false;
    }
    else if (!strcmp(fileName, "title")){
        pos = array[index].tit;
        numCmp = false;
    }
    else if (!strcmp(fileName, "content")){
        pos = array[index].con;
        numCmp = false;
    }
    else if (!strcmp(fileName, "author")){
        pos = array[index].aut;
        numCmp = false;
    }
    else if (!strcmp(fileName, "keyword")){
        pos = array[index].key;
        numCmp = false;
    }
    else if (!strcmp(fileName, "favoriteCount")){
        pos = array[index].fav;
        numCmp = true;
    }
    else if (!strcmp(fileName, "viewCount")){
        pos = array[index].vie;
        numCmp = true;
    }
    else if (!strcmp(fileName, "duration")){
        pos = array[index].dur;
        numCmp = true;
    }
    else if (!strcmp(fileName, "category")){
        pos = array[index].cat;
        numCmp = false;
    }
    else
        printf("error no position found.....");
    
    /* which column end  */
    
    return pos;
}
/* posGet End */

/* Compare Function Start */
int compare(char *str_1,char *str_2){
    if (numCmp == true && inc == true )
        return (atoi(str_1) > atoi(str_2)) ? 1 : 0 ;        // numCmp
    else if (numCmp == true && inc == false )
        return (atoi(str_1) < atoi(str_2)) ? 1 : 0 ;        // numCmp, inc
    else if (numCmp == false && inc == true && insensitive == false && numerical == false)
        return (strcmp(str_1,str_2) > 0) ? 1 : 0 ;          //
    else if (numCmp == false && inc == false && insensitive == false && numerical == false)
        return (strcmp(str_1,str_2) < 0) ? 1 : 0 ;          // inc
    
    else if (numCmp == false && inc == true && insensitive == true && numerical == false)
        return (strcicmp(str_1,str_2) > 0) ? 1 : 0 ;          // insensitive
    else if (numCmp == false && inc == false && insensitive == true && numerical == false)
        return (strcicmp(str_1,str_2) < 0) ? 1 : 0 ;          // inc, insensitive
    
    else if (numCmp == false && inc == true && insensitive == false && numerical == true)
        return (strlen(str_1) > strlen(str_2)) ? 1 : 0 ;          // numerical
    else if (numCmp == false && inc == false && insensitive == false&& numerical == true)
        return (strlen(str_1) < strlen(str_2)) ? 1 : 0 ;          // inc, numerical
    
    else if (numCmp == false && inc == true && insensitive == true&& numerical == true)
        return (strlen(str_1) > strlen(str_2)) ? 1 : 0 ;          // insensitive, numerical
    else if (numCmp == false && inc == false && insensitive == true&& numerical == true)
        return (strlen(str_1) < strlen(str_2)) ? 1 : 0 ;          // inc, insensitive, numerical
    else
        printf("compare error");
    return 100;             // error
}
/* Compare Function End */

/* String insensitive Start */
int strcicmp(char const *a, char const *b){
    for (;; a++, b++) {
        int d = tolower(*a) - tolower(*b);
        if (d != 0 || !*a)
            return d;
    }
}
/* String insensitive End */

/* Quick Sort Start */
void swap(struct recNode *x,struct recNode *y)
{
    struct recNode temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
void quick_sort(struct recNode *array,int m,int n,char *recName) {
    
    int i,j,k,index;
    char key[1000],ch,str1[1000],str2[1000];
    if( m < n)
    {
        k = ((m+n) /2);
        swap(&array[m],&array[k]);
        
        index=0;
        fseek(fPtr, posGet(array, recName,m), SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' && index<499){
            key[index] = ch;
            index++;
        }
        key[index] = '\0';
        
        i = m;
        j = n+1;
        
        while(i <= j){
            do{
                i++;
                index=0;
                fseek(fPtr, posGet(array, recName,i), SEEK_SET);
                while ( (ch = getc(fPtr)) != '@' && index<499 ){
                    str1[index] = ch;
                    index++;
                }
                str1[index] = '\0';
            }while((i <= n) && compare(key,str1));
            
            do{
                j--;
                index=0;
                fseek(fPtr, posGet(array, recName,j), SEEK_SET);
                while ( (ch = getc(fPtr)) != '@' && index<499){
                    str2[index] = ch;
                    index++;
                }
                str2[index] = '\0';
            }while((j >= m) && compare(str2,key));
            
            if( i < j)
                swap(&array[i],&array[j]);
        }
        /* swap two elements */
        swap(&array[m],&array[j]);
        
        /* recursively sort the lesser list */
        quick_sort(array,m,j-1,recName);
        quick_sort(array,j+1,n,recName);
    }
}
/* Quick Sort End */

/* Merge Sort Start */
void merge_sort(struct recNode *list, int list_size,char *recName){
    if (list_size > 1){
        struct recNode *list1 = list;
        int list1_size = list_size / 2;
        struct recNode *list2 = list + list_size / 2;
        int list2_size = list_size - list1_size;
        
        merge_sort(list1, list1_size,recName);
        merge_sort(list2, list2_size,recName);
        
        merge_array(list1, list1_size, list2, list2_size,recName);
    }
}

void merge_array(struct recNode *list1, int list1_size, struct recNode *list2, int list2_size,char *recName){
    
    int i, j, k;
    i = j = k = 0;
    
    struct recNode *list = malloc((list1_size + list2_size) * sizeof(struct recNode));
    
    while (i < list1_size && j < list2_size){
        char str1[1000],str2[1000],ch;
        int index = 0 ;
        fseek(fPtr, posGet(list1, recName,i), SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' ){
            str1[index] = ch;
            index++;
        }
        str1[index] = '\0';
        index=0;
        fseek(fPtr, posGet(list2, recName,j), SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' ){
            str2[index] = ch;
            index++;
        }
        str2[index] = '\0';
        list[k++] = compare(str1,str2) ? list2[j++] : list1[i++];
    }
    
    while (i < list1_size)
        list[k++] = list1[i++];
    
    while (j < list2_size)
        list[k++] = list2[j++];
    
    int ii;
    for (ii = 0; ii < (list1_size + list2_size); ++ii)
        list1[ii] = list[ii];
    free(list);
}
/* Merge Sort End */

/* Output File Start */
void Output(struct recNode *Data, int total, char *fileName){
    
    FILE *fPtrOut;   /*宣告FILE資料型態的 指標*/
    char ch;
    
    fPtrOut = fopen("output.txt","w");  /* fopen function , 給予檔案名稱，和寫入方式 */
    
    int i;
    fprintf(fPtrOut,"{\n    \"records\" : [\n");
    fprintf(fPtrOut,"        { \"id\" :\"ID\" , \"published\" :\"Published\" , \"updated\" :\"Updated\" , \"title\" :\"Title\" , \"content\" :\"Content\" , \"author\" :\"Author\" , \"keyword\" :\"Keyword\" , \"favoriteCount\" :\"Favorit Count\", \"viewCount\" :\"View Count\", \"duration\" :\"Duration\", \"category\" :\"Category\" }\n");
    for (i=0; i < total; i++) {
        fprintf(fPtrOut,"       ,{ ");
        fprintf(fPtrOut," \"id\":\"");
        fseek(fPtr, Data[i].id, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                putc(' ',fPtrOut);
            else
                putc(ch,fPtrOut);
        fprintf(fPtrOut,"\", \"published\":\"");
        fseek(fPtr, Data[i].pub, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                putc(' ',fPtrOut);
            else
                putc(ch,fPtrOut);
        fprintf(fPtrOut,"\", \"updated\":\"");
        fseek(fPtr, Data[i].upd, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                putc(' ',fPtrOut);
            else
                putc(ch,fPtrOut);
        fprintf(fPtrOut,"\", \"title\":\"");
        fseek(fPtr, Data[i].tit, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                putc(' ',fPtrOut);
            else
                putc(ch,fPtrOut);
        fprintf(fPtrOut,"\", \"content\":\"");
        fseek(fPtr, Data[i].con, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                putc(' ',fPtrOut);
            else
                putc(ch,fPtrOut);
        fprintf(fPtrOut,"\", \"author\":\"");
        fseek(fPtr, Data[i].aut, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                putc(' ',fPtrOut);
            else
                putc(ch,fPtrOut);
        fprintf(fPtrOut,"\", \"keyword\":\"");
        fseek(fPtr, Data[i].key, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                putc(' ',fPtrOut);
            else
                putc(ch,fPtrOut);
        fprintf(fPtrOut,"\", \"favoriteCount\":\"");
        fseek(fPtr, Data[i].fav, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                putc(' ',fPtrOut);
            else
                putc(ch,fPtrOut);
        fprintf(fPtrOut,"\", \"viewCount\":\"");
        fseek(fPtr, Data[i].vie, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                putc(' ',fPtrOut);
            else
                putc(ch,fPtrOut);
        fprintf(fPtrOut,"\", \"duration\":\"");
        fseek(fPtr, Data[i].dur, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                putc(' ',fPtrOut);
            else
                putc(ch,fPtrOut);
        fprintf(fPtrOut,"\", \"category\":\"");
        fseek(fPtr, Data[i].cat, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                putc(' ',fPtrOut);
            else
                putc(ch,fPtrOut);
        
        fprintf(fPtrOut,"\"  }\n");
    }
    fprintf(fPtrOut,"	]\n}");

    fclose(fPtrOut); /* 關閉檔案 */
}
/* Output File End */

/* Display JSON Start */
void Display(struct recNode *Data, int total, char *fileName){
    
    char ch;
    int i;
    
    printf("{\n    \"records\" : [\n");
    printf("        { \"id\" :\"ID\" , \"published\" :\"Published\" , \"updated\" :\"Updated\" , \"title\" :\"Title\" , \"content\" :\"Content\" , \"author\" :\"Author\" , \"keyword\" :\"Keyword\" , \"favoriteCount\" :\"Favorit Count\", \"viewCount\" :\"View Count\", \"duration\" :\"Duration\", \"category\" :\"Category\", \"src\" :\"src\" }\n");
    for (i = (page-1)*10; i < total && i < page*10; i++) {
        printf("       ,{ ");
        printf(" \"id\":\"");
        fseek(fPtr, Data[i].id, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                printf(" ");
            else
                printf("%c", ch);
        printf("\", \"published\":\"");
        fseek(fPtr, Data[i].pub, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                printf(" ");
            else
                printf("%c", ch);
        printf("\", \"updated\":\"");
        fseek(fPtr, Data[i].upd, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                printf(" ");
            else
                printf("%c", ch);
        printf("\", \"title\":\"");
        fseek(fPtr, Data[i].tit, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                printf(" ");
            else
                printf("%c", ch);
        printf("\", \"content\":\"");
        fseek(fPtr, Data[i].con, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                printf(" ");
            else
                printf("%c", ch);
        printf("\", \"author\":\"");
        fseek(fPtr, Data[i].aut, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                printf(" ");
            else
                printf("%c", ch);
        printf("\", \"keyword\":\"");
        fseek(fPtr, Data[i].key, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                printf(" ");
            else
                printf("%c", ch);
        printf("\", \"favoriteCount\":\"");
        fseek(fPtr, Data[i].fav, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                printf(" ");
            else
                printf("%c", ch);
        printf("\", \"viewCount\":\"");
        fseek(fPtr, Data[i].vie, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                printf(" ");
            else
                printf("%c", ch);
        printf("\", \"duration\":\"");
        fseek(fPtr, Data[i].dur, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                printf(" ");
            else
                printf("%c", ch);
        printf("\", \"category\":\"");
        fseek(fPtr, Data[i].cat, SEEK_SET);
        while ( (ch = getc(fPtr)) != '@' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                printf(" ");
            else
                printf("%c", ch);
        printf("\", \"src\":\"");
        fseek(fPtr, Data[i].src, SEEK_SET);
        while ( (ch = getc(fPtr)) != '\n' )
            if (ch=='\n'||ch=='['||ch==']'||ch=='\"'||ch=='{'||ch=='}'||ch=='\r'||ch=='\\')
                printf(" ");
            else
                printf("%c", ch);
        
        
        printf("\"  }\n");
    }
    printf("	],\"recordsNum\":%d }\n", total);
    
}
/* Display JSON End */

/* return minimum Start */
int min(int a,int b,int c){
    if (a<b && a<c)
        return 1;
    else if(b<a && b<c)
        return 2;
    else if(c<a && c<b)
        return 3;
    else
        return 2;
}
/* return minimum End */

/* Approximate String Matching Start */
int ASM(char *text, char *pattern){
//    printf("%s",text);
//    printf("%d",strlen(text));
//    printf("%s\n",pattern);
    int minmum = 99, i, j;
    int array[strlen(text)+1][strlen(pattern)+1];
    int prev[strlen(text)+1][strlen(pattern)+1];
    /* 
        1 : <-
        2 : 左上
        3 : ^
    */
    
    for (i = 0; i <= strlen(text); i++){
        array[i][0]=0;
    }
    for (i = 0; i <= strlen(pattern); i++){
        array[0][i]=i;
    }
    
    for (i = 1; i <= strlen(text); i++)
        for (j = 1; j <= strlen(pattern); j++){
            prev[i][j]=min(array[i-1][j]+1, array[i-1][j-1] + (text[i-1] != pattern[j-1]), array[i][j-1]+1);
            if (prev[i][j]==1)
                array[i][j] = array[i-1][j]+1;
            else if(prev[i][j]==2)
                array[i][j] = array[i-1][j-1] + (text[i-1] != pattern[j-1]);
            else
                array[i][j] = array[i][j-1]+1;
        }
/*  table display  */
//    for (i = 0; i <= strlen(pattern); i++){
//        for (j = 0; j <= strlen(text); j++)
//            printf("%d ",array[j][i]);
//        printf("\n");
//    }
//    printf("\n");
//    for (i = 1; i <= strlen(pattern); i++){
//        for (j = 1; j <= strlen(text); j++)
//            printf("%d ",prev[j][i]);
//        printf("\n");
//    }
    for (i = 1; i <= strlen(text); i++){
        if (array[i][strlen(pattern)]<minmum) {
            minmum = array[i][strlen(pattern)];
        }
    }
    return minmum;
}
/* Approximate String Matching End */
