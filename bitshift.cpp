/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

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
    bits[7] = value & 1 ? '1' : '0';
    bits[8] = '\0';
    return bits;
}

void print_uint64_data(uint64_t val)
{
    unsigned char *uint_bytes = (unsigned char*)&val;
    printf("(%d) (%d) (%d) (%d) (%d) (%d) (%d) (%d)\n",
        uint_bytes[7], 
        uint_bytes[6], 
        uint_bytes[5], 
        uint_bytes[4], 
        uint_bytes[3], 
        uint_bytes[2], 
        uint_bytes[1], 
        uint_bytes[0]);
    
    printf("(%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s)\n",
        get_binary_str(uint_bytes[7]), 
        get_binary_str(uint_bytes[6]), 
        get_binary_str(uint_bytes[5]), 
        get_binary_str(uint_bytes[4]),
        get_binary_str(uint_bytes[3]), 
        get_binary_str(uint_bytes[2]), 
        get_binary_str(uint_bytes[1]), 
        get_binary_str(uint_bytes[0]));
}

void print_uint8_data(uint8_t val)
{
    unsigned char *uint_bytes = (unsigned char*)&val;
    printf("(%d)\n", uint_bytes[0]);
    printf("(%s)\n", get_binary_str(uint_bytes[0]));
}

void print_float_data(float val)
{
    unsigned char *uint_bytes = (unsigned char*)&val;
    printf("(%d) (%d) (%d) (%d)\n",
        uint_bytes[3], 
        uint_bytes[2], 
        uint_bytes[1], 
        uint_bytes[0]);
    
    printf("(%s) (%s) (%s) (%s)\n",
        get_binary_str(uint_bytes[3]), 
        get_binary_str(uint_bytes[2]), 
        get_binary_str(uint_bytes[1]), 
        get_binary_str(uint_bytes[0]));
}

void print_uint32_data(uint32_t val)
{
    unsigned char *uint_bytes = (unsigned char*)&val;
    printf("(%d) (%d) (%d) (%d)\n",
        uint_bytes[3], 
        uint_bytes[2], 
        uint_bytes[1], 
        uint_bytes[0]);
    
    printf("(%s) (%s) (%s) (%s)\n",
        get_binary_str(uint_bytes[3]), 
        get_binary_str(uint_bytes[2]), 
        get_binary_str(uint_bytes[1]), 
        get_binary_str(uint_bytes[0]));
}

class my_uint_32b_64
{
public:
    my_uint_32b_64(uint32_t pre, uint32_t hi, uint32_t lo)
    {
        _pre = pre;
        _hi = hi;
        _lo = lo;
    }
    
    my_uint_32b_64(uint64_t val)
    {
        unsigned char *uint_bytes = (unsigned char*)&val;
        
        auto hi_bytes  = (unsigned char*)&_hi;
        auto lo_bytes  = (unsigned char*)&_lo;
        
        _pre = 0;
        hi_bytes[3] = uint_bytes[7];
        hi_bytes[2] = uint_bytes[6];
        hi_bytes[1] = uint_bytes[5];
        hi_bytes[0] = uint_bytes[4];
        lo_bytes[3] = uint_bytes[3];
        lo_bytes[2] = uint_bytes[2];
        lo_bytes[1] = uint_bytes[1];
        lo_bytes[0] = uint_bytes[0];
    }
    
    uint64_t convert_back()
    {
        uint64_t res;
        unsigned char *uint_bytes = (unsigned char*)&res;
        auto hi_bytes  = (unsigned char*)&_hi;
        auto lo_bytes  = (unsigned char*)&_lo;
        
        uint_bytes[7] = hi_bytes[3];
        uint_bytes[6] = hi_bytes[2];
        uint_bytes[5] = hi_bytes[1];
        uint_bytes[4] = hi_bytes[0];
        uint_bytes[3] = lo_bytes[3];
        uint_bytes[2] = lo_bytes[2];
        uint_bytes[1] = lo_bytes[1];
        uint_bytes[0] = lo_bytes[0];
        
        return res;
    }
    
    my_uint_32b_64 operator<<=(const int& val)
    {
        auto dec = val%32;
        if (dec != 0)
        {
            _pre = (_pre << dec) + (_hi >> 32-dec);
            _hi = (_hi << dec) + (_lo >> 32-dec);
            _lo = _lo << dec;
        }
        
        if (val >= 32)
        {
            _pre = _hi;
            _hi = _lo;
            _lo = 0;
            
            if (val >= 64)
            {
                _pre = _hi;
                _hi = _lo;
                _lo = 0;
            }
        }
        return *this;
    }
    my_uint_32b_64 operator<<(const int& val)
    {
        auto r = my_uint_32b_64(_pre, _hi, _lo);
        return r <<= val;
    }
    
    my_uint_32b_64 operator>>=(const int& val)
    {
        auto dec = val%32;
        
        if (dec != 0)
        {
            _lo = (_lo >> dec) + (_hi << 32-dec);
            _hi = (_hi >> dec) + (_pre << 32-dec);
            _pre = _pre >> dec;
        }
        
        if (val >= 32)
        {
            _lo = _hi;
            _hi = _pre;
            _pre = 0;
            
            if (val >= 64)
            {
                _lo = _hi;
                _hi = _pre;
                _pre = 0;
            }
        }
        
        return *this;
    }
    my_uint_32b_64 operator>>(const int& val)
    {
        auto r = my_uint_32b_64(_pre, _hi, _lo);
        return r >>= val;
    }
    
private:
    //_pre is for double mult only ignored
    uint32_t _pre;
    uint32_t _hi;
    uint32_t _lo;
};

class my_uint_chr_64
{
public:
    my_uint_chr_64()
    {
    }
    my_uint_chr_64(uint64_t val)
    {
        unsigned char *uint_bytes = (unsigned char*)&val;
        
        data[0] = uint_bytes[7];
        data[1] = uint_bytes[6];
        data[2] = uint_bytes[5];
        data[3] = uint_bytes[4];
        data[4] = uint_bytes[3];
        data[5] = uint_bytes[2];
        data[6] = uint_bytes[1];
        data[7] = uint_bytes[0];
    }
    
    uint64_t convert_back()
    {
        uint64_t res;
        unsigned char *uint_bytes = (unsigned char*)&res;
        
        uint_bytes[0] = data[7];
        uint_bytes[1] = data[6];
        uint_bytes[2] = data[5];
        uint_bytes[3] = data[4];
        uint_bytes[4] = data[3];
        uint_bytes[5] = data[2];
        uint_bytes[6] = data[1];
        uint_bytes[7] = data[0];
        
        return res;
    }
    
    void  recursive_left(const int& val)
    { 
        if (val > 0)
        {
            data[0] = data[1];
            data[1] = data[2];
            data[2] = data[3];
            data[3] = data[4];
            data[4] = data[5];
            data[5] = data[6];
            data[6] = data[7];
            data[7] = 0;
            
            recursive_left(val-8);
        }
    }
    
    void  recursive_right(const int& val)
    { 
        if (val > 0)
        {
            data[7] = data[6];
            data[6] = data[5];
            data[5] = data[4];
            data[4] = data[3];
            data[3] = data[2];
            data[2] = data[1];
            data[1] = data[0];
            data[0] = 0;
            
            recursive_right(val-8);
        }
    }
    
    my_uint_chr_64 operator<<(const int& val)
    {
        auto dec = val%8;
        data[0] = (data[0] << dec) + (data[1] >> 8-dec);
        data[1] = (data[1] << dec) + (data[2] >> 8-dec);
        data[2] = (data[2] << dec) + (data[3] >> 8-dec);
        data[3] = (data[3] << dec) + (data[4] >> 8-dec);
        data[4] = (data[4] << dec) + (data[5] >> 8-dec);
        data[5] = (data[5] << dec) + (data[6] >> 8-dec);
        data[6] = (data[6] << dec) + (data[7] >> 8-dec);
        data[7] = data[7] << dec;
        recursive_left(val-8);
        
        return *this;
    }
    
    my_uint_chr_64 operator>>(const int& val)
    {
        auto dec = val%8;
        data[7] = (data[7] >> dec) + (data[6] << 8-dec);
        data[6] = (data[6] >> dec) + (data[5] << 8-dec);
        data[5] = (data[5] >> dec) + (data[4] << 8-dec);
        data[4] = (data[4] >> dec) + (data[3] << 8-dec);
        data[3] = (data[3] >> dec) + (data[2] << 8-dec);
        data[2] = (data[2] >> dec) + (data[1] << 8-dec);
        data[1] = (data[1] >> dec) + (data[0] << 8-dec);
        data[0] = data[0] >> dec;
        recursive_right(val-8);
        
        return *this;
    }
private:
    unsigned char data[8];
};


















class my_uint_64b_64
{
public:
    my_uint_64b_64(uint32_t pre_hi, uint32_t pre_lo, uint32_t hi, uint32_t lo)
    {
        _pre_hi = pre_hi;
        _pre_lo = pre_lo;
        _hi = hi;
        _lo = lo;
    }
    
    my_uint_64b_64(uint64_t val)
    {
        unsigned char *uint_bytes = (unsigned char*)&val;
        
        auto hi_bytes  = (unsigned char*)&_hi;
        auto lo_bytes  = (unsigned char*)&_lo;
        
        _pre_hi = 0;
        _pre_lo = 0;
        hi_bytes[3] = uint_bytes[7];
        hi_bytes[2] = uint_bytes[6];
        hi_bytes[1] = uint_bytes[5];
        hi_bytes[0] = uint_bytes[4];
        lo_bytes[3] = uint_bytes[3];
        lo_bytes[2] = uint_bytes[2];
        lo_bytes[1] = uint_bytes[1];
        lo_bytes[0] = uint_bytes[0];
    }
    
    uint64_t convert_back()
    {
        uint64_t res;
        unsigned char *uint_bytes = (unsigned char*)&res;
        auto hi_bytes  = (unsigned char*)&_hi;
        auto lo_bytes  = (unsigned char*)&_lo;
        
        uint_bytes[7] = hi_bytes[3];
        uint_bytes[6] = hi_bytes[2];
        uint_bytes[5] = hi_bytes[1];
        uint_bytes[4] = hi_bytes[0];
        uint_bytes[3] = lo_bytes[3];
        uint_bytes[2] = lo_bytes[2];
        uint_bytes[1] = lo_bytes[1];
        uint_bytes[0] = lo_bytes[0];
        
        return res;
    }
    
    my_uint_64b_64 operator<<=(const int& val)
    {
        auto dec = val%32;
        if (dec != 0)
        {
            _pre_hi = (_pre_hi << dec) + (_pre_lo >> 32-dec);
            _pre_lo = (_pre_lo << dec) + (_hi >> 32-dec);
            _hi = (_hi << dec) + (_lo >> 32-dec);
            _lo = _lo << dec;
        }
        
        if (val >= 32)
        {
            _pre_hi = _pre_lo;
            _pre_lo = _hi;
            _hi = _lo;
            _lo = 0;
            
            if (val >= 64)
            {
                 _pre_hi = _pre_lo;
                _pre_lo = _hi;
                _hi = _lo;
                _lo = 0;
                
                if (val >= 96)
                {
                     _pre_hi = _pre_lo;
                    _pre_lo = _hi;
                    _hi = _lo;
                    _lo = 0;
                }
            }
        }
        return *this;
    }
    my_uint_64b_64 operator<<(const int& val)
    {
        auto r = my_uint_64b_64(_pre_hi, _pre_lo, _hi, _lo);
        return r <<= val;
    }
    
    my_uint_64b_64 operator>>=(const int& val)
    {
        auto dec = val%32;
        
        if (dec != 0)
        {
            _lo = (_lo >> dec) + (_hi << 32-dec);
            _hi = (_hi >> dec) + (_pre_lo << 32-dec);
            _pre_lo = (_pre_lo >> dec) + (_pre_hi << 32-dec);
            _pre_hi = _pre_hi >> dec;
        }
        
        if (val >= 32)
        {
            _lo = _hi;
            _hi = _pre_lo;
            _pre_lo = _pre_hi;
            _pre_hi = 0;
            
            if (val >= 64)
            {
                _lo = _hi;
                _hi = _pre_lo;
                _pre_lo = _pre_hi;
                _pre_hi = 0;
                if (val >= 96)
                {
                    _lo = _hi;
                    _hi = _pre_lo;
                    _pre_lo = _pre_hi;
                    _pre_hi = 0;
                }
            }
        }
        
        return *this;
    }
    my_uint_64b_64 operator>>(const int& val)
    {
        auto r = my_uint_64b_64(_pre_hi, _pre_lo, _hi, _lo);
        return r >>= val;
    }
    
private:
    //_pre is for double mult only ignored
    uint32_t _pre_hi;
    uint32_t _pre_lo;
    uint32_t _hi;
    uint32_t _lo;
};


















void dec_test(uint64_t num, short dec, bool left)
{
    uint64_t num_cpp = num;
    auto num_chr = my_uint_chr_64(num);
    auto num_32b = my_uint_32b_64(num);
    auto num_64b = my_uint_64b_64(num);
    
    cout << "\nDOING:" << num << (left ? " << ":" >> ") << dec << "\n";
    
    uint64_t cpp_dec;
    auto chr_dec = my_uint_chr_64(num);
    auto b32_dec = my_uint_32b_64(num);
    auto b64_dec = my_uint_64b_64(num);
    
    auto cpp_start = chrono::high_resolution_clock::now();
    if (left)
    {
        cpp_dec = num_cpp << dec;
    }
    else
    {
        cpp_dec = num_cpp >> dec;
    }
    auto cpp_end = std::chrono::high_resolution_clock::now();
    auto chr_start = chrono::high_resolution_clock::now();
    if (left)
    {
        chr_dec = num_chr << dec;
    }
    else
    {
        chr_dec = num_chr >> dec;
    }
    auto chr_end = std::chrono::high_resolution_clock::now();
    auto b32_start = chrono::high_resolution_clock::now();
    if (left)
    {
        b32_dec = num_32b << dec;
    }
    else
    {
        b32_dec = num_32b >> dec;
    }
    auto b32_end = std::chrono::high_resolution_clock::now();
    
    auto b64_start = chrono::high_resolution_clock::now();
    if (left)
    {
        b64_dec = num_64b << dec;
    }
    else
    {
        b64_dec = num_64b >> dec;
    }
    auto b64_end = std::chrono::high_resolution_clock::now();
    
    cout << "CPP RES=" << cpp_dec << " <> This took:" << chrono::duration<double, std::milli>(cpp_end-cpp_start).count() << "ms\n";
    cout << "CHR RES=" << chr_dec.convert_back() << " <> This took:" << chrono::duration<double, std::milli>(chr_end-chr_start).count() << "ms\n";
    cout << "32B RES=" << b32_dec.convert_back() << " <> This took:" << chrono::duration<double, std::milli>(b32_end-b32_start).count() << "ms\n";
    cout << "64B RES=" << b32_dec.convert_back() << " <> This took:" << chrono::duration<double, std::milli>(b64_end-b64_start).count() << "ms\n";
}

class my_float
{
public:
    my_float()
    {
        unsigned char *data = (unsigned char*)&_data;
        data[0] = 0xFF;
        data[1] = 0xFF;
        data[2] = 0xFF;
        data[3] = 0xFF;
        
    }
    my_float(float f)
    {
        unsigned char *f_bytes = (unsigned char*)&f;
        unsigned char *data = (unsigned char*)&_data;
        
        _data = f;
        
        data[0] = f_bytes[0];
        data[1] = f_bytes[1];
        data[2] = f_bytes[2];
        data[3] = f_bytes[3];
        
        _exponent = 0;
        _exponent |= (_data & (1 << 30)) >> 23;
        _exponent |= (_data & (1 << 29)) >> 22;
        _exponent |= (_data & (1 << 28)) >> 21;
        _exponent |= (_data & (1 << 27)) >> 20;
        _exponent |= (_data & (1 << 26)) >> 19;
        _exponent |= (_data & (1 << 25)) >> 18;
        _exponent |= (_data & (1 << 24)) >> 17;
        _exponent |= (_data & (1 << 23)) >> 16;
        cout << "EXPONENT U8\n";
        print_uint8_data(_exponent);
    }
    
    float convert_back()
    {
        float res;
        unsigned char *f_bytes = (unsigned char*)&res;
        unsigned char *data = (unsigned char*)&_data;
        
        f_bytes[0] = data[0];
        f_bytes[1] = data[1];
        f_bytes[2] = data[2];
        f_bytes[3] = data[3];
        
        return res;
    }
    
    uint32_t raw_data()
    {
        return _data;
    }
    
    void print()
    {
        print_uint32_data(_data);
    }
private:
    uint32_t _data;
    uint8_t _exponent;
};

void float_testing()
{
    float pi = 3.14159265459;
    my_float max;
    my_float my_pi = my_float(pi);
    
    cout << "CPP PI=" << pi;
    cout << "\nMF  PI=" << my_pi.convert_back() << "\n";
    cout << "MF MAX=" << max.convert_back() << "\n";
    my_pi.print();
    max.print();
}

int main()
{
    /*
    float f = 2.308563232421875;
    print_float_data(f);
    cout << "(01000000) (00010011) (10111111) (10000000)\n"; 
    my_float my_f = my_float(f);
    
    cout << "CPP PI=" << f;
    cout << "\nMF  PI=" << my_f.convert_back() << "\n";
    my_f.print();
    auto raw_data = my_f.raw_data();
    cout << "\nBINARY LIST FROM RAW DATA\n";
    cout << (((raw_data & (1 << 31)) >> 31) ? '1' : '0');
    
    cout << (((raw_data & (1 << 30)) >> 30) ? '1' : '0');
    cout << (((raw_data & (1 << 29)) >> 29) ? '1' : '0');
    cout << (((raw_data & (1 << 28)) >> 28) ? '1' : '0');
    cout << (((raw_data & (1 << 27)) >> 27) ? '1' : '0');
    cout << (((raw_data & (1 << 26)) >> 26) ? '1' : '0');
    cout << (((raw_data & (1 << 25)) >> 25) ? '1' : '0');
    cout << (((raw_data & (1 << 24)) >> 24) ? '1' : '0');
    cout << (((raw_data & (1 << 23)) >> 23) ? '1' : '0');
    
    cout << ((raw_data & (1 << 22)) >> 22) ? '1' : '0';
    cout << ((raw_data & (1 << 21)) >> 21) ? '1' : '0';
    cout << ((raw_data & (1 << 20)) >> 20) ? '1' : '0';
    cout << ((raw_data & (1 << 19)) >> 19) ? '1' : '0';
    cout << ((raw_data & (1 << 18)) >> 18) ? '1' : '0';
    cout << ((raw_data & (1 << 17)) >> 17) ? '1' : '0';
    cout << ((raw_data & (1 << 16)) >> 16) ? '1' : '0';
    cout << ((raw_data & (1 << 15)) >> 15) ? '1' : '0';
    cout << ((raw_data & (1 << 14)) >> 14) ? '1' : '0';
    cout << ((raw_data & (1 << 13)) >> 13) ? '1' : '0';
    cout << ((raw_data & (1 << 12)) >> 12) ? '1' : '0';
    cout << ((raw_data & (1 << 11)) >> 11) ? '1' : '0';
    cout << ((raw_data & (1 << 10)) >> 10) ? '1' : '0';
    cout << ((raw_data & (1 << 9)) >> 9) ? '1' : '0';
    cout << ((raw_data & (1 << 8)) >> 8) ? '1' : '0';
    cout << ((raw_data & (1 << 7)) >> 7) ? '1' : '0';
    cout << ((raw_data & (1 << 6)) >> 6) ? '1' : '0';
    cout << ((raw_data & (1 << 5)) >> 5) ? '1' : '0';
    cout << ((raw_data & (1 << 4)) >> 4) ? '1' : '0';
    cout << ((raw_data & (1 << 3)) >> 3) ? '1' : '0';
    cout << ((raw_data & (1 << 2)) >> 2) ? '1' : '0';
    cout << ((raw_data & (1 << 1)) >> 1) ? '1' : '0';
    cout << (raw_data  & 1) ? '1' : '0';
    */
    
    //2181038080
    dec_test(12345678, 1, true);
    dec_test(12345678, 10, true);
    dec_test(12345678, 32, true);
    
    dec_test(12345678, 1, false);
    dec_test(12345678, 10, false);
    dec_test(12345678, 31, false);
    
    dec_test(12345678, 64, true);
    dec_test(12345678, 62, true);
    dec_test(12345678, 40, true);
    
    dec_test(12345678, 64, false);
    dec_test(12345678, 62, false);
    dec_test(12345678, 40, false);
    
    //float_testing();
    return 0;
}