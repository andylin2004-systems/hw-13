#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct pop_entry{
    int year;
    int population;
    char boro[15];
};

int countLines(char* string){
    int i = 0;
    int total = 1;
    while (string[i])
    {
        if (string[i] == '\n'){
            total++;
        }
    }
    return total;
}

void create(char* string){
    int year, man, bk, qns, bx, si;
    int start = 0;
    char *addrLineEnd = strchr(string, '\n');
    int length = addrLineEnd - string;
    char line[100];
    while (addrLineEnd)
    {
        strncpy(line, string+start, length);
        line[length] = '\0';
        printf("%s\n", line);
        sscanf(line, "%d,%d,%d,%d,%d,%d", &year, &man, &bk, &qns, &bx, &si);

        start += length + 1;
        addrLineEnd = strchr(string + start, '\n');
        length = addrLineEnd - (string + start);
    }
    
}

int read_csv()
{
    int year, man, bk, qns, bx, si;
    char doc[1000];
    FILE *file = open("nyc_pop.csv", 0644);
    read(file, doc, sizeof(doc));
    create(doc);
}

int main(){
    read_csv();
}