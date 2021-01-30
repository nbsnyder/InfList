/*
InfList by Nathan Snyder: This library (which follows the C++17 standard) allows
users to work with and evaluate infinite lists by using a lazy evaluation model.
*/

#ifndef INFLIST_H_
#define INFLIST_H_

#include <string>
#include <vector>
#include <variant>

template<typename T>
class InfList {
    // Type declarations
    typedef T(*_il_func)(T); // Function pointer that takes in a T and returns a T (these can be both functions and unary operators)
    typedef T(*_il_binop)(T,T); // Function pointer that takes in two T's and returns a T (binary operator)
    typedef std::variant<T, _il_func, InfList<T>> _il_types_noop; // Variant of types that functions can take (no operations)

public:
    // Constructors
    InfList();
    InfList(_il_types_noop obj);
    InfList(_il_types_noop obj, T start, T step);

    // Getters and setters
    inline T getStart() const;
    inline T getStep() const;
    inline void setStart(T start);
    inline void setStep(T step);

    // Get the value of the list element at index `x`
    T at(int x) const;

    // Return a vector of the first `num` elements in the list
    inline std::vector<T> first(int num) const;

    // Return the result of the `binop` operation applied to the first `num` elements in the list
    inline T foldFirst(int num, const char *binop) const;
    inline T foldFirst(int num, const char *binop, T arg) const;
    inline T foldFirst(int num, _il_binop binop) const;
    inline T foldFirst(int num, _il_binop binop, T arg) const;

    // Return a vector of the elements in the list between the `start` and `end` indices
    std::vector<T> range(int start, int end) const;

    // Return the result of the `binop` operation applied to the elements in the list between the `start` and `end` indices
    inline T foldRange(int start, int end, const char *binop) const;
    inline T foldRange(int start, int end, const char *binop, T arg) const;
    T foldRange(int start, int end, _il_binop binop) const;
    T foldRange(int start, int end, _il_binop binop, T arg) const;

    // Public methods for applying an operator (either unary or binary) to every element in the list
    inline void map(_il_func unop);
    inline void map(const char *unop);
    inline void map(_il_types_noop obj, _il_binop binop);
    inline void map(_il_types_noop obj, const char *binop);

    // Operator overloads
    T operator[](int x) const;
    const InfList<T>& operator++();
    const InfList<T>& operator--();
    const InfList<T>& operator++(int x); // dummy argument
    const InfList<T>& operator--(int x); // dummy argument
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
    InfList<T> operator+(_il_types_noop obj) const;
    InfList<T> operator-(_il_types_noop obj) const;
    InfList<T> operator*(_il_types_noop obj) const;
    InfList<T> operator/(_il_types_noop obj) const;
    InfList<T> operator&(_il_types_noop obj) const;
    InfList<T> operator^(_il_types_noop obj) const;
    InfList<T> operator|(_il_types_noop obj) const;
    InfList<T> operator%(_il_types_noop obj) const;
    InfList<T> operator>>(_il_types_noop obj) const;
    InfList<T> operator<<(_il_types_noop obj) const;

private:
    T start; // start of the list (default: 0)
    T step; // step of the list (default: 1)
    std::vector<std::pair<int, int>> evalList; // stores pairs of integers that correspond to entries in the next four vectors:
    std::vector<T> nums; // stores numbers (0)
    std::vector<_il_func> funcs; // stores functions (1) and unary operators (2)
    std::vector<_il_binop> binops; // stores binary operators (3)

    // Convert the string representation of an operator into its functional form
    inline static _il_func stringToUnop(const char *unop);
    inline static _il_binop stringToBinop(const char *binop);

    // Private methods for applying an operator (either unary or binary) to every element in the list
    const InfList<T>& addUnOp(_il_func unop);
    const InfList<T>& addBinOp(_il_types_noop obj, _il_binop func_op);

    // Private helper methods for adding objects to the InfList's evaluation list
    void addOp(_il_types_noop obj);
    void addOp_num(T obj);
    void addOp_func(_il_func obj);
    void addOp_list(const InfList<T>& obj);

    // Operator methods
    inline static T inc_op(T x);
    inline static T dec_op(T x);
    inline static T add_op(T x1, T x2);
    inline static T sub_op(T x1, T x2);
    inline static T mult_op(T x1, T x2);
    inline static T div_op(T x1, T x2);
    inline static T and_op(T x1, T x2);
    inline static T or_op(T x1, T x2);
    inline static T xor_op(T x1, T x2);
    inline static T mod_op(T x1, T x2);
    inline static T rshift_op(T x1, T x2);
    inline static T lshift_op(T x1, T x2);
    inline static T max_op(T x1, T x2);
    inline static T min_op(T x1, T x2);
};



// Constructors

template<typename T>
InfList<T>::InfList() : start((T) 0), step((T) 1) {
    addOp((T) 0);
}

template<typename T>
InfList<T>::InfList(_il_types_noop obj) {
    if(auto val = std::get_if<InfList<T>>(&obj)) {
        this->start = val->start;
        this->step = val->step;
    } else {
        this->start = (T) 0;
        this->step = (T) 1;
    }

    addOp(obj);
}

template<typename T>
InfList<T>::InfList(_il_types_noop obj, T start, T step) : start(start) {
    setStep(step);
    addOp(obj);
}



// Getters and setters

template<typename T>
inline T InfList<T>::getStart() const {
    return this->start;
}

template<typename T>
inline T InfList<T>::getStep() const {
    return this->step;
}

template<typename T>
inline void InfList<T>::setStart(T start) {
    this->start = start;
}

template<typename T>
inline void InfList<T>::setStep(T step) {
    this->step = (step == 0) ? 1 : step;
}



// Get the value of the list element at index `x`

template<typename T>
T InfList<T>::at(int x) const {
    std::vector<T> evalStack;
    for (std::pair elem : evalList) {
        T val1, val2;
        switch (elem.first) {
            case 0: // nums
                evalStack.push_back(nums[elem.second]);
                break;
            case 1: // funcs (function)
                evalStack.push_back(funcs[elem.second](start + (step * (T) x)));
                break;
            case 2: // funcs (unary operator)
                val1 = evalStack.back();
                evalStack.pop_back();
                evalStack.push_back(funcs[elem.second](val1));
                break;
            case 3: // binops
                val1 = evalStack.back();
                evalStack.pop_back();
                val2 = evalStack.back();
                evalStack.pop_back();
                evalStack.push_back(binops[elem.second](val2, val1));
                break;
        }
    }

    return evalStack[0];
}



// Return a vector of the first `num` elements in the list

template<typename T>
inline std::vector<T> InfList<T>::first(int num) const {
    return (num < 1) ? (std::vector<T>) {} : range(0, num - 1);
}



// Return the result of the `binop` operation applied to the first `num` elements in the list

template<typename T>
inline T InfList<T>::foldFirst(int num, const char *binop) const {
    return foldFirst(num, stringToBinop(binop));
}

template<typename T>
inline T InfList<T>::foldFirst(int num, const char* binop, T arg) const {
    return foldFirst(num, stringToBinop(binop), arg);
}

template<typename T>
inline T InfList<T>::foldFirst(int num, _il_binop binop) const {
    return (num < 1) ? at(0) : foldRange(0, num - 1, binop);
}

template<typename T>
inline T InfList<T>::foldFirst(int num, _il_binop binop, T arg) const {
    return (num < 1) ? at(0) : foldRange(0, num - 1, binop, arg);
}



// Return a vector of the elements in the list between the `start` and `end` indices

template<typename T>
std::vector<T> InfList<T>::range(int start, int end) const {
    std::vector<T> arr;

    if ((start >= end) && (step < 0)) {
        for (int i = start; i >= end; i--) arr.push_back(at(i));
    }
    else if ((start <= end) && (step > 0)) {
        for (int i = start; i <= end; i++) arr.push_back(at(i));
    }

    return arr;
}



// Return the result of the `binop` operation applied to the elements in the list between the `start` and `end` indices

template<typename T>
inline T InfList<T>::foldRange(int start, int end, const char *binop) const {
    return foldRange(start, end, stringToBinop(binop));
}

template<typename T>
inline T InfList<T>::foldRange(int start, int end, const char *binop, T arg) const {
    return foldRange(start, end, stringToBinop(binop), arg);
}

template<typename T>
T InfList<T>::foldRange(int start, int end, _il_binop binop) const {
    T ret = at(start);

    if ((start > end) && (step < 0)) {
        for (int i = start - 1; i >= end; i--) ret = (*binop)(ret, at(i));
    }
    else if ((start < end) && (step > 0)) {
        for (int i = start + 1; i <= end; i++) ret = (*binop)(ret, at(i));
    }
    
    return ret;
}

template<typename T>
T InfList<T>::foldRange(int start, int end, _il_binop binop, T arg) const {
    T ret = (*binop)(at(start), arg);

    if ((end < start) && (step < 0)) {
        for (int i = start - 1; i >= end; i--) ret = (*binop)(ret, at(i));
    }
    else if ((start < end) && (step > 0)) {
        for (int i = start + 1; i <= end; i++) ret = (*binop)(ret, at(i));
    }
    
    return ret;
}



// Public methods for applying an operator (either unary or binary) to every element in the list

template<typename T>
inline void InfList<T>::map(_il_func unop) {
    addUnOp(unop);
}

template<typename T>
inline void InfList<T>::map(const char *unop) {
    addUnOp(stringToUnop(unop));
}

template<typename T>
inline void InfList<T>::map(_il_types_noop obj, _il_binop binop) {
    addBinOp(obj, binop);
}

template<typename T>
inline void InfList<T>::map(_il_types_noop obj, const char *binop) {
    addBinOp(obj, stringToBinop(binop));
}



// Operator overloads

template<typename T>
T InfList<T>::operator[](int x) const {
    return this->at(x);
}

template<typename T>
const InfList<T>& InfList<T>::operator++() {
    return addUnOp(InfList<T>::inc_op);
}

template<typename T>
const InfList<T>& InfList<T>::operator--() { 
    return addUnOp(InfList<T>::dec_op);
}

template<typename T>
const InfList<T>& InfList<T>::operator++(int x) {
    return (this->operator++());
}

template<typename T>
const InfList<T>& InfList<T>::operator--(int x) {
    return (this->operator--());
}

template<typename T>
const InfList<T>& InfList<T>::operator+=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::add_op);
}

template<typename T>
const InfList<T>& InfList<T>::operator-=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::sub_op);
}

template<typename T>
const InfList<T>& InfList<T>::operator*=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::mult_op);
}

template<typename T>
const InfList<T>& InfList<T>::operator/=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::div_op);
}

template<typename T>
const InfList<T>& InfList<T>::operator&=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::and_op);
}

template<typename T>
const InfList<T>& InfList<T>::operator|=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::or_op);
}

template<typename T>
const InfList<T>& InfList<T>::operator^=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::xor_op);
}

template<typename T>
const InfList<T>& InfList<T>::operator%=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::mod_op);
}

template<typename T>
const InfList<T>& InfList<T>::operator>>=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::rshift_op);
}

template<typename T>
const InfList<T>& InfList<T>::operator<<=(_il_types_noop obj) {
    return addBinOp(obj, InfList<T>::lshift_op);
}

template<typename T>
InfList<T> InfList<T>::operator+(_il_types_noop obj) const {
    return InfList(*this) += obj;
}

template<typename T>
InfList<T> InfList<T>::operator-(_il_types_noop obj) const {
    return InfList(*this) -= obj;
}

template<typename T>
InfList<T> InfList<T>::operator*(_il_types_noop obj) const {
    return InfList(*this) *= obj;
}

template<typename T>
InfList<T> InfList<T>::operator/(_il_types_noop obj) const {
    return InfList(*this) /= obj;
}

template<typename T>
InfList<T> InfList<T>::operator&(_il_types_noop obj) const {
    return InfList(*this) &= obj;
}

template<typename T>
InfList<T> InfList<T>::operator|(_il_types_noop obj) const {
    return InfList(*this) |= obj;
}

template<typename T>
InfList<T> InfList<T>::operator^(_il_types_noop obj) const {
    return InfList(*this) ^= obj;
}

template<typename T>
InfList<T> InfList<T>::operator%(_il_types_noop obj) const {
    return InfList(*this) %= obj;
}

template<typename T>
InfList<T> InfList<T>::operator>>(_il_types_noop obj) const {
    return InfList(*this) >>= obj;
}

template<typename T>
InfList<T> InfList<T>::operator<<(_il_types_noop obj) const {
    return InfList(*this) <<= obj;
}



// Convert the string representation of an operator into its functional form

template<typename T>
inline auto InfList<T>::stringToUnop(const char *unop) -> _il_func {
    std::string str(unop);
    return (str == "++") ? inc_op : 
           (str == "--") ? dec_op : 
           nullptr;
}

template<typename T>
inline auto InfList<T>::stringToBinop(const char *binop) -> _il_binop {
    std::string str(binop);
    return (str == "+") ? add_op : 
           (str == "-") ? sub_op : 
           (str == "*") ? mult_op : 
           (str == "/") ? div_op : 
           (str == "%") ? mod_op : 
           (str == "&") ? and_op : 
           (str == "|") ? or_op : 
           (str == "^") ? xor_op : 
           (str == ">>") ? rshift_op : 
           (str == "<<") ? lshift_op : 
           (str == "max") ? max_op : 
           (str == "min") ? min_op : 
           nullptr;
}



// Private methods for applying an operator (either unary or binary) to every element in the list

template<typename T>
const InfList<T>& InfList<T>::addUnOp(_il_func unop) {
    this->funcs.push_back(unop);
    this->evalList.push_back(std::pair<int, int>(2, funcs.size()-1));
    return *this;
}

template<typename T>
const InfList<T>& InfList<T>::addBinOp(_il_types_noop obj, _il_binop func_op) {
    addOp(obj);

    this->binops.push_back(func_op);
    this->evalList.push_back(std::pair<int, int>(3, binops.size()-1));
    return *this;
}



// Private helper methods for adding objects to the InfList's evaluation list

template<typename T>
void InfList<T>::addOp(_il_types_noop obj) {
    if(auto val = std::get_if<T>(&obj)) {
        addOp_num(*val);
    } else if(auto val = std::get_if<_il_func>(&obj)) {
        addOp_func(*val);
    } else if(auto val = std::get_if<InfList<T>>(&obj)) {
        addOp_list(*val);
    }
}

template<typename T>
void InfList<T>::addOp_num(T obj) {
    this->nums.push_back(obj);
    this->evalList.push_back(std::pair<int, int>(0, nums.size()-1));
}

template<typename T>
void InfList<T>::addOp_func(_il_func obj) {
    this->funcs.push_back(obj);
    this->evalList.push_back(std::pair<int, int>(1, funcs.size()-1));
}

template<typename T>
void InfList<T>::addOp_list(const InfList<T>& obj) {
    for (std::pair elem : obj.evalList) {
        switch (elem.first) {
            case 0: // nums
                this->nums.push_back(obj.nums[elem.second]);
                this->evalList.push_back(std::pair<int, int>(0, nums.size() - 1));
                break;
            case 1: // funcs (function)
                this->funcs.push_back(obj.funcs[elem.second]);
                this->evalList.push_back(std::pair<int, int>(1, funcs.size() - 1));
                break;
            case 2: // funcs (unary operator)
                this->funcs.push_back(obj.funcs[elem.second]);
                this->evalList.push_back(std::pair<int, int>(2, funcs.size() - 1));
                break;
            case 3: // binops
                this->binops.push_back(obj.binops[elem.second]);
                this->evalList.push_back(std::pair<int, int>(3, binops.size() - 1));
                break;
        }
    }
}



// Operator methods

template<typename T> 
inline T InfList<T>::inc_op(T x) {
    return ++x;
}

template<typename T> 
inline T InfList<T>::dec_op(T x) {
    return --x;
}

template<typename T>
inline T InfList<T>::add_op(T x1, T x2) { 
    return x1 + x2;
}

template<typename T>
inline T InfList<T>::sub_op(T x1, T x2) { 
    return x1 - x2;
}

template<typename T>
inline T InfList<T>::mult_op(T x1, T x2) { 
    return x1 * x2;
}

template<typename T>
inline T InfList<T>::div_op(T x1, T x2) { 
    return x1 / x2;
}

template<typename T>
inline T InfList<T>::and_op(T x1, T x2) { 
    return (int) x1 & (int) x2;
}

template<typename T>
inline T InfList<T>::or_op(T x1, T x2) { 
    return (int) x1 | (int) x2;
}

template<typename T>
inline T InfList<T>::xor_op(T x1, T x2) { 
    return (int) x1 ^ (int) x2;
}

template<typename T>
inline T InfList<T>::mod_op(T x1, T x2) { 
    return (int) x1 % (int) x2;
}

template<typename T>
inline T InfList<T>::rshift_op(T x1, T x2) { 
    return (int) x1 >> (int) x2;
}

template<typename T>
inline T InfList<T>::lshift_op(T x1, T x2) { 
    return (int) x1 << (int) x2;
}

template<typename T>
inline T InfList<T>::max_op(T x1, T x2) {
    return (x1 > x2) ? x1 : x2;
}

template<typename T>
inline T InfList<T>::min_op(T x1, T x2) {
    return (x1 < x2) ? x1 : x2;
}

#endif // INFLIST_H_
