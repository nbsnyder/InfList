# InfList

Infinite List Library for C++
Written by Nathan Snyder

## Install and use

Download InfList.h to your build tree. Make sure to use a C++17 compiler or later.

## Usage examples

### Calculating definite integrals

#### Code

```C++
#include "InfList.h"

double y(double x) { return (2 * x * x) - (3 * x) + 7; }

InfList<double> yIntegral(&y);

std::cout << "Definite integral of y=2x^2-3x+7 in the range [0, 10] by step size:\n";
for (double i = 1.0; i >= 0.000001; i /= 10)
    std::cout << i << " --> " << i * yIntegral.sumRange(0, 10, i) << "\n";
```

---

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



### Calculating constants (like pi) using infinite series

#### Code

```C++
#include "InfList.h"

// Calculate pi using the Leibniz series
double leibnizSeries(double a) {
    return 4 * ((((int) a) % 2) ? -1.0 : 1.0) / ((2.0 * a) + 1.0);
}

InfList<double> calc_pi(&leibnizSeries);

std::cout << "Value of the sum of the Leibniz series by number of terms:\n";
for (double i = 1.0; i <= 1000000.0; i *= 10)
    std::cout << i << " --> " << calc_pi.sumFirst(i) << "\n";
```

---

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

