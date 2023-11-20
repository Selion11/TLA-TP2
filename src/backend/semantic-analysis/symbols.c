#include "stdlib.h"
#include "stdio.h"
#include "string.h"

typedef struct symbol{
    char * name;
    symbol * next;

} symbol;

symbol * listStart = NULL;
symbol * last = NULL;

/* 1 no match found
 0 match found*/
int verify(char * check){
    if(listStart == NULL){
        return 1;
    }
    symbol * node = listStart;
    while(node != NULL){
        if(strcmp(check,node->name)){
            return 0;
        }
        node = node->next;
    }
    return 1;

}

int addToList(char * name){
    if(verify(name) == 0){
        symbol * newNode = calloc(1,sizeof(symbol));
        newNode->name = malloc(sizeof(char) * (strlen(name) + 1));
        strcpy(newNode->name, name);
        if(listStart == NULL){
            listStart = newNode;
            last = listStart;
        }
        last->next = newNode;
        last = last->next;
        return 0;
    }
    return 1;
}