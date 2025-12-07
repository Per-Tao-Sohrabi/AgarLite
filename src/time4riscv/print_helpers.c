#include "dtekv-lib.h"

void print_float(float f) {
    int integer_part = (int)f;
    int decimal_part = (int)((f - integer_part) * 100);
    if (decimal_part < 0) decimal_part = -decimal_part;
    
    print_dec(integer_part);
    print(".");
    if (decimal_part < 10) print("0");
    print_dec(decimal_part);
}