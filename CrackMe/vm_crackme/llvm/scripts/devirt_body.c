#include <stdio.h>
#include <string.h>


int dispatcher(char *input);

int main(int argc, char **argv) 
{
    char input[64];

    if (fgets(input, sizeof(input), stdin) == NULL) 
    {
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    int result = dispatcher(input);
    if (!result)
    {
        printf("Nice!!! U got it!\n");
    }

    return 0;
}