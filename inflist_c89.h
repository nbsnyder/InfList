/*
InfList by Nathan Snyder: This library (which follows the C89 standard) allows
users to work with and evaluate infinite lists by using a lazy evaluation model.
*/

#ifndef _INFLIST_H_
#define _INFLIST_H_

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifndef _inflist_type
#define _inflist_type int
#endif



/* Type definitions */

typedef _inflist_type(*_il_func)(_inflist_type);

typedef _inflist_type(*_il_binop)(_inflist_type, _inflist_type);

typedef struct {
    /* Index 0 of the list starts at func(start) [default: 0] */
    _inflist_type start;

    /* Each index in the list progresses by step [default: 1] */
    _inflist_type step;

    /* stores pairs of integers that correspond to entries in the next four vectors: */
    struct {
        uint16_t *first, *second, currSize, maxSize;
    } eval_list;

    /* stores numbers (0) */
    struct {
        _inflist_type *num;
        uint16_t currSize, maxSize;
    } nums;

    /* stores functions (1) and unary operators (2) */
    struct {
        _il_func *func;
        uint16_t currSize, maxSize;
    } funcs;

    /* stores binary operators (3) */
    struct {
        _il_binop *binop;
        uint16_t currSize, maxSize;
    } binops;
} inflist;



/* Functions to initiate the list */
void inflist_init(inflist* list);
void inflist_init_from_num(inflist* list, _inflist_type num);
void inflist_init_from_func(inflist* list, _il_func obj);
void inflist_init_from_list(inflist* list, inflist* otherList);
void _il_init(inflist* list);
_inflist_type inflist_getStart(inflist* list);
_inflist_type inflist_getStep(inflist* list);
void inflist_setStart(inflist *list, _inflist_type start);
void inflist_setStep(inflist *list, _inflist_type step);

/* Functions for reducing the list */
_inflist_type inflist_at(inflist *list, int x);
_inflist_type* inflist_first(inflist *list, int num);
_inflist_type inflist_fold_first_str(inflist *list, int num, const char *binop);
_inflist_type inflist_fold_first_str_arg(inflist *list, int num, const char *binop, _inflist_type arg);
_inflist_type inflist_fold_first_func(inflist *list, int num, _il_binop binop);
_inflist_type inflist_fold_first_func_arg(inflist *list, int num, _il_binop binop, _inflist_type arg);
_inflist_type* inflist_range(inflist *list, int start, int end);
_inflist_type inflist_fold_range_str(inflist *list, int start, int end, const char *binop);
_inflist_type inflist_fold_range_str_arg(inflist *list, int start, int end, const char *binop, _inflist_type arg);
_inflist_type inflist_fold_range_func(inflist *list, int start, int end, _il_binop binop);
_inflist_type inflist_fold_range_func_arg(inflist *list, int start, int end, _il_binop binop, _inflist_type arg);

/* Functions that add a value to a specific _il_vector */
void _il_add_to_eval_list(inflist *list, int first, int second);
void _il_add_to_nums(inflist *list, _inflist_type a);
void _il_add_to_funcs(inflist *list, _il_func a);
void _il_add_to_binops(inflist *list, _il_binop a);

/* Functions that add something to the inflist */
void _il_add_num(inflist* list, _inflist_type num);
void _il_add_func(inflist* list, _il_func obj);
void _il_add_unop(inflist* list, _il_func obj);
void _il_add_binop(inflist* list, _il_binop obj);
void _il_add_list(inflist* list, inflist* otherList);
void inflist_map_unop_func(inflist* list, _il_func unop);
void inflist_map_num_binop_func(inflist* list, _inflist_type num, _il_binop binop);
void inflist_map_func_binop_func(inflist* list, _il_func func, _il_binop binop);
void inflist_map_list_binop_func(inflist* list, inflist* otherList, _il_binop binop);
void inflist_map_unop_str(inflist* list, const char *unop);
void inflist_map_num_binop_str(inflist* list, _inflist_type num, const char *binop);
void inflist_map_func_binop_str(inflist* list, _il_func func, const char *binop);
void inflist_map_list_binop_str(inflist* list, inflist* otherList, const char *binop);

_il_func inflist_string_to_unop(const char *unop);
_il_binop inflist_string_to_binop(const char *binop);

/* Operators */
_inflist_type inc_op(_inflist_type x);
_inflist_type dec_op(_inflist_type x);
_inflist_type add_op(_inflist_type x1, _inflist_type x2);
_inflist_type sub_op(_inflist_type x1, _inflist_type x2);
_inflist_type mult_op(_inflist_type x1, _inflist_type x2);
_inflist_type div_op(_inflist_type x1, _inflist_type x2);
_inflist_type and_op(_inflist_type x1, _inflist_type x2);
_inflist_type or_op(_inflist_type x1, _inflist_type x2);
_inflist_type xor_op(_inflist_type x1, _inflist_type x2);
_inflist_type mod_op(_inflist_type x1, _inflist_type x2);
_inflist_type rshift_op(_inflist_type x1, _inflist_type x2);
_inflist_type lshift_op(_inflist_type x1, _inflist_type x2);
_inflist_type max_op(_inflist_type x1, _inflist_type x2);
_inflist_type min_op(_inflist_type x1, _inflist_type x2);



/* Functions to initiate the list */

void inflist_init(inflist* list) {
    if (list == NULL) return;
    _il_init(list);
    _il_add_num(list, (_inflist_type) 0);
}

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
    /* list->eval_list.pair = (_il_pair*) malloc(list->eval_list.maxSize * sizeof(_il_pair)); */
    list->eval_list.first = (uint16_t*) malloc(list->eval_list.maxSize * sizeof(uint16_t));
    list->eval_list.second = (uint16_t*) malloc(list->eval_list.maxSize * sizeof(uint16_t));
    list->nums.num = (_inflist_type*) malloc(list->nums.maxSize * sizeof(_inflist_type));
    list->funcs.func = (_il_func*) malloc(list->funcs.maxSize * sizeof(_il_func));
    list->binops.binop = (_il_binop*) malloc(list->binops.maxSize * sizeof(_il_binop));
}

_inflist_type inflist_getStart(inflist *list) {
    return list->start;
}

_inflist_type inflist_getStep(inflist *list) {
    return list->step;
}

void inflist_setStart(inflist *list, _inflist_type start) {
    list->start = start;
}

void inflist_setStep(inflist *list, _inflist_type step) {
    if (step != 0) list->step = step;
}



/* Functions for reducing the list */

_inflist_type inflist_at(inflist *list, int x) {
    if (list == NULL) return (_inflist_type) 0;

    _inflist_type evalStack[3];
    int i, second, evalStackCurrSize = 0;
    _inflist_type val1, val2;
    for (i = 0; i < list->eval_list.currSize; i++) {
        second = list->eval_list.second[i];
        switch (list->eval_list.first[i]) {
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

_inflist_type inflist_fold_first_str(inflist *list, int num, const char *binop) {
    return inflist_fold_first_func(list, num, inflist_string_to_binop(binop));
}

_inflist_type inflist_fold_first_str_arg(inflist *list, int num, const char *binop, _inflist_type arg) {
    return inflist_fold_first_func_arg(list, num, inflist_string_to_binop(binop), arg);
}

_inflist_type inflist_fold_first_func(inflist *list, int num, _il_binop binop) {
    return (num < 1) ? inflist_at(list, 0) : inflist_fold_range_func(list, 0, num - 1, binop);
}

_inflist_type inflist_fold_first_func_arg(inflist *list, int num, _il_binop binop, _inflist_type arg) {
    return (num < 1) ? inflist_at(list, 0) : inflist_fold_range_func_arg(list, 0, num - 1, binop, arg);
}

_inflist_type* inflist_range(inflist *list, int start, int end) {
    _inflist_type *arr = NULL;
    int i;

    if ((start >= end) && (list->step < 0)) {
        arr = (_inflist_type*) malloc((start - end + 1) * sizeof(_inflist_type));
        for (i = end; i <= start; i++) arr[i - end] = inflist_at(list, i);
    } else if ((start <= end) && (list->step > 0)) {
        arr = (_inflist_type*) malloc((end - start + 1) * sizeof(_inflist_type));
        for (i = start; i <= end; i++) arr[i - start] = inflist_at(list, i);
    }

    return arr;
}

_inflist_type inflist_fold_range_str(inflist *list, int start, int end, const char *binop) {
    return inflist_fold_range_func(list, start, end, inflist_string_to_binop(binop));
}

_inflist_type inflist_fold_range_str_arg(inflist *list, int start, int end, const char *binop, _inflist_type arg) {
    return inflist_fold_range_func_arg(list, start, end, inflist_string_to_binop(binop), arg);
}

_inflist_type inflist_fold_range_func(inflist *list, int start, int end, _il_binop binop) {
    _inflist_type ret = inflist_at(list, start);
    int i;

    if ((start > end) && (list->step < 0)) {
        for (i = start - 1; i >= end; i--) ret = (*binop)(ret, inflist_at(list, i));
    } else if ((start < end) && (list->step > 0)) {
        for (i = start + 1; i <= end; i++) ret = (*binop)(ret, inflist_at(list, i));
    }
    
    return ret;
}

_inflist_type inflist_fold_range_func_arg(inflist *list, int start, int end, _il_binop binop, _inflist_type arg) {
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

void _il_add_to_eval_list(inflist *list, int first, int second) {
    if (list == NULL) return;

    if (list->eval_list.currSize >= list->eval_list.maxSize - 1) {
        list->eval_list.maxSize *= 2;
        list->eval_list.first = (uint16_t*) realloc(list->eval_list.first, list->eval_list.maxSize * sizeof(uint16_t));
        list->eval_list.second = (uint16_t*) realloc(list->eval_list.second, list->eval_list.maxSize * sizeof(uint16_t));
    }

    list->eval_list.first[list->eval_list.currSize] = first;
    list->eval_list.second[list->eval_list.currSize] = second;
    list->eval_list.currSize++;
}

void _il_add_to_nums(inflist *list, _inflist_type a) {
    if (list == NULL) return;

    if (list->nums.currSize >= list->nums.maxSize - 1) {
        list->nums.maxSize *= 2;
        list->nums.num = (_inflist_type*) realloc(list->nums.num, list->nums.maxSize * sizeof(_inflist_type));
    }

    list->nums.num[list->nums.currSize] = a;
    list->nums.currSize++;
}

void _il_add_to_funcs(inflist *list, _il_func a) {
    if ((list == NULL) || (a == NULL)) return;

    if (list->funcs.currSize >= list->funcs.maxSize - 1) {
        list->funcs.maxSize *= 2;
        list->funcs.func = (_il_func*) realloc(list->funcs.func, list->funcs.maxSize * sizeof(_il_func));
    }

    list->funcs.func[list->funcs.currSize] = a;
    list->funcs.currSize++;
}

void _il_add_to_binops(inflist *list, _il_binop a) {
    if ((list == NULL) || (a == NULL)) return;

    /* Resize vector if needed */
    if (list->binops.currSize >= list->binops.maxSize - 1) {
        list->binops.maxSize *= 2;
        list->binops.binop = (_il_binop*) realloc(list->binops.binop, list->binops.maxSize * sizeof(_il_binop));
    }

    list->binops.binop[list->binops.currSize] = a;
    list->binops.currSize++;
}



/* Functions that add something to the inflist */

void _il_add_num(inflist* list, _inflist_type num) {
    if (list == NULL) return;

    _il_add_to_nums(list, num);
    uint16_t first = 0;
    uint16_t second = list->nums.currSize - 1;
    _il_add_to_eval_list(list, first, second);
}

void _il_add_func(inflist* list, _il_func obj) {
    if ((list == NULL) || (obj == NULL)) return;

    _il_add_to_funcs(list, obj);
    uint16_t first = 1;
    uint16_t second = list->funcs.currSize - 1;
    _il_add_to_eval_list(list, first, second);
}

void _il_add_unop(inflist* list, _il_func obj) {
    if ((list == NULL) || (obj == NULL)) return;

    _il_add_to_funcs(list, obj);
    uint16_t first = 2;
    uint16_t second = list->funcs.currSize - 1;
    _il_add_to_eval_list(list, first, second);
}

void _il_add_binop(inflist* list, _il_binop obj) {
    if ((list == NULL) || (obj == NULL)) return;

    _il_add_to_binops(list, obj);
    uint16_t first = 3;
    uint16_t second = list->binops.currSize - 1;
    _il_add_to_eval_list(list, first, second);
}

void _il_add_list(inflist* list, inflist* otherList) {
    if ((list == NULL) || (otherList == NULL)) return;

    int i, second;
    for (i = 0; i < otherList->eval_list.currSize; i++) {
        second = otherList->eval_list.second[i];
        switch (otherList->eval_list.first[i]) {
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

void inflist_map_unop_func(inflist* list, _il_func unop) {
    if ((list == NULL) || (unop == NULL)) return;

    _il_add_to_funcs(list, unop);
    uint16_t first = 2;
    uint16_t second = list->funcs.currSize - 1;
    _il_add_to_eval_list(list, first, second);
}

void inflist_map_num_binop_func(inflist* list, _inflist_type num, _il_binop binop) {
    if ((list == NULL) || (binop == NULL)) return;

    _il_add_num(list, num);

    _il_add_to_binops(list, binop);
    uint16_t first = 3;
    uint16_t second = list->binops.currSize - 1;
    _il_add_to_eval_list(list, first, second);
}

void inflist_map_func_binop_func(inflist* list, _il_func func, _il_binop binop) {
    if ((list == NULL) || (binop == NULL)) return;

    _il_add_func(list, func);

    _il_add_to_binops(list, binop);
    uint16_t first = 3;
    uint16_t second = list->binops.currSize - 1;
    _il_add_to_eval_list(list, first, second);
}

void inflist_map_list_binop_func(inflist* list, inflist* otherList, _il_binop binop) {
    if ((list == NULL) || (binop == NULL)) return;

    _il_add_list(list, otherList);

    _il_add_to_binops(list, binop);
    uint16_t first = 3;
    uint16_t second = list->binops.currSize - 1;
    _il_add_to_eval_list(list, first, second);
}

void inflist_map_unop_str(inflist* list, const char *unop) {
    inflist_map_unop_func(list, inflist_string_to_unop(unop));
}

void inflist_map_num_binop_str(inflist* list, _inflist_type num, const char *binop) {
    inflist_map_num_binop_func(list, num, inflist_string_to_binop(binop));
}

void inflist_map_func_binop_str(inflist* list, _il_func func, const char *binop) {
    inflist_map_func_binop_func(list, func, inflist_string_to_binop(binop));
}

void inflist_map_list_binop_str(inflist* list, inflist* otherList, const char *binop) {
    inflist_map_list_binop_func(list, otherList, inflist_string_to_binop(binop));
}



_il_func inflist_string_to_unop(const char *unop) {
    return (strcmp(unop, "++") == 0) ? inc_op :
           (strcmp(unop, "--") == 0) ? dec_op :
           NULL;
}

_il_binop inflist_string_to_binop(const char *binop) {
    return (strcmp(binop, "+") == 0) ? add_op : 
           (strcmp(binop, "-") == 0) ? sub_op : 
           (strcmp(binop, "*") == 0) ? mult_op : 
           (strcmp(binop, "/") == 0) ? div_op : 
           (strcmp(binop, "%") == 0) ? mod_op : 
           (strcmp(binop, "&") == 0) ? and_op : 
           (strcmp(binop, "|") == 0) ? or_op : 
           (strcmp(binop, "^") == 0) ? xor_op : 
           (strcmp(binop, ">>") == 0) ? rshift_op : 
           (strcmp(binop, "<<") == 0) ? lshift_op : 
           (strcmp(binop, "max") == 0) ? max_op : 
           (strcmp(binop, "min") == 0) ? min_op : 
           NULL;
}



/* Operator methods */

_inflist_type inc_op(_inflist_type x) {
    return ++x;
}

_inflist_type dec_op(_inflist_type x) {
    return --x;
}

_inflist_type add_op(_inflist_type x1, _inflist_type x2) { 
    return x1 + x2; 
}

_inflist_type sub_op(_inflist_type x1, _inflist_type x2) { 
    return x1 - x2; 
}

_inflist_type mult_op(_inflist_type x1, _inflist_type x2) { 
    return x1 * x2; 
}

_inflist_type div_op(_inflist_type x1, _inflist_type x2) { 
    return x1 / x2; 
}

_inflist_type and_op(_inflist_type x1, _inflist_type x2) { 
    return (_inflist_type) ((int) x1 & (int) x2); 
}

_inflist_type or_op(_inflist_type x1, _inflist_type x2) { 
    return (_inflist_type) ((int) x1 | (int) x2); 
}

_inflist_type xor_op(_inflist_type x1, _inflist_type x2) { 
    return (_inflist_type) ((int) x1 ^ (int) x2); 
}

_inflist_type mod_op(_inflist_type x1, _inflist_type x2) { 
    return (_inflist_type) ((int) x1 % (int) x2); 
}

_inflist_type rshift_op(_inflist_type x1, _inflist_type x2) { 
    return (_inflist_type) ((int) x1 >> (int) x2); 
}

_inflist_type lshift_op(_inflist_type x1, _inflist_type x2) { 
    return (_inflist_type) ((int) x1 << (int) x2); 
}

_inflist_type max_op(_inflist_type x1, _inflist_type x2) {
    return (x1 > x2) ? x1 : x2;
}

_inflist_type min_op(_inflist_type x1, _inflist_type x2) {
    return (x1 < x2) ? x1 : x2;
}



#endif
