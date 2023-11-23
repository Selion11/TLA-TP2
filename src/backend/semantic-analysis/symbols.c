#include "symbols.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define MAX_SIZE 100

// Global variables
char *list[MAX_SIZE];
int size = 0;

// Function to add an element to the list if it is not repeated
void addToList(char *element) {
    if (!searchInList(element)) {
        if (size < MAX_SIZE) {
            list[size] = malloc(strlen(element) + 1);
            strcpy(list[size], element);
            size++;
            printf("Element '%s' added to the list.\n", element);
        } else {
            printf("The list is full. Cannot add more elements.\n");
        }
    } else {
        printf("The element '%s' is already in the list. It will not be added.\n", element);
    }
}

// Function to search for an element in the list
int searchInList(char *element) {
    for (int i = 0; i < size; i++) {
        if (strcmp(list[i], element) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to free the memory allocated for the list
void freeList() {
    for (int i = 0; i < size; i++) {
        free(list[i]);
    }
}