
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
typedef struct splitted
{
    char *str;
    struct splitted *next;

} splitted_t;

typedef struct
{
    char **slices;
    int len;
} result_t;

char *test(char *string, char *pattern)
{
    char *pointer = strstr(string, pattern);

    if (pointer != NULL)
    {
        *pointer = '\0';
        pointer += strlen(pattern);
    }

    return pointer;
}

splitted_t *_split(char *string, char *pattern)
{
    int len = strlen(string);

    char *clone = (char *)malloc(sizeof(char) * strlen(string));
    strcpy(clone, string);
    splitted_t *result = (splitted_t *)malloc(sizeof(splitted_t));
    splitted_t *walker = result;

    walker->str = string;
    walker->next = malloc(sizeof(splitted_t));
    walker = walker->next;

    char *tail = test(string, pattern);

    while (tail != NULL && tail < &string[len])
    {
        walker->str = tail;
        walker->next = malloc(sizeof(splitted_t));
        walker = walker->next;
        tail = test(tail, pattern);
    }

    walker = NULL;

    free(clone);
    return result;
}

result_t *result_new()
{

    return malloc(sizeof(result_t));
}

void result_ctor(result_t *obj)
{
    obj->slices = malloc(sizeof(char *));
    obj->len = 0;
}

int split(char *string, char *pattern, char **result_final)
{
    result_t *result = result_new();
    result->slices = result_final;
    // result_ctor(result);
    result->len = 0;

    splitted_t *slices = _split(string, pattern);
    splitted_t *walker = slices;

    for (; walker->next != NULL; result->len++)
    {
        result->slices[result->len] = malloc(strlen(walker->str) + 1);
        strcpy(result->slices[result->len], walker->str);
        walker = walker->next;
    }

    // free(result);

    return result->len;
}
