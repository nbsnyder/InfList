# InfList

An infinite list library for C++. Written by Nathan Snyder.

## Install and use

Download InfList.h and place it in your build tree. The InfList library uses C++17 features, so make sure to compile your project with a C++17-compatible compiler or newer.



---



## Usage Examples

### Computing Numbers to Various Precisions

#### Code

```C++
// Calculate pi to various precisions using the Leibniz series and the Wallis product
#include <iostream>
#include "InfList.h"

// Function that calculates the nth term in the Leibniz series
// The sum of all the terms add up to pi/4
double leibnizSeries(double n) {
    return (((int) n % 2 == 0) ? 1.0 : -1.0) / ((2.0 * n) + 1.0);
}

// Function that calculates the nth term in the Wallis product
// The product of all the terms multiply to pi/2
double wallisProduct(double x) {
    return (4 * x * x) / ((4 * x * x) - 1);
}

int main() {
    // Creates an infinite list with the leibnizSeries function
    // The list, by default, starts from 0.0 and has a step size of 1.0
    InfList<double> calc_pi_over_4(&leibnizSeries);

    // Creates an infinite list with the wallisProduct function
    // The list starts from 1.0 and has a step size of 1.0
    InfList<double> calc_pi_over_2(&wallisProduct, 1.0, 1.0);

    std::cout << "Value of the sum of the Leibniz series by number of terms:\n";
    for (int i = 1; i <= 1000000; i *= 10)
        // sumFirst(i) adds the first i entries in the list
        std::cout << i << " --> " << 4 * calc_pi_over_4.sumFirst(i) << "\n";
    
    std::cout << "\n";

    std::cout << "Value of the product of the Wallis product by number of terms:\n";
    for (int i = 1; i <= 1000000; i *= 10)
        // prodFirst(i) multiplies the first i entries in the list
        std::cout << i << " --> " << 2 * calc_pi_over_2.prodFirst(i) << "\n";
    
    return 0;
}
```

#### Output

```
Value of the sum of the Leibniz series by number of terms:
1 --> 4
10 --> 3.04184
100 --> 3.13159
1000 --> 3.14059
10000 --> 3.14149
100000 --> 3.14158
1000000 --> 3.14159

Value of the product of the Wallis product by number of terms:
1 --> 2.66667
10 --> 3.0677
100 --> 3.13379
1000 --> 3.14081
10000 --> 3.14151
100000 --> 3.14158
1000000 --> 3.14159
```



### Calculating Definite Integrals

#### Code

```C++
// Calculate the definite integral of a function using various step sizes
#include <iostream>
#include "InfList.h"

// y = 2x^2 - 3x + 7
double y(double x) {
    return (2.0 * x * x) - (3.0 * x) + 7.0;
}

int main() {
    // Creates an infinite list with the y function
    // The list, by default, starts from 0.0 and has a step size of 1.0
    InfList<double> y_integral(&y);

    std::cout << "Definite integral of y=2x^2-3x+7 in the range [0, 10] by step size:\n";
    for (double i = 1.0; i >= 0.000001; i /= 10) {
        y_integral.setStep(i);
        std::cout << i << " --> " << i * y_integral.sumFirst(10/i) << "\n";
    }

    return 0;
}
```

#### Output

```
Definite integral of y=2x^2-3x+7 in the range [0, 10] by step size:
1 --> 505
0.1 --> 578.2
0.01 --> 585.817
0.001 --> 586.582
0.0001 --> 586.658
1e-05 --> 586.664
1e-06 --> 586.666
```



### Perform Operations on Every List Element

#### Code

```C++
#include <iostream>
#include "InfList.h"

int initialFunc(int x) { return (9 * x) + 2; }
int magicFunc(int x) { return x * (x - 1); }

int main() {
    // Create an infinite list with with the initialFunc function
    // The list, by default, starts from 0 and has a step size of 1
    InfList<int> test1(&initialFunc);

    // Apply magicFunc to every element in the list
    test1.apply(&magicFunc);

    // Perform other operations on the elements in the list
    test1 = (test1 << 1) % 15;
    test1 ^= 0xF;
    
    // Print the first 10 elements ([0..9]) in the list
    std::cout << "[";
    for (int i : test1.first(10))
        std::cout << i << " ";
    std::cout << "\b]\n";

    // Print elements [1..10] in the list
    std::cout << "[";
    for (int i : test1.range(1, 10))
        std::cout << i << " ";
    std::cout << "\b]\n";

    return 0;
}
```

#### Output

```
[11 5 5 11 8 11 5 5 11 8]
[5 5 11 8 11 5 5 11 8 11]
```



---



## Brief Documentation

### Initializing the InfList

The list can be initialized without any arguments. This will make every element in the list 0:
```C++
InfList<int> a; // a = [0, 0, 0, ...]
```

The list can be initilized with a number, function reference, or another InfList:
```C++
InfList<float> b(5.0); // b = [5.0, 5.0, 5.0, ...]
InfList<int> c(&func); // c = [func(0), func(1), func(2), ...]
InfList<float> d(b);   // d = [5.0, 5.0, 5.0, ...]
```

Additionally, you can specify the starting position and the step size of the list. If you don't include the starting position and step size, the defaults are 0 and 1 respectively.
```C++
InfList<int> e(&func, 3, -2); // e = [func(3), func(1), func(-1), ...]
```



### InfList Operations

You can change the starting position and step size at any time:
```C++
InfList<int> f(&intFunc, 0, 1);
f.setStart(1);
f.setStep(2);
// f.getStart() = 1
// f.getStep() = 2
```

InfLists support operations like +, -, *, /, &, |, ^, %, <<, >>, ++, and --. Also, you may specify your own function with the apply() method:
```C++
InfList<double> g(&func1);
g %= 17;
g.apply(&func2);
g = (g++) / 2;
// g = [(func2(func1(0) % 17) + 1) / 2, (func2(func1(1) % 17) + 1) / 2, ...]
```



### Accessing InfList Elements

You can access the value at any space in the list with [] or the valAt() method:
```C++
InfList<int> h(&func, 0, 2);
// h[0] = func(0)
// h[1] = func(2)
// h.valAt(2) = func(4)
```

You can access the first number of elements with the first() method:
```C++
InfList<int> h(&func, 0, 2);
// h.first(3) = [func(0), func(2), func(4)]
```

You can access a range of elements with the range() method:
```C++
InfList<int> h(&func, 0, 2);
// h.range(0, 2) = [func(0), func(2), func(4)]
// h.range(4, 7) = [func(8), func(10), func(12), func(14)]
```

You can get the sum or product of the first() or range() methods:
```C++
InfList<int> h(&func, 0, 2);
// h.prodFirst(3) = func(0) * func(2) * func(4)
// h.sumRange(0, 2) = func(0) + func(2) + func(4)
```

