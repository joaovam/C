#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "strtok.h"

int main(int argc, char **argv)
{
    char **result = malloc(sizeof(char *));
    char string[] = "Hello %% guys %% how are you? %%";

    int len = split(string, "%%", result);

    for (int i = 0; i < len; i++)
    {

        fprintf(stdout, "String:%s\n", result[i]);
    }
    free(result);
}