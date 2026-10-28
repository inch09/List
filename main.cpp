#include <stdio.h>
#include <string.h>
#include <assert.h> 
#include <malloc.h>

//----------------------------------------------------------------------------
struct List{
    size_t fullSizeOfList;
    size_t sizeOfList;
    int* data;
    int* next;
    size_t head;
    size_t tail;
};



void listInit(List* list);
void listDestroy(List* list);
void listPush(List* list, int pushElem, size_t pushPosition);
int findEmptyCell(List* list);
size_t findElemByPosition(List* list, size_t position);
void listDump(List* list);
//----------------------------------------------------------------------------
int main(){

    List list = {};
    listInit(&list);
    listPush(&list, 23, 1);
    listPush(&list, 25, 2);
    listDump(&list);
    listDestroy(&list);

    return 0;
}
//----------------------------------------------------------------------------
void listInit(List* list){
    assert(list);

    list->fullSizeOfList = 16;
    list->sizeOfList = 0;

    list->data = (int*) calloc(list->fullSizeOfList + 1, sizeof(list->data[0]));
    assert(list->data);

    list->next = (int*) calloc(list->fullSizeOfList + 1, sizeof(list->next[0]));
    assert(list->next);

    list->next[0] = 0;
    for(size_t i = 1; i < list->fullSizeOfList; i++){
        list->next[i] = -1;
    }

    list->head = 1;
    list->tail = 1;
}
//----------------------------------------------------------------------------
void listDestroy(List* list){
    assert(list);

    free(list->data);
    list->data = NULL;

    free(list->next);
    list->next = NULL;

    list->head = 0;
    list->tail = 0;

    list = NULL;
}
//----------------------------------------------------------------------------
void listPush(List* list, int pushElem, size_t pushPosition){
    assert(list);
    assert((pushPosition >= 1) && (pushPosition <= (list->sizeOfList + 1)));

    int numOfEmptyCell = findEmptyCell(list);
    //printf("%d", numOfEmptyCell);
    if(numOfEmptyCell < 0){
        assert(strcmp("PUSH", "ERROR") == 0);
    }
    else{
        if(pushPosition == 1){
            list->data[numOfEmptyCell] = pushElem;
            list->next[numOfEmptyCell] = list->tail;
            list->tail = numOfEmptyCell;
            list->sizeOfList++;
            return;
        }
        if(pushPosition == (list->sizeOfList + 1)){
            list->data[numOfEmptyCell] = pushElem;
            list->next[list->head] = numOfEmptyCell;
            list->next[numOfEmptyCell] = 0;
            list->head = numOfEmptyCell;
            list->sizeOfList++;
            return;
        }

        int indOfPreviousPos = findElemByPosition(list, pushPosition - 1);
        list->data[numOfEmptyCell] = pushElem;
        list->next[numOfEmptyCell] = list->next[indOfPreviousPos];
        list->next[indOfPreviousPos] = numOfEmptyCell; 
        list->sizeOfList++;
        return;
    }
}
//----------------------------------------------------------------------------
int findEmptyCell(List* list){ //return index in array next
    assert(list);

    for(size_t i = 0; i < list->fullSizeOfList; i++){
        if(list->next[i] < 0){
            return i;
        }
    }

    return -1;
}
//----------------------------------------------------------------------------
size_t findElemByPosition(List* list, size_t position){
    assert(list);
    assert((position > 0) && (position <= list->sizeOfList));
    
    if(position == 1){
        return list->tail;
    }
    if(position == list->sizeOfList){
        return list->head;
    }
    
    size_t nextInd = list->next[list->tail];
    for(size_t positionNow = 1; positionNow < position - 1; positionNow++){
        nextInd = list->next[nextInd];
    }

    size_t indInNext = nextInd;
    return indInNext;
}
//----------------------------------------------------------------------------
void listDump(List* list){
    assert(list);

    for(size_t i = 0; i < list->fullSizeOfList; i++){
        printf("%2d ", list->data[i]);
    }
    printf("\n");

    for(size_t i = 0; i < list->fullSizeOfList; i++){
        printf("%2d ", list->next[i]);
    }
}