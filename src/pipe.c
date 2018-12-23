#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if(argc != 2)
    {
        printf("%s module: wrong number of call arguments\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    uint32_t value;
    read(fd, &value, sizeof(value));
    close(fd);
    
    printf("%s module input = %u\n", argv[0], value);

    if(value == UINT32_MAX)
        printf("%s module: uint32_t overflow\n", argv[0]);

    ++value;

    printf("%s module output = %u\n", argv[0], value);

    exit(EXIT_SUCCESS);
}
