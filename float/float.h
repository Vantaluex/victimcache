#ifndef __FLOAT_H__
#define __FLOAT_H__

#define exp_bits  8                         // # of exponent bits
#define frac_bits 23                        // # of fraction bits

#define exp_bias  127                       // Exponent bias
#define exp_max   255                       // Max of exponent
#define exp_mask  0x7f800000                // Exponent mask
#define frac_mask 0x007fffff                // Fraction mask

#define isNaN(x) (((x & exp_mask) == exp_mask) &&  (x & frac_mask))
#define isInf(x) (((x & exp_mask) == exp_mask) && !(x & frac_mask))

#include <iostream>

class _float_ {
public:
    _float_(void);                          // Class constructor
    _float_(const float &v);                // Class constructor
    _float_(const _float_ &f);              // Class copy constructor
    ~_float_(void);                         // Class destructor

    friend std::ostream& operator<<(std::ostream &os, const _float_ &f);
    _float_& operator=(const float &v);     // Assignment = operator
    _float_& operator=(const _float_ &f);   // Assignment = operator
    _float_ operator-(void);                // Unary - operator
    _float_ operator+(const _float_ &f);    // Add + operator
    _float_ operator-(const _float_ &f);    // Subtract - operator
    _float_ operator*(const _float_ &f);    // Multiply * operator
    _float_ operator/(const _float_ &f);    // Divide / operator

private:
    _float_(const unsigned d);              // Class constructor
    unsigned data;                          // 32-bit data
};

#endif
