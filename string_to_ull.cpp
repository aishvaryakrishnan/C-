#include <iostream>
#include <string.h>
#include <math.h>
#include <stdio.h>

using namespace std;

const static size_t SIZEOF_UINT64_T = sizeof(uint64_t);

void encode_in_ull(const char *text, uint64_t *uints)
{
    size_t text_len = strlen(text);
	
    int text_idx = 0;
    int uints_cnt = 0;
    while (text_idx < text_len)
    {
       char buffer[SIZEOF_UINT64_T];
       strncpy(buffer, text + text_idx, SIZEOF_UINT64_T);
       uint64_t x;
       memcpy(&x, &buffer, sizeof(buffer));

       text_idx += SIZEOF_UINT64_T;
            
       uints[uints_cnt++] = x;
    }

    for(auto i = 0; i < uints_cnt; ++i)
    {
       cout << uints[i] << endl;
    }
}

void decode(uint64_t *code)
{
    puts((char*)code);
}

int main()
{
    const char *s = "Hurray, this works, phew!";
    size_t text_len = strlen(s);
    size_t num_uints = ceil (text_len * 1.0 / SIZEOF_UINT64_T);
    uint64_t* code = (uint64_t*)malloc(sizeof(uint64_t[num_uints]));
    
    cout << "code is:" << endl;
    encode_in_ull(s, code);
    
    cout << "decoding:" << endl;
    decode(code);

    free(code);
    
    return 0;
}
