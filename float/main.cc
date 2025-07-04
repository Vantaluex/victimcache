#include "float.h"

using namespace std;

int main(void) {
    /******************************************
     * DO NOT MODIFY THE CODE BELOW THIS LINE *
     ******************************************/
    cout << "======== [Tests for grading] ========" << endl;

    _float_ x, y;
    unsigned t = 0, u = 0;

    // Add two positive numbers.
    x = 4.29;
    y = 0.511;
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add two numbers of different signs.
    x = -100.11;
    y = 5.83;
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add two negative numbers.
    x = -2.38e+3;
    y = -8.923e+2;
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add zero.
    x = 0.0;
    y = -4.82e+1;
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add two large numbers.
    x = 7.428e+31;
    y = 5.29e+29;
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add two denormalized numbers.
    u = 1;                                  // Set the LSB.
    x = *((float*)&u);
    u <<= 2;                                // Set the third LSB
    y = *((float*)&u);
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add max and min.
    u = (0xfe << frac_bits) + frac_mask;    // Max (exponent=254, all fraction bits set)
    x = *((float*)&u);
    u = 1;                                  // Min (exponent=0, LSB of fraction set)
    y = *((float*)&u);
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add two values of opposite signs.
    u = (0xfe << frac_bits) + frac_mask;    // Max (exponent=254, all fraction bits set)
    x = *((float*)&u);
    y = -x;
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add infinity.
    x = 1.34;
    u = 0xff << frac_bits;                  // Infinity (exponent=255, fraction=0)
    y = *((float*)&u);
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add a normalized number and a denormalized number.
    u = 0x1 << frac_bits;                   // Normalized   (exponent=1, fraction=0)
    x = *((float*)&u);
    u = 0x1 << (frac_bits - 1);             // Denormalized (exponent=0, MSB of fraction set)
    y = *((float*)&u);
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Subtract two positive numbers.
    x = 2.475;
    y = 14.34;
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;

    // Subtract the same number.
    x = -9.992;
    y = x;
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;
   
    // Subtract two small numbers.
    x = -9.334e-29;
    y = -2.1e-31;
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;

    // Subtract infinity.
    x = -1.23;
    u = 0xff << frac_bits;                  // Infinity (exponent=255, fraction=0) 
    y = *((float*)&u);
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;

    // Subtract NaN;
    x = 4.56;
    u = 0x1ff << (frac_bits - 1);           // NaN (exponent=255, MSB of fraction set)
    y = *((float*)&u);
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;

    // Subtract a normalized number from a denormalized number.
    u = 0x1 << (frac_bits - 1);             // Denormalized (exponent=0, MSB of fraction set)
    x = *((float*)&u);
    u = 0x1 << frac_bits;                   // Normalized   (exponent=1, fraction=0)
    y = *((float*)&u);
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;

    // Subtract the denormalized numbers of opposite signs.
    u = 0x1 << (frac_bits - 1);             // Denormalized (exponent=0, MSB of fraction set)
    x = *((float*)&u);
    y = -x;
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;

    // Multiply two positive numbers.
    x = 3.28;
    y = 4.1;
    cout << "Test #" << ++t << ": " << x << " * " << y << " = " << x * y << endl;

    // Multiply two negative numbers.
    x = -0.64;
    y = -3.93e+1;
    cout << "Test #" << ++t << ": " << x << " * " << y << " = " << x * y << endl;

    // Multiply two numbers of different signs.
    x = 81.3;
    y = -3.85;
    cout << "Test #" << ++t << ": " << x << " * " << y << " = " << x * y << endl;

    // Multiply zero.
    x = 284.2;
    y = 0.0;
    cout << "Test #" << ++t << ": " << x << " * " << y << " = " << x * y << endl;

    // Multiply two large numbers.
    x = 3.8e+20;
    y = 7.3e+17;
    cout << "Test #" << ++t << ": " << x << " * " << y << " = " << x * y << endl;

    // Multiply two small numbers.
    x = -7.3e-25;
    y = 9.2e-13;
    cout << "Test #" << ++t << ": " << x << " * " << y << " = " << x * y << endl;

    // Multiply two smaller numbers.
    x = 8.2e-31;
    y = 5.2e-25;
    cout << "Test #" << ++t << ": " << x << " * " << y << " = " << x * y << endl;

    // Multiply -infinity.
    u = 0x1ff << frac_bits;                 // -Infinity (sign=1, exponent=255, fraction=0) 
    x = *((float*)&u);
    y = -5.98;
    cout << "Test #" << ++t << ": " << x << " * " << y << " = " << x * y << endl;

    // Divide two positive numbers.
    x = 2.25e+5;
    y = 0.8e+2;
    cout << "Test #" << ++t << ": " << x << " / " << y << " = " << x / y << endl;

    // Divide two numbers of different signs.
    x = 0.0075;
    y = -12.5;
    cout << "Test #" << ++t << ": " << x << " / " << y << " = " << x / y << endl;

    // Divide a small number by a large number.
    x = 8.45e-13;
    y = 2.5e+25;
    cout << "Test #" << ++t << ": " << x << " / " << y << " = " << x / y << endl;

    // Divide a large number by a small number.
    x = 1.5e+27;
    y = -4.5e-11;
    cout << "Test #" << ++t << ": " << x << " / " << y << " = " << x / y << endl;

    // Divide by zero.
    x = -91.5;
    y = 0.0;
    cout << "Test #" << ++t << ": " << x << " / " << y << " = " << x / y << endl;

    cout << "======== [End of tests] ========" << endl;

    /******************************************
     * DO NOT MODIFY THE CODE ABOVE THIS LINE *
     ******************************************/

    return 0;
}

