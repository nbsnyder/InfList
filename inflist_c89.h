/*
InfList by Nathan Snyder: This library (following the C89 / ANSI C standard) allows
users to work with and evaluate infinite lists by using a lazy evaluation model.
*/

#ifndef _INFLIST_H_
#define _INFLIST_H_

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
    _il_vector eval_list; /* stores pairs of integers that correspond to entries in the next four vectors: */
    _il_vector nums; /* stores numbers (0) */
    _il_vector funcs; /* stores functions (1) and unary operators (2) */
    _il_vector binops; /* stores binary operators (3) */
} inflist;



/* Functions to initiate the list */
void inflist_init_from_num(inflist* list, _inflist_type num);
void inflist_init_from_func(inflist* list, _il_func obj);
void inflist_init_from_list(inflist* list, inflist* otherList);
void _il_init(inflist* list);

/* Functions for reducing the list */
_inflist_type inflist_at(inflist *list, int x);
_inflist_type* inflist_first(inflist *list, int num);
_inflist_type inflist_fold_first(inflist *list, int num, _il_binop binop);
_inflist_type inflist_fold_first_arg(inflist *list, int num, _il_binop binop, _inflist_type arg);
_inflist_type* inflist_range(inflist *list, int start, int end);
_inflist_type inflist_fold_range(inflist *list, int start, int end, _il_binop binop);
_inflist_type inflist_fold_range_arg(inflist *list, int start, int end, _il_binop binop, _inflist_type arg);

/* Functions that add a value to a specific _il_vector */
void _il_add_to_eval_list(inflist *list, _il_pair a);
void _il_add_to_nums(inflist *list, _inflist_type a);
void _il_add_to_funcs(inflist *list, _il_func a);
void _il_add_to_binops(inflist *list, _il_binop a);
void _il_resize_vector(_il_vector *vector, int typeVal);

/* Functions that add something to the inflist */
void _il_add_num(inflist* list, _inflist_type num);
void _il_add_func(inflist* list, _il_func obj);
void _il_add_unop(inflist* list, _il_func obj);
void _il_add_binop(inflist* list, _il_binop obj);
void _il_add_list(inflist* list, inflist* otherList);
void inflist_map_unop(inflist* list, _il_func unop);
void inflist_map_num_binop(inflist* list, _inflist_type num, _il_binop binop);
void inflist_map_func_binop(inflist* list, _il_func func, _il_binop binop);
void inflist_map_list_binop(inflist* list, inflist* otherList, _il_binop binop);

/* Operators */
_inflist_type _il_inc_op(_inflist_type x);
_inflist_type _il_dec_op(_inflist_type x);
_inflist_type _il_add_op(_inflist_type x1, _inflist_type x2);
_inflist_type _il_sub_op(_inflist_type x1, _inflist_type x2);
_inflist_type _il_mult_op(_inflist_type x1, _inflist_type x2);
_inflist_type _il_div_op(_inflist_type x1, _inflist_type x2);
_inflist_type _il_and_op(_inflist_type x1, _inflist_type x2);
_inflist_type _il_or_op(_inflist_type x1, _inflist_type x2);
_inflist_type _il_xor_op(_inflist_type x1, _inflist_type x2);
_inflist_type _il_mod_op(_inflist_type x1, _inflist_type x2);
_inflist_type _il_rshift_op(_inflist_type x1, _inflist_type x2);
_inflist_type _il_lshift_op(_inflist_type x1, _inflist_type x2);
_inflist_type _il_max_op(_inflist_type x1, _inflist_type x2);
_inflist_type _il_min_op(_inflist_type x1, _inflist_type x2);



/* Functions to initiate the list */

void inflist_init_from_num(inflist* list, _inflist_type num) {
    if (list == NULL) return;
    _il_init(list);
    _il_add_num(list, num);
}

void inflist_init_from_func(inflist* list, _il_func obj) {
    if ((list == NULL) || (obj == NULL)) return;
    _il_init(list);
    _il_add_func(list, obj);
}

void inflist_init_from_list(inflist* list, inflist* otherList) {
    if ((list == NULL) || (otherList == NULL)) return;
    _il_init(list);
    _il_add_list(list, otherList);
}

void _il_init(inflist* list) {
    if (list == NULL) return;

    list->start = (_inflist_type) 0;
    list->step = (_inflist_type) 1;

    /* Set all currSizes to 0 and all maxSizes to 8 */
    list->eval_list.currSize = list->nums.currSize = list->funcs.currSize = list->binops.currSize = 0;
    list->eval_list.maxSize = list->nums.maxSize = list->funcs.maxSize = list->binops.maxSize = 8;

    /* Allocate memory for all of the _il_vectors */
    list->eval_list.pair = (_il_pair*) malloc(list->eval_list.maxSize * sizeof(_il_pair));
    list->nums.num = (_inflist_type*) malloc(list->nums.maxSize * sizeof(_inflist_type));
    list->funcs.func = (_il_func*) malloc(list->funcs.maxSize * sizeof(_il_func));
    list->binops.binop = (_il_binop*) malloc(list->binops.maxSize * sizeof(_il_binop));
}



/* Functions for reducing the list */

_inflist_type inflist_at(inflist *list, int x) {
    if (list == NULL) return (_inflist_type) 0;

    _inflist_type evalStack[3];
    int i, second, evalStackCurrSize = 0;
    _inflist_type val1, val2;
    for (i = 0; i < list->eval_list.currSize; i++) {
        second = list->eval_list.pair[i].second;
        switch (list->eval_list.pair[i].first) {
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

_inflist_type* inflist_first(inflist *list, int num) {
    return inflist_range(list, 0, num - 1);
}

_inflist_type inflist_fold_first(inflist *list, int num, _il_binop binop) {
    return (num < 1) ? inflist_at(list, 0) : inflist_fold_range(list, 0, num - 1, binop);
}

_inflist_type inflist_fold_first_arg(inflist *list, int num, _il_binop binop, _inflist_type arg) {
    return (num < 1) ? inflist_at(list, 0) : inflist_fold_range_arg(list, 0, num - 1, binop, arg);
}

_inflist_type* inflist_range(inflist *list, int start, int end) {
    _inflist_type *arr = NULL;
    int i;

    if ((start >= end) && (list->step < 0)) {
        arr = malloc((start - end + 1) * sizeof(_inflist_type));
        for (i = end; i <= start; i++) arr[i - end] = inflist_at(list, i);
    } else if ((start <= end) && (list->step > 0)) {
        arr = malloc((end - start + 1) * sizeof(_inflist_type));
        for (i = start; i <= end; i++) arr[i - start] = inflist_at(list, i);
    }

    return arr;
}

_inflist_type inflist_fold_range(inflist *list, int start, int end, _il_binop binop) {
    _inflist_type ret = inflist_at(list, start);
    int i;

    if ((start > end) && (list->step < 0)) {
        for (i = start - 1; i >= end; i--) ret = (*binop)(ret, inflist_at(list, i));
    } else if ((start < end) && (list->step > 0)) {
        for (i = start + 1; i <= end; i++) ret = (*binop)(ret, inflist_at(list, i));
    }
    
    return ret;
}

_inflist_type inflist_fold_range_arg(inflist *list, int start, int end, _il_binop binop, _inflist_type arg) {
    _inflist_type ret = (*binop)(inflist_at(list, start), arg);
    int i;

    if ((end < start) && (list->step < 0)) {
        for (i = start - 1; i >= end; i--) ret = (*binop)(ret, inflist_at(list, i));
    } else if ((start < end) && (list->step > 0)) {
        for (i = start + 1; i <= end; i++) ret = (*binop)(ret, inflist_at(list, i));
    }
    
    return ret;
}



/* Functions that add a value to a specific _il_vector */

void _il_add_to_eval_list(inflist *list, _il_pair a) {
    if (list == NULL) return;

    if (list->eval_list.currSize >= list->eval_list.maxSize - 1) _il_resize_vector(&list->eval_list, -1);

    list->eval_list.pair[list->eval_list.currSize] = a;
    list->eval_list.currSize++;
}

void _il_add_to_nums(inflist *list, _inflist_type a) {
    if (list == NULL) return;

    if (list->nums.currSize >= list->nums.maxSize - 1) _il_resize_vector(&list->nums, 0);

    list->nums.num[list->nums.currSize] = a;
    list->nums.currSize++;
}

void _il_add_to_funcs(inflist *list, _il_func a) {
    if ((list == NULL) || (a == NULL)) return;

    if (list->funcs.currSize >= list->funcs.maxSize - 1) _il_resize_vector(&list->funcs, 1);

    list->funcs.func[list->funcs.currSize] = a;
    list->funcs.currSize++;
}

void _il_add_to_binops(inflist *list, _il_binop a) {
    if ((list == NULL) || (a == NULL)) return;

    if (list->binops.currSize >= list->binops.maxSize - 1) _il_resize_vector(&list->binops, 3);

    list->binops.binop[list->binops.currSize] = a;
    list->binops.currSize++;
}

void _il_resize_vector(_il_vector *vector, int typeVal) {
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



/* Functions that add something to the inflist */

void _il_add_num(inflist* list, _inflist_type num) {
    if (list == NULL) return;

    _il_pair newPair;
    _il_add_to_nums(list, num);
    newPair.first = 0;
    newPair.second = list->nums.currSize - 1;
    _il_add_to_eval_list(list, newPair);
}

void _il_add_func(inflist* list, _il_func obj) {
    if ((list == NULL) || (obj == NULL)) return;

    _il_pair newPair;
    _il_add_to_funcs(list, obj);
    newPair.first = 1;
    newPair.second = list->funcs.currSize - 1;
    _il_add_to_eval_list(list, newPair);
}

void _il_add_unop(inflist* list, _il_func obj) {
    if ((list == NULL) || (obj == NULL)) return;

    _il_pair newPair;
    _il_add_to_funcs(list, obj);
    newPair.first = 2;
    newPair.second = list->funcs.currSize - 1;
    _il_add_to_eval_list(list, newPair);
}

void _il_add_binop(inflist* list, _il_binop obj) {
    if ((list == NULL) || (obj == NULL)) return;

    _il_pair newPair;
    _il_add_to_binops(list, obj);
    newPair.first = 3;
    newPair.second = list->binops.currSize - 1;
    _il_add_to_eval_list(list, newPair);
}

void _il_add_list(inflist* list, inflist* otherList) {
    if ((list == NULL) || (otherList == NULL)) return;

    int i, second;
    for (i = 0; i < otherList->eval_list.currSize; i++) {
        second = otherList->eval_list.pair[i].second;
        switch (otherList->eval_list.pair[i].first) {
            case 0: /* nums */
                _il_add_num(list, otherList->nums.num[second]);
                break;
            case 1: /* funcs (function) */
                _il_add_func(list, otherList->funcs.func[second]);
                break;
            case 2: /* funcs (unary operator) */
                _il_add_unop(list, otherList->funcs.func[second]);
                break;
            case 3: /* binops */
                _il_add_binop(list, otherList->binops.binop[second]);
                break;
        }
    }
}

void inflist_map_unop(inflist* list, _il_func unop) {
    if ((list == NULL) || (unop == NULL)) return;

    _il_pair newPair;
    _il_add_to_funcs(list, unop);
    newPair.first = 2;
    newPair.second = list->funcs.currSize - 1;
    _il_add_to_eval_list(list, newPair);
}

void inflist_map_num_binop(inflist* list, _inflist_type num, _il_binop binop) {
    if ((list == NULL) || (binop == NULL)) return;

    _il_add_num(list, num);

    _il_pair newPair;
    _il_add_to_binops(list, binop);
    newPair.first = 3;
    newPair.second = list->binops.currSize - 1;
    _il_add_to_eval_list(list, newPair);
}

void inflist_map_func_binop(inflist* list, _il_func func, _il_binop binop) {
    if ((list == NULL) || (binop == NULL)) return;

    _il_add_func(list, func);

    _il_pair newPair;
    _il_add_to_binops(list, binop);
    newPair.first = 3;
    newPair.second = list->binops.currSize - 1;
    _il_add_to_eval_list(list, newPair);
}

void inflist_map_list_binop(inflist* list, inflist* otherList, _il_binop binop) {
    if ((list == NULL) || (binop == NULL)) return;

    _il_add_list(list, otherList);

    _il_pair newPair;
    _il_add_to_binops(list, binop);
    newPair.first = 3;
    newPair.second = list->binops.currSize - 1;
    _il_add_to_eval_list(list, newPair);
}



/* Operator methods */

_inflist_type _il_inc_op(_inflist_type x) {
    return ++x;
}

_inflist_type _il_dec_op(_inflist_type x) {
    return --x;
}

_inflist_type _il_add_op(_inflist_type x1, _inflist_type x2) { 
    return x1 + x2; 
}

_inflist_type _il_sub_op(_inflist_type x1, _inflist_type x2) { 
    return x1 - x2; 
}

_inflist_type _il_mult_op(_inflist_type x1, _inflist_type x2) { 
    return x1 * x2; 
}

_inflist_type _il_div_op(_inflist_type x1, _inflist_type x2) { 
    return x1 / x2; 
}

_inflist_type _il_and_op(_inflist_type x1, _inflist_type x2) { 
    return (int) x1 & (int) x2; 
}

_inflist_type _il_or_op(_inflist_type x1, _inflist_type x2) { 
    return (int) x1 | (int) x2; 
}

_inflist_type _il_xor_op(_inflist_type x1, _inflist_type x2) { 
    return (int) x1 ^ (int) x2; 
}

_inflist_type _il_mod_op(_inflist_type x1, _inflist_type x2) { 
    return (int) x1 % (int) x2; 
}

_inflist_type _il_rshift_op(_inflist_type x1, _inflist_type x2) { 
    return (int) x1 >> (int) x2; 
}

_inflist_type _il_lshift_op(_inflist_type x1, _inflist_type x2) { 
    return (int) x1 << (int) x2; 
}

_inflist_type _il_max_op(_inflist_type x1, _inflist_type x2) {
    return (x1 > x2) ? x1 : x2;
}

_inflist_type _il_min_op(_inflist_type x1, _inflist_type x2) {
    return (x1 < x2) ? x1 : x2;
}



#endif
