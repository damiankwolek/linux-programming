#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>


char fifo_path[] = "/tmp/my_fifo";


void clean(int sig)
{
    struct stat fifo_stat;
    if(stat(fifo_path, &fifo_stat)>=0)
    {
        unlink(fifo_path);
        printf("\nPipe %s unlinked\n", fifo_path);
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, clean);
    char pipe_command[30];
    uint32_t value;

    while(1)
    {
        printf("Enter a number from the range [0-%u]: ", UINT32_MAX);
        
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
            char value_str[15];
            snprintf(value_str, 15, "%u", value);
            char* const tmp_argv[4] = {"argument", fifo_path, value_str, NULL};
        
            execv("./argument", tmp_argv);
            exit(EXIT_SUCCESS);
        }

        snprintf(pipe_command, 30, "./pipe %s", fifo_path);
        int system_ret = system(pipe_command);

        if(WIFSIGNALED(system_ret) && WTERMSIG(system_ret) == SIGINT)
        {
            clean(0);
            exit(EXIT_FAILURE);
        }
    }
}
