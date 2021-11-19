#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//
typedef struct
{
    char **elements;
    int count;
    int id;
} Set;

typedef struct
{
    char ***elements;
    int count;
    int id;
} Relation;

typedef struct
{
    Set **sets;
    int setsCout;
    Relation **relations;
    int relationsCount;
    Set *universum;
    bool err;
} Data;

/**
 * @brief nacte set do structu
 *
 * @param file ukazatel na soubor
 * @param set
 * @return int
 *      @retval 0 vporadku
 *      @retval -1 chyba
 */

/**
 * @brief nacte set do structu
 *
 * @param file ukazatel na soubor
 * @param set
 * @return int
 *      @retval 0 vporadku
 *      @retval -1 chyba
 */
int loadSet(FILE *file, Set *set)
{
    fgetc(file); //preskoci prvni mezeru

    //inicializace
    int elementsBufferSize = 20;
    int charArrayBufferSize = 20;

    char c = 0;
    int j = 0;
    set->elements = (char **)malloc(elementsBufferSize * sizeof(char *));
    set->elements[0] = (char *)malloc(charArrayBufferSize * sizeof(char));

    //pokud se nepodvedla alokace
    if (set->elements[0] == NULL || set->elements == NULL)
    {
        return -1;
    }
    //nacita znaky dokud nenarazi na konec radku
    int i = 0;
    for (; (c = fgetc(file)) != '\n'; i++)
    {
        //pokud je treba nacist vice znaku byl alokovan buffer, tak se zvetsi o velikost puvodniho bufferu
        if (i >= charArrayBufferSize)
        {
            charArrayBufferSize += charArrayBufferSize;
            set->elements[j] = (char *)realloc(set->elements[j], charArrayBufferSize * sizeof(char));
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

            j++;
            //pokud je treba nacist vice prvku byl alokovan buffer, tak se zvetsi o velikost puvodniho bufferu
            if (j >= elementsBufferSize)
            {
                elementsBufferSize += elementsBufferSize;
                set->elements = (char **)realloc(set->elements, elementsBufferSize * sizeof(char *));
            }
            //pocet prvku je o 1 vetsi jak index
            set->count = j + 1;
            //alokace místa pro další prvek
            set->elements[j] = (char *)malloc(charArrayBufferSize * sizeof(char));
            //pokud se nepodvedla alokace
            if (set->elements[j] == NULL)
            {
                return -1;
            }
            //vynulovani i (-1 aby po inkrementaci bylo 0)
            i = -1;
            continue;
        }
        set->elements[j][i] = c;
    }

    //pokud neni misto na null na konci stringu, tak se zvetsi o 1
    if (i >= charArrayBufferSize)
    {
        set->elements[j] = (char *)realloc(set->elements[j], (charArrayBufferSize + 1) * sizeof(char));
    }
    //da 0 na konci stringu
    set->elements[j][i] = 0;
    return 0;
}

/**
 * @brief tiskne relaci
 *
 * @param relation
 */
void printRelation(Relation relation)
{
    printf("R");

    for (int j = 0; j < relation.count; j++)
    {

        printf(" (%s ", relation.elements[j][0]);
        printf("%s)", relation.elements[j][1]);
    }
}
/**
 * @brief Nacte realaci ze souboru
 *
 * @param file ukazatel na soubor
 * @param relation ukazatel na relaci
 * @return int
 *  @retval 0 vporadu
 *  @retval 1 chyba
 */
int loadRelation(FILE *file, Relation *relation)
{
    //inicializace
    relation->count = 0;
    Set set;
    //int relationsBufferSize = 20;
    //nacteni prvku ze radku oddelenych mezerou TODO: udelat specialni funkci na toto a nepouzivat loadSet
    loadSet(file, &set);

    //alokace
    relation->elements = (char ***)malloc((set.count/2) * sizeof(char **));
    for (int i = 0; i < set.count / 2; i++)
    {
        relation->elements[i] = (char **)malloc(2 * sizeof(char *));
    }
    int f = 0;
    for (int i = 0; i < set.count; i++)
    {

        //i % 2 protoye potrebuju umistit prvky stridave do noveho pole, a %2 bude stridvae vracet 0 a 1
        relation->elements[f][(i % 2)] = (char *)malloc(32 * sizeof(char));

        int o = 0;//aby bylo mozne preskocit pocatecni zavorku je potreba pouzit 2 promenne
        for (int j = 0; set.elements[i][j] != 0; j++)
        {

            //cyklus by skoncil nez by se stihl ukonci string 0, proto tato kontrola
            if (set.elements[i][j + 1] == 0)
            {
                relation->elements[f][(i % 2)][o + 1] = 0;
            }
              //vymaze yavorku na konci
            if (set.elements[i][j] == ')')
            {
                relation->elements[f][(i % 2)][o] = 0;
                break;
            }
             //preskoci zavorku na zacatku
            if (set.elements[i][j] == '(')
            {
                continue; //preskoci ulozeni znaku a zaroven inkrementaci "o", takze "o" bude o 1 pozadu
            }

            relation->elements[f][(i % 2)][o] = set.elements[i][j];
            o++;
        }
        //pocet prvku je o 1 vetsi jak index na kterem je ulozen
        relation->count = f + 1;

        //je potreba krokovat pole elementu relace polovicnim krokem
        f += i % 2;
    }
    return 0;
}

/**
 * @brief Tiskne set
 *
 * @param set
 */
void printSet(Set set)
{
    printf("S");
    for (int j = 0; j < set.count; j++)
    {
        printf(" %s", set.elements[j]);
    }
}

/**
 * @brief prints all in Data struct
 *
 * @param data
 */
void printData(Data data)
{

    printf("\n universum:\n\n");

    printSet(*data.universum);

    for (int j = 0; j < data.universum->count; j++)
    {

        printf("%s", data.universum->elements[j]);

        printf(" ");
    }
    printf("\n\n sets: \n\n");
    for (int i = 0; i < data.setsCout; i++)
    {
        printf("%d: ", data.sets[i]->id);

        printSet(*data.sets[i]);

        printf("\n");
    }

    printf("\n\n relations: \n\n");
    for (int i = 0; i < data.relationsCount; i++)
    {
        printf("%d: ", data.relations[i]->id);

        printRelation(*data.relations[i]);

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
    data.relationsCount = 0;
    data.err = false;
    FILE *fp = fopen(file, "r");
    char c = 0;
    int dataSetBufferSize = 5;
    int dataRealtionBufferSize = 5;
    data.sets = (Set **)malloc(dataSetBufferSize * sizeof(Set *));
    data.relations = (Relation **)malloc(dataSetBufferSize * sizeof(Relation *));
    data.universum = (Set *)malloc(sizeof(Set));
    int line = 0;

    if (data.sets == NULL || data.universum == NULL || data.relations == NULL)
    {
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
            //pokud je treba nacist vice setu nez byl alokovan buffer, tak se zvetsi o velikost puvodniho bufferu
            if (data.setsCout >= dataSetBufferSize)
            {
                dataSetBufferSize += dataSetBufferSize;
                data.sets = (Set **)realloc(data.sets, dataSetBufferSize * sizeof(Set *));
            }
            data.sets[data.setsCout] = (Set *)malloc(sizeof(Set));
            //pokud se alkoce nepoved, vrati data s err
            if (data.sets[data.setsCout] == NULL)
            {
                data.err = true;
                return data;
            }
            loadSet(fp, data.sets[data.setsCout]);
            data.sets[data.setsCout]->id = line;
            data.setsCout++;
            break;
        case 'R':
            //pokud je treba nacist vice setu nez byl alokovan buffer, tak se zvetsi o velikost puvodniho bufferu
            if (data.relationsCount >= dataRealtionBufferSize)
            {
                dataRealtionBufferSize += dataRealtionBufferSize;
                data.relations = (Relation **)realloc(data.relations, dataRealtionBufferSize * sizeof(Relation *));
            }
            data.relations[data.relationsCount] = (Relation *)malloc(sizeof(Relation));
            //pokud se alkoce nepoved, vrati data s err
            if (data.relations[data.relationsCount] == NULL)
            {
                data.err = true;
                return data;
            }
            loadRelation(fp, data.relations[data.relationsCount]);
            data.relations[data.relationsCount]->id = line;
            data.relationsCount++;
            break;
        case 'C': //TODO:
            break;
        }
        line++;
    }

    return data;
}

int main(void)
{

    printData(Load("test.txt"));

    //TODO: dopsat free na DATA
    return 0;
}
