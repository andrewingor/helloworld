#ifndef XSTR
#define XSTR

#include <string>
#include <cstring>

#include <stdexcept>

#include <stdint.h>
#include <intrin.h>
#include <zlib.h>

using namespace std;

namespace hide {

#define SALT     0xA7BC

static const char* B64chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const int B64index [256] = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
   56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
    7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,
    0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
   41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };


class line: public string {
union {
    uint16_t  key16;
    uint8_t   key8;
};

std::string b64encode(const void* data, const size_t len)
{
    unsigned char* p = (unsigned char*)data;
    size_t d = len % 3;
    std::string str64(4 * (int(d > 0) + len / 3), '=');

    for (size_t i = 0, j = 0; i < len - d; i += 3)
    {
        int n = int(p[i]) << 16 | int(p[i + 1]) << 8 | p[i + 2];
        str64[j++] = B64chars[n >> 18];
        str64[j++] = B64chars[n >> 12 & 0x3F];
        str64[j++] = B64chars[n >> 6 & 0x3F];
        str64[j++] = B64chars[n & 0x3F];
    }
    if (d--)    /// padding
    {
        int n = d ? int(p[len - 2]) << 8 | p[len - 1] : p[len - 1];
        str64[str64.size() - 2] = d ? B64chars[(n & 0xF) << 2] : '=';
        str64[str64.size() - 3] = d ? B64chars[n >> 4 & 0x03F] : B64chars[(n & 3) << 4];
        str64[str64.size() - 4] = d ? B64chars[n >> 10] : B64chars[n >> 2];
    }
    return str64;
}

std::string b64decode(const void* data, const size_t len)
{
    unsigned char* p = (unsigned char*)data;
    int pad = len > 0 && (len % 4 || p[len - 1] == '=');
    const size_t L = ((len + 3) / 4 - pad) * 4;
    std::string str(L / 4 * 3 + pad, '\0');

    for (size_t i = 0, j = 0; i < L; i += 4)
    {
        int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
        str[j++] = n >> 16;
        str[j++] = n >> 8 & 0xFF;
        str[j++] = n & 0xFF;
    }
    if (pad)
    {
        int n = B64index[p[L]] << 18 | B64index[p[L + 1]] << 12;
        str[str.size() - 1] = n >> 16;

        if (len > L + 2 && p[L + 2] != '=')
        {
            n |= B64index[p[L + 2]] << 6;
            str.push_back(n >> 8 & 0xFF);
        }
    }
    return str;
}

std::string b64decode(const std::string& str64)
    { return b64decode(str64.c_str(), str64.size()); }

uint16_t    key;

public:
    line() {} 
    line(const char* s) { string::operator=(s); } 
    ~line() {
        key16 = 0;
        this->clear();
    }

    void encode () { 
        key16 = SALT;
        uint8_t     shift;

        for ( string::iterator i = begin(); i != end(); i++ ) { // Got hash shift + sum
            shift = *i & 0x0f;
            key16 = (key16 << shift) | (key16 >> (16 - shift));
            key16 += *i;
        }
        key = key16;

        for ( string::iterator i = begin(); i != end(); i++ ) {
            *i ^= key8;
            key16 = (key16 << 1) | (key16 >> 15);
        }
       key16 = key;
       push_back(key8);
       key16 >>= 8;
       push_back(key8);

        string::operator= ( b64encode(data(), length()) );
    }

    void decode () {
        string::operator= ( b64decode(data(), length()) );

        if(size() < 3) { string::operator=(""); return; }

        key8 = *(end()-1);
        key16 <<= 8;
        string::erase( size() - 1 );
        key8 = *(end()-1);
        string::erase( size() - 1 );

        for ( string::iterator i = begin(); i != end(); i++ ) {
            *i ^= key8;
            key16 = (key16 << 1) | (key16 >> 15);
        }
    }
};


}//namespace hide

#endif