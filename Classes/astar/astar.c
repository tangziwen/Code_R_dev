#include "astar.h"
#include <stdlib.h>
#include <assert.h>

#include <stdio.h>
static GScoreFunc gscore;
static HScoreFunc hscore;
static GetAdjFunc get_adjs;

typedef struct AstarNode AstarNode;
struct AstarNode {
    int index;
    int g, h;
    AstarNode *parent;
};

typedef struct Set {
    int len, cap;
    AstarNode **arr;
} Set;

static Set *open_set, *close_set;

static AstarNode* create_node(void *ud, int index, int dst, AstarNode *parent) {
    AstarNode *node = (AstarNode *)malloc(sizeof(*node));
    node->index = index;
    node->parent = parent;
    node->g = gscore(ud, parent->index, index) + parent->g;
    node->h = hscore(ud, index, dst);
    return node;
}

static Set* create_set() {
    Set *set = (Set *)calloc(1, sizeof(*set));
    return set;
}

static void set_release(Set **pset) {
    Set *set = *pset;
    int i;
    for (i = 0; i < set->len; i++) {
        free(set->arr[i]);
    }
    free(set->arr);
    free(set);
    *pset = NULL;
}

static int set_find(Set *set, int index) {
    int i;
    for (i = 0; i < set->len; i++) {
        AstarNode *node = set->arr[i];
        if (node->index == index)
            return i;
    }
    return -1;
}

static AstarNode* set_index(Set *set, int index) {
    if (index >= set->len)
        return NULL;
    return set->arr[index];
}

static AstarNode* set_pop(Set *set) {
    if (set->len == 0)
        return NULL;
    return set->arr[--set->len];
}

static void set_insert(Set *set, AstarNode *node) {
    if (set->len == set->cap) {
        set->cap = (set->cap + 1) * 2;
        set->arr = (AstarNode **)realloc(set->arr, set->cap * sizeof(AstarNode*));
    }

    // the least f will at the last of the array
    int f = node->g + node->h;
    int i = set->len;
    while (i > 0) {
        AstarNode *p = set->arr[--i];
        if (p->g + p->h > f) {
            ++i;
            break;
        }
        set->arr[i+1] = set->arr[i];
    }
    set->arr[i] = node;
    set->len++;
}

static void set_push(Set *set, AstarNode *node) {
    if (set->len == set->cap) {
        set->cap = (set->cap + 1) * 2;
        set->arr = (AstarNode **)realloc(set->arr, set->cap * sizeof(AstarNode*));
    }
    set->arr[set->len++] = node;
}

static void set_bubble(Set *set, int i) {
    AstarNode *node = set->arr[i];
    int f = node->g + node->h;
    while (++i < set->len) {
        AstarNode *p = set->arr[i];
        if (p->g + p->h < f)
            break;
        set->arr[i-1] = set->arr[i];
    }
    set->arr[i-1] = node;
}

AstarArray* create_array(int cap) {
    AstarArray *arr = (AstarArray *)malloc(sizeof(*arr));
    arr->arr = (int *)malloc(sizeof(int) * cap);
    arr->cap = cap;
    arr->len = 0;
    return arr;
}

void array_release(AstarArray **parr) {
    AstarArray *arr = *parr;
    free(arr->arr);
    free(arr);
    *parr = NULL;
}

void array_pushback(AstarArray *arr, int index) {
    if (arr->len == arr->cap) {
        arr->cap *= 2;
        arr->arr = (int *)realloc(arr->arr, sizeof(int) * arr->cap);
    }
    arr->arr[arr->len++] = index;
}

static AstarArray* path_backtrace(AstarNode *node, int dst) {
    AstarArray *arr = create_array(1);
    array_pushback(arr, dst);
    while (node) {
        array_pushback(arr, node->index);
        node = node->parent;
    }
    return arr;
}

static AstarArray* search(AstarNode *current, int dst, Set *open_set, Set *close_set, void *ud) {
    AstarArray *adjs = NULL;
    if (!current)
        return NULL;

    adjs = get_adjs(ud, current->index);
    int j;
    for (j = 0; j < adjs->len; j++) {
        int i = adjs->arr[j];

        if (i == dst) {
            return path_backtrace(current, dst);
        }

        if (set_find(close_set, i) != -1)
            continue;
        int new_g = gscore(ud, current->index, i) + current->g;

        int index;
        if ((index = set_find(open_set, i)) != -1) {
            AstarNode *node = set_index(open_set, index);
            if (node->g < new_g) {
                continue;
            }
            node->g = new_g;
            node->parent = current;
            set_bubble(open_set, index);
        } else {
            AstarNode *node = create_node(ud, i, dst, current);
            set_insert(open_set, node);
        }
    }
    array_release(&adjs);
    int x = current->index % 30;
    int y = current->index / 30;
    printf("current is %d %d\n",x,y );
    fflush(stdout);
    set_push(close_set, current);
    AstarNode *next = set_pop(open_set);
    return search(next, dst, open_set, close_set, ud);
}

typedef struct AStarInfo {
    Set *open_set, *close_set;
} AStarInfo;

static AStarInfo* create_info() {
    AStarInfo *info = (AStarInfo *)malloc(sizeof(*info));
    info->open_set = create_set();
    info->close_set = create_set();
    return info;
}

static void info_release(AStarInfo **pinfo) {
    AStarInfo *info = *pinfo;
    set_release(&info->open_set);
    set_release(&info->close_set);
    free(info);
    *pinfo = NULL;
}

AstarArray* astar_search(void *ud, int src, int dst) {
    AStarInfo *info = create_info(ud);
    AstarNode *start = (AstarNode *)calloc(1, sizeof(*start));
    start->index = src;
    start->g = 0;
    AstarArray *path = search(start, dst, info->open_set, info->close_set, ud);
    info_release(&info);
    return path;
}

void astar_init(GScoreFunc g, HScoreFunc h, GetAdjFunc n) {
    assert(g != NULL && h != NULL && n != NULL);
    gscore = g;
    hscore = h;
    get_adjs = n;
}
