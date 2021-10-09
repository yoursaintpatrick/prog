#include <stdio.h>
#include <string.h>

void string_count(FILE * f)
{
    char cur = '_';
    while ((cur != '\n') && (!feof(f)))
    {
        fscanf(f, "%c", &cur);
    }
}

void word_count(FILE * f)
{
    char cur = '_';
    while ((cur != ' ') && (cur != '\n') && (!feof(f)))
    {
        fscanf(f, "%c", &cur);
    }
}

int main(int argc, char * argv[])
{
    char* fname = argv[argc - 1];
    FILE * f = fopen(fname, "r");
    int cnt = 0;
    if ((strcmp(argv[1], "-b")  && strcmp(argv[1], "--bytes")) == 0)
    {
       fseek(f, 0, SEEK_END);
       cnt = ftell(f) + 1;
    }
    else
    while (!feof(f))
    {
       if ((strcmp(argv[1], "-l") && strcmp(argv[1], "--lines"))  == 0)
           string_count(f);
       else if ((strcmp(argv[1], "-w")  && strcmp(argv[1], "--words"))== 0)
           word_count(f);
       else
          return 1;
       ++cnt;
    }
    printf("%d\n", cnt - 1);
    fclose(f);
    return 0;
}
