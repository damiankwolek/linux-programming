#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


const char* fifo_path = "/tmp/my_fifo";


void clean(int sig)
{
    struct stat fifo_stat;
    if(stat(fifo_path, &fifo_stat)>=0)
    {
        unlink(fifo_path);
        printf("Pipe %s unlinked\n", fifo_path);
    }
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, clean);

    

    printf("Enter a number from the range [0-%u]: ", UINT32_MAX);
    uint32_t value;
    scanf("%u", &value);

    printf("%s module input = %u\n", argv[0], value);

    if(value == UINT32_MAX)
        printf("%s module: uint32_t overflow\n", argv[0]);

    ++value;

    printf("%s module output = %u\n", argv[0], value);

    struct stat fifo_stat;

    if(stat(fifo_path, &fifo_stat) < 0)
    {
        mkfifo(fifo_path, 0666);
        printf("Pipe %s created\n", fifo_path);
    }

    if(fork() == 0)
    {
        const char* tmp_argv[4];
        tmp_argv[0] = "argument";
        tmp_argv[1] = fifo_path;
        char value_str[15];
        snprintf(value_str, 15, "%u", value);
        tmp_argv[2] = value_str;
        tmp_argv[3] = NULL;
        
        execv("./argument", tmp_argv);
        exit(EXIT_SUCCESS);
    }

    char pipe_command[30];

    snprintf(pipe_command, 30, "./pipe %s", fifo_path);
    int system_ret = system(pipe_command);

     if(WIFSIGNALED(system_ret) && WTERMSIG(system_ret) == SIGINT)
     {
         clean(0);
         exit(EXIT_FAILURE);
     }

    clean(0);

    exit(EXIT_SUCCESS);
}
