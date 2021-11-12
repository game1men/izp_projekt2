#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    char **elements;
    int count;
} Set;

typedef struct
{
    Set **sets;
    int setsCout;
    Set *universum;
} Data;

/**
 * @brief nacte set do structu (max 100 setu maximalne 100 znaku dlouhych TODO:nekonecno znaku a setu)
 *
 * @param file ukazatel na soubor
 * @param set
 */
void loadSet(FILE *file, Set *set)
{

    set->elements = (char **)malloc(100 * sizeof(char *));
    char c = 0;
    int j = 0;
    set->elements[0] = (char *)malloc(100 * sizeof(char));

    for (int i = 0; (c = fgetc(file)) != '\n'; i++)
    {
        if (c == EOF)
        {
            break;
        }
        if (c == ' ')
        {
            set->elements[j][i] = 0;
            j++;
            i = -1;
            set->count = j + 1;
            set->elements[j] = (char *)malloc(100 * sizeof(char));
            continue;
        }

        set->elements[j][i] = c;
    }

    return;
}

/**
 * @brief prints all in Data struct
 *
 * @param data
 */
void printData(Data data)
{

    printf("\n universum:\n\n");
    for (int j = 0; j < data.universum->count; j++)
    {

        printf("%s",data.universum->elements[j]);

        printf(" ");
    }
    printf("\n sets: \n\n ");
    for (int i = 0; i < data.setsCout; i++)
    {

        for (int j = 0; j < data.sets[i]->count; j++)
        {

            printf("%s",data.sets[i]->elements[j]);

            printf(" ");
        }
        printf("\n");
    }

    printf("\n");
}

/**
 * @brief Nacte data ze souboru, a vrati je ve forme structu data
 *
 * @param file cesta k souboru
 * @return Data
 */
Data Load(char file[])
{
    //inicializace
    Data data;
    data.setsCout = 0;
    FILE *fp = fopen(file, "r");
    char c = 0;
    data.sets = (Set **)malloc(100 * sizeof(Set *));
    data.universum = (Set *)malloc(sizeof(Set));

    //nacitani prvniho znaku na kazdym radku
    while ((c = fgetc(fp)) != EOF)
    {
        switch (c)
        {

        case 'U':
            loadSet(fp, data.universum);
            break;
        case 'S':
            data.sets[data.setsCout] = (Set *)malloc(sizeof(Set));
            loadSet(fp, data.sets[data.setsCout]);
            data.setsCout++;
            break;
        case 'R': //TODO:
            break;
        case 'C': //TODO:
            break;
        }
    }
    //printing for debug ----------------------- TODO: remove

    printData(data);
    return data;
}

int main(void)
{

    Load("test.txt");
    return 0;
}
