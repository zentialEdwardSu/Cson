#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#ifndef __SONDS_H
#include"SONDS.h"
#endif


/*LastEdit 1:52 2022-5-3*/
#ifndef __LIST_H
#define __LIST_H

#define EX_RANGE 3
#define double_size sizeof(double)
#define int_size sizeof(int)
#define str_size sizeof(Str)
typedef enum{
    type_NULL = 0,
    type_int,
    type_double,
    type_Str,
    type_jobj
}Type_data;

typedef struct{
    int length;
    int currentLength;
    Type_data type;
    int type_size;
    void *content_Hook;
}List;


List List_newList(int initSize,Type_data type);
List *List_initList(int initSize,Type_data type);
void List_dropList(List *L);
void List_ClearList(List *L);
void List_append(List *L,void *appendData);
void *List_pop(List *L,int popos);
void *List_divide_Arr(List *L,int low,int high);
List *List_divide_List(List *L,int low,int high);
void *List_insert(List *L,void *insertdata,int pos);

void List_consoleLog(List *L);
bool List_isFull(List *L);

void *List_loadInt(int *data);
void *List_loadDouble(double *data);
void *List_loadStr(SONDS *data);
int *List_contentDumpInt(List *L);
double *List_contentDumpDouble(List *L);
Str *List_contentDumpStr(List *L);

List List_newList(int initSize,Type_data type){
    List newList;
    newList.length = initSize+EX_RANGE;
    newList.currentLength = 0;
    newList.type = type;
    switch(type){
        case type_int:  newList.content_Hook = malloc(int_size*newList.length);   newList.type_size = int_size;   memset(newList.content_Hook,0,int_size*newList.length);break;
        case type_double:   newList.content_Hook = malloc(double_size*newList.length);   newList.type_size = double_size;   memset(newList.content_Hook,0,double_size*newList.length);   break;
        case type_Str:  newList.content_Hook = malloc(str_size*newList.length);   newList.type_size = str_size;   memset(newList.content_Hook,0,double_size*newList.length);    break;
        default: printf("\n[ERROR] Undentified data Type\n"); newList.content_Hook = NULL;   newList.type_size = 0; break;
    }
    if(!newList.content_Hook){printf("[WARN] Get Space failed");}

    return newList;
}

List *List_initList(int initSize,Type_data type){
    List *newList = (List *)malloc(sizeof(List));
    newList->length = initSize+EX_RANGE;
    newList->currentLength = 0;
    newList->type = type;
    switch(type){
        case type_int:  newList->content_Hook = malloc(int_size*newList->length);   newList->type_size = int_size;   ;break;
        case type_double:   newList->content_Hook = malloc(double_size*newList->length);   newList->type_size = double_size;   break;
        case type_Str:  newList->content_Hook = malloc(str_size*newList->length);   newList->type_size = str_size;   break;
        default: printf("\n[ERROR] Undentified data Type\n"); newList->content_Hook = NULL;   newList->type_size = 0; break;
    }
    if(!newList->content_Hook){printf("[WARN] Get Space failed");}

    return newList;
}

void List_consoleLog(List *L){
    printf("Length:%d\nCLength:%d\nType:%d\nTypesize:%d\n\r",L->length,L->currentLength,L->type,L->type_size);
}

void List_dropList(List *L){
    free(L->content_Hook);
    L->content_Hook = NULL;
    L->length = 0;
    L->currentLength = 0;
    free(L);
    L = NULL;
}

void List_ClearList(List *L){
    L->currentLength = 0;
}

/*Append new item to current List L
    No return*/
void List_append(List *L,void *appendData){
    if(!L){     printf("[ERROR] NULL List ptr");    return;     }
    if(!appendData){    printf("[ERROR] NULL appendData ptr");  return;     }
    if(List_isFull(L)){     L->content_Hook = realloc(L->content_Hook,L->type_size*(L->length+EX_RANGE));   L->length+=EX_RANGE;    }

    unsigned long long posHook = (long long)L->content_Hook;

    // printf("Hook pos:%ld\n",posHook);
    // printf("Append pos:%ld\n",posHook+(L->currentLength*L->type_size));
    // printf("Append data pos:%ld\n",(long long)appendData);
    memcpy((void *)(posHook+(L->currentLength*L->type_size)),appendData,L->type_size);
    L->currentLength++;
}

/*Pop List[popos] and move forward
    retrun>>poped out data*/
void *List_pop(List *L,int popos){
    if(popos<0){    popos = L->currentLength+popos;     }
    if(popos>L->currentLength-1){
        printf("[WARN] Invaild pop position\n");
        return NULL;
    }
    int moveNum = L->currentLength-popos-1;
    char *contentCursor = L->content_Hook;
    contentCursor += popos*L->type_size;
    void *res = malloc(L->type_size);
    memcpy(res,contentCursor,L->type_size);
    // printf("Here\n");
    L->currentLength-=1;
    memmove(contentCursor,contentCursor+L->type_size,moveNum*L->type_size);

    return res;
}

/*Get silce by low,high 
    retrun void[] for silce
    
    ps: [2,3,2,3] divide(1,3)=> 3,2,3*/
void *List_divide_Arr(List *L,int low,int high){
    if(low<0)low = L->currentLength+low;
    if(high)high = L->currentLength+high;
    if(high>L->currentLength-1){
        printf("[WARN] Invaild pos");
        return NULL;
    }
    if(low>high){
        printf("[WARN] Invaild range");
        return NULL;
    }
    int range = high-low+1;
    void *res = malloc(L->type_size*range);

    char *contentCursor = L->content_Hook;
    contentCursor+=(low)*L->type_size;

    // printf("res pos:%ld\n",(long long)res);
    // printf("divde length:%d\n",range);
    // printf("Hookpos:%ld\n",(long long)L->content_Hook);
    // printf("divide start pos:%ld\n",(long long)contentCursor);
    memcpy(res,contentCursor,range*L->type_size);

    return res;
}

/*Get silce by low,high 
    retrun List for silce
    
    ps: [2,3,2,3] divide(1,3)=> 3,2,3*/
List *List_divide_List(List *L,int low,int high){
    if(low<0)low = L->currentLength+low;
    if(high<0)high = L->currentLength+high;
    if(high>L->currentLength-1){
        printf("[WARN] Invaild divide pos");
        return NULL;
    }
    if(low>high){
        printf("[WARN] Invaild divide range");
        return NULL;
    }
    int range = high-low+1;
    void *resHook = malloc(L->type_size*range);

    List *res = List_initList(range,L->type);

    char *contentCursor = L->content_Hook;
    contentCursor+=(low)*L->type_size;

    memcpy(res->content_Hook,contentCursor,range*L->type_size);

    return res;
}

//TODO:List_insert
void *List_insert(List *L,void *insertdata,int pos){
    if(pos<0){      pos = L->length+pos;    }
    printf("Insert pos:%d\n",pos);
    if(pos > L->currentLength-1){
        printf("[WARN] Invaild insert position\n");
        return NULL;
    }
    if(List_isFull(L)){     
        L->content_Hook = realloc(L->content_Hook,L->type_size*(L->length+EX_RANGE));  
        L->length+=EX_RANGE;
        printf("[MSG] Space full malloc new space\n");    
    }
    int moveNum = L->currentLength - pos;
    char *moveCursor = L->content_Hook;
    moveCursor+= pos*L->type_size;//move to back 1 of insert pos
    memmove(moveCursor+L->type_size,moveCursor,moveNum*L->type_size);

    memcpy(moveCursor,insertdata,L->type_size);

    L->currentLength+=1;

    return L;

}

void *List_loadInt(int *data){
    int *tmp=data;
    return (void *)tmp;
}

void *List_loadDouble(double *data){
    double *tmp=data;
    return (void *)tmp;
}

void *List_loadStr(SONDS *data){
    return (void *)data;
}

int *List_contentDumpInt(List *L){
    unsigned long long tmp = (unsigned long long)L->content_Hook;
    return (int *)tmp;
}

double *List_contentDumpDouble(List *L){
    unsigned long long tmp = (unsigned long long)L->content_Hook;
    return (double *)tmp;
}

Str *List_contentDumpStr(List *L){
    unsigned long long tmp = (unsigned long long)L->content_Hook;
    return (Str *)tmp;
}

/*L->currentLength here are regard as the length after operation*/
bool List_isFull(List *L){
    return L->currentLength >= L->length?true:false;
}
#endif