#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define FILE_NAME "access_log_Jul95"
struct SRequest
{
    char* remote_addr;
    time_t local_time;
    char* request;
    unsigned short status;
    char bytes_send[10];

};

struct SWindow
{
    time_t opening;
    unsigned amount;
    time_t closing;
};

void getuntil(char** str, char stop_c, FILE* f)
{
    char cur;
    fscanf(f, "%c", &cur);
    int size = 0;
    while ((cur != stop_c) && (!feof(f)))
    {
        fscanf(f, "%c", &cur);
        ++size;
    }
    if (feof(f))
    {
        return 1;
    }
    fseek(f, -(size + 1), SEEK_CUR);
    if (*str == NULL)
        *str = (char*)malloc(size + 1);
    else
        *str = (char*)realloc(*str, size + 1);
    fgets(*str, size + 1, f);
    fseek(f, 1, SEEK_CUR);
}

int getmonth(char* mnth)
{
    int mnth_num;
    if (!strcmp(mnth, "Jan"))
        mnth_num = 0;
    else if (!strcmp(mnth, "Feb"))
        mnth_num = 1;
    else if (!strcmp(mnth, "Mar"))
        mnth_num = 2;
    else if (!strcmp(mnth, "Apr"))
        mnth_num = 3;
    else if (!strcmp(mnth, "May"))
        mnth_num = 4;
    else if (!strcmp(mnth, "Jun"))
        mnth_num = 5;
    else if (!strcmp(mnth, "Jul"))
        mnth_num = 6;
    else if (!strcmp(mnth, "Aug"))
        mnth_num = 7;
    else if (!strcmp(mnth, "Sep"))
        mnth_num = 8;
    else if (!strcmp(mnth, "Oct"))
        mnth_num = 9;
    else if (!strcmp(mnth, "Nov"))
        mnth_num = 10;
    else if (!strcmp(mnth, "Dec"))
        mnth_num = 11;
    return mnth_num;
}

void print_request(struct SRequest req)
{
    printf("%s\n %s %s\n %u %s\n", req.remote_addr, ctime(&(req.local_time)), req.request, req.status, req.bytes_send);
}

void scan_request(FILE* f, struct SRequest* req)
{
    getuntil(&(req->remote_addr), ' ', f);
    struct tm lt;
    char str_mnth[4];
    if (!feof(f))
        fscanf(f, "- - [%d/%c%c%c/%d", &lt.tm_mday, &str_mnth[0], &str_mnth[1], &str_mnth[2], &lt.tm_year);
    lt.tm_mon = getmonth(str_mnth);
    lt.tm_year -= 1900;
    fscanf(f, ":%d:%d:%d -0400] \"", &lt.tm_hour, &lt.tm_min, &lt.tm_sec);
    lt.tm_isdst = 1;
    req->local_time = mktime(&lt);
    getuntil(&(req->request), '"', f);
    fscanf(f, "%hu", &(req->status));
    fgets(req->bytes_send, 10, f);
}

void print_window(struct SWindow window)
{
    printf("\n%s", ctime(&window.opening));
    printf("%s", ctime(&window.closing));
    printf("amount = %u\n", window.amount);
}

int main()
{
    FILE* f = fopen(FILE_NAME, "r");
    if (!f)
    {
        perror("File opening failed");
        return 1;
    }
    unsigned amount = 0;
    unsigned max_amount = 0;
    int pos;
    double dif;
    scanf("%lf", &dif);
    int size = (int)dif * 10;
    time_t* reqwin = (time_t*)malloc(sizeof(time_t) * size);
    int cur = 0;
    int opening = 0;
    struct SRequest ex;
    ex.remote_addr = NULL;
    ex.request = NULL;
    struct SWindow cur_window;
    struct SWindow max_window;
    max_window.amount = 0;
    while (!feof(f))
    {
        scan_request(f, &ex);
        // список с ошибкой 5xx
        if (ex.status / 100 == 5)
            printf("%s %s\n", ctime(&(ex.local_time)), ex.request);
        // поиск временного окна
        reqwin[cur] = ex.local_time;
        ++amount;
        if (amount > size)
            printf("owerflow");
        if (difftime(reqwin[cur], reqwin[opening]) >= dif)
        {
            if ((amount - 1) > max_window.amount)
            {
                max_window.amount = amount - 1;
                max_window.opening = reqwin[opening];
                max_window.closing = reqwin[cur - 1];
            }
            while(difftime(reqwin[cur], reqwin[opening]) >= dif)
            {
                opening = (opening + 1) % size;
                --amount;
            }
        }
        cur = (cur + 1) % size;

    }
    print_window(max_window);
    free(ex.request);
    free(ex.remote_addr);
    fclose(f);
    return 0;
}
