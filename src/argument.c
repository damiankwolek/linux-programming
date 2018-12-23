#include <stdint.h>
#include <stdlib.h> 
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if(argc != 3)
    {
        printf("%s module: wrong number of call arguments.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    uint32_t value = atoi(argv[2]);

    printf("%s module input = %u\n", argv[0], value);

    if(value == UINT32_MAX)
        printf("%s module: uint32_t overflow\n", argv[0]);

    ++value;
    printf("%s module output = %u\n", argv[0], value);

    int fd = open(argv[1], O_WRONLY);

    if(fd < 0)
        printf("%s module: pipe open failed\n", argv[0]);

    write(fd, &value, sizeof(value));

    close(fd);
    
    exit(EXIT_SUCCESS);
}
