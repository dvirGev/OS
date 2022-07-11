#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 30; ++i)
    {
        if (!(fork()))
        {
            printf("fork %d\n", i);
            char *args[3] = {"./client", argv[1],  NULL};
            execvp(args[0], args);
        }
    }

    return 0;
}
