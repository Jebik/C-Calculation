/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#include <iostream>
#include <chrono>
using namespace std;

char* get_binary_str(unsigned char value)
{
    char* bits = (char*) calloc(9, 8);
    bits[0] = (value & (1 << 7)) >> 7 ? '1' : '0';
    bits[1] = (value & (1 << 6)) >> 6 ? '1' : '0';
    bits[2] = (value & (1 << 5)) >> 5 ? '1' : '0';
    bits[3] = (value & (1 << 4)) >> 4 ? '1' : '0';
    bits[4] = (value & (1 << 3)) >> 3 ? '1' : '0';
    bits[5] = (value & (1 << 2)) >> 2 ? '1' : '0';
    bits[6] = (value & (1 << 1)) >> 1 ? '1' : '0';
    bits[7] = value & 1;
    bits[8] = '\0';
    return bits;
}


bool is_little_endian()
{
    int test = 4278190080;
    unsigned char *i_bytes = (unsigned char*)&test;
    
    if (i_bytes[3] == 255)
    {
        return true;
    }
    else
    {
        return false;
    }
}

class my_float
{
public:
    my_float(float f)
    {
        unsigned char *f_bytes = (unsigned char*)&f;
        
        /* Chercher les if little endian to get that on compilation
        if (is_little_endian())
        {
            if (f_bytes[3] >> 7 == 1)
            {
                cout << "SIGN IS NEG DID NOT PLAN THIS READ CODE\n";
            }
        */
            exponent = f_bytes[3] << 1;
            exponent += f_bytes[2] >> 7;
               
            mantissa[0] = f_bytes[2] << 1 >> 1;
            mantissa[1] = f_bytes[1];
            mantissa[2] = f_bytes[0];
            /*
        }
        else
        {
/*
    Float is a 32-bit IEEE 754 single-precision floating-point number.

    1-bit for the sign, 8-bit for exponent, 23-bit for the value or mantissa.
    
    Let's considere the float 
    f_bytes[0] = 11111111
    f_bytes[1] = 11111111
    f_bytes[2] = 11111111
    f_bytes[3] = 11111111
    
    by decaling 11111111 << 1 we got 1 11111110 the first bit who is sign disapear
    (i assume it's never null i print a warning for ut)
    exponent = f_bytes[0] << 1
    exponent = 11111110
    but last bit is in next char
    f_bytes[1] = 11111111
    exponent += f_bytes[1] >> 7  = 00000001
    exponent = 11111110 + 00000001
    exponent = 11111111 = 255
    
    then mantissa
    f_bytes[1] << 1
    this 11111111 give 11111110 because first bit is already take and was the last of exponent
    RES(f_bytes[1] << 1) >> 1;
    this 11111110 give 01111111 We just remove the first bit and store the rest in our two last bytes
    
    
    DEBUG
            printf("(%d) (%d) (%d) (%d)\n", f_bytes[3], f_bytes[2], f_bytes[1], f_bytes[0]);
        
            printf("(%s) (%s) (%s) (%s)\n",
                get_binary_str(f_bytes[3]), 
                get_binary_str(f_bytes[2]), 
                get_binary_str(f_bytes[1]), 
                get_binary_str(f_bytes[0]));
*/
/*
            if (f_bytes[0] >> 7 == 1)
            {
                cout << "SIGN IS NEG DID NOT PLAN THIS READ CODE\n";
            }
            exponent = f_bytes[0] << 1;
            exponent += f_bytes[1] >> 7;
            
            mantissa[0] = f_bytes[1] << 1 >> 1;
            mantissa[1] = f_bytes[2];
            mantissa[2] = f_bytes[3];
        }
        */
    }
    
    uint32_t mul(const uint32_t& x)
    {
        uint64_t res;
        /*  
            Exponent(exp) go from -127 to 128
            Float is 2^exp * mantissa
            So NUMBER * Float = NUMBER*2^exp* mantissa
            
            RAPPEL OPERATOR DE BIT
            2^exp*NUMBER == NUMBER << exp 
            NUMBER/2^exp == NUMBER >> exp
            So Float * NUMBER = 
            if exp > 128
            {
                NUMBER << (exp-127)
            }
            else
            {
                NUMBER >> (127-exp)
            }
            If float is > 2^32 = 4294967296  float * x = 0 fromage
        */
        /*
        if (exponent >= 128)
        {*/
            res = (uint64_t)(x) << (exponent-127);
            res = multiply_mantissa(res);
            /*
        }
        else
        {
            res = (uint64_t)(x);
            res = multiply_mantissa(res); 
            res = res >> (127-exponent);
        }
        */
        return res;
    }
    
    uint32_t multiply_mantissa(const uint64_t &x)
    {
        uint64_t new_x = x << 32;
        uint64_t res = new_x;
        
        //Mantis one we ignot first bit atm but should make a if later
        res += ((mantissa[0] & (1 << 6)) >> 6) * new_x >>1;
        res += ((mantissa[0] & (1 << 5)) >> 5) * new_x >>2;
        res += ((mantissa[0] & (1 << 4)) >> 4) * new_x >>3;
        res += ((mantissa[0] & (1 << 3)) >> 3) * new_x >>4;
        res += ((mantissa[0] & (1 << 2)) >> 2) * new_x >>5;
        res += ((mantissa[0] & (1 << 1)) >> 1) * new_x >>6;
        res += ((mantissa[0] & (1 << 0)) >> 0) * new_x >>7;
        //Mantis two 
        res += ((mantissa[1] & (1 << 7)) >> 7) * new_x >>8;
        res += ((mantissa[1] & (1 << 6)) >> 6) * new_x >>9;
        res += ((mantissa[1] & (1 << 5)) >> 5) * new_x >>10;
        res += ((mantissa[1] & (1 << 4)) >> 4) * new_x >>11;
        res += ((mantissa[1] & (1 << 3)) >> 3) * new_x >>12;
        res += ((mantissa[1] & (1 << 2)) >> 2) * new_x >>13;
        res += ((mantissa[1] & (1 << 1)) >> 1) * new_x >>14;
        res += ((mantissa[1] & (1 << 0)) >> 0) * new_x >>15;
        //Mantis three
        res += ((mantissa[2] & (1 << 7)) >> 7) * new_x >>16;
        res += ((mantissa[2] & (1 << 6)) >> 6) * new_x >>17;
        res += ((mantissa[2] & (1 << 5)) >> 5) * new_x >>18;
        res += ((mantissa[2] & (1 << 4)) >> 4) * new_x >>19;
        res += ((mantissa[2] & (1 << 3)) >> 3) * new_x >>20;
        res += ((mantissa[2] & (1 << 2)) >> 2) * new_x >>21;
        res += ((mantissa[2] & (1 << 1)) >> 1) * new_x >>22;
        res += ((mantissa[2] & (1 << 0)) >> 0) * new_x >>23;
        
        return res >> 32;
    }
    
private:
    unsigned short exponent;
    uint32_t mantissa[3];
};
/*
=======================
Float is a 32-bit IEEE 754 single-precision floating-point number.

1-bit for the sign, 8-bit for exponent, 23-bit for the value or mantissa.
The size of a float is 4-bytes(32 bit), i.e., a float variable requires 4-bytes of computer memory space.
Float has 6-digits of precision which means we can use up to 6 digits after the decimal; 
otherwise, it will truncate anything after that. For example, 12.4356716 can be stored in a variable using float data type.
 
The C++ program below shows the 6-digit precision of the float variable and truncating digits after that.
=======================
=======================
Double is a 64-bit IEEE 754 double-precision floating-point number.


51 Is first mantiss

    cout << ((raw_data & (1 << 51)) >> 51) ? '1' : '0'; * NUM >> 1
    cout << ((raw_data & (1 << 50)) >> 50) ? '1' : '0'; * NUM >> 2


1-bit for the sign, 11-bit for exponent, 52-bit for the value of mantissa.
Precision is the total number of digits (or significant digits) of a real number.
The size of a double is 8-bytes(64 bit), i.e., a double variable requires 8-bytes of computer memory space.
Double has 15-digits of precision which means the double variable is significant up to 15 decimal digits, 
and hence it will truncate anything after that.  
For example, 12.435671123654328 can be stored in a variable using a double data type.
=======================
*/

void testing_mul(float num_f, uint32_t mul)
{
    cout << "Calculating Approx " << num_f << " * " << mul << "\n";
    auto cpp_start = chrono::high_resolution_clock::now();
    auto cpp_mult = (uint32_t)(num_f * mul);
    auto cpp_end = std::chrono::high_resolution_clock::now();
    auto my_start = chrono::high_resolution_clock::now();
    auto my_mult = my_float(num_f).mul(mul);
    auto my_end = std::chrono::high_resolution_clock::now();
    
    cout << "CPP RES=" << cpp_mult << " <> This took:" << chrono::duration<double, std::milli>(cpp_end-cpp_start).count() << "ms\n";
    cout << "MY  RES=" << my_mult << " <> This took:" << chrono::duration<double, std::milli>(my_end-my_start).count() << "ms\n\n";
}

int main()
{
    testing_mul(3.14159265459, 1000000000);
    testing_mul(3.14159265459, 100000000);
    testing_mul(3.14159265459, 10000000);
    testing_mul(3.14159265459, 1000000);
    testing_mul(3.14159265459, 100000);
    testing_mul(3.14159265459, 10000);
    testing_mul(3.14159265459, 1000);
    testing_mul(3.14159265459, 100);
    testing_mul(3.14159265459, 10);
    testing_mul(3.14159265459, 1);
    testing_mul(3.14159265459, 12);
    testing_mul(3.14159265459, 123);
    testing_mul(3.14159265459, 1234);
    testing_mul(3.14159265459, 12345);
    testing_mul(3.14159265459, 123456);
    testing_mul(3.14159265459, 1234567);
    testing_mul(3.14159265459, 12345678);
    return 0;
}
//38785091.3207

