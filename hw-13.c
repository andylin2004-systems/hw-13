#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <strings.h>

struct pop_entry
{
    int year;
    int population;
    char boro[15];
};

int countLines(char *doc)
{
    int i = 0;
    int total = 1;
    while (doc[i])
    {
        if (doc[i] == '\n')
        {
            total++;
        }
        i++;
    }
    return total;
}

void read_csv()
{
    char doc[1000];
    FILE *file = open("nyc_pop.csv", O_RDONLY, 0644);
    read(file, doc, sizeof(doc));
    close(file);

    // start the reading process
    int year;
    int start = 0;
    char line[100];
    int lineCount = 0;
    struct pop_entry *popInfo = malloc(sizeof(struct pop_entry) * (countLines(doc) - 1) * 5);
    char *addrLineEnd = strchr(doc, '\n');
    int length = addrLineEnd - doc;
    while (addrLineEnd)
    {
        if (lineCount != 0){
            strncpy(line, doc + start, length);
            line[length] = '\0';
            sscanf(line, "%d,%d,%d,%d,%d,%d", &year, &(popInfo[(lineCount - 1) * 5].population), &(popInfo[(lineCount - 1) * 5 + 1].population), &(popInfo[(lineCount - 1) * 5 + 2].population), &(popInfo[(lineCount - 1) * 5 + 3].population), &(popInfo[(lineCount - 1) * 5 + 4].population));
            popInfo[(lineCount - 1) * 5].year = year;
            strcpy(popInfo[(lineCount - 1) * 5].boro, "Manhattan");
            popInfo[(lineCount - 1) * 5 + 1].year = year;
            strcpy(popInfo[(lineCount - 1) * 5 + 1].boro, "Brooklyn");
            popInfo[(lineCount - 1) * 5 + 2].year = year;
            strcpy(popInfo[(lineCount - 1) * 5 + 2].boro, "Queens");
            popInfo[(lineCount - 1) * 5 + 3].year = year;
            strcpy(popInfo[(lineCount - 1) * 5 + 3].boro, "Bronx");
            popInfo[(lineCount - 1) * 5 + 4].year = year;
            strcpy(popInfo[(lineCount - 1) * 5 + 4].boro, "Staten Island");
        }
        start += length + 1;
        addrLineEnd = strchr(doc + start, '\n');
        length = addrLineEnd - (doc + start);
        lineCount++;
    }
    strcpy(line, doc + start);
    sscanf(line, "%d,%d,%d,%d,%d,%d", &year, &(popInfo[(lineCount - 1) * 5].population), &(popInfo[(lineCount - 1) * 5 + 1].population), &(popInfo[(lineCount - 1) * 5 + 2].population), &(popInfo[(lineCount - 1) * 5 + 3].population), &(popInfo[(lineCount - 1) * 5 + 4].population));
    popInfo[(lineCount - 1) * 5].year = year;
    strcpy(popInfo[(lineCount - 1) * 5].boro, "Manhattan");
    popInfo[(lineCount - 1) * 5 + 1].year = year;
    strcpy(popInfo[(lineCount - 1) * 5 + 1].boro, "Brooklyn");
    popInfo[(lineCount - 1) * 5 + 2].year = year;
    strcpy(popInfo[(lineCount - 1) * 5 + 2].boro, "Queens");
    popInfo[(lineCount - 1) * 5 + 3].year = year;
    strcpy(popInfo[(lineCount - 1) * 5 + 3].boro, "Bronx");
    popInfo[(lineCount - 1) * 5 + 4].year = year;
    strcpy(popInfo[(lineCount - 1) * 5 + 4].boro, "Staten Island");

    FILE *newFile = open("nyc_pop.bin", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(newFile, popInfo, sizeof(struct pop_entry) * (countLines(doc) - 1) * 5);
    close(newFile);
}

void read_data()
{
    FILE *file = open("nyc_pop.bin", O_RDONLY);
    struct stat fileStat;
    stat("nyc_pop.bin", &fileStat);
    struct pop_entry *boroArray = malloc(fileStat.st_size);
    read(file, boroArray, fileStat.st_size);
    int i;
    for (i = 0; i < fileStat.st_size / sizeof(struct pop_entry); i++)
    {
        printf("%d: year: %d boro: %s pop: %d\n", i, boroArray[i].year, boroArray[i].boro, boroArray[i].population);
    }
    close(file);
}

void add_data()
{
    char input[100];
    printf("Enter info in following format: year boro pop:\n");
    read(STDIN_FILENO, input, sizeof(input));
    FILE *file = open("nyc_pop.bin", O_RDONLY, 0644);
    struct stat fileStat;
    stat("nyc_pop.bin", &fileStat);
    struct pop_entry *boroArray = malloc(fileStat.st_size + sizeof(struct pop_entry));
    read(file, boroArray, fileStat.st_size);
    close(file);

    printf("%d", fileStat.st_size);
    int i;
    for (i = 0; i < fileStat.st_size / sizeof(struct pop_entry); i++)
    {
        printf("%d: year: %d boro: %s pop: %d\n", i, boroArray[i].year, boroArray[i].boro, boroArray[i].population);
    }

    int appendAt = fileStat.st_size / sizeof(struct pop_entry);
    file = open("nyc_pop.bin", O_WRONLY | O_TRUNC);
    sscanf(input, "%d %s %d", &(boroArray[appendAt].year), boroArray[appendAt].boro, &(boroArray[appendAt].population));
    write(file, boroArray, fileStat.st_size + sizeof(struct pop_entry));
    close(file);
}

int main()
{
    read_csv();
    read_data();
    add_data();
    read_data();
}