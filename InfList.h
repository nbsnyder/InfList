/*
InfList: the Infinite List Library for C++
Written by Nathan Snyder
*/

#ifndef INFLIST_H
#define INFLIST_H

#include <vector>
#include <variant>

template<class T>
class InfList {
public:
    typedef T(*_il_func)(T);
    typedef T(*_il_binop)(T,T);
    typedef std::variant<T, _il_func, const InfList<T>> _il_types_noop;

    // Constructors
    InfList();
    InfList(T start);
    InfList(T start, T step);
    InfList(_il_func func);
    InfList(_il_func func, T start);
    InfList(_il_func func, T start, T step);

    // Getters and Setters
    T getStart() const;
    T getStep() const;
    void setStart(T start);
    void setStep(T step);

    // Operator overloads with no parameters
    const InfList<T>& operator++();
    const InfList<T>& operator--();
    InfList<T> operator++(int x);
    InfList<T> operator--(int x);

    // operator overloads that accept T, _il_func, and const InfList<T>
    const InfList<T>& operator+=(_il_types_noop obj);
    const InfList<T>& operator-=(_il_types_noop obj);
    const InfList<T>& operator*=(_il_types_noop obj);
    const InfList<T>& operator/=(_il_types_noop obj);
    const InfList<T>& operator&=(_il_types_noop obj);
    const InfList<T>& operator|=(_il_types_noop obj);
    const InfList<T>& operator^=(_il_types_noop obj);
    const InfList<T>& operator%=(_il_types_noop obj);
    const InfList<T>& operator>>=(_il_types_noop obj);
    const InfList<T>& operator<<=(_il_types_noop obj);
    const InfList<T>& operator+(_il_types_noop obj) const;
    const InfList<T>& operator-(_il_types_noop obj) const;
    const InfList<T>& operator*(_il_types_noop obj) const;
    const InfList<T>& operator/(_il_types_noop obj) const;
    const InfList<T>& operator&(_il_types_noop obj) const;
    const InfList<T>& operator|(_il_types_noop obj) const;
    const InfList<T>& operator^(_il_types_noop obj) const;
    const InfList<T>& operator%(_il_types_noop obj) const;
    const InfList<T>& operator>>(_il_types_noop obj) const;
    const InfList<T>& operator<<(_il_types_noop obj) const;

    // Public methods
    T valAt(T x) const;

    std::vector<T> first(T num) const;
    std::vector<T> first(T num, T step) const;
    T sumFirst(T num) const;
    T sumFirst(T num, T step) const;

    std::vector<T> range(T start, T end) const;
    std::vector<T> range(T start, T end, T step) const;
    T sumRange(T start, T end) const;
    T sumRange(T start, T end, T step) const;

    T start; // default = 0
    T step; // default = 1

    // List of functions to make the list
    std::vector<std::pair<int, int>> evalList;
    std::vector<T> nums;
    std::vector<_il_func> funcs;
    std::vector<_il_func> unops;
    std::vector<_il_binop> binops;

    // Binary operators
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

    // Unary operators
    static T inc_op(T x);
    static T dec_op(T x);

    const InfList<T>& addUnOp(_il_func unop);
    const InfList<T>& addBinOp(_il_types_noop obj, _il_binop func_op);

};


// Constructors

template<class T> InfList<T>::InfList() : start(0), step(1) {
    nums.push_back((T) 0);
    evalList.push_back(std::pair<int, int>(0, 0));
}

template<class T> InfList<T>::InfList(T start) : start(start), step(1) {
    nums.push_back((T) 0);
    evalList.push_back(std::pair<int, int>(0, 0));
}

template<class T> InfList<T>::InfList(T start, T step) : start(start) {
    setStep(step);
    nums.push_back((T) 0);
    evalList.push_back(std::pair<int, int>(0, 0));
}

template<class T> InfList<T>::InfList(_il_func func) : start(0), step(1) {
    funcs.push_back(func);
    evalList.push_back(std::pair<int, int>(1, 0));
}

template<class T> InfList<T>::InfList(_il_func func, T start) : start(start), step(1) {
    funcs.push_back(func);
    evalList.push_back(std::pair<int, int>(1, 0));
}

template<class T> InfList<T>::InfList(_il_func func, T start, T step) : start(start) {
    setStep(step);
    funcs.push_back(func);
    evalList.push_back(std::pair<int, int>(1, 0));
}




// Getters and Setters

template<class T> T InfList<T>::getStart() const {
    return this->start;
}

template<class T> T InfList<T>::getStep() const {
    return this->step;
}

template<class T> void InfList<T>::setStart(T start) {
    this->start = start;
}

template<class T> void InfList<T>::setStep(T step) {
    this->step = (step == 0) ? 1 : step;
}



// Operator overloads with no parameters

template<class T> const InfList<T>& InfList<T>::operator++() {
    return addUnOp(InfList<T>::inc_op);
}

template<class T> const InfList<T>& InfList<T>::operator--() { 
    return addUnOp(InfList<T>::dec_op);
}

template<class T>
InfList<T> InfList<T>::operator++(int x) {
    return (this->operator++());
}

template<class T>
InfList<T> InfList<T>::operator--(int x) {
    return (this->operator--());
}



// operator overloads

template<class T> const InfList<T>& InfList<T>::operator+=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::add_op);
}

template<class T> const InfList<T>& InfList<T>::operator-=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::sub_op);
}

template<class T> const InfList<T>& InfList<T>::operator*=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::mult_op);
}

template<class T> const InfList<T>& InfList<T>::operator/=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::div_op);
}

template<class T> const InfList<T>& InfList<T>::operator&=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::and_op);
}

template<class T> const InfList<T>& InfList<T>::operator|=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::or_op);
}

template<class T> const InfList<T>& InfList<T>::operator^=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::xor_op);
}

template<class T> const InfList<T>& InfList<T>::operator%=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::mod_op);
}

template<class T> const InfList<T>& InfList<T>::operator>>=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::rshift_op);
}

template<class T> const InfList<T>& InfList<T>::operator<<=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::lshift_op);
}

template<class T> const InfList<T>& InfList<T>::operator+(_il_types_noop obj) const {
    return InfList(*this) += obj;
}

template<class T> const InfList<T>& InfList<T>::operator-(_il_types_noop obj) const {
    return InfList(*this) -= obj;
}

template<class T> const InfList<T>& InfList<T>::operator*(_il_types_noop obj) const {
    return InfList(*this) *= obj;
}

template<class T> const InfList<T>& InfList<T>::operator/(_il_types_noop obj) const {
    return InfList(*this) /= obj;
}

template<class T> const InfList<T>& InfList<T>::operator&(_il_types_noop obj) const {
    return InfList(*this) &= obj;
}

template<class T> const InfList<T>& InfList<T>::operator|(_il_types_noop obj) const {
    return InfList(*this) |= obj;
}

template<class T> const InfList<T>& InfList<T>::operator^(_il_types_noop obj) const {
    return InfList(*this) ^= obj;
}

template<class T> const InfList<T>& InfList<T>::operator%(_il_types_noop obj) const {
    return InfList(*this) %= obj;
}

template<class T> const InfList<T>& InfList<T>::operator>>(_il_types_noop obj) const {
    return InfList(*this) >>= obj;
}

template<class T> const InfList<T>& InfList<T>::operator<<(_il_types_noop obj) const {
    return InfList(*this) <<= obj;
}



// Public methods

template<class T> T InfList<T>::valAt(T x) const {
    std::vector<T> evalStack;
    T val1, val2;
    for (std::pair elem : evalList) {
        switch (elem.first) {
            case 0: // nums
                evalStack.push_back(nums[elem.second]);
                break;
            case 1: // funcs
                evalStack.push_back(funcs[elem.second](x));
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

template<class T> std::vector<T> InfList<T>::first(T num) const {
    return range(this->start, this->start + (num * this->step) - 1);
}

template<class T> std::vector<T> InfList<T>::first(T num, T step) const {
    if (step == 0) step = this->step;
    return range(this->start, this->start + (num * step) - 1, step);
}

template<class T> T InfList<T>::sumFirst(T num) const {
    return sumRange(this->start, this->start + (num * step) - 1);
}

template<class T> T InfList<T>::sumFirst(T num, T step) const {
    if (step == 0) step = this->step;
    return sumRange(this->start, this->start + (num * step) - 1, step);
}

template<class T> std::vector<T> InfList<T>::range(T start, T end) const {
    std::vector<T> arr;

    if ((this->step < 0) && (start > end)) {
        for (T i = start; i >= end; i += this->step) arr.push_back(valAt(i));
    } else if (start < end) {
        for (T i = start; i <= end; i += this->step) arr.push_back(valAt(i));
    }

    return arr;
}

template<class T> std::vector<T> InfList<T>::range(T start, T end, T step) const {
    std::vector<T> arr;
    if (step == 0) step = this->step;

    if ((step < 0) && (start >= end)) {
        for (T i = start; i >= end; i += step) arr.push_back(valAt(i));
    } else if (start <= end) {
        for (T i = start; i <= end; i += step) arr.push_back(valAt(i));
    }

    return arr;
}

template<class T> T InfList<T>::sumRange(T start, T end) const {
    T sum = (T) 0;

    if ((this->step < 0) && (start >= end)) {
        for (T i = start; i >= end; i += this->step) sum += valAt(i);
    } else if (start <= end) {
        for (T i = start; i <= end; i += this->step) sum += valAt(i);
    }

    return sum;
}

template<class T> T InfList<T>::sumRange(T start, T end, T step) const {
    T sum = (T) 0;
    if (step == 0) step = this->step;

    if ((step < 0) && (start >= end)) {
        for (T i = start; i >= end; i += step) sum += valAt(i);
    } else if (start <= end) {
        for (T i = start; i <= end; i += step) sum += valAt(i);
    }
    
    return sum;
}



// Binary operators

template<class T> T InfList<T>::add_op(T x1, T x2) { 
    return x2 + x1; 
}

template<class T> T InfList<T>::sub_op(T x1, T x2) { 
    return x2 - x1; 
}

template<class T> T InfList<T>::mult_op(T x1, T x2) { 
    return x2 * x1; 
}

template<class T> T InfList<T>::div_op(T x1, T x2) { 
    return x2 / x1; 
}

template<class T> T InfList<T>::and_op(T x1, T x2) { 
    return x2 & x1; 
}

template<class T> T InfList<T>::or_op(T x1, T x2) { 
    return x2 | x1; 
}

template<class T> T InfList<T>::xor_op(T x1, T x2) { 
    return x2 ^ x1; 
}

template<class T> T InfList<T>::mod_op(T x1, T x2) { 
    return x2 % x1; 
}

template<class T> T InfList<T>::rshift_op(T x1, T x2) { 
    return x2 >> x1; 
}

template<class T> T InfList<T>::lshift_op(T x1, T x2) { 
    return x2 << x1; 
}



// Unary operators
template<class T> 
T InfList<T>::inc_op(T x) {
    return ++x;
}

template<class T> 
T InfList<T>::dec_op(T x) {
    return --x;
}



template<class T>
const InfList<T>& InfList<T>::addUnOp(_il_func unop) {
    this->unops.push_back(unop);
    this->evalList.push_back(std::pair<int, int>(2, unops.size()-1));
    return *this;
}



template<class T> const InfList<T>& InfList<T>::addBinOp(_il_types_noop obj, _il_binop func_op) {
    try {
        for (std::pair elem : std::get<2>(obj).evalList) {
            switch (elem.first) {
                case 0: // nums
                    this->nums.push_back(std::get<2>(obj).nums[elem.second]);
                    this->evalList.push_back(std::pair<int, int>(0, nums.size()-1));
                    break;
                case 1: // funcs
                    this->funcs.push_back(std::get<2>(obj).funcs[elem.second]);
                    this->evalList.push_back(std::pair<int, int>(1, funcs.size()-1));
                    break;
                case 2: // unops
                    this->unops.push_back(std::get<2>(obj).unops[elem.second]);
                    this->evalList.push_back(std::pair<int, int>(2, unops.size()-1));
                    break;
                case 3: // binops
                    this->binops.push_back(std::get<2>(obj).binops[elem.second]);
                    this->evalList.push_back(std::pair<int, int>(3, binops.size()-1));
                    break;
            }
        }
    } catch (const std::exception &e1) {
        try {
            this->funcs.push_back(std::get<1>(obj));
            this->evalList.push_back(std::pair<int, int>(1, funcs.size()-1));
        } catch (const std::exception &e2) {
            this->nums.push_back(std::get<0>(obj));
            this->evalList.push_back(std::pair<int, int>(0, nums.size()-1));
        }
    }

    this->binops.push_back(func_op);
    this->evalList.push_back(std::pair<int, int>(3, binops.size()-1));
    return *this;
}

#endif
