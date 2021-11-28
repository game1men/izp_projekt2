#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    char **elements; //elementy setu
    int count;       //pocet elementu setu
    int id;          // radek na kterem je set
} Set;

typedef struct
{
    char ***elements; //elementy relace
    int count;        //pocet elementu relace
    int id;           //radek na kterem je relace
} Relation;

typedef struct
{
    void **lines;         //reference na sety a relace podle radku
    Set **sets;           // vsechny nactene sety
    int setsCout;         //pocet setu
    Relation **relations; // vsechny nactene relace
    int relationsCount;   //pocet relaci
    Set *universum;       // universum
    bool err;             //nastal error
} Data;

void printSet(Set set);

/**
 * @brief true nebo false podle toho, zda se prvky rovnají
 *
 * @param relation
 * @param first prvni prvek relace (např. u (aa bb) to je aa)
 * @param second druhy prvek relace
 */
bool isInRelation(Relation *relation, char *first, char *second)
{
    for(int i = 0; i < relation->count; i++ )
    {
        if(strcmp(relation->elements[i][0], first) == 0)
        {
            if(strcmp(relation->elements[i][1], second) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief true nebo false jestli je sym
 *
 * @param relation
 */
void symmetric(Relation *relation)
{
    for(int i = 0; i < relation->count; i++)
    {
        if(isInRelation(relation, relation->elements[i][1], relation->elements[i][0]) == true)
        {
        }
        else
        {
            printf("false\n");
            return;
        }

    }
    printf("true\n");
}
/**
 * @brief true nebo false jestli je antisym
 *
 * @param relation
 */
void antisymmetric(Relation *relation)
{
    for(int i = 0; i < relation->count; i++)
    {
        if(isInRelation(relation, relation->elements[i][1], relation->elements[i][0]) == true)
        {
            if(strcmp(relation->elements[i][1],relation->elements[i][0]) == 0)
            {
                continue;
            }
            printf("false\n");
            return;
        }
    }
    printf("true\n");
}

/**
 * @brief true nebo false jestli se jedna o relaci funkce
 *
 * @param function
 */
void function(Relation *relation)
{
    int a;
    for(int i = 0; i < relation->count; i++)
    {
        a = 0;
        for(int j = 0; j < relation->count; j++)
        {
            if(strcmp(relation->elements[i][0], relation->elements[j][0]) == 0)
            {
                a++;
            }
            if(a > 1)
            {
                printf("false\n");
                return;
            }
        }
    }
    printf("true\n");
    return;
}


/**
 * @brief porovnava mnoziny jestli se rovnaji, a podle toho vypisuje true nebo false
 * 
 * @param setA 
 * @param setB 
 */
void equals(Set *setA, Set *setB)
{
    if(setA->count != setB->count)
    {
        printf("false\n");
        return;
    }

    bool isIn = false;
    for(int i = 0; i < setA->count; i++)
    {
        isIn = false;
        for(int j = 0; j < setB->count; j++)
        {
            if(strcmp(setB->elements[j],setA->elements[i]) == 0)
            {
                isIn = true;
            }
        }
        if(isIn == false)
        {
            printf("false\n");
            return;
        }
    }
    printf("true\n");
    return;
}

void reflexive(Relation *relation)
{
    for(int i = 0; i < relation->count; i++)
    {
        if( isInRelation(relation, relation->elements[i][1], relation->elements[i][1]) && 
            isInRelation(relation, relation->elements[i][0], relation->elements[i][0]) == true)
            {                
            }
        else
        {
            printf("false\n");
            return;
        }
    }
    printf("true\n");
}

/**
 * @brief true nebo false jestli je tranzitivni
 *
 * @param relation 
 */
void transitive(Relation *relation)
{
    for(int i = 0; i < relation->count; i++)
    {
        for(int j = 0; j < relation->count; j++)
        {
            if(strcmp(relation->elements[i][1], relation->elements[j][0]) == 0)
            {
                if(isInRelation(relation, relation->elements[i][0], relation->elements[j][1]) == false)
                {
                    printf("false\n");
                    return;
                }
            }
        }

    }
    printf("true\n");
    return;
}

/**
 * @brief vraci bool podle toho, zda prvek patri do mnoziny
 *
 * @param rElement prvek relace
 * @param set mnozina
 */
bool isInSet(char *rElemnt, Set *set)
{
    for(int i = 0; i < set->count; i++)
    {
        if(strcmp(rElemnt, set->elements[i]) == 0)
        {
            return true;
        }

    }
    return false;
}

/**
 * @brief tiskne true nebo false podle hodnotu parametru bol
 *
 * @param bol bool 
 */
void boolPrint(bool bol)
{
    if(bol == true)
    {
        printf("true\n");
        return;
    }
    else
    {
        printf("false\n");
        return;
    }

}
/**
 * @brief zjišťuje jestli je relace injektivní a jestli její prvky patří do množin A a B, podle toho vrací bool
 * 
 * @param relation
 * @param setA 
 * @param setB 
 */
bool injective(Relation *relation, Set *setA, Set *setB)
{
    for(int i = 0; i < relation->count; i++)
    {
        if( (isInSet(relation->elements[i][0], setA) && isInSet(relation->elements[i][0], setB)) == false)
        {
            return false;
        }
    }
    
    int a;
    for(int i = 0; i < relation->count; i++)
    {
        a = 0;
        for(int j = 0; j < relation->count; j++)
        {
            if(strcmp(relation->elements[i][0], relation->elements[j][0]) == 0)
            {
                a++;
            }
            if(a > 1)
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief zjišťuje jestli je relace surjektivni a jestli její prvky patří do množin A a B, podle toho vrací bool
 * 
 * @param relation
 * @param setA 
 * @param setB 
 */
bool surjective(Relation *relation, Set *setA, Set *setB)
{
    for(int i = 0; i < relation->count; i++)
    {
        if( (isInSet(relation->elements[i][0], setA) && isInSet(relation->elements[i][0], setB)) == false)
        {
            return false;
        }
    }
    
    //podle soucasneho zadání platí surjektivita vždy, protože ji testujeme nad relací a ne nád množinami, jediné co nás zajímá je to, zda prvky patri do relace
    //pozn. surjektivita = každý prvek z B má k soubě nějaké A, těch může mít i víc 
    //pozn. aby dávalo určení relace smysl, tak by v zadání muselo být napsané něco ve smyslu určete, zda relace R znázorňuje surjektivitu mnozin A a B

    return true;
}

/**
 * @brief zjišťuje jestli je relace bijektivni(surjektivni i injektivni) a jestli její prvky patří do množin A a B, podle toho vrací bool
 * 
 * @param relation
 * @param setA 
 * @param setB 
 */
bool bijective(Relation *relation, Set *setA, Set *setB)
{
    if((surjective(relation, setA, setB) && injective(relation, setA, setB)) == true)
    {
        return true;
    }
    return false;
}


/**
 * @brief tiskne počet prvků v množině A (definované na řádku A).
 *
 * @param set
 */
void card(Set *set)
{
    printf("%d\n", set->count);
}

/**
 * @brief vypisuje doplnok množiny A (vzhladom k univerzu)
 *
 * @param set
 * @param universum
 */
void complement(Set *set, Set *universum)
{
    printf("S");
    bool complementE;
    for(int i = 0; i < universum->count;  i++)
    {
        complementE = true;
        for(int j = 0; j < set->count; j++)
        {
            if(strcmp(universum->elements[i],set->elements[j]) == 0)
            {
                complementE = false;
                break;
            }
        }

        if(complementE) printf(" %s", universum->elements[i]);
    }
    printf("\n");
}

/**
 * @brief vypisuje zjednotenie množín A a B
 *
 * @param setA
 * @param setB
 */
void unionAB(Set *setA, Set *setB)
{
    bool unionB;
    printf("S");
    for(int i = 0; i < setA->count; i++)
    {
        printf(" %s", setA->elements[i]);
    }

    for (int j = 0; j < setB->count; j++)
    {
        unionB = true;
        for (int k = 0; k < setA->count; k++)
        {
            if(strcmp(setA->elements[k],setB->elements[j]) == 0)
            {
                unionB = false;
                break;
            }
        }
        if(unionB) printf(" %s", setB->elements[j]);
    }
    printf("\n");
}

/**
 * @brief vypisuje prienik množín A a B
 *
 * @param setA
 * @param setB
 */
void intersect(Set *setA, Set *setB)
{
    printf("S");
    for(int i = 0; i < setA->count; i++)
    {
        for(int j = 0; j < setB->count; j++)
        {
            if(strcmp(setA->elements[i], setB->elements[j]) == 0)
            {
                printf(" %s",setA->elements[i]);
                break;
            }
        }
    }
    printf("\n");
}

/**
 * @brief vypise rozdiel množín A \ B
 *
 * @param setA
 * @param setB
 */
void minus(Set *setA, Set *setB)
{
    bool minusA;
    printf("S");
    for (int i = 0; i < setA->count; i++)
    {
        minusA = true;
        for (int j = 0; j < setB->count; j++)
        {
            if(strcmp(setA->elements[i],setB->elements[j]) == 0)
            {
                minusA = false;
                break;
            }
        }
        if(minusA) printf(" %s", setA->elements[i]);
    }
    printf("\n");
}

/**
 * @brief vypisuje true alebo false, ak množina A je podmnožinou množiny B
 *
 * @param setA
 * @param setB
 */
void subseteq(Set *setA, Set *setB)
{
    bool subseteqB;
    for (int i = 0; i < setA->count; i++)
    {
        subseteqB = true;
        for (int j = 0; j < setB->count; j++)
        {
            if(strcmp(setA->elements[i],setB->elements[j]) == 0)
            {
                subseteqB = false;
                break;
            }
        }
        if(subseteqB){
            printf("false\n");
            return;
        }
    }
    printf("true\n");
}

/**
 * @brief vypisuje true alebo false, ak je množina A vlastnou podmnožinou množiny B
 *
 * @param setA
 * @param setB
 */
void subset(Set *setA, Set *setB)
{
    bool subsetB;
    int countB = 0;
    for (int i = 0; i < setA->count; i++)
    {
        subsetB = true;
        for (int j = 0; j < setB->count; j++)
        {
            if(strcmp(setA->elements[i],setB->elements[j]) == 0)
            {
                subsetB = false;
                countB++;
                break;
            }
        }
        if(subsetB){
            printf("false\n");
            return;
        }
    }
    if(setB->count != countB) printf("true\n");
    else printf("false\n");
}

/**
 * @brief Provede spravny prikaz, ktery je uveden na radku
 *
 * @param file ukazatel na soubor
 * @param data
 */
void doCommand(FILE *file, Data data)
{

    //buffer 32 znaků bude určitě stačit, jelikoz predem zname vsechny mozne prikazi
    char cmd[32];
    //fscanf umí brát stringy podle mezer
    fscanf(file, "%31s", cmd);

    if (strcmp(cmd, "empty") == 0)
    {
        printf("empty\n");
    }
    else if (strcmp(cmd, "card") == 0)
    {
        int id = 0;
        //nacte radek ktery by mel byt hned za prikazem TODO: osetrit chybejici radek, spatny format atd ...
        fscanf(file, "%d", &id);
        //id je potreba dat o 1 mensi protoze se indexuje od nuly
        card((Set *)(data.lines[id - 1]));
    }
    else if (strcmp(cmd, "complement") == 0)
    {
        int id = 0;
        fscanf(file, "%d", &id);
        complement((Set *)(data.lines[id - 1]), (Set *)(data.universum));

    }
    else if (strcmp(cmd, "union") == 0)
    {
        int idA = 0;
        int idB = 0;
        fscanf(file, "%d", &idA);
        fscanf(file, "%d", &idB);
        unionAB((Set *)(data.lines[idA - 1]), (Set *)(data.lines[idB - 1]));
    }
    else if (strcmp(cmd, "intersect") == 0)
    {
        int idA = 0;
        int idB = 0;
        fscanf(file, "%d", &idA);
        fscanf(file, "%d", &idB);
        intersect((Set *)(data.lines[idA - 1]), (Set *)(data.lines[idB - 1]));
    }
    else if (strcmp(cmd, "minus") == 0)
    {
        int idA = 0;
        int idB = 0;
        fscanf(file, "%d", &idA);
        fscanf(file, "%d", &idB);
        minus((Set *)(data.lines[idA - 1]), (Set *)(data.lines[idB - 1]));
    }
    else if (strcmp(cmd, "subseteq") == 0)
    {
        int idA = 0;
        int idB = 0;
        fscanf(file, "%d", &idA);
        fscanf(file, "%d", &idB);
        subseteq((Set *)(data.lines[idA - 1]), (Set *)(data.lines[idB - 1]));
    }
    else if (strcmp(cmd, "subset") == 0)
    {
        int idA = 0;
        int idB = 0;
        fscanf(file, "%d", &idA);
        fscanf(file, "%d", &idB);
        subset((Set *)(data.lines[idA - 1]), (Set *)(data.lines[idB - 1]));
    }
    else if (strcmp(cmd, "equals") == 0)
    {
        int idA = 0;
        int idB = 0;
        fscanf(file, "%d", &idA);
        fscanf(file, "%d", &idB);
        equals((Set *)(data.lines[idA - 1]), (Set *)(data.lines[idB - 1]));
    }
    else if (strcmp(cmd, "reflexive") == 0)
    {
        int id = 0;
        fscanf(file, "%d", &id);
        reflexive((Relation *)(data.lines[id - 1]));
    }
    else if (strcmp(cmd, "symmetric") == 0)
    {
        int id = 0;
        fscanf(file, "%d", &id);
        symmetric((Relation *)(data.lines[id - 1]));
    }
    else if (strcmp(cmd, "antisymmetric") == 0)
    {
        int id = 0;
        fscanf(file, "%d", &id);
        antisymmetric((Relation *)(data.lines[id - 1]));
    }
    else if (strcmp(cmd, "transitive") == 0)
    {
        int id = 0;
        fscanf(file, "%d", &id);
        transitive((Relation *)(data.lines[id - 1]));
    }
    else if (strcmp(cmd, "function") == 0)
    {
        int id = 0;
        fscanf(file, "%d", &id);
        function((Relation *)(data.lines[id - 1]));
    }
    else if (strcmp(cmd, "domain") == 0)
    {
        printf("domain\n");
    }
    else if (strcmp(cmd, "codomain") == 0)
    {
        printf("codomain\n");
    }
    else if (strcmp(cmd, "injective") == 0)
    {
        int idA = 0;
        int idB = 0;
        int id = 0;
        fscanf(file, "%d", &id);
        fscanf(file, "%d", &idA);
        fscanf(file, "%d", &idB);
        bool a = injective((Relation *)(data.lines[id - 1]),(Set *)(data.lines[idA - 1]), (Set *)(data.lines[idB - 1]));
        boolPrint(a);
    }
    else if (strcmp(cmd, "surjective") == 0)
    {
        int idA = 0;
        int idB = 0;
        int id = 0;
        fscanf(file, "%d", &id);
        fscanf(file, "%d", &idA);
        fscanf(file, "%d", &idB);
        bool a = surjective((Relation *)(data.lines[id - 1]),(Set *)(data.lines[idA - 1]), (Set *)(data.lines[idB - 1]));
        boolPrint(a);
    }
    else if (strcmp(cmd, "bijective") == 0)
    {
        int idA = 0;
        int idB = 0;
        int id = 0;
        fscanf(file, "%d", &id);
        fscanf(file, "%d", &idA);
        fscanf(file, "%d", &idB);
        bool a = bijective((Relation *)(data.lines[id - 1]),(Set *)(data.lines[idA - 1]), (Set *)(data.lines[idB - 1]));
        boolPrint(a);
    }
    return;
}
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

    //inicializace
    int elementsBufferSize = 20;
    int charArrayBufferSize = 20;
    set->count = 0;

    int j = 0;

    set->elements = (char **)malloc(elementsBufferSize * sizeof(char *));
    set->elements[0] = (char *)malloc(charArrayBufferSize * sizeof(char));

    //pokud se nepodvedla alokace
    if (set->elements[0] == NULL || set->elements == NULL)
    {
        return -1;
    }

    char c = 0;

    if ((c = fgetc(file)) == '\n')
    { //preskoci prvni mezeru, pokud je to konec radku, tak ukonci funkci
        return 0;
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
    //pocet prvku je o 1 vetsi jak index
    set->count = j + 1;

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
    relation->elements = (char ***)malloc((set.count / 2) * sizeof(char **));
    for (int i = 0; i < set.count / 2; i++)
    {
        relation->elements[i] = (char **)malloc(2 * sizeof(char *));
    }
    int f = 0;
    for (int i = 0; i < set.count; i++)
    {

        //i % 2 protoye potrebuju umistit prvky stridave do noveho pole, a %2 bude stridvae vracet 0 a 1
        relation->elements[f][(i % 2)] = (char *)malloc(32 * sizeof(char));

        int o = 0; //aby bylo mozne preskocit pocatecni zavorku je potreba pouzit 2 promenne
        for (int j = 0; set.elements[i][j] != 0; j++)
        {

            //cyklus by skoncil nez by se stihl ukonci string 0, proto tato kontrola
            if (set.elements[i][j + 1] == 0)
            {
                relation->elements[f][(i % 2)][o + 1] = 0;
            }
            //vymaze zavorku na konci
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
 * @brief Tiskne universum
 *
 * @param set
 */
void printUniversum(Set set)
{
    printf("U");
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
    int dataSetBufferSize = 50;
    int dataRealtionBufferSize = 50;
    data.sets = (Set **)malloc(dataSetBufferSize * sizeof(Set *));
    data.relations = (Relation **)malloc(dataSetBufferSize * sizeof(Relation *));
    data.lines = (void **)malloc((dataSetBufferSize + dataSetBufferSize + 1) * sizeof(void *)); //+1 protoze obsahuje i universum
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
            data.lines[line] = data.universum;
            printUniversum(*data.universum);
            printf("\n");
            break;
        case 'S':
            //pokud je treba nacist vice setu nez byl alokovan buffer, tak se zvetsi o velikost puvodniho bufferu
            if (data.setsCout >= dataSetBufferSize)
            {
                dataSetBufferSize += dataSetBufferSize;
                data.sets = (Set **)realloc(data.sets, dataSetBufferSize * sizeof(Set *));
                data.lines = (void **)realloc(data.lines, (dataSetBufferSize + dataSetBufferSize + 1) * sizeof(void *));
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
            data.lines[line] = data.sets[data.setsCout];
            printSet(*data.sets[data.setsCout]);
            printf("\n");
            data.setsCout++;
            break;
        case 'R':
            //pokud je treba nacist vice setu nez byl alokovan buffer, tak se zvetsi o velikost puvodniho bufferu
            if (data.relationsCount >= dataRealtionBufferSize)
            {
                dataRealtionBufferSize += dataRealtionBufferSize;
                data.relations = (Relation **)realloc(data.relations, dataRealtionBufferSize * sizeof(Relation *));
                data.lines = (void **)realloc(data.lines,(dataSetBufferSize+dataSetBufferSize+1) * sizeof(void*));
            }
            data.relations[data.relationsCount] = (Relation *)malloc(sizeof(Relation));
            //pokud se alkoce nepoved, vrati data s err
            if (data.relations[data.relationsCount] == NULL)
            {
                data.err = true;
                return data;
            }
            loadRelation(fp, data.relations[data.relationsCount]);
            printRelation(*data.relations[data.relationsCount]);
            printf("\n");
            data.lines[line] = data.relations[data.relationsCount];
            data.relations[data.relationsCount]->id = line;
            data.relationsCount++;
            break;
        case 'C':
            doCommand(fp, data);
            break;
        }
        line++;
    }

    return data;
}

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        Load(argv[1]);
    }
    // Load("test.txt");
  //  printData(Load("test.txt"));

    //TODO: dopsat free na DATA
    return 0;
}
