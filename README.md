# InfList

An infinite List Library for C++. Written by Nathan Snyder.

## Install and use

Download InfList.h and place it in your build tree. The InfList library uses C++17 features, so make sure to compile your project with a C++17 compiler or newer.

## Usage examples

### Calculating definite integrals

#### Code

```C++
// Calculate the definite integral of a function using various step sizes
#include "InfList.h"

double y(double x) {
    return (2.0 * x * x) - (3.0 * x) + 7.0;
}

int main() {
    // Creates an infinite list with the y function
    InfList<double> yIntegral(&y);

    std::cout << "Definite integral of y=2x^2-3x+7 in the range [0, 10] by step size:\n";
    for (double i = 1.0; i >= 0.000001; i /= 10)
        // adds the entries in the list from 0 to 10 with step size i
        std::cout << i << " --> " << i * yIntegral.sumRange(0, 10, i) << "\n";

    return 0;
}
```

#### Output

```
Definite integral of y=2x^2-3x+7 in the range [0, 10] by step size:
1 --> 682
0.1 --> 595.9
0.01 --> 587.587
0.001 --> 586.759
0.0001 --> 586.676
1e-05 --> 586.668
1e-06 --> 586.667
```


---


### Calculating numbers to various precisions using infinite series

#### Code

```C++
// Calculate pi to various precisions using the Leibniz series
#include "InfList.h"

// Function that calculates the nth term in the Leibniz series
// The sum of all the terms in the Leibniz series add up to pi/4
double leibnizSeries(double n) {
    return ((int) n % 2 ? -1.0 : 1.0) / ((2.0 * n) + 1.0);
}

int main() {
    // Creates an infinite list with the leibnizSeries function
    // The list starts from 0 and has a step size of 1
    InfList<double> calc_pi_over_4(&leibnizSeries, 0, 1);

    std::cout << "Value of the sum of the Leibniz series by number of terms:\n";
    for (double i = 1.0; i <= 1000000.0; i *= 10)
        // Adds the first i entries in the list
        std::cout << i << " --> " << 4 * calc_pi_over_4.sumFirst(i) << "\n";

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
1e+06 --> 3.14159
```

