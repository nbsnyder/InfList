/*
InfList: the Infinite List Library for C++
Written by Nathan Snyder
*/

#ifndef INFLIST_H
#define INFLIST_H

#include <vector>
#include <variant>

template<typename T, typename U = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class InfList {
private: // Type declarations
    typedef T(*_il_func)(T); // Function pointer that takes in a T and returns a T (these can be both functions and unary operators)
    typedef T(*_il_binop)(T,T); // Function pointer that takes in two T's and returns a T (binary operator)
    typedef std::variant<T, _il_func, const InfList<T, U>> _il_types_noop; // Variant of types that functions can take (no operations)

public: // Constructors
    InfList();
    InfList(_il_types_noop obj);
    InfList(_il_types_noop obj, T start, T step);

private: // Private class fields
    T start; // start of the list (default: 0)
    T step; // step of the list (default: 1)
    std::vector<std::pair<int, int>> evalList; // stores pairs of integers that correspond to entries in the next four vectors:
    std::vector<T> nums;
    std::vector<_il_func> funcs;
    std::vector<_il_func> unops;
    std::vector<_il_binop> binops;

public:
    // Getters and Setters:
    T getStart() const;
    T getStep() const;
    void setStart(T start);
    void setStep(T step);

    T valAt(int x) const;

    std::vector<T> first(int num) const;
    T sumFirst(int num) const;
    T prodFirst(int num) const;

    std::vector<T> range(int start, int end) const;
    T sumRange(int start, int end) const;
    T prodRange(int start, int end) const;

    // Public methods for applying unary and binary operators:
    void apply(_il_func unop);
    void apply(_il_types_noop obj, _il_binop binop);

private:
    // Private methods for applying unary and binary operators:
    const InfList<T, U>& addUnOp(_il_func unop);
    const InfList<T, U>& addBinOp(_il_types_noop obj, _il_binop func_op);

    // Private methods for applying numbers, functions, and other InfLists:
    void addOp(_il_types_noop obj);
    void addOp_num(T obj);
    void addOp_func(_il_func obj);
    void addOp_list(const InfList<T, U>& obj);

public:
    // Operator methods
    static T inc_op(T x);
    static T dec_op(T x);
    static T add_op(T x1, T x2);
    static T sub_op(T x1, T x2);
    static T mult_op(T x1, T x2);
    static T div_op(T x1, T x2);
    static T and_op(T x1, T x2);
    static T or_op(T x1, T x2);
    static T xor_op(T x1, T x2);
    static T mod_op(T x1, T x2);
    static T rshift_op(T x1, T x2);
    static T lshift_op(T x1, T x2);

    // Operator overloads
    InfList<T, U> operator[](T num) const;
    const InfList<T, U>& operator++();
    const InfList<T, U>& operator--();
    InfList<T, U> operator++(int x);
    InfList<T, U> operator--(int x);
    const InfList<T, U>& operator+=(_il_types_noop obj);
    const InfList<T, U>& operator-=(_il_types_noop obj);
    const InfList<T, U>& operator*=(_il_types_noop obj);
    const InfList<T, U>& operator/=(_il_types_noop obj);
    const InfList<T, U>& operator&=(_il_types_noop obj);
    const InfList<T, U>& operator|=(_il_types_noop obj);
    const InfList<T, U>& operator^=(_il_types_noop obj);
    const InfList<T, U>& operator%=(_il_types_noop obj);
    const InfList<T, U>& operator>>=(_il_types_noop obj);
    const InfList<T, U>& operator<<=(_il_types_noop obj);
    InfList<T, U> operator+(_il_types_noop obj) const;
    InfList<T, U> operator-(_il_types_noop obj) const;
    InfList<T, U> operator*(_il_types_noop obj) const;
    InfList<T, U> operator/(_il_types_noop obj) const;
    InfList<T, U> operator&(_il_types_noop obj) const;
    InfList<T, U> operator^(_il_types_noop obj) const;
    InfList<T, U> operator|(_il_types_noop obj) const;
    InfList<T, U> operator%(_il_types_noop obj) const;
    InfList<T, U> operator>>(_il_types_noop obj) const;
    InfList<T, U> operator<<(_il_types_noop obj) const;
};



// Constructors

template<typename T, typename U>
InfList<T, U>::InfList() : start((T) 0), step((T) 1) {
    addOp((T) 0);
}

template<typename T, typename U>
InfList<T, U>::InfList(_il_types_noop obj) : start((T) 0), step((T) 1) {
    addOp(obj);
}

template<typename T, typename U>
InfList<T, U>::InfList(_il_types_noop obj, T start, T step) : start(start) {
    setStep(step);
    addOp(obj);
}



// Public methods

template<typename T, typename U>
T InfList<T, U>::getStart() const {
    return this->start;
}

template<typename T, typename U>
T InfList<T, U>::getStep() const {
    return this->step;
}

template<typename T, typename U>
void InfList<T, U>::setStart(T start) {
    this->start = start;
}

template<typename T, typename U>
void InfList<T, U>::setStep(T step) {
    this->step = (step == 0) ? 1 : step;
}

// x is the index of the array
template<typename T, typename U>
T InfList<T, U>::valAt(int x) const {
    std::vector<T> evalStack;
    T val1, val2;
    for (std::pair elem : evalList) {
        switch (elem.first) {
            case 0: // nums
                evalStack.push_back(nums[elem.second]);
                break;
            case 1: // funcs
                evalStack.push_back(funcs[elem.second](start + (step * (T) x)));
                break;
            case 2: // unops
                val1 = evalStack.back();
                evalStack.pop_back();
                evalStack.push_back(unops[elem.second](val1));
                break;
            case 3: // binops
                val1 = evalStack.back();
                evalStack.pop_back();
                val2 = evalStack.back();
                evalStack.pop_back();
                evalStack.push_back(binops[elem.second](val1, val2));
                break;
        }
    }

    return evalStack[0];
}

template<typename T, typename U>
std::vector<T> InfList<T, U>::first(int num) const {
    return (num < 1) ? (std::vector<T>) {} : range(0, num - 1);
}

template<typename T, typename U>
T InfList<T, U>::sumFirst(int num) const {
    return (num < 1) ? (T) 0 : sumRange(0, num - 1);
}

template<typename T, typename U>
T InfList<T, U>::prodFirst(int num) const {
    return (num < 1) ? (T) 0 : prodRange(0, num - 1);
}

template<typename T, typename U>
std::vector<T> InfList<T, U>::range(int start, int end) const {
    std::vector<T> arr;

    if ((start >= end) && (step < 0)) {
        for (int i = start; i >= end; i--) arr.push_back(valAt(i));
    }
    else if ((start <= end) && (step > 0)) {
        for (int i = start; i <= end; i++) arr.push_back(valAt(i));
    }

    return arr;
}

template<typename T, typename U>
T InfList<T, U>::sumRange(int start, int end) const {
    T sum = (T) 0;

    if ((start >= end) && (step < 0)) {
        for (int i = start; i >= end; i--) sum += valAt(i);
    }
    else if ((start <= end) && (step > 0)) {
        for (int i = start; i <= end; i++) sum += valAt(i);
    }
    
    return sum;
}

template<typename T, typename U>
T InfList<T, U>::prodRange(int start, int end) const {
    T prod = (T) 1;

    if ((start >= end) && (step < 0)) {
        for (int i = start; i >= end; i--) prod *= valAt(i);
    }
    else if ((start <= end) && (step > 0)) {
        for (int i = start; i <= end; i++) prod *= valAt(i);
    }
    
    return prod;
}

template<typename T, typename U>
void InfList<T, U>::apply(_il_func unop) {
    addUnOp(unop);
}

template<typename T, typename U>
void InfList<T, U>::apply(_il_types_noop obj, _il_binop binop) {
    addBinOp(obj, binop);
}



// Private methods

template<typename T, typename U>
const InfList<T, U>& InfList<T, U>::addUnOp(_il_func unop) {
    this->unops.push_back(unop);
    this->evalList.push_back(std::pair<int, int>(2, unops.size()-1));
    return *this;
}

template<typename T, typename U>
const InfList<T, U>& InfList<T, U>::addBinOp(_il_types_noop obj, _il_binop func_op) {
    addOp(obj);

    this->binops.push_back(func_op);
    this->evalList.push_back(std::pair<int, int>(3, binops.size()-1));
    return *this;
}

template<typename T, typename U>
void InfList<T, U>::addOp(_il_types_noop obj) {
    try {
        addOp_num(std::get<0>(obj));
    } catch (const std::exception &e1) {
        try {
            addOp_func(std::get<1>(obj));
        } catch (const std::exception &e2) {
            addOp_list(std::get<2>(obj));
        }
    }
}

template<typename T, typename U>
void InfList<T, U>::addOp_num(T obj) {
    this->nums.push_back(obj);
    this->evalList.push_back(std::pair<int, int>(0, nums.size()-1));
}

template<typename T, typename U>
void InfList<T, U>::addOp_func(_il_func obj) {
    this->funcs.push_back(obj);
    this->evalList.push_back(std::pair<int, int>(1, funcs.size()-1));
}

template<typename T, typename U>
void InfList<T, U>::addOp_list(const InfList<T, U>& obj) {
    for (std::pair elem : obj.evalList) {
        switch (elem.first) {
            case 0: // nums
                this->nums.push_back(obj.nums[elem.second]);
                this->evalList.push_back(std::pair<int, int>(0, nums.size() - 1));
                break;
            case 1: // funcs
                this->funcs.push_back(obj.funcs[elem.second]);
                this->evalList.push_back(std::pair<int, int>(1, funcs.size() - 1));
                break;
            case 2: // unops
                this->unops.push_back(obj.unops[elem.second]);
                this->evalList.push_back(std::pair<int, int>(2, unops.size() - 1));
                break;
            case 3: // binops
                this->binops.push_back(obj.binops[elem.second]);
                this->evalList.push_back(std::pair<int, int>(3, binops.size() - 1));
                break;
        }
    }
}



// Operator methods

template<typename T, typename U> 
T InfList<T, U>::inc_op(T x) {
    return ++x;
}

template<typename T, typename U> 
T InfList<T, U>::dec_op(T x) {
    return --x;
}

template<typename T, typename U>
T InfList<T, U>::add_op(T x1, T x2) { 
    return x2 + x1; 
}

template<typename T, typename U>
T InfList<T, U>::sub_op(T x1, T x2) { 
    return x2 - x1; 
}

template<typename T, typename U>
T InfList<T, U>::mult_op(T x1, T x2) { 
    return x2 * x1; 
}

template<typename T, typename U>
T InfList<T, U>::div_op(T x1, T x2) { 
    return x2 / x1; 
}

template<typename T, typename U>
T InfList<T, U>::and_op(T x1, T x2) { 
    return x2 & x1; 
}

template<typename T, typename U>
T InfList<T, U>::or_op(T x1, T x2) { 
    return x2 | x1; 
}

template<typename T, typename U>
T InfList<T, U>::xor_op(T x1, T x2) { 
    return x2 ^ x1; 
}

template<typename T, typename U>
T InfList<T, U>::mod_op(T x1, T x2) { 
    return (int) x2 % (int) x1; 
}

template<typename T, typename U>
T InfList<T, U>::rshift_op(T x1, T x2) { 
    return (int) x2 >> (int) x1; 
}

template<typename T, typename U>
T InfList<T, U>::lshift_op(T x1, T x2) { 
    return (int) x2 << (int) x1; 
}



// Operator overloads

template<typename T, typename U>
InfList<T, U> InfList<T, U>::operator[](T num) const {
    return this->valAt(num);
}

template<typename T, typename U>
const InfList<T, U>& InfList<T, U>::operator++() {
    return addUnOp(InfList<T, U>::inc_op);
}

template<typename T, typename U>
const InfList<T, U>& InfList<T, U>::operator--() { 
    return addUnOp(InfList<T, U>::dec_op);
}

template<typename T, typename U>
InfList<T, U> InfList<T, U>::operator++(int x) {
    return (this->operator++());
}

template<typename T, typename U>
InfList<T, U> InfList<T, U>::operator--(int x) {
    return (this->operator--());
}

template<typename T, typename U>
const InfList<T, U>& InfList<T, U>::operator+=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T, U>::add_op);
}

template<typename T, typename U>
const InfList<T, U>& InfList<T, U>::operator-=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T, U>::sub_op);
}

template<typename T, typename U>
const InfList<T, U>& InfList<T, U>::operator*=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T, U>::mult_op);
}

template<typename T, typename U>
const InfList<T, U>& InfList<T, U>::operator/=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T, U>::div_op);
}

template<typename T, typename U>
const InfList<T, U>& InfList<T, U>::operator&=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T, U>::and_op);
}

template<typename T, typename U>
const InfList<T, U>& InfList<T, U>::operator|=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T, U>::or_op);
}

template<typename T, typename U>
const InfList<T, U>& InfList<T, U>::operator^=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T, U>::xor_op);
}

template<typename T, typename U>
const InfList<T, U>& InfList<T, U>::operator%=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T, U>::mod_op);
}

template<typename T, typename U>
const InfList<T, U>& InfList<T, U>::operator>>=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T, U>::rshift_op);
}

template<typename T, typename U>
const InfList<T, U>& InfList<T, U>::operator<<=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T, U>::lshift_op);
}

template<typename T, typename U>
InfList<T, U> InfList<T, U>::operator+(_il_types_noop obj) const {
    return InfList(*this) += obj;
}

template<typename T, typename U>
InfList<T, U> InfList<T, U>::operator-(_il_types_noop obj) const {
    return InfList(*this) -= obj;
}

template<typename T, typename U>
InfList<T, U> InfList<T, U>::operator*(_il_types_noop obj) const {
    return InfList(*this) *= obj;
}

template<typename T, typename U>
InfList<T, U> InfList<T, U>::operator/(_il_types_noop obj) const {
    return InfList(*this) /= obj;
}

template<typename T, typename U>
InfList<T, U> InfList<T, U>::operator&(_il_types_noop obj) const {
    return InfList(*this) &= obj;
}

template<typename T, typename U>
InfList<T, U> InfList<T, U>::operator|(_il_types_noop obj) const {
    return InfList(*this) |= obj;
}

template<typename T, typename U>
InfList<T, U> InfList<T, U>::operator^(_il_types_noop obj) const {
    return InfList(*this) ^= obj;
}

template<typename T, typename U>
InfList<T, U> InfList<T, U>::operator%(_il_types_noop obj) const {
    return InfList(*this) %= obj;
}

template<typename T, typename U>
InfList<T, U> InfList<T, U>::operator>>(_il_types_noop obj) const {
    return InfList(*this) >>= obj;
}

template<typename T, typename U>
InfList<T, U> InfList<T, U>::operator<<(_il_types_noop obj) const {
    return InfList(*this) <<= obj;
}

#endif
