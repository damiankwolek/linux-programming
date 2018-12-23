#include <stdint.h>

int main(int argc, char const *argv[])
{
    if(argc < 1)
        return 1;

    uint32_t value = atoi(argv[1]);
    printf("%u\n", ++value);
    return 0;
}
