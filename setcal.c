#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    char **elements;
    int count;
    int id;
} Set;

typedef struct
{
    Set **sets;
    int setsCout;
    Set *universum;
    bool err;
} Data;

/**
 * @brief nacte set do structu (max 100 setu maximalne 100 znaku dlouhych TODO:nekonecno znaku a setu)
 *
 * @param file ukazatel na soubor
 * @param set
 * @return int
 *      @retval 0 vporadku
 *      @retval -1 chyba
 */
int loadSet(FILE *file, Set *set)
{

    int elementsBufferSize = 20;
    int charArrayBufferSize = 3;
    int elementsBufferMultiplier=2;
    int charArrayBufferMultiplier=2;
   //inicializace
    char c = 0;
    int j = 0;
    set->elements = (char **)malloc(elementsBufferSize * sizeof(char *));
    set->elements[0] = (char *)malloc(charArrayBufferSize * sizeof(char));

    if(set->elements[0] == NULL || set->elements == NULL){
        return -1;
    }
    //nacita znaky dokud nenarazi na konec radku
    int i = 0;
    for (; (c = fgetc(file)) != '\n'; i++)
    {

        if (i>=charArrayBufferSize)
        {
            charArrayBufferSize *=charArrayBufferMultiplier;
           set->elements[j]  = (char *)realloc(set->elements[j],charArrayBufferSize * sizeof(char));
            charArrayBufferMultiplier++;
        }
        //ukonci se pokud je na konci souboru
        if (c == EOF)
        {
            break;
        }
        //rozdeli jednotlive prvky podle mezer (TODO: detekce vice mezer za sebou)
        if (c == ' ')
        {
            //ukonci retezec 0
            set->elements[j][i] = 0;
            //alokace místa pro další prvek
            j++;

            if (j>=elementsBufferSize)
            {    elementsBufferSize *=  elementsBufferMultiplier;
                set->elements = (char **)realloc(  set->elements,elementsBufferSize * sizeof(char *));
            elementsBufferMultiplier++;
            }
            set->count = j+1;
            set->elements[j] = (char *)malloc(charArrayBufferSize * sizeof(char));
             if( set->elements[j] == NULL){
                    return -1;
              }
            //vynulovani i (-1 aby po inkrementaci bylo 0)
            i = -1;
            continue;
        }
        set->elements[j][i] = c;
    }



        if (i>=charArrayBufferSize)
        {
            charArrayBufferSize *=charArrayBufferMultiplier;
           set->elements[j]  = (char *)realloc(set->elements[j],charArrayBufferSize * sizeof(char));
            charArrayBufferMultiplier++;
        }
        set->elements[j][i] = 0;
    return 0;
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
        printf("%d: ", data.sets[i]->id);
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
 * @brief Nacte data ze souboru, a vrati je ve forme structu data (omezeno jen na 100 setu TODO:nekonecno setu)
 *
 * @param file cesta k souboru
 * @return Data
 */
Data Load(char file[])
{
    //inicializace
    Data data;
    data.setsCout = 0;
    data.err = false;
    FILE *fp = fopen(file, "r");
    char c = 0;
    data.sets = (Set **)malloc(100 * sizeof(Set *));
    data.universum = (Set *)malloc(sizeof(Set));
    int line = 0;

    if(data.sets == NULL || data.universum == NULL ){
        data.err = true;
        return data;
    }
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
            if(data.sets[data.setsCout] == NULL){
                data.err = true;
                 return data;
            }
            loadSet(fp, data.sets[data.setsCout]);
            data.sets[data.setsCout]->id = line;
            data.setsCout++;
            break;
        case 'R': //TODO:
            break;
        case 'C': //TODO:
            break;
        }
        line++;
    }
    printData(data);
    return data;
}


int main(void)
{

    Load("test.txt");

    //TODO: dopsat free na DATA
    return 0;
}
