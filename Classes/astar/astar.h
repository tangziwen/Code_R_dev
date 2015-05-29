#ifndef ASTAR_H
#define ASTAR_H

// f = g + h

// get g score
typedef int(*GScoreFunc)(void *ud, int indexFrom, int indexEnd);
// get h score
typedef int (*HScoreFunc)(void *ud, int src, int dst);

typedef struct AstarArray {
    int len;
    int cap;
    int *arr;
} AstarArray;
AstarArray* create_array(int cap);
void array_release(AstarArray **parr);
void array_pushback(AstarArray *arr, int index);

// get next adjacency index
typedef AstarArray* (*GetAdjFunc)(void *ud, int pos);

void astar_init(GScoreFunc g, HScoreFunc h, GetAdjFunc n);

// return an array of index as path, or null if path not found
AstarArray* astar_search(void *ud, int src, int dst);

#endif
