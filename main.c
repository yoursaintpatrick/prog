#include <stdio.h>

int main(int argc, char * argv[])
{
    char* fname = argv[argc - 1];
    FILE * f = fopen(fname, "r");
    char wrd[20];
    int cnt = 0;
    while (!feof(f))
    {
       fgets(wrd, 20, f);
       ++cnt;
    }
    printf("%d", cnt - 1);
    fclose(f);
    return 0;
}
