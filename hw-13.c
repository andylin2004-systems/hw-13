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
    if (errno){
        printf("Error: %s", strerror(errno));
        return;
    }
    read(file, doc, sizeof(doc));
    if (errno){
        printf("Error: %s", strerror(errno));
        return;
    }
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
        if (lineCount != 0)
        {
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
    if (errno){
        printf("Error: %s", strerror(errno));
        return;
    }
    write(newFile, popInfo, sizeof(struct pop_entry) * (countLines(doc) - 1) * 5);
    if (errno){
        printf("Error: %s", strerror(errno));
        return;
    }
    free(popInfo);
    close(newFile);
}

int read_data()
{
    FILE *file = open("nyc_pop.bin", O_RDONLY);
    if (errno){
        printf("Error: %s", strerror(errno));
        return 0;
    }
    struct stat fileStat;
    stat("nyc_pop.bin", &fileStat);
    struct pop_entry *boroArray = malloc(fileStat.st_size);
    read(file, boroArray, fileStat.st_size);
    if (errno){
        printf("Error: %s", strerror(errno));
        return 0;
    }
    int i;
    for (i = 0; i < fileStat.st_size / sizeof(struct pop_entry); i++)
    {
        printf("%d: year: %d boro: %s pop: %d\n", i, boroArray[i].year, boroArray[i].boro, boroArray[i].population);
    }
    close(file);
    free(boroArray);
    return fileStat.st_size / sizeof(struct pop_entry);
}

void add_data()
{
    char input[100];
    printf("Enter info in following format: year boro pop:\n");
    read(STDIN_FILENO, input, sizeof(input));
    FILE *file = open("nyc_pop.bin", O_RDONLY, 0644);
    if (errno)
    {
        printf("Error: %s", strerror(errno));
        return;
    }
    struct stat fileStat;
    stat("nyc_pop.bin", &fileStat);
    struct pop_entry *boroArray = malloc(fileStat.st_size + sizeof(struct pop_entry));
    read(file, boroArray, fileStat.st_size);
    if (errno)
    {
        printf("Error: %s", strerror(errno));
        return;
    }
    close(file);
    int appendAt = fileStat.st_size / sizeof(struct pop_entry);
    file = open("nyc_pop.bin", O_WRONLY | O_TRUNC);
    if (errno)
    {
        printf("Error: %s", strerror(errno));
        return;
    }
    sscanf(input, "%d %s %d", &(boroArray[appendAt].year), boroArray[appendAt].boro, &(boroArray[appendAt].population));
    write(file, boroArray, fileStat.st_size + sizeof(struct pop_entry));
    if (errno)
    {
        printf("Error: %s", strerror(errno));
        return;
    }
    close(file);
    free(boroArray);
}

void update_data()
{
    int arrayLength = read_data();
    char input[100];
    int row;
    struct stat fileStat;
    stat("nyc_pop.bin", &fileStat);
    struct pop_entry *boroArray = malloc(fileStat.st_size);
    FILE *file = open("nyc_pop.bin", O_RDONLY);
    if (errno)
    {
        printf("Error: %s", strerror(errno));
        return;
    }
    read(file, boroArray, fileStat.st_size);
    if (errno)
    {
        printf("Error: %s", strerror(errno));
        return;
    }
    close(file);
    printf("Enter row to edit:\n");
    read(STDIN_FILENO, input, sizeof(input));
    sscanf(input, "%d", &row);
    if (row >= arrayLength || row < 0)
    {
        printf("Row out of bounds.");
    }
    else
    {
        printf("Enter info in following format to update: year boro pop:\n");
        read(STDIN_FILENO, input, sizeof(input));
        sscanf(input, "%d %s %d", &(boroArray[row].year), boroArray[row].boro, &(boroArray[row].population));
        file = open("nyc_pop.bin", O_WRONLY | O_TRUNC);
        write(file, boroArray, fileStat.st_size);
        if (errno)
        {
            printf("Error: %s", strerror(errno));
            return;
        }
        close(file);
    }
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        if (strcmp(argv[1], "-read_csv"))
        {
            read_csv();
        }
        else if (strcmp(argv[1], "-read_data"))
        {
            read_data();
        }
        else if (strcmp(argv[1], "-add_data"))
        {
            add_data();
        }
        else if (strcmp(argv[1], "-update_data"))
        {
            update_data();
        }
        else
        {
            printf("Invalid command line argument.\n-read_csv to read csv file\n-read_data to read data file\n-add_data to add data to data file\n-update_data to update data in data file");
        }
    }
    else
    {
        printf("No command line argument found.\n-read_csv to read csv file\n-read_data to read data file\n-add_data to add data to data file\n-update_data to update data in data file");
    }
}