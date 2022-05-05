#include <iostream>
#include <bitset>
#include <string>
#include <cstring>

int isBigEndian(void)
{
    union
    {
        uint32_t i;
        char c[4];
    } e = {0x01000000};

    return e.c[0];
}

void swap(char *s, int a, int b)
{
    char tmp;

    tmp = s[a];
    s[a] = s[b];
    s[b] = tmp;
}

// Converts string to int taking endianess into account
uint32_t convertLittleEndian(const char *bits, size_t n)
{
    uint32_t ret = 0;
    char *cp = (char *)malloc(n * sizeof(char));

    memcpy(cp, bits, n);
    if (!isBigEndian())
    {
        for (int i = 0; i < n / 2; i++)
            swap(cp, i, n - 1 - i);
    }

    ret = *((uint32_t *)cp);
    free(cp);
    return ret;
}
