#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct pop_entry
{
    int year;
    int population;
    char boro[15];
};

int countLines(char *string)
{
    int i = 0;
    int total = 1;
    while (string[i])
    {
        if (string[i] == '\n')
        {
            total++;
        }
        i++;
    }
    return total;
}

struct pop_entry *createArray(char *string)
{
    int year, man, bk, qns, bx, si;
    int start = 0;
    char line[100];
    int lineCount = 0;
    int lineTotal = countLines(string);
    struct pop_entry *popInfo = malloc(sizeof(struct pop_entry) * countLines(string) * 5);
    char *addrLineEnd = strchr(string, '\n');
    int length = addrLineEnd - string;
    while (addrLineEnd)
    {
        strncpy(line, string + start, length);
        line[length] = '\0';
        sscanf(line, "%d,%d,%d,%d,%d,%d", &year, &(popInfo[lineCount * 5].population), &(popInfo[lineCount * 5 + 1].population), &(popInfo[lineCount * 5 + 2].population), &(popInfo[lineCount * 5 + 3].population), &(popInfo[lineCount * 5 + 4].population));
        popInfo[lineCount * 5].year = year;
        strcpy(popInfo[lineCount * 5].boro, "Manhattan");
        popInfo[lineCount * 5 + 1].year = year;
        strcpy(popInfo[lineCount * 5 + 1].boro, "Brooklyn");
        popInfo[lineCount * 5 + 2].year = year;
        strcpy(popInfo[lineCount * 5 + 2].boro, "Queens");
        popInfo[lineCount * 5 + 3].year = year;
        strcpy(popInfo[lineCount * 5 + 3].boro, "Bronx");
        popInfo[lineCount * 5 + 4].year = year;
        strcpy(popInfo[lineCount * 5 + 4].boro, "Staten Island");
        start += length + 1;
        addrLineEnd = strchr(string + start, '\n');
        length = addrLineEnd - (string + start);
        lineCount++;
    }
    strcpy(line, string + start);
    sscanf(line, "%d,%d,%d,%d,%d,%d", &year, &(popInfo[lineCount * 5].population), &(popInfo[lineCount * 5 + 1].population), &(popInfo[lineCount * 5 + 2].population), &(popInfo[lineCount * 5 + 3].population), &(popInfo[lineCount * 5 + 4].population));
    popInfo[lineCount * 5].year = year;
    strcpy(popInfo[lineCount * 5].boro, "Manhattan");
    popInfo[lineCount * 5 + 1].year = year;
    strcpy(popInfo[lineCount * 5 + 1].boro, "Brooklyn");
    popInfo[lineCount * 5 + 2].year = year;
    strcpy(popInfo[lineCount * 5 + 2].boro, "Queens");
    popInfo[lineCount * 5 + 3].year = year;
    strcpy(popInfo[lineCount * 5 + 3].boro, "Bronx");
    popInfo[lineCount * 5 + 4].year = year;
    strcpy(popInfo[lineCount * 5 + 4].boro, "Staten Island");
    return popInfo;
}

int read_csv()
{
    int year, man, bk, qns, bx, si;
    char doc[1000];
    FILE *file = open("nyc_pop.csv", 0644);
    read(file, doc, sizeof(doc));
    struct pop_entry *boroArray = createArray(doc);
    FILE *newFile = open("nyc_pop.bin", O_CREAT | O_WRONLY, 0644);
    write(newFile, boroArray, sizeof(boroArray));
}

int main()
{
    read_csv();
}