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
    // Constructors and Destructors
    InfList();
    InfList(int start);
    InfList(int start, int step);
    InfList(T (*func)(int));
    InfList(T (*func)(int), int start);
    InfList(T (*func)(int), int start, int step);
    ~InfList();

    // Getters and Setters
    int getStart() const;
    int getStep() const;
    void setStart(int start);
    void setStep(int step);

    // Operator overloads with no parameters
    const InfList<T>& operator++() const;
    const InfList<T>& operator--() const;

    // Operator overloads with other InfLists
    const InfList<T>& operator+=(const InfList<T>& list);
    const InfList<T>& operator-=(const InfList<T>& list);
    const InfList<T>& operator*=(const InfList<T>& list);
    const InfList<T>& operator/=(const InfList<T>& list);
    const InfList<T>& operator&=(const InfList<T>& list);
    const InfList<T>& operator|=(const InfList<T>& list);
    const InfList<T>& operator^=(const InfList<T>& list);
    const InfList<T>& operator%=(const InfList<T>& list);
    const InfList<T>& operator>>=(const InfList<T>& list);
    const InfList<T>& operator<<=(const InfList<T>& list);
    const InfList<T>& operator+(const InfList<T>& list) const;
    const InfList<T>& operator-(const InfList<T>& list) const;
    const InfList<T>& operator*(const InfList<T>& list) const;
    const InfList<T>& operator/(const InfList<T>& list) const;
    const InfList<T>& operator&(const InfList<T>& list) const;
    const InfList<T>& operator|(const InfList<T>& list) const;
    const InfList<T>& operator^(const InfList<T>& list) const;
    const InfList<T>& operator%(const InfList<T>& list) const;
    const InfList<T>& operator>>(const InfList<T>& list) const;
    const InfList<T>& operator<<(const InfList<T>& list) const;

    // Operator overloads with functions
    const InfList<T>& operator=(T (*func)(int));
    const InfList<T>& operator+=(T (*func)(int));
    const InfList<T>& operator-=(T (*func)(int));
    const InfList<T>& operator*=(T (*func)(int));
    const InfList<T>& operator/=(T (*func)(int));
    const InfList<T>& operator&=(T (*func)(int));
    const InfList<T>& operator|=(T (*func)(int));
    const InfList<T>& operator^=(T (*func)(int));
    const InfList<T>& operator%=(T (*func)(int));
    const InfList<T>& operator>>=(T (*func)(int));
    const InfList<T>& operator<<=(T (*func)(int));
    const InfList<T>& operator+(T (*func)(int)) const;
    const InfList<T>& operator-(T (*func)(int)) const;
    const InfList<T>& operator*(T (*func)(int)) const;
    const InfList<T>& operator/(T (*func)(int)) const;
    const InfList<T>& operator&(T (*func)(int)) const;
    const InfList<T>& operator|(T (*func)(int)) const;
    const InfList<T>& operator^(T (*func)(int)) const;
    const InfList<T>& operator%(T (*func)(int)) const;
    const InfList<T>& operator>>(T (*func)(int)) const;
    const InfList<T>& operator<<(T (*func)(int)) const;

    // Operator overloads with numbers
    const InfList<T>& operator=(T num);
    const InfList<T>& operator+=(T num);
    const InfList<T>& operator-=(T num);
    const InfList<T>& operator*=(T num);
    const InfList<T>& operator/=(T num);
    const InfList<T>& operator&=(T num);
    const InfList<T>& operator|=(T num);
    const InfList<T>& operator^=(T num);
    const InfList<T>& operator%=(T num);
    const InfList<T>& operator>>=(T num);
    const InfList<T>& operator<<=(T num);
    const InfList<T>& operator+(T num) const;
    const InfList<T>& operator-(T num) const;
    const InfList<T>& operator*(T num) const;
    const InfList<T>& operator/(T num) const;
    const InfList<T>& operator&(T num) const;
    const InfList<T>& operator|(T num) const;
    const InfList<T>& operator^(T num) const;
    const InfList<T>& operator%(T num) const;
    const InfList<T>& operator>>(T num) const;
    const InfList<T>& operator<<(T num) const;

    // Public methods
    T valAt(int x) const;

    std::vector<T> first(int num) const;
    std::vector<T> first(int num, int step) const;
    T sumFirst(int num) const;
    T sumFirst(int num, int step) const;

    std::vector<T> range(int start, int end) const;
    std::vector<T> range(int start, int end, int step) const;
    T sumRange(int start, int end) const;
    T sumRange(int start, int end, int step) const;

private:
    int start; // default = 0
    int step; // default = 1
public:
    // List of functions to make the list
    std::vector<std::variant<T, T (*)(int), T (*)(T,T)>> funcs;

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

    // Private methods
    InfList<T>& addOps(const InfList<T> &list, T (*func_op)(T,T));
    InfList<T>& addOps(T (*func)(int), T (*func_op)(T,T));
    InfList<T>& addOps(T num, T (*func_op)(T,T));
};



// Constructors and Destructors

template<class T> InfList<T>::InfList() : start(0), step(1) {
    funcs.push_back(0);
}

template<class T> InfList<T>::InfList(int start) : start(start), step(1) {
    funcs.push_back(0);
}

template<class T> InfList<T>::InfList(int start, int step) : start(start) {
    setStep(step);
    funcs.push_back(0);
}

template<class T> InfList<T>::InfList(T (*func)(int)) : start(0), step(1) {
    funcs.push_back(func);
}

template<class T> InfList<T>::InfList(T (*func)(int), int start) : start(start), step(1) {
    funcs.push_back(func);
}

template<class T> InfList<T>::InfList(T (*func)(int), int start, int step) : start(start) {
    setStep(step);
    funcs.push_back(func);
}

template<class T> InfList<T>::~InfList() {
    funcs.clear();
}



// Getters and Setters

template<class T> int InfList<T>::getStart() const {
    return this->start;
}

template<class T> int InfList<T>::getStep() const {
    return this->step;
}

template<class T> void InfList<T>::setStart(int start) {
    this->start = start;
}

template<class T> void InfList<T>::setStep(int step) {
    this->step = (step == 0) ? 1 : step;
}



// Operator overloads with no parameters

template<class T> const InfList<T>& InfList<T>::operator++() const { 
    return InfList(*this) += 1; 
}

template<class T> const InfList<T>& InfList<T>::operator--() const { 
    return InfList(*this) -= 1; 
}



// Operator overloads with other InfLists

template<class T> const InfList<T>& InfList<T>::operator+=(const InfList<T>& list) { 
    return addOps(list, InfList<T>::add_op); 
}

template<class T> const InfList<T>& InfList<T>::operator-=(const InfList<T>& list) { 
    return addOps(list, InfList<T>::sub_op); 
}

template<class T> const InfList<T>& InfList<T>::operator*=(const InfList<T>& list) { 
    return addOps(list, InfList<T>::mult_op); 
}

template<class T> const InfList<T>& InfList<T>::operator/=(const InfList<T>& list) { 
    return addOps(list, InfList<T>::div_op); 
}

template<class T> const InfList<T>& InfList<T>::operator&=(const InfList<T>& list) { 
    return addOps(list, InfList<T>::and_op); 
}

template<class T> const InfList<T>& InfList<T>::operator|=(const InfList<T>& list) { 
    return addOps(list, InfList<T>::or_op); 
}

template<class T> const InfList<T>& InfList<T>::operator^=(const InfList<T>& list) { 
    return addOps(list, InfList<T>::xor_op); 
}

template<class T> const InfList<T>& InfList<T>::operator%=(const InfList<T>& list) { 
    return addOps(list, InfList<T>::mod_op); 
}

template<class T> const InfList<T>& InfList<T>::operator>>=(const InfList<T>& list) { 
    return addOps(list, InfList<T>::rshift_op); 
}

template<class T> const InfList<T>& InfList<T>::operator<<=(const InfList<T>& list) { 
    return addOps(list, InfList<T>::lshift_op); 
}

template<class T> const InfList<T>& InfList<T>::operator+(const InfList<T>& list) const { 
    return InfList(*this) += list; 
}

template<class T> const InfList<T>& InfList<T>::operator-(const InfList<T>& list) const { 
    return InfList(*this) -= list; 
}

template<class T> const InfList<T>& InfList<T>::operator*(const InfList<T>& list) const { 
    return InfList(*this) *= list; 
}

template<class T> const InfList<T>& InfList<T>::operator/(const InfList<T>& list) const { 
    return InfList(*this) /= list; 
}

template<class T> const InfList<T>& InfList<T>::operator&(const InfList<T>& list) const { 
    return InfList(*this) &= list; 
}

template<class T> const InfList<T>& InfList<T>::operator|(const InfList<T>& list) const { 
    return InfList(*this) |= list; 
}

template<class T> const InfList<T>& InfList<T>::operator^(const InfList<T>& list) const { 
    return InfList(*this) ^= list; 
}

template<class T> const InfList<T>& InfList<T>::operator%(const InfList<T>& list) const { 
    return InfList(*this) %= list; 
}

template<class T> const InfList<T>& InfList<T>::operator>>(const InfList<T>& list) const { 
    return InfList(*this) >>= list; 
}

template<class T> const InfList<T>& InfList<T>::operator<<(const InfList<T>& list) const { 
    return InfList(*this) <<= list; 
}



// Operator overloads with functions

template<class T> const InfList<T>& InfList<T>::operator=(T (*func)(int)) {
    funcs.clear();
    funcs.push_back(func);
    return *this;
}

template<class T> const InfList<T>& InfList<T>::operator+=(T (*func)(int)) { 
    return addOps(func, InfList<T>::add_op); 
}

template<class T> const InfList<T>& InfList<T>::operator-=(T (*func)(int)) { 
    return addOps(func, InfList<T>::sub_op); 
}

template<class T> const InfList<T>& InfList<T>::operator*=(T (*func)(int)) { 
    return addOps(func, InfList<T>::mult_op); 
}

template<class T> const InfList<T>& InfList<T>::operator/=(T (*func)(int)) { 
    return addOps(func, InfList<T>::div_op); 
}

template<class T> const InfList<T>& InfList<T>::operator&=(T (*func)(int)) { 
    return addOps(func, InfList<T>::and_op); 
}

template<class T> const InfList<T>& InfList<T>::operator|=(T (*func)(int)) { 
    return addOps(func, InfList<T>::or_op); 
}

template<class T> const InfList<T>& InfList<T>::operator^=(T (*func)(int)) { 
    return addOps(func, InfList<T>::xor_op); 
}

template<class T> const InfList<T>& InfList<T>::operator%=(T (*func)(int)) { 
    return addOps(func, InfList<T>::mod_op); 
}

template<class T> const InfList<T>& InfList<T>::operator>>=(T (*func)(int)) { 
    return addOps(func, InfList<T>::rshift_op); 
}

template<class T> const InfList<T>& InfList<T>::operator<<=(T (*func)(int)) { 
    return addOps(func, InfList<T>::lshift_op); 
}

template<class T> const InfList<T>& InfList<T>::operator+(T (*func)(int)) const { 
    return InfList(*this) += func; 
}

template<class T> const InfList<T>& InfList<T>::operator-(T (*func)(int)) const { 
    return InfList(*this) -= func; 
}

template<class T> const InfList<T>& InfList<T>::operator*(T (*func)(int)) const { 
    return InfList(*this) *= func; 
}

template<class T> const InfList<T>& InfList<T>::operator/(T (*func)(int)) const { 
    return InfList(*this) /= func; 
}

template<class T> const InfList<T>& InfList<T>::operator&(T (*func)(int)) const { 
    return InfList(*this) &= func; 
}

template<class T> const InfList<T>& InfList<T>::operator|(T (*func)(int)) const { 
    return InfList(*this) |= func; 
}

template<class T> const InfList<T>& InfList<T>::operator^(T (*func)(int)) const { 
    return InfList(*this) ^= func; 
}

template<class T> const InfList<T>& InfList<T>::operator%(T (*func)(int)) const { 
    return InfList(*this) %= func; 
}

template<class T> const InfList<T>& InfList<T>::operator>>(T (*func)(int)) const { 
    return InfList(*this) >>= func; 
}

template<class T> const InfList<T>& InfList<T>::operator<<(T (*func)(int)) const { 
    return InfList(*this) <<= func; 
}



// Operator overloads with numbers

template<class T> const InfList<T>& InfList<T>::operator=(T num) {
    funcs.clear();
    funcs.push_back(num);
    return *this;
}

template<class T> const InfList<T>& InfList<T>::operator+=(T num) { 
    return addOps(num, InfList<T>::add_op); 
}

template<class T> const InfList<T>& InfList<T>::operator-=(T num) { 
    return addOps(num, InfList<T>::sub_op); 
}

template<class T> const InfList<T>& InfList<T>::operator*=(T num) { 
    return addOps(num, InfList<T>::mult_op); 
}

template<class T> const InfList<T>& InfList<T>::operator/=(T num) { 
    return addOps(num, InfList<T>::div_op); 
}

template<class T> const InfList<T>& InfList<T>::operator&=(T num) { 
    return addOps(num, InfList<T>::and_op); 
}

template<class T> const InfList<T>& InfList<T>::operator|=(T num) { 
    return addOps(num, InfList<T>::or_op); 
}

template<class T> const InfList<T>& InfList<T>::operator^=(T num) { 
    return addOps(num, InfList<T>::xor_op); 
}

template<class T> const InfList<T>& InfList<T>::operator%=(T num) { 
    return addOps(num, InfList<T>::mod_op); 
}

template<class T> const InfList<T>& InfList<T>::operator>>=(T num) { 
    return addOps(num, InfList<T>::rshift_op); 
}

template<class T> const InfList<T>& InfList<T>::operator<<=(T num) { 
    return addOps(num, InfList<T>::lshift_op); 
}

template<class T> const InfList<T>& InfList<T>::operator+(T num) const { 
    return InfList(*this) += num; 
}

template<class T> const InfList<T>& InfList<T>::operator-(T num) const { 
    return InfList(*this) -= num; 
}

template<class T> const InfList<T>& InfList<T>::operator*(T num) const { 
    return InfList(*this) *= num; 
}

template<class T> const InfList<T>& InfList<T>::operator/(T num) const { 
    return InfList(*this) /= num; 
}

template<class T> const InfList<T>& InfList<T>::operator&(T num) const { 
    return InfList(*this) &= num; 
}

template<class T> const InfList<T>& InfList<T>::operator|(T num) const { 
    return InfList(*this) |= num; 
}

template<class T> const InfList<T>& InfList<T>::operator^(T num) const { 
    return InfList(*this) ^= num; 
}

template<class T> const InfList<T>& InfList<T>::operator%(T num) const { 
    return InfList(*this) %= num; 
}

template<class T> const InfList<T>& InfList<T>::operator>>(T num) const { 
    return InfList(*this) >>= num; 
}

template<class T> const InfList<T>& InfList<T>::operator<<(T num) const { 
    return InfList(*this) <<= num; 
}



// Public methods

template<class T> T InfList<T>::valAt(int x) const {
    std::vector<T> evalStack;

    for (auto func : funcs) {
        try {
            // try to cast the function to one with two parameters and evaluate it
            T x = (*(std::get<2>(func)))(1,1);

            // if it doesn't throw an error, the function takes two parameters and it's an operation function
            // pop the last two numbers from the stack
            T val1 = evalStack.back();
            evalStack.pop_back();
            T val2 = evalStack.back();
            evalStack.pop_back();
            // and evalute the operation on them
            evalStack.push_back((*(std::get<2>(func)))(val1, val2));
        } catch (const std::exception& e) {
            try {
                // try to cast the function to one with one parameter and evaluate it
                T y = (*(std::get<1>(func)))(1);

                // if it doesn't throw an error, the function takes one parameter and it's a number function
                // evaluate it and put its answer onto the stack
                evalStack.push_back((*(std::get<1>(func)))(x));
            } catch (const std::exception& e) {
                // cast it to a number
                T z = std::get<0>(func);

                // if it doesn't throw an error, function is just a number
                // add it to the stack
                evalStack.push_back(z);
            }
        }
    }

    return evalStack[0];
}

template<class T> std::vector<T> InfList<T>::first(int num) const {
    return range(this->start, this->start + (num * this->step) - 1);
}

template<class T> std::vector<T> InfList<T>::first(int num, int step) const {
    if (step == 0) step = this->step;
    return range(this->start, this->start + (num * step) - 1, step);
}

template<class T> T InfList<T>::sumFirst(int num) const {
    return sumRange(this->start, this->start + (num * step) - 1);
}

template<class T> T InfList<T>::sumFirst(int num, int step) const {
    if (step == 0) step = this->step;
    return sumRange(this->start, this->start + (num * step) - 1, step);
}

template<class T> std::vector<T> InfList<T>::range(int start, int end) const {
    std::vector<T> arr;

    if ((this->step < 0) && (start > end)) {
        for (int i = start; i >= end; i += this->step) arr.push_back(valAt(i));
    } else if (start < end) {
        for (int i = start; i <= end; i += this->step) arr.push_back(valAt(i));
    }

    return arr;
}

template<class T> std::vector<T> InfList<T>::range(int start, int end, int step) const {
    std::vector<T> arr;
    if (step == 0) step = this->step;

    if ((step < 0) && (start >= end)) {
        for (int i = start; i >= end; i += step) arr.push_back(valAt(i));
    } else if (start <= end) {
        for (int i = start; i <= end; i += step) arr.push_back(valAt(i));
    }

    return arr;
}

template<class T> T InfList<T>::sumRange(int start, int end) const {
    T sum = (T) 0;

    if ((this->step < 0) && (start >= end)) {
        for (int i = start; i >= end; i += this->step) sum += valAt(i);
    } else if (start <= end) {
        for (int i = start; i <= end; i += this->step) sum += valAt(i);
    }

    return sum;
}

template<class T> T InfList<T>::sumRange(int start, int end, int step) const {
    T sum = (T) 0;
    if (step == 0) step = this->step;

    if ((step < 0) && (start >= end)) {
        for (int i = start; i >= end; i += step) sum += valAt(i);
    } else if (start <= end) {
        for (int i = start; i <= end; i += step) sum += valAt(i);
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



// Private methods

template<class T> InfList<T>& InfList<T>::addOps(const InfList<T> &list, T (*func_op)(T,T)) {
    for (auto func : list.funcs) this->funcs.push_back(func);
    this->funcs.push_back(func_op);
    return *this;
}

template<class T> InfList<T>& InfList<T>::addOps(T (*func)(int), T (*func_op)(T,T)) {
    this->funcs.push_back(func);
    this->funcs.push_back(func_op);
    return *this;
}

template<class T> InfList<T>& InfList<T>::addOps(T num, T (*func_op)(T,T)) {
    this->funcs.push_back(num);
    this->funcs.push_back(func_op);
    return *this;
}

#endif
