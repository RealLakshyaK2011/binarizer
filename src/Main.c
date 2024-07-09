#include <stdio.h>
#include <stdlib.h>

#ifndef AND
    #define AND &&
#endif

void print_string_of_len(char* str, unsigned int len, void (*printf_func)(const char *format, ...))
{
    if(len == 0)
    {
        return;
    }

    char* str_nulled = (char*) malloc(sizeof(char)*len);

    for(int i = 0; i < len; i++)
    {
        if(str[i] == '\n'){
            break;
        }
        if(i == len-1 AND str[i] != '\n'){
            str_nulled[i] = '\n';
            break;
        }

        str_nulled[i] = str[i];
    }
    (*printf_func)("%s", str_nulled);
}