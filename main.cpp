#include <stdio.h>
#include <string.h>
#include <assert.h> 
#include <malloc.h>

//----------------------------------------------------------------------------
#define POISON 67
typedef int list_elem_type;
#define SPECIFIER "%2d"
//----------------------------------------------------------------------------
struct List{
    size_t fullSizeOfList;
    size_t sizeOfList;
    list_elem_type* data;
    int* next;
    int* prev;
    size_t head;
    size_t tail;
};
//----------------------------------------------------------------------------
void listInit(List* list);
void listDestroy(List* list);
//----------------------------------------------------------------------------
void listPush(List* list, list_elem_type pushElem, size_t pushPosition);
void listPushInTail(List* list, list_elem_type pushElem);
void listPushInHead(List* list, list_elem_type pushElem);
void listPushInsideList(List* list, list_elem_type pushElem, size_t pushPosition);
//----------------------------------------------------------------------------
void listPop(List* list, size_t popPosition);
void listPopTail(List* list);
void listPopHead(List* list);
void listPopInternalElem(List* list, size_t popPosition);
//----------------------------------------------------------------------------
int findEmptyCell(List* list);
size_t findElemByPosition(List* list, size_t position);
//----------------------------------------------------------------------------
void listDump(List* list);
void listViz(List* list, const char* nameTextFile);
void printDataArr(List* list);
void printNextArr(List* list);
void printPrevArr(List* list);
void printListParameters(List* list);
void printList(List* list);
void printIndexes(List* list);
//----------------------------------------------------------------------------
int main(){

    List list = {};
    listInit(&list);
    for(int i = 1; i < 10; i++){
        listPush(&list, i, i);
    }
    listPush(&list, 52, 6);
    listPush(&list, 23, 1);
    listPush(&list, 11, 2);
    listPush(&list, 24, 3);
    listPush(&list, 17, 4);
    listPush(&list, 13, 3);
    listPop(&list, 1);
    listPop(&list, 4);
    listPop(&list, 13);
    //listPop(&list, 2);
    //listPop(&list, 1);
    //listPop(&list, 1);
    //listPop(&list, 1);

    //listPush(&list, 25, 2);
    listDump(&list);
    listViz(&list, "dump.txt");
    listDestroy(&list);

    return 0;
}
//----------------------------------------------------------------------------
void listInit(List* list){
    assert(list);

    list->fullSizeOfList = 16;
    list->sizeOfList = 0;

    list->data = (list_elem_type*) calloc(list->fullSizeOfList + 1, sizeof(list->data[0]));
    assert(list->data);

    list->next = (list_elem_type*) calloc(list->fullSizeOfList + 1, sizeof(list->next[0]));
    assert(list->next);

    list->prev = (list_elem_type*) calloc(list->fullSizeOfList + 1, sizeof(list->prev[0]));
    assert(list->prev);

    for(size_t i = 0; i < list->fullSizeOfList + 1; i++){
        list->data[i] = POISON;
    }

    list->next[0] = 0;
    for(size_t i = 1; i < list->fullSizeOfList + 1; i++){
        list->next[i] = -1;
    }

    list->prev[0] = 0;
    for(size_t i = 1; i < list->fullSizeOfList + 1; i++){
        list->prev[i] = -1;
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
void listPush(List* list, list_elem_type pushElem, size_t pushPosition){
    assert(list);
    assert((pushPosition >= 1) && (pushPosition <= (list->sizeOfList + 1)));

    int numOfEmptyCell = findEmptyCell(list);
    //printf("%d", numOfEmptyCell);
    if(numOfEmptyCell < 0){
        assert(strcmp("PUSH", "ERROR") == 0);
    }
    else{
        if(pushPosition == 1){
            listPushInTail(list, pushElem);
            return;
        }
        if(pushPosition == (list->sizeOfList + 1)){
            listPushInHead(list, pushElem);
            return;
        }

        listPushInsideList(list, pushElem, pushPosition);
    }
}
//----------------------------------------------------------------------------
void listPushInTail(List* list, list_elem_type pushElem){
    assert(list);

    int numOfEmptyCell = findEmptyCell(list);
    
    if(list->sizeOfList == 0){
        list->data[numOfEmptyCell] = pushElem;
        list->next[numOfEmptyCell] = 0;
        list->prev[numOfEmptyCell] = 0;
        list->tail = numOfEmptyCell;
        list->head = numOfEmptyCell;
    }
    else{
        list->data[numOfEmptyCell] = pushElem;
        list->next[numOfEmptyCell] = list->tail;
        list->prev[numOfEmptyCell] = 0;
        list->prev[list->tail] = numOfEmptyCell;
        list->tail = numOfEmptyCell;
    }
    list->sizeOfList++;
    return;
}
//----------------------------------------------------------------------------
void listPushInHead(List* list, list_elem_type pushElem){
    assert(list);
    
    int numOfEmptyCell = findEmptyCell(list);

    list->data[numOfEmptyCell] = pushElem;
    list->next[list->head] = numOfEmptyCell;
    list->prev[numOfEmptyCell] = list->head;
    list->next[numOfEmptyCell] = 0;
    list->head = numOfEmptyCell;
    list->sizeOfList++;
    return;
}
//----------------------------------------------------------------------------
void listPushInsideList(List* list, list_elem_type pushElem, size_t pushPosition){
    assert(list);
    
    int numOfEmptyCell = findEmptyCell(list);
    int indOfPreviousPos = findElemByPosition(list, pushPosition - 1);

    list->data[numOfEmptyCell] = pushElem;//
    list->next[numOfEmptyCell] = list->next[indOfPreviousPos];//
    list->prev[numOfEmptyCell] = indOfPreviousPos;//
    list->prev[list->next[indOfPreviousPos]] = numOfEmptyCell;//
    list->next[indOfPreviousPos] = numOfEmptyCell;//
    list->sizeOfList++;
    return;
}
//----------------------------------------------------------------------------
void listPop(List* list, size_t popPosition){
    assert(list);
    assert(list->sizeOfList > 0);
    assert((popPosition >= 1) && (popPosition <= list->sizeOfList));

    int indOfPopElem = findElemByPosition(list, popPosition);
    printf("ind of pop = %d\n", indOfPopElem);

    if(popPosition == 1){
        listPopTail(list);
        return;
    }
    if(popPosition == list->sizeOfList){
        listPopHead(list);
        return;
    }

    listPopInternalElem(list, popPosition);
    return;
}
//----------------------------------------------------------------------------
void listPopTail(List* list){
    assert(list);
    printf("6767676");

    if(list->sizeOfList == 1){
        list->data[list->tail] = POISON;
        list->next[list->tail] = -1;
        list->prev[list->tail] = -1;
        list->tail = 1;
        list->head = 1;
    }
    else{
        list->data[list->tail] = POISON;
        list->prev[list->next[list->tail]] = 0;
        list->tail = list->next[list->tail];
        list->next[list->prev[list->tail]] = -1;
        list->prev[list->prev[list->tail]] = -1;
    }
    list->sizeOfList--;

    return;
}
//----------------------------------------------------------------------------
void listPopHead(List* list){
    assert(list);

    list->data[list->head] = POISON;
    list->next[list->prev[list->head]] = 0;

    size_t temp = list->head;
    list->head = list->prev[list->head];
    list->prev[temp] = -1;
    list->next[temp] = -1;
    list->sizeOfList--;
    return;

}
//----------------------------------------------------------------------------
void listPopInternalElem(List* list, size_t popPosition){
    assert(list);

    int indOfPopElem = findElemByPosition(list, popPosition);

    list->data[indOfPopElem] = POISON;
    list->next[list->prev[indOfPopElem]] = list->next[indOfPopElem];
    list->prev[list->next[indOfPopElem]] = list->prev[indOfPopElem];
    list->sizeOfList--;

    return;
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

    printDataArr(list);
    printNextArr(list);
    printPrevArr(list);
    printListParameters(list);
    printList(list);
    printIndexes(list);

}
//----------------------------------------------------------------------------
void printDataArr(List* list){
    assert(list);

    printf("                            Data arr\n");
    printf("%2d| ", list->data[0]);
    for(size_t i = 1; i < list->fullSizeOfList + 1; i++){
        printf(SPECIFIER" ", list->data[i]);
    }
    printf("\n");
    printf("----------------------------------------------------------------------------\n");
}
//----------------------------------------------------------------------------
void printNextArr(List* list){
    assert(list);
    
    printf("                            Next arr\n");
    printf("%2d| ", list->next[0]);
    for(size_t i = 1; i < list->fullSizeOfList + 1; i++){
        printf("%2d ", list->next[i]);
    }
    printf("\n");
    printf("----------------------------------------------------------------------------\n");
}
//----------------------------------------------------------------------------
void printPrevArr(List* list){
    assert(list);
    
    printf("                            Prev arr\n");
    printf("%2d| ", list->prev[0]);
    for(size_t i = 1; i < list->fullSizeOfList + 1; i++){
        printf("%2d ", list->prev[i]);
    }
    printf("\n");
    printf("----------------------------------------------------------------------------\n");
}
//----------------------------------------------------------------------------
void printListParameters(List* list){
    assert(list);
    
    printf("tail = %lu\n",      (long unsigned) list->tail);
    printf("head = %lu\n",      (long unsigned) list->head);
    printf("size = %lu\n",      (long unsigned) list->sizeOfList);
    printf("full size = %lu\n", (long unsigned) list->fullSizeOfList);
    printf("----------------------------------------------------------------------------\n");

}
//----------------------------------------------------------------------------
void printList(List* list){
    assert(list);

    printf("                            LIST\n");

    size_t ind = list->tail;
    for(size_t i = 0; i < list->fullSizeOfList; i++){
        printf(SPECIFIER" ", list->data[ind]);
        if(ind == list->head){
            break;
        }
        else{
            ind = list->next[ind];
        }
    }
    printf("\n");
}
//----------------------------------------------------------------------------
void printIndexes(List* list){
    assert(list);

    printf("                            INDEXES\n");

    size_t ind = list->tail;
    for(size_t i = 0; i < list->fullSizeOfList; i++){
        printf("%2d ", ind);
        if(ind == list->head){
            break;
        }
        else{
            ind = list->next[ind];
        }
    }
    printf("\n");
}
//----------------------------------------------------------------------------
void listVerify(List* list){
    assert(list);
    if(list == NULL){
        assert(strcmp("LIST NULL", "POINTER") == 0);
    }
    if(list->data == NULL){
        assert(strcmp("LIST DATA NULL", "POINTER") == 0);
    }
    if(list->next == NULL){
        assert(strcmp("LIST NEXT NULL", "POINTER") == 0);
    }
    if(list->prev == NULL){
        assert(strcmp("LIST PREV NULL", "POINTER") == 0);
    }
    if((list->head <= 0) || (list->head >= list->fullSizeOfList)){
        assert(strcmp("HEAD IS NOT", "IN THE ARRAY") == 0);
    }
    if((list->tail <= 0) || (list->tail >= list->fullSizeOfList)){
        assert(strcmp("LIST IS NOT", "IN THE ARRAY") == 0);
    }
    if(list->sizeOfList > list->fullSizeOfList){
        assert(strcmp("LIST OVER", "FLOW") == 0);
    }
}
//----------------------------------------------------------------------------
void expandList(List* list){
    assert(list);

}
// struct List{
//     size_t fullSizeOfList;
//     size_t sizeOfList;
//     int* data;
//     int* next;
//     int* prev;
//     size_t head;
//     size_t tail;
// };
void listViz(List* list, const char* nameTextFile){
    assert(list);
    assert(nameTextFile);

    FILE* fileptr = fopen(nameTextFile, "w");
    assert(fileptr);

    fprintf(fileptr, "digraph{\n");
    fprintf(fileptr, "rankdir = LR\n");
    fprintf(fileptr, "  node[color = \"black\", fontsize = 13];\n");
    fprintf(fileptr, "  edge[color = \"white\", fontcolor = \"black\", fontsize = 10];\n");

    for(size_t i = 0; i < list->fullSizeOfList + 1; i++){
        if(i == list->tail){
            fprintf(fileptr, "  cell%lu[shape = \"record\", label = \"{value = "SPECIFIER" |", (unsigned long) i, list->data[i]);
            fprintf(fileptr, "  next = %lu | prev = %lu}\", style = \"filled\", fillcolor = \"lightgreen\"];\n", (unsigned long) list->next[i], (unsigned long) list->prev[i]);
        }
        if(i == list->head){
            fprintf(fileptr, "  cell%lu[shape = \"record\", label = \"{value = "SPECIFIER" |", (unsigned long) i, list->data[i]);
            fprintf(fileptr, "  next = %lu | prev = %lu}\", style = \"filled\", fillcolor = \"pink2\"];\n", (unsigned long) list->next[i], (unsigned long) list->prev[i]);
        }
        else{
            fprintf(fileptr, "  cell%lu[shape = \"record\", label = \"{value = "SPECIFIER" |", (unsigned long) i, list->data[i]);
            fprintf(fileptr, "  next = %lu | prev = %lu}\"];\n", (unsigned long) list->next[i], (unsigned long) list->prev[i]);
        }
    }

    for(size_t i = 0; i < list->fullSizeOfList; i++){
        fprintf(fileptr, "  cell%d->cell%d [weight = 10];\n", i, i + 1);
    }

    fprintf(fileptr, "  edge[color = \"lightgreen\", fontcolor = \"black\", fontsize = 10];\n");
    size_t ind = list->tail;
    printf("tail = %d", ind);
    for(size_t i = 0; i < list->fullSizeOfList + 1; i++){
        fprintf(fileptr, "  cell%d->cell%d [weight = 1];\n", ind, list->next[ind]);
        if(ind == list->head){
            break;
        }
        ind = list->next[ind];
    }





    fprintf(fileptr, "}");
    fclose(fileptr);
}