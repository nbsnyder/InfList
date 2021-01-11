
#ifndef inflist_C_H
#define inflist_C_H

#include <stdlib.h>

#ifndef _inflist_type
    #define _inflist_type int
#endif



/* Type definitions */
typedef _inflist_type(*_il_func)(_inflist_type);

typedef _inflist_type(*_il_binop)(_inflist_type, _inflist_type);

typedef struct {
    int first;
    int second;
} _il_pair;

typedef struct {
    union {
        _il_pair *pair; /* -1 */
        _inflist_type *num; /* 0 */
        _il_func *func; /* 1 & 2 */
        _il_binop *binop; /* 3 */
    };
    int currSize;
    int maxSize;
} _il_vector;

typedef struct {
    _inflist_type start; /* start of the list (default: 0) */
    _inflist_type step; /* step of the list (default: 1) */
    _il_vector evalList; /* stores pairs of integers that correspond to entries in the next four vectors: */
    _il_vector nums; /* stores numbers (0) */
    _il_vector funcs; /* stores functions (1) and unary operators (2) */
    _il_vector binops; /* stores binary operators (3) */
} inflist;



/* Functions to initiate the inflist */
void inflist_init_from_num(inflist* list, _inflist_type num);
void inflist_init_from_func(inflist* list, _il_func obj);
void inflist_init_from_list(inflist* list, inflist* otherList);
void _il_init(inflist* list);

/* Functions to add things to individual _il_vectors */
void _il_addToEvalList(inflist *list, _il_pair a);
void _il_addToNums(inflist *list, _inflist_type a);
void _il_addToFuncs(inflist *list, _il_func a);
void _il_addToBinops(inflist *list, _il_binop a);
void _il_resizeVector(_il_vector *vector, int typeVal);

/* Functions to add things to the inflist */
void _il_addOp_num(inflist* list, _inflist_type num);
void _il_addOp_func(inflist* list, _il_func obj);
void _il_addOp_list(inflist* list, inflist* otherList);
void inflist_push_unop(inflist* list, _il_func obj);
void inflist_push_binop(inflist* list, _il_binop obj);

_inflist_type at(inflist *list, int x);



void inflist_init_from_num(inflist* list, _inflist_type num) {
    if (list == NULL) return;
    _il_init(list);
    _il_addOp_num(list, num);
}

void inflist_init_from_func(inflist* list, _il_func obj) {
    if ((list == NULL) || (obj == NULL)) return;
    _il_init(list);
    _il_addOp_func(list, obj);
}

void inflist_init_from_list(inflist* list, inflist* otherList) {
    if ((list == NULL) || (otherList == NULL)) return;
    _il_init(list);
    _il_addOp_list(list, otherList);
}

void _il_init(inflist* list) {
    if (list == NULL) return;

    list->evalList.currSize = 0;
    list->evalList.maxSize = 8;
    list->evalList.pair = (_il_pair*) malloc(list->evalList.maxSize * sizeof(_il_pair));

    list->nums.currSize = 0;
    list->nums.maxSize = 8;
    list->nums.num = (_inflist_type*) malloc(list->nums.maxSize * sizeof(_inflist_type));

    list->funcs.currSize = 0;
    list->funcs.maxSize = 8;
    list->funcs.func = (_il_func*) malloc(list->funcs.maxSize * sizeof(_il_func));

    list->binops.currSize = 0;
    list->binops.maxSize = 8;
    list->binops.binop = (_il_binop*) malloc(list->binops.maxSize * sizeof(_il_binop));

    list->start = (_inflist_type) 0;
    list->step = (_inflist_type) 1;
}



void _il_addToEvalList(inflist *list, _il_pair a) {
    if (list == NULL) return;

    if (list->evalList.currSize >= list->evalList.maxSize - 1) _il_resizeVector(&list->evalList, -1);

    list->evalList.pair[list->evalList.currSize] = a;
    list->evalList.currSize++;
}

void _il_addToNums(inflist *list, _inflist_type a) {
    if (list == NULL) return;

    if (list->nums.currSize >= list->nums.maxSize - 1) _il_resizeVector(&list->nums, 0);

    list->nums.num[list->nums.currSize] = a;
    list->nums.currSize++;
}

void _il_addToFuncs(inflist *list, _il_func a) {
    if ((list == NULL) || (a == NULL)) return;

    if (list->funcs.currSize >= list->funcs.maxSize - 1) _il_resizeVector(&list->funcs, 1);

    list->funcs.func[list->funcs.currSize] = a;
    list->funcs.currSize++;
}

void _il_addToBinops(inflist *list, _il_binop a) {
    if ((list == NULL) || (a == NULL)) return;

    if (list->binops.currSize >= list->binops.maxSize - 1) _il_resizeVector(&list->binops, 3);

    list->binops.binop[list->binops.currSize] = a;
    list->binops.currSize++;
}

void _il_resizeVector(_il_vector *vector, int typeVal) {
    if ((typeVal < -1) || (typeVal > 3)) return;
    if (vector == NULL) return;

    vector->maxSize *= 2;
    
    switch(typeVal) {
        case -1: /* _il_vector pair */
            vector->pair = (_il_pair*) realloc(vector->pair, vector->maxSize * sizeof(_il_pair));
            break;
        case 0: /* _il_vector num */
            vector->num = (_inflist_type*) realloc(vector->num, vector->maxSize * sizeof(_inflist_type));
            break;
        case 1:
        case 2:  /* _il_vector func */
            vector->func = (_il_func*) realloc(vector->func, vector->maxSize * sizeof(_il_func));
            break;
        case 3:  /* _il_vector binop */
            vector->binop = (_il_binop*) realloc(vector->binop, vector->maxSize * sizeof(_il_binop));
            break;
    }
}



void _il_addOp_num(inflist* list, _inflist_type num) {
    if (list == NULL) return;

    _il_pair newPair;
    _il_addToNums(list, num);
    newPair.first = 0;
    newPair.second = list->nums.currSize - 1;
    _il_addToEvalList(list, newPair);
}

void _il_addOp_func(inflist* list, _il_func obj) {
    if ((list == NULL) || (obj == NULL)) return;

    _il_pair newPair;
    _il_addToFuncs(list, obj);
    newPair.first = 1;
    newPair.second = list->funcs.currSize - 1;
    _il_addToEvalList(list, newPair);
}

void _il_addOp_list(inflist* list, inflist* otherList) {
    if ((list == NULL) || (otherList == NULL)) return;

    int i, second;
    for (i = 0; i < otherList->evalList.currSize; i++) {
        second = otherList->evalList.pair[i].second;
        switch (otherList->evalList.pair[i].first) {
            case 0: /* nums */
                _il_addOp_num(list, otherList->nums.num[second]);
                break;
            case 1: /* funcs (function) */
                _il_addOp_func(list, otherList->funcs.func[second]);
                break;
            case 2: /* funcs (unary operator) */
                inflist_push_unop(list, otherList->funcs.func[second]);
                break;
            case 3: /* binops */
                inflist_push_binop(list, otherList->binops.binop[second]);
                break;
        }
    }
}

void inflist_push_unop(inflist* list, _il_func obj) {
    if ((list == NULL) || (obj == NULL)) return;

    _il_pair newPair;
    _il_addToFuncs(list, obj);
    newPair.first = 2;
    newPair.second = list->funcs.currSize - 1;
    _il_addToEvalList(list, newPair);
}

void inflist_push_binop(inflist* list, _il_binop obj) {
    if ((list == NULL) || (obj == NULL)) return;

    _il_pair newPair;
    _il_addToBinops(list, obj);
    newPair.first = 3;
    newPair.second = list->binops.currSize - 1;
    _il_addToEvalList(list, newPair);
}



_inflist_type at(inflist *list, int x) {
    if (list == NULL) return;

    _inflist_type evalStack[3];
    int i, second, evalStackCurrSize = 0;
    _inflist_type val1, val2;
    for (i = 0; i < list->evalList.currSize; i++) {
        second = list->evalList.pair[i].second;
        switch (list->evalList.pair[i].first) {
            case 0: /* nums */
                if (evalStackCurrSize == 3) break;
                evalStack[evalStackCurrSize] = list->nums.num[second];
                evalStackCurrSize++;
                break;
            case 1: /* funcs (function) */
                if (evalStackCurrSize == 3) break;
                evalStack[evalStackCurrSize] = list->funcs.func[second](list->start + (list->step * (_inflist_type) x));
                evalStackCurrSize++;
                break;
            case 2: /* funcs (unary operator) */
                val1 = evalStack[evalStackCurrSize - 1];
                evalStack[evalStackCurrSize - 1] = list->funcs.func[second](val1);
                break;
            case 3: /* binops */
                val1 = evalStack[evalStackCurrSize - 1];
                val2 = evalStack[evalStackCurrSize - 2];
                evalStack[evalStackCurrSize - 2] = list->binops.binop[second](val2, val1);
                evalStackCurrSize--;
                break;
        }
    }

    return evalStack[0];
}



#endif
