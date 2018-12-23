#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


void sigint_handler(int sig)
{
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, sigint_handler);

    printf("Podaj liczbę z zakresu [0-%u]: ", UINT32_MAX);
    uint32_t value;
    scanf("%u", &value);

    if(value == UINT32_MAX)
        printf("Calculation error: uint32_t overflow\n");

    ++value;

    char command[30];
    snprintf(command, 30, "./argument %u", value);

    system(command);

    return 0;
}
