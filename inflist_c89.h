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

    /* Stores references to entries in nums, funcs, and binops */
    struct {
        uint16_t *first; /* Stores a list of numbers 0-3 that correspond to either nums, funcs, or binops */
        uint16_t *second; /* Stores the index of the entry in the vector specified by `first` */
        uint16_t currSize, maxSize;
    } eval_list;

    /* Stores numbers (0) */
    struct {
        _inflist_type *num;
        uint16_t currSize, maxSize;
    } nums;

    /* Stores functions (1) and unary operators (2) */
    struct {
        _il_func *func;
        uint16_t currSize, maxSize;
    } funcs;

    /* Stores binary operators (3) */
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

/* Getters and setters */
_inflist_type inflist_getStart(inflist* list);
_inflist_type inflist_getStep(inflist* list);
void inflist_setStart(inflist *list, _inflist_type start);
void inflist_setStep(inflist *list, _inflist_type step);

/* Get the value of the list element at index `x` */
_inflist_type inflist_at(inflist *list, int x);

/* Return a vector of the first `num` elements in the list */
_inflist_type* inflist_first(inflist *list, int num);

/* Return the result of the `binop` operation applied to the first `num` elements in the list */
_inflist_type inflist_fold_first_str(inflist *list, int num, const char *binop);
_inflist_type inflist_fold_first_str_arg(inflist *list, int num, const char *binop, _inflist_type arg);
_inflist_type inflist_fold_first_func(inflist *list, int num, _il_binop binop);
_inflist_type inflist_fold_first_func_arg(inflist *list, int num, _il_binop binop, _inflist_type arg);

/* Return a vector of the elements in the list between the `start` and `end` indices */
_inflist_type* inflist_range(inflist *list, int start, int end);

/* Return the result of the `binop` operation applied to the elements in the list between the `start` and `end` indices */
_inflist_type inflist_fold_range_str(inflist *list, int start, int end, const char *binop);
_inflist_type inflist_fold_range_str_arg(inflist *list, int start, int end, const char *binop, _inflist_type arg);
_inflist_type inflist_fold_range_func(inflist *list, int start, int end, _il_binop binop);
_inflist_type inflist_fold_range_func_arg(inflist *list, int start, int end, _il_binop binop, _inflist_type arg);

/* Public methods for applying an operator (either unary or binary) to every element in the list */
void inflist_map_unop_func(inflist* list, _il_func unop);
void inflist_map_num_binop_func(inflist* list, _inflist_type num, _il_binop binop);
void inflist_map_func_binop_func(inflist* list, _il_func func, _il_binop binop);
void inflist_map_list_binop_func(inflist* list, inflist* otherList, _il_binop binop);
void inflist_map_unop_str(inflist* list, const char *unop);
void inflist_map_num_binop_str(inflist* list, _inflist_type num, const char *binop);
void inflist_map_func_binop_str(inflist* list, _il_func func, const char *binop);
void inflist_map_list_binop_str(inflist* list, inflist* otherList, const char *binop);

/* Convert the string representation of an operator into its functional form */
_il_func inflist_string_to_unop(const char *unop);
_il_binop inflist_string_to_binop(const char *binop);

/* Operators */
_inflist_type inflist_inc_op(_inflist_type x);
_inflist_type inflist_dec_op(_inflist_type x);
_inflist_type inflist_add_op(_inflist_type x1, _inflist_type x2);
_inflist_type inflist_sub_op(_inflist_type x1, _inflist_type x2);
_inflist_type inflist_mult_op(_inflist_type x1, _inflist_type x2);
_inflist_type inflist_div_op(_inflist_type x1, _inflist_type x2);
_inflist_type inflist_and_op(_inflist_type x1, _inflist_type x2);
_inflist_type inflist_or_op(_inflist_type x1, _inflist_type x2);
_inflist_type inflist_xor_op(_inflist_type x1, _inflist_type x2);
_inflist_type inflist_mod_op(_inflist_type x1, _inflist_type x2);
_inflist_type inflist_rshift_op(_inflist_type x1, _inflist_type x2);
_inflist_type inflist_lshift_op(_inflist_type x1, _inflist_type x2);
_inflist_type inflist_max_op(_inflist_type x1, _inflist_type x2);
_inflist_type inflist_min_op(_inflist_type x1, _inflist_type x2);



/* Initiate an inflist with 0 */
void inflist_init(inflist* list) {
    if (list == NULL) return;
    _il_init(list);
    _il_add_num(list, (_inflist_type) 0);
}

/* Initiate an inflist with a number */
void inflist_init_from_num(inflist* list, _inflist_type num) {
    if (list == NULL) return;
    _il_init(list);
    _il_add_num(list, num);
}

/* Initiate an inflist with a function */
void inflist_init_from_func(inflist* list, _il_func obj) {
    if ((list == NULL) || (obj == NULL)) return;
    _il_init(list);
    _il_add_func(list, obj);
}

/* Initiate an inflist from another list */
void inflist_init_from_list(inflist* list, inflist* otherList) {
    if ((list == NULL) || (otherList == NULL)) return;
    _il_init(list);
    _il_add_list(list, otherList);
}

/* Private initiation function */
void _il_init(inflist* list) {
    if (list == NULL) return;

    list->start = (_inflist_type) 0;
    list->step = (_inflist_type) 1;

    /* Set all currSizes to 0 and all maxSizes to 8 */
    list->eval_list.currSize = list->nums.currSize = list->funcs.currSize = list->binops.currSize = 0;
    list->eval_list.maxSize = list->nums.maxSize = list->funcs.maxSize = list->binops.maxSize = 8;

    /* Allocate memory for all of the vectors */
    list->eval_list.first = (uint16_t*) malloc(list->eval_list.maxSize * sizeof(uint16_t));
    list->eval_list.second = (uint16_t*) malloc(list->eval_list.maxSize * sizeof(uint16_t));
    list->nums.num = (_inflist_type*) malloc(list->nums.maxSize * sizeof(_inflist_type));
    list->funcs.func = (_il_func*) malloc(list->funcs.maxSize * sizeof(_il_func));
    list->binops.binop = (_il_binop*) malloc(list->binops.maxSize * sizeof(_il_binop));
}



/* Add a value to eval_list */
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

/* Add a value to nums */
void _il_add_to_nums(inflist *list, _inflist_type a) {
    if (list == NULL) return;

    if (list->nums.currSize >= list->nums.maxSize - 1) {
        list->nums.maxSize *= 2;
        list->nums.num = (_inflist_type*) realloc(list->nums.num, list->nums.maxSize * sizeof(_inflist_type));
    }

    list->nums.num[list->nums.currSize] = a;
    list->nums.currSize++;
}

/* Add a value to funcs */
void _il_add_to_funcs(inflist *list, _il_func a) {
    if ((list == NULL) || (a == NULL)) return;

    if (list->funcs.currSize >= list->funcs.maxSize - 1) {
        list->funcs.maxSize *= 2;
        list->funcs.func = (_il_func*) realloc(list->funcs.func, list->funcs.maxSize * sizeof(_il_func));
    }

    list->funcs.func[list->funcs.currSize] = a;
    list->funcs.currSize++;
}

/* Add a value to binops */
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



/* Add a number to the inflist */
void _il_add_num(inflist* list, _inflist_type num) {
    if (list == NULL) return;

    _il_add_to_nums(list, num);
    _il_add_to_eval_list(list, 0, list->nums.currSize - 1);
}

/* Add a function to the inflist */
void _il_add_func(inflist* list, _il_func obj) {
    if ((list == NULL) || (obj == NULL)) return;

    _il_add_to_funcs(list, obj);
    _il_add_to_eval_list(list, 1, list->funcs.currSize - 1);
}

/* Add a unary operator to the inflist */
void _il_add_unop(inflist* list, _il_func obj) {
    if ((list == NULL) || (obj == NULL)) return;

    _il_add_to_funcs(list, obj);
    _il_add_to_eval_list(list, 2, list->funcs.currSize - 1);
}

/* Add a binary operator to the inflist */
void _il_add_binop(inflist* list, _il_binop obj) {
    if ((list == NULL) || (obj == NULL)) return;

    _il_add_to_binops(list, obj);
    _il_add_to_eval_list(list, 3, list->binops.currSize - 1);
}

/* Add another list to the inflist */
void _il_add_list(inflist* list, inflist* otherList) {
    int i, second;

    if ((list == NULL) || (otherList == NULL)) return;

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



/* Get the value of start */
_inflist_type inflist_getStart(inflist *list) {
    return (list != NULL) ? list->start : (_inflist_type) 0;
}

/* Get the value of step */
_inflist_type inflist_getStep(inflist *list) {
    return (list != NULL) ? list->step : (_inflist_type) 0;
}

/* Set the value of start */
void inflist_setStart(inflist *list, _inflist_type start) {
    if (list != NULL) list->start = start;
}

/* Set the value of step */
void inflist_setStep(inflist *list, _inflist_type step) {
    if ((list != NULL) && (step != 0)) list->step = step;
}



/* Get the value of the list element at index `x` */
_inflist_type inflist_at(inflist *list, int x) {
    _inflist_type val1, val2, evalStack[3];
    int i, first, second, evalStackCurrSize;

    if (list != NULL) {
        evalStackCurrSize = 0;

        for (i = 0; i < list->eval_list.currSize; i++) {
            first = list->eval_list.first[i];
            second = list->eval_list.second[i];
            if ((first == 0) && (evalStackCurrSize < 3)) {
                evalStack[evalStackCurrSize] = list->nums.num[second];
                evalStackCurrSize++;
            } else if ((first == 1) && (evalStackCurrSize < 3)) {
                evalStack[evalStackCurrSize] = list->funcs.func[second](list->start + (list->step * (_inflist_type) x));
                evalStackCurrSize++;
            } else if ((first == 2) && (evalStackCurrSize > 0)) {
                val1 = evalStack[evalStackCurrSize - 1];
                evalStack[evalStackCurrSize - 1] = list->funcs.func[second](val1);
            } else if ((first == 3) && (evalStackCurrSize > 1)) {
                val1 = evalStack[evalStackCurrSize - 1];
                val2 = evalStack[evalStackCurrSize - 2];
                evalStack[evalStackCurrSize - 2] = list->binops.binop[second](val2, val1);
                evalStackCurrSize--;
            }
        }
    }

    return evalStack[0];
}



/* Return a vector of the first `num` elements in the list */
_inflist_type* inflist_first(inflist *list, int num) {
    return inflist_range(list, 0, num - 1);
}



/* Return the result of the `binop` operation applied to the first `num` elements in the list */
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



/* Return a vector of the elements in the list between the `start` and `end` indices */
_inflist_type* inflist_range(inflist *list, int start, int end) {
    int i;
    _inflist_type *arr = NULL;

    if (list != NULL) {
        if ((start >= end) && (list->step < 0)) {
            arr = (_inflist_type*) malloc((start - end + 1) * sizeof(_inflist_type));
            for (i = end; i <= start; i++) arr[i - end] = inflist_at(list, i);
        } else if ((start <= end) && (list->step > 0)) {
            arr = (_inflist_type*) malloc((end - start + 1) * sizeof(_inflist_type));
            for (i = start; i <= end; i++) arr[i - start] = inflist_at(list, i);
        }
    }

    return arr;
}



/* Return the result of the `binop` operation applied to the elements in the list between the `start` and `end` indices */
_inflist_type inflist_fold_range_str(inflist *list, int start, int end, const char *binop) {
    return inflist_fold_range_func(list, start, end, inflist_string_to_binop(binop));
}

_inflist_type inflist_fold_range_str_arg(inflist *list, int start, int end, const char *binop, _inflist_type arg) {
    return inflist_fold_range_func_arg(list, start, end, inflist_string_to_binop(binop), arg);
}

_inflist_type inflist_fold_range_func(inflist *list, int start, int end, _il_binop binop) {
    int i;
    _inflist_type ret = (_inflist_type) 0;

    if ((list != NULL) && (binop != NULL)) {
        ret = inflist_at(list, start);

        if ((start > end) && (list->step < 0)) {
            for (i = start - 1; i >= end; i--) ret = (*binop)(ret, inflist_at(list, i));
        } else if ((start < end) && (list->step > 0)) {
            for (i = start + 1; i <= end; i++) ret = (*binop)(ret, inflist_at(list, i));
        }
    }
    
    return ret;
}

_inflist_type inflist_fold_range_func_arg(inflist *list, int start, int end, _il_binop binop, _inflist_type arg) {
    int i;
    _inflist_type ret = (_inflist_type) 0;

    if ((list != NULL) && (binop != NULL)) {
        ret = (*binop)(inflist_at(list, start), arg);

        if ((end < start) && (list->step < 0)) {
            for (i = start - 1; i >= end; i--) ret = (*binop)(ret, inflist_at(list, i));
        } else if ((start < end) && (list->step > 0)) {
            for (i = start + 1; i <= end; i++) ret = (*binop)(ret, inflist_at(list, i));
        }
    }
    
    return ret;
}



/* Apply a unary operator to every element in the list */
void inflist_map_unop_func(inflist* list, _il_func unop) {
    if ((list == NULL) || (unop == NULL)) return;

    _il_add_to_funcs(list, unop);
    _il_add_to_eval_list(list, 2, list->funcs.currSize - 1);
}

void inflist_map_unop_str(inflist* list, const char *unop) {
    inflist_map_unop_func(list, inflist_string_to_unop(unop));
}

/* Apply a binary operator to a number and every element in the list */
void inflist_map_num_binop_func(inflist* list, _inflist_type num, _il_binop binop) {
    if ((list == NULL) || (binop == NULL)) return;

    _il_add_num(list, num);

    _il_add_to_binops(list, binop);
    _il_add_to_eval_list(list, 3, list->binops.currSize - 1);
}

void inflist_map_num_binop_str(inflist* list, _inflist_type num, const char *binop) {
    inflist_map_num_binop_func(list, num, inflist_string_to_binop(binop));
}

/* Apply a binary operator to a function and every element in the list */
void inflist_map_func_binop_func(inflist* list, _il_func func, _il_binop binop) {
    if ((list == NULL) || (binop == NULL)) return;

    _il_add_func(list, func);

    _il_add_to_binops(list, binop);
    _il_add_to_eval_list(list, 3, list->binops.currSize - 1);
}

void inflist_map_func_binop_str(inflist* list, _il_func func, const char *binop) {
    inflist_map_func_binop_func(list, func, inflist_string_to_binop(binop));
}

/* Apply a binary operator to a list and every element in the list */
void inflist_map_list_binop_func(inflist* list, inflist* otherList, _il_binop binop) {
    if ((list == NULL) || (otherList == NULL) || (binop == NULL)) return;

    _il_add_list(list, otherList);

    _il_add_to_binops(list, binop);
    _il_add_to_eval_list(list, 3, list->binops.currSize - 1);
}

void inflist_map_list_binop_str(inflist* list, inflist* otherList, const char *binop) {
    inflist_map_list_binop_func(list, otherList, inflist_string_to_binop(binop));
}



/* Convert the string representation of a unary operator into its functional form */
_il_func inflist_string_to_unop(const char *unop) {
    return (strcmp(unop, "++") == 0) ? inflist_inc_op :
           (strcmp(unop, "--") == 0) ? inflist_dec_op :
           NULL;
}

/* Convert the string representation of a binary operator into its functional form */
_il_binop inflist_string_to_binop(const char *binop) {
    return (strcmp(binop, "+") == 0) ? inflist_add_op : 
           (strcmp(binop, "-") == 0) ? inflist_sub_op : 
           (strcmp(binop, "*") == 0) ? inflist_mult_op : 
           (strcmp(binop, "/") == 0) ? inflist_div_op : 
           (strcmp(binop, "%") == 0) ? inflist_mod_op : 
           (strcmp(binop, "&") == 0) ? inflist_and_op : 
           (strcmp(binop, "|") == 0) ? inflist_or_op : 
           (strcmp(binop, "^") == 0) ? inflist_xor_op : 
           (strcmp(binop, ">>") == 0) ? inflist_rshift_op : 
           (strcmp(binop, "<<") == 0) ? inflist_lshift_op : 
           (strcmp(binop, "max") == 0) ? inflist_max_op : 
           (strcmp(binop, "min") == 0) ? inflist_min_op : 
           NULL;
}



/* Operators */
_inflist_type inflist_inc_op(_inflist_type x) {
    return ++x;
}

_inflist_type inflist_dec_op(_inflist_type x) {
    return --x;
}

_inflist_type inflist_add_op(_inflist_type x1, _inflist_type x2) { 
    return x1 + x2; 
}

_inflist_type inflist_sub_op(_inflist_type x1, _inflist_type x2) { 
    return x1 - x2; 
}

_inflist_type inflist_mult_op(_inflist_type x1, _inflist_type x2) { 
    return x1 * x2; 
}

_inflist_type inflist_div_op(_inflist_type x1, _inflist_type x2) { 
    return x1 / x2; 
}

_inflist_type inflist_and_op(_inflist_type x1, _inflist_type x2) { 
    return (_inflist_type) ((int) x1 & (int) x2); 
}

_inflist_type inflist_or_op(_inflist_type x1, _inflist_type x2) { 
    return (_inflist_type) ((int) x1 | (int) x2); 
}

_inflist_type inflist_xor_op(_inflist_type x1, _inflist_type x2) { 
    return (_inflist_type) ((int) x1 ^ (int) x2); 
}

_inflist_type inflist_mod_op(_inflist_type x1, _inflist_type x2) { 
    return (_inflist_type) ((int) x1 % (int) x2); 
}

_inflist_type inflist_rshift_op(_inflist_type x1, _inflist_type x2) { 
    return (_inflist_type) ((int) x1 >> (int) x2); 
}

_inflist_type inflist_lshift_op(_inflist_type x1, _inflist_type x2) { 
    return (_inflist_type) ((int) x1 << (int) x2); 
}

_inflist_type inflist_max_op(_inflist_type x1, _inflist_type x2) {
    return (x1 > x2) ? x1 : x2;
}

_inflist_type inflist_min_op(_inflist_type x1, _inflist_type x2) {
    return (x1 < x2) ? x1 : x2;
}

#endif
