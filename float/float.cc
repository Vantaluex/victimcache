#include "float.h"
#include <bitset>
using namespace std;

// Class constructor
_float_::_float_(void) : data(0) { /* Nothing to do */ }

// Class constructor
_float_::_float_(const float &v) : data(*((unsigned*)&v)) { /* Nothing to do */ }

// Class constructor (private)
_float_::_float_(const unsigned d) : data(d) { /* Nothing to do */ }
   
// Class copy constructor
_float_::_float_(const _float_ &f) : data(f.data) { /* Nothing to do */ }

// Class destructor
_float_::~_float_(void) { /* Nothing to do */ }

// cout << operator
ostream& operator<<(ostream &os, const _float_ &f) { os << *((float*)&f.data); return os; }

// Assignment = operator
_float_& _float_::operator=(const float &v) { data = *((unsigned*)&v); return *this; }

// Assignment = operator
_float_& _float_::operator=(const _float_ &f) { data = f.data; return *this; }

// Unary - operator
_float_ _float_::operator-(void) { return _float_(data ^ (unsigned(1) << (exp_bits + frac_bits))); }





/************************************************************
 * EEE3530 Assignment #3: Floating-point numbers            *
 * Implement the floating-point add, subtract, multiply,    *
 * and divide operators below. The functions should perform *
 * bit-level operations to produce the results.             *
 ************************************************************/

// Add + operator
_float_ _float_::operator+(const _float_ &y) {
    /********************************************************
     * EEE3530 Assignment #3                                *
     * Implement the floating-point add function.           *
     ********************************************************/

    // An example to extract the sign, exponent, and fraction of x (*this).
    bool x_sign = data >> (exp_bits + frac_bits);
    int  x_exp  = (data & exp_mask) >> frac_bits;
    int x_frac = data & frac_mask;
    if (x_exp != 0 && x_exp != 255)  x_frac |= (1 << frac_bits); // add implicit 1, unless the exponent is 0 or 255
    
    // An example to extract the sign, exponent, and fraction of y (arg).
    bool y_sign = y.data >> (exp_bits + frac_bits);
    int  y_exp  = (y.data & exp_mask) >> frac_bits;
    int y_frac = y.data & frac_mask;
    if (y_exp != 0 && y_exp != 255)  y_frac |= (1 << frac_bits);// add implicit 1, unless the exponent is 0 or 255


    int sign; //initialize output values these will be the values used for the final output.
    int exp;
    int frac;

    if(x_exp == 255 || y_exp == 255){
        exp = 255;
        bool xory = (x_exp == 255) ? 1 : 0; // check which one is inf or nan. x or y?
        if(xory){ // if its x, we take x value as output
            sign = x_sign;
            frac = x_frac;
        }
        else{ // if its y, we take y value as output
            sign = y_sign;
            frac = y_frac;
        }
    }
    else if(x_sign == y_sign){ // if same sign, just add normally
        sign = x_sign; // set sign to whatever input sign
        exp = (x_exp > y_exp) ? x_exp : y_exp; // set exponent to the greater exponent
        frac = (x_exp > y_exp) ? (x_frac + (y_frac >> (x_exp - y_exp))) : (y_frac + (x_frac >> (y_exp - x_exp))); // simple addition of the fraction bits, after shifting the smaller fraction bit
    } 
    else{
        if(x_exp == y_exp){
            sign = (x_frac > y_frac) ? x_sign : y_sign;
            if(x_frac == y_frac){
                _float_ r;
                return r.data = 0; // output is 0 if the sign is different and values are the same
            }
            else{
                exp = x_exp; // set exponent to whatever exponent, since they're the same
                frac = (x_exp > y_exp) ? (x_frac - (y_frac >> (x_exp - y_exp))) : (y_frac - (x_frac >> (y_exp - x_exp))); // same thing as simple addition, except we're subtracting
            }
        }
        else{
            sign = (x_exp > y_exp) ? x_sign : y_sign;
            exp = (x_exp > y_exp) ? x_exp : y_exp; // same exponent changer as addition
            frac = (x_exp > y_exp) ? (x_frac - (y_frac >> (x_exp - y_exp))) : (y_frac - (x_frac >> (y_exp - x_exp))); // same thing as simple addition, except we're subtracting
        }
    }
    // The code from here on will adjust the exponent, as it could have shifted from addition or subtraction
    int j = 1 << (frac_bits + 2);  // set j = 2^25, in case of overflow
    if(frac > 0 && exp != 255){
        for(int i = 0; i < 24; i++){ // shift the exponent based on the MSB of the fraction output at the moment.
            if(frac >= j){
                if (frac == j && exp == 0) {
                    exp += 1;
                }
                exp += 2 - i;
                if(exp < 0){ // exponent cannot be negative, so just let it go. It just means that the value is very small
                    exp = 0;
                    break;
                }
                frac = (2 - i < 0) ? frac << (i - 2) : frac >> (2 - i); // shift the fraction based on how much we shifted the exponent
                break;
            }
            j = j >> 1;
        }
    }
    if(exp == 1){
        frac = frac << 1; // there was a bug with test case 10 where the bit shifting was not working properly with exp = 1. This was used to fix that
    }
    frac = frac & frac_mask; //format the exponent
    // Put the calculated sign, exponent, and fraction into r.data.
    _float_ r;
    r.data = ((sign  << (exp_bits + frac_bits)) + (exp <<  (frac_bits)) + frac);
    return r;
}

// Subtract - operator
_float_ _float_::operator-(const _float_ &y) {
    /********************************************************
     * EEE3530 Assignment #3                                *
     * Implement the floating-point subtract function.      *
     ********************************************************/
    _float_ ny(y);
    ny.data = y.data ^ (unsigned(1) << (exp_bits + frac_bits));
    return operator+(ny);
}

// Multiply * operator
_float_ _float_::operator*(const _float_ &y) {
    /********************************************************
     * EEE3530 Assignment #3                                *
     * Implement the floating-point multiply function.      *
     ********************************************************/
    bool x_sign = data >> (exp_bits + frac_bits);
    int  x_exp  = (data & exp_mask) >> frac_bits;
    int x_frac = data & frac_mask;
    if (x_exp != 0 && x_exp != 255)  x_frac |= (1 << frac_bits); // add implicit 1, unless the exponent is 0 or 255
    
    // An example to extract the sign, exponent, and fraction of y (arg).
    bool y_sign = y.data >> (exp_bits + frac_bits);
    int  y_exp  = (y.data & exp_mask) >> frac_bits;
    int y_frac = y.data & frac_mask;
    if (y_exp != 0 && y_exp != 255)  y_frac |= (1 << frac_bits);// add implicit 1, unless the exponent is 0 or 255

    int sign = x_sign ^ y_sign;; //initialize output values these will be the values used for the final output.
    int exp;
    int frac = 0;
    exp = x_exp + y_exp - 127;

    if(x_exp + x_frac == 0 || y_exp + y_frac == 0 || exp < 1){ // case where either value is 0, or both values are very small
        _float_ r;
        return r.data = 0; // immediately return 0 as multiplication of any number with 0 is 0
    }
    if(x_exp == 255 || y_exp == 255){
        exp = 255;
        bool xory = (x_exp == 255) ? 1 : 0; // check which one is inf or nan. x or y?
        if(xory){ // if its x, we take x value as output
            frac = x_frac;
        }
        else{ // if its y, we take y value as output
            frac = y_frac;
        }
    }
    else{
        int t1 = 1 << (frac_bits + 1); // set these two values to extract the 1 and 0s from y_frac
        int t2 = 1 << (frac_bits);
        for(int i = 0; i < frac_bits + 1; i++){ // multiplication function
            int ta = (x_frac * ((y_frac % t1) / t2)) >> (i); // shifting to check each bit to see if its 1 or 0, and shifting
            frac += ta; // adding the value to the final fraction
            t1 = t1 >> 1;
            t2 = t2 >> 1;
        }
    }
    
    // The code from here on will adjust the exponent, as it could have shifted from addition or subtraction
    int j = 1 << (frac_bits + 2);  // set j = 2^25, in case of overflow
    if(frac > 0 && exp != 255){
        for(int i = 0; i < 24; i++){ // shift the exponent based on the MSB of the fraction output at the moment.
            if(frac >= j){
                if (frac == j && exp == 0) {
                    exp += 1;
                }
                exp += 2 - i;
                if(exp < 0){ // exponent cannot be negative, so just let it go. It just means that the value is very small
                    exp = 0;
                    break;
                }
                frac = (2 - i < 0) ? frac << (i - 2) : frac >> (2 - i); // shift the fraction based on how much we shifted the exponent
                break;
            }
            j = j >> 1;
        }
    }
    frac = frac & frac_mask; //format the exponent
    // Put the calculated sign, exponent, and fraction into r.data.
    _float_ r;
    r.data = ((sign  << (exp_bits + frac_bits)) + (exp <<  (frac_bits)) + frac);
    return r;
}

// Divide / operator
_float_ _float_::operator/(const _float_ &y) {
    /********************************************************
     * EEE3530 Assignment #3                                *
     * Implement the floating-point division function.      *
     ********************************************************/
    bool x_sign = data >> (exp_bits + frac_bits);
    int  x_exp  = (data & exp_mask) >> frac_bits;
    int x_frac = data & frac_mask;
    if (x_exp != 0 && x_exp != 255)  x_frac |= (1 << frac_bits); // add implicit 1, unless the exponent is 0 or 255
    
    // An example to extract the sign, exponent, and fraction of y (arg).
    bool y_sign = y.data >> (exp_bits + frac_bits);
    int  y_exp  = (y.data & exp_mask) >> frac_bits;
    int y_frac = y.data & frac_mask;
    if (y_exp != 0 && y_exp != 255)  y_frac |= (1 << frac_bits); // add implicit 1, unless the exponent is 0 or 255

    int sign = x_sign ^ y_sign; //take XOR of sign
    int exp = x_exp - y_exp + 127; // calculate the exponent for the result
    int frac = 0;

    if(x_exp + x_frac == 0 || y_exp + y_frac == 0){ // case where either value is 0
        _float_ r;
        r.data = (sign << (exp_bits + frac_bits)) | (255 << frac_bits); //return infinity with correct sign
        return r;
    }
    if (x_exp == 255 || y_exp == 255) { // handle infinity and NaN cases (just in case)
        _float_ r;
        exp = 255;
        bool xory = (x_exp == 255) ? 1 : 0; // check which one is inf or nan. x or y?
        if(xory){ // if its x, we take x value as output
            frac = x_frac;
        } else{ // if its y, we take y value as output
             frac = y_frac;
        }
        r.data = (sign << (exp_bits + frac_bits)) | (exp << frac_bits) | frac;
        return r;
    }

    long long int dividend = (long long int)x_frac << frac_bits; // scale the dividend and divisor to use the available bits effectively
    long long int divisor = y_frac;
    
    for (int i = frac_bits; i >= 0; i--) { // binary long division to get the fraction bits
        if (dividend >= (divisor << i)) {
            dividend -= (divisor << i);
            frac |= (1 << i);
        }
    }
    // The code from here on will adjust the exponent, as it could have shifted from addition or subtraction
    int j = 1 << (frac_bits + 2);  // set j = 2^25, in case of overflow
    if(frac > 0 && exp != 255){
        for(int i = 0; i < 24; i++){ // shift the exponent based on the MSB of the fraction output at the moment.
            if(frac >= j){
                if (frac == j && exp == 0) {
                    exp += 1;
                }
                exp += 2 - i;
                if(exp < 0){ // exponent cannot be negative, so just let it go. It just means that the value is very small
                    exp = 0;
                    break;
                }
                frac = (2 - i < 0) ? frac << (i - 2) : frac >> (2 - i); // shift the fraction based on how much we shifted the exponent
                break;
            }
            j = j >> 1;
        }
    }
    frac = frac & frac_mask; //format the exponent
    
    // Put the calculated sign, exponent, and fraction into r.data.
    _float_ r;
    r.data = (sign << (exp_bits + frac_bits)) | (exp << frac_bits) | frac;
    return r;
}
