#include <stdio.h>

void main(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("Usage: jlox [script]");
        return 1;
    }
    else if (argc == 2)
    {
        runFile(argv[1]);
    }
    else
    {
        runPrompt();
    }
}