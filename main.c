#include <stdio.h>
#include <string.h>
int main(int argc, char * argv[])
{
    char* fname = argv[argc - 1];
    FILE * f = fopen(fname, "r");
    char word[20];
    int cnt = 0;
    if (strcmp(argv[1], "-b") == 0)
    {
       fseek(f, 0, SEEK_END);
       cnt = ftell(f) + 1;
    }
    else
    while (!feof(f))
    {
       if (strcmp(argv[1], "-l") == 0)
          fgets(word, 20, f);
       else if (strcmp(argv[1], "-w") == 0)
          fscanf(f, "%s", word);
       else
          return 1;
       ++cnt;
    }
    printf("%d\n", cnt - 1);
    fclose(f);
    return 0;
}
