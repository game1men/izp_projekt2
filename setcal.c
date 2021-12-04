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

typedef enum
{
    NONE,
    RELATION,
    SET,
    UNIVERSUM,
} TypeOfLine;

typedef struct
{
    void *line;            //ukazatel na relaci/mnozinu ulozenou na danem radku
    TypeOfLine typeOfLine; //udava jestli se jedna o mnozinu/relaci/univesum/...
} Line;

typedef struct
{
    Line *lines;          //reference na sety a relace podle radku
    Set **sets;           // vsechny nactene sety
    int setsCout;         //pocet setu
    Relation **relations; // vsechny nactene relace
    int relationsCount;   //pocet relaci
    Set *universum;       // universum
    bool err;             //nastal error
    int lineCount;        //pocet nahranych radku
} Data;

/**
 * @brief uvolní paměť od všeho co bylo dynamicky alokováno v structu Relation
 *
 * @param relation
 */
void FreeRelation(Relation *relation)
{

    for (int i = 0; i < relation->count; i++)
    {
        free(relation->elements[i][0]);
        free(relation->elements[i][1]);
        free(relation->elements[i]);
    }

    free(relation->elements);
}

/**
 * @brief uvolní paměť od všeho co bylo dynamicky alokováno v structu Set
 *
 * @param set
 */
void FreeSet(Set *set)
{

    for (int i = 0; i < set->count; i++)
    {
        free(set->elements[i]);
    }
    free(set->elements);
}

/**
 * @brief uvolní paměť od všeho co bylo dynamicky alokováno v structu Data
 *
 * @param data
 */
void FreeData(Data data)
{
    for (int i = 0; i < data.relationsCount; i++)
    {
        FreeRelation(data.relations[i]);
        free(data.relations[i]);
    }
    for (int i = 0; i < data.setsCout; i++)
    {
        FreeSet(data.sets[i]);
        free(data.sets[i]);
    }

    FreeSet(data.universum);
    free(data.universum);
    free(data.lines);
    free(data.relations);
    free(data.sets);
}

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

    for (int i = 0; i < relation->count; i++)
    {
        if (strcmp(relation->elements[i][0], first) == 0)
        {
            if (strcmp(relation->elements[i][1], second) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief tiskne true nebo false podle toho, jestli je mnozina definovana na radku A prazdna nebo neprazdna.
 *
 * @param set
 */
void empty(Set *set)
{
    bool result = true;
    if (set->count > 0)
        result = false;
    printf("%s", result ? "true\n" : "false\n");
}

/**
 * @brief true nebo false jestli je sym
 *
 * @param relation
 */
void symmetric(Relation *relation)
{
    for (int i = 0; i < relation->count; i++)
    {

        if (isInRelation(relation, relation->elements[i][1], relation->elements[i][0]) == true)
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
    for (int i = 0; i < relation->count; i++)
    {

        if (isInRelation(relation, relation->elements[i][1], relation->elements[i][0]) == true)
        {
            if (strcmp(relation->elements[i][1], relation->elements[i][0]) == 0)

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
    for (int i = 0; i < relation->count; i++)
    {
        a = 0;
        for (int j = 0; j < relation->count; j++)
        {
            if (strcmp(relation->elements[i][0], relation->elements[j][0]) == 0)
            {
                a++;
            }
            if (a > 1)
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
    if (setA->count != setB->count)
    {
        printf("false\n");
        return;
    }

    bool isIn = false;
    for (int i = 0; i < setA->count; i++)
    {
        isIn = false;
        for (int j = 0; j < setB->count; j++)
        {
            if (strcmp(setB->elements[j], setA->elements[i]) == 0)
            {
                isIn = true;
            }
        }
        if (isIn == false)
        {
            printf("false\n");
            return;
        }
    }
    printf("true\n");
    return;
}

void reflexive(Relation *relation, Set *universum)
{
    for (int i = 0; i < universum->count; i++)
    {
        if (isInRelation(relation, universum->elements[i], universum->elements[i]) == true)
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
    for (int i = 0; i < relation->count; i++)
    {
        for (int j = 0; j < relation->count; j++)
        {
            if (strcmp(relation->elements[i][1], relation->elements[j][0]) == 0)
            {
                if (isInRelation(relation, relation->elements[i][0], relation->elements[j][1]) == false)
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
    for (int i = 0; i < set->count; i++)
    {
        if (strcmp(rElemnt, set->elements[i]) == 0)
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
    if (bol == true)
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
    for (int i = 0; i < relation->count; i++)
    {
        if ((isInSet(relation->elements[i][0], setA) && isInSet(relation->elements[i][0], setB)) == false)
        {
            return false;
        }
    }

    int a;
    for (int i = 0; i < relation->count; i++)
    {
        a = 0;
        for (int j = 0; j < relation->count; j++)
        {
            if (strcmp(relation->elements[i][0], relation->elements[j][0]) == 0)
            {
                a++;
            }
            if (a > 1)
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
    for (int i = 0; i < relation->count; i++)
    {
        if ((isInSet(relation->elements[i][0], setA) && isInSet(relation->elements[i][0], setB)) == false)
        {
            return false;
        }
    }

    //podle soucasneho zadání platí surjektivita vždy, protože ji testujeme nad relací a ne nad množinami, jediné co nás zajímá je to, zda prvky patri do relace
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
    if ((surjective(relation, setA, setB) && injective(relation, setA, setB)) == true)
    {
        return true;
    }
    return false;
}


/**
 * @brief vypisuje definicni obor nebo obor hodnot, podle toho jestli se jedna o domain nebo codomain
 *
 * @param relation
 * @param identification promenna slouzici k odliseni domain a codomain
 */
void dom_cod (Relation *relation, int identification)
{
    int usedElements[relation->count + 1];
    for(int i = 0; i < relation->count + 1; i++)
    {
        usedElements[i] = -1;
    }
    int j = 0;
    bool con;
    printf("S");
    for(int i = 0; i < relation->count; i++)
    {
        con = false;
        for(int x = 0; x < j; x++)
        {
            
            if(strcmp(relation->elements[i][identification], relation->elements[(usedElements[x])][identification]) == 0)
            {
                con = true;
                break;
            }
        }   

        if(con == false)
        {
            usedElements[j] = i;
            j++;
            printf(" %s", relation->elements[i][identification]);
        }
    }
    printf("\n");
}

/**
 * @brief vypisuje obor hodnot
 *
 * @param relation
 */
void domain(Relation *relation)
{
    int identification = 0; //promenna slouzici k odliseni domain a codomain
    dom_cod(relation, identification);
    return;
}


/**
 * @brief vypisuje definicni obor
 *
 * @param relation
 */
void codomain(Relation *relation)
{
    int identification = 1; //promenna slouzici k odliseni domain a codomain
    dom_cod(relation, identification);
    return;
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
    for (int i = 0; i < universum->count; i++)
    {
        complementE = true;
        for (int j = 0; j < set->count; j++)
        {
            if (strcmp(universum->elements[i], set->elements[j]) == 0)
            {
                complementE = false;
                break;
            }
        }

        if (complementE)
            printf(" %s", universum->elements[i]);
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
    for (int i = 0; i < setA->count; i++)
    {
        printf(" %s", setA->elements[i]);
    }

    for (int j = 0; j < setB->count; j++)
    {
        unionB = true;
        for (int k = 0; k < setA->count; k++)
        {
            if (strcmp(setA->elements[k], setB->elements[j]) == 0)
            {
                unionB = false;
                break;
            }
        }
        if (unionB)
            printf(" %s", setB->elements[j]);
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
    for (int i = 0; i < setA->count; i++)
    {
        for (int j = 0; j < setB->count; j++)
        {
            if (strcmp(setA->elements[i], setB->elements[j]) == 0)
            {
                printf(" %s", setA->elements[i]);
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
            if (strcmp(setA->elements[i], setB->elements[j]) == 0)
            {
                minusA = false;
                break;
            }
        }
        if (minusA)
            printf(" %s", setA->elements[i]);
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
            if (strcmp(setA->elements[i], setB->elements[j]) == 0)
            {
                subseteqB = false;
                break;
            }
        }
        if (subseteqB)
        {
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
            if (strcmp(setA->elements[i], setB->elements[j]) == 0)
            {
                subsetB = false;
                countB++;
                break;
            }
        }
        if (subsetB)
        {
            printf("false\n");
            return;
        }
    }
    if (setB->count != countB)
        printf("true\n");
    else
        printf("false\n");
}

/**
 * @brief Získá všehny id z daneho radku.
 *
 * @param file ukazatel na soubor
 * @param ids bere pole ids, do ktereho ulozi ziskana id
 * @param parsed pocet id, ktera byl nactena
 */
int getIds(FILE *file, int ids[4], int *parsed)
{

    char c[100] = {0}; //buffer ynaku
    int x = 0;
    //cte radek, dokud nenarazi na konec radku
    for (; (c[x] = fgetc(file)) != '\n'; x++)
    {
        //pokud je x > jak 100, tak ukonci cyklus
        if (x >= 99)
        {
            break;
        }
    }
    //pokud je x <=1, tak nebyly nalezena zadna id
    if (x <= 1)
    {
        *parsed = 0;
        return 0;
    }
    //pomoci sscanf ze ziskaneho stringu sezene veskera id.
    *parsed = sscanf(c, "%d%d%d%d", &ids[0], &ids[1], &ids[2], &ids[3]);
    //pokud bylo nalezeno vice jak 3 id, jedna se o chybu
    if (ids[3] != -1)
    {
        return -1;
    }
    return 0;
}

/**
 * @brief Provede spravny prikaz, ktery je uveden na radku
 *
 * @param file ukazatel na soubor
 * @param data
 * @return int
 *  @retval -1 error
 *  @retval 0 vse OK
 */
int doCommand(FILE *file, Data data)
{

    //buffer 32 znaků bude určitě stačit, jelikoz predem zname vsechny mozne prikazi
    char cmd[32];

    //fscanf umí brát stringy podle mezer
    fscanf(file, "%31s", cmd);
    //nastavime pole id do -1, aby se dalo poznat ktera id byla nalezena, a ktera ne (tam kde je -1 nebylo ulozeno zadne id)
    int ids[4] = {-1, -1, -1, -1};

    //ziskame veskera id
    int parsed;
    if (getIds(file, ids, &parsed) == -1)
    {
        fprintf(stderr, "Prilis mnoho argumentu");
        return -1;
    }

    //pokud nebyla zadana zadna id, tak vratime error.
    if (ids[0] == -1)
    {
        fprintf(stderr, "Nebyly zadany relacy/mnoziny se kterymi se ma pracovat");
        return -1;
    }

    //zkontrolujeme jestli na mnozinovy prikazi jdou jenom mnoziny, a to stejny s relacema
    if (parsed != EOF && ids[0] != -1)
    {
        //zjistime typ radku podle prvniho id
        TypeOfLine tol = data.lines[ids[0] - 1].typeOfLine;
        for (int x = 1; x < parsed; x++)
        {
            //pokud nebyl id nacteno ukoncime cyklus
            if (ids[x] == -1)
            {
                break;
            }
            //pokud se typ radku podle predchoziho id nerovna typu nasledujiciho, tak vratime error
            if (data.lines[ids[x] - 1].typeOfLine != tol)
            {
                //zkontrolujeme, jestli se nejedna o 3 vyjimky, ktere pracuji zaroven s mnozinami a relacemi
                if (parsed >= 3)
                {
                    if (((strcmp(cmd, "injective") == 0) || (strcmp(cmd, "surjective") == 0) || (strcmp(cmd, "bijective") == 0)) && (data.lines[ids[0] - 1].typeOfLine = RELATION && data.lines[ids[1] - 1].typeOfLine == SET && data.lines[ids[2] - 1].typeOfLine == SET))
                    {
                        break;
                    }
                }
                fprintf(stderr, "Nelze provest prikaz mezi mnouzinou a relaci");
                return -1;
            }
        }
    }

    //do mnozinovejch prikazu pustime jenom mnoziny
    if (data.lines[ids[0] - 1].typeOfLine == SET || data.lines[ids[0] - 1].typeOfLine == UNIVERSUM)
    {

        if (strcmp(cmd, "empty") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            empty((Set *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "card") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            card((Set *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "complement") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            complement((Set *)(data.lines[ids[0] - 1].line), (Set *)(data.universum));
        }
        else if (strcmp(cmd, "union") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[2] != -1 || ids[0] == -1 || ids[1] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            unionAB((Set *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line));
        }
        else if (strcmp(cmd, "intersect") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[2] != -1 || ids[0] == -1 || ids[1] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            intersect((Set *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line));
        }
        else if (strcmp(cmd, "minus") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[2] != -1 || ids[0] == -1 || ids[1] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            minus((Set *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line));
        }
        else if (strcmp(cmd, "subseteq") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[2] != -1 || ids[0] == -1 || ids[1] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            subseteq((Set *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line));
        }
        else if (strcmp(cmd, "subset") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[2] != -1 || ids[0] == -1 || ids[1] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            subset((Set *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line));
        }
        else if (strcmp(cmd, "equals") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[2] != -1 || ids[0] == -1 || ids[1] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            equals((Set *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line));
        }
        else
        {
            fprintf(stderr, "Neplatny prikaz, nebo prikaz nelze provest na mnozine");
            return -1;
        }
    }
    else if (data.lines[ids[0] - 1].typeOfLine == RELATION) //do relacnich prikazu pustime jenom relace
    {
        if (strcmp(cmd, "reflexive") == 0)
        {
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            reflexive((Relation *)(data.lines[ids[0] - 1].line), (Set *)(data.universum));
        }
        else if (strcmp(cmd, "symmetric") == 0)
        {
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            symmetric((Relation *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "antisymmetric") == 0)
        {
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            antisymmetric((Relation *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "transitive") == 0)
        {
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            transitive((Relation *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "function") == 0)
        {
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            function((Relation *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "domain") == 0)
        {
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            domain((Relation *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "codomain") == 0)
        {  
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu");
                return -1;
            }
            codomain((Relation *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "injective") == 0)
        {
            bool a = injective((Relation *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line), (Set *)(data.lines[ids[2] - 1].line));
            boolPrint(a);
        }
        else if (strcmp(cmd, "surjective") == 0)
        {
            bool a = surjective((Relation *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line), (Set *)(data.lines[ids[2] - 1].line));
            boolPrint(a);
        }
        else if (strcmp(cmd, "bijective") == 0)
        {
            bool a = bijective((Relation *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line), (Set *)(data.lines[ids[2] - 1].line));
            boolPrint(a);
        }
        else
        {
            fprintf(stderr, "Neplatny prikaz, nebo prikaz nelze provest na relaci");
            return -1;
        }
    }
    else //pokud se nejedna o prikaz na mnozine ani o prikaz na relaci, tak se jedna o chybu
    {
        fprintf(stderr, "Prikaz nelze provest na tomto radku");
        return -1;
    }
    return 0;
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
        fprintf(stderr,"Chyba pri allokaci.");
        return -1;
    }

    char c = 0;

    if ((c = fgetc(file)) == '\n')
    { //preskoci prvni mezeru, pokud se jedna o znak noveho radku ukonci funkci

        return 0;
    }
    if (c != ' ')
    { //pokud prvni znak nebyl mezera ukonci funkci a vrati error
        fprintf(stderr, "Chybi mezera za prvnim znakem radku!");
        return -1;
    }

    //nacita znaky dokud nenarazi na konec radku
    int i = 0;
    for (; (c = fgetc(file)) != '\n'; i++)
    {
        //pokud obsahuje prvky co nepatri do nazvu prvku/relace vyhod chybu
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '(' || c == ')'))
        {
            fprintf(stderr, "Neplatny znak v nazvu prvku!!");
            return -1;
        }
        //pokud je treba nacist vice znaku byl alokovan buffer, tak se zvetsi o velikost puvodniho bufferu
        if (i >= charArrayBufferSize)
        {
            charArrayBufferSize += charArrayBufferSize;
            set->elements[j] = (char *)realloc(set->elements[j], charArrayBufferSize * sizeof(char));
            if (set->elements[j] == NULL)
            {
                fprintf(stderr,"Chyba pri reallokaci.");
                return -1;
            }
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
                if (set->elements == NULL)
                {
                    fprintf(stderr,"Chyba pri reallokaci.");
                    return -1;
                }

            }

            //alokace místa pro další prvek
            set->elements[j] = (char *)malloc(charArrayBufferSize * sizeof(char));
            //pokud se nepodvedla alokace
            if (set->elements[j] == NULL)
            {
                fprintf(stderr,"Chyba pri allokaci.");
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
        if (set->elements[j] == NULL)
        {
            fprintf(stderr,"Chyba pri reallokaci.");
            return -1;
        }
    }
    //da 0 na konci stringu
    set->elements[j][i] = 0;
    //pocet prvku je o 1 vetsi jak index
    set->count = j + 1;
    
    //kontrola duplicitnich prvku
    int elInSet = 0;
    for(int i = 0; i < set->count; i++)
    {
        if(set->elements[i][0] == '(')
            break;
        elInSet = 0;
        for(int j = 0; j < set->count; j++)
        {
            if (strcmp(set->elements[j], set->elements[i]) == 0)
            {
                elInSet++;
            }
            if(elInSet >= 2)
            {
                fprintf(stderr,"Duplicitni prvek v mnozine.");
                return -1;
            }
        }
    }
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
    if (loadSet(file, &set) == -1)
    {
        return -1;
    }

    //alokace
    relation->elements = (char ***)malloc((set.count / 2) * sizeof(char **));

    if (relation->elements == NULL)
    {
        fprintf(stderr,"Chyba pri allokaci.");
        return -1;
    }

    for (int i = 0; i < set.count / 2; i++)
    {
        relation->elements[i] = (char **)malloc(2 * sizeof(char *));

        if (relation->elements[i] == NULL)
        {
            fprintf(stderr,"Chyba pri allokaci.");
            return -1;
        }
    }
    int f = 0;
    for (int i = 0; i < set.count; i++)
    {

        //i % 2 protoye potrebuju umistit prvky stridave do noveho pole, a %2 bude stridvae vracet 0 a 1
        relation->elements[f][(i % 2)] = (char *)malloc(32 * sizeof(char));
        if (relation->elements[f][(i % 2)] == NULL)
        {
            fprintf(stderr,"Chyba pri allokaci.");
            return -1;
        }

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
    FreeSet(&set);
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
 * @brief Kontroluje, jestli mnozina obsahuje pouze prvky ze setA
 *
 * @param setA
 * @param relation
 * @return int
 *  @retval 0 obsahuje prvky pouze ze setA
 *  @retval 0 obsahuje i jine prvky jak jsou v setA
 */
int SetContainsOnlyElementsFromSetA(Set *setA, Set *setB)
{
    for (int x = 0; x < setB->count; x++)
    {
        int b = 0;
        for (int z = 0; z < setA->count; z++)
        {

            if (strcmp(setB->elements[x], setA->elements[z]) == 0)
            {
                b = 1;
            }
        }
        if (b != 1)
        {
            return -1;
        }
    }
    return 0;
}

/**
 * @brief Kontroluje, zda se v relaci vyskytuji duplicitni prvky a podle toho vraci bool
 *
 * @param relation
 * @return bool
 */
bool DuplicitElementInRelation(Relation *relation)
{
    int elIsIn;
    for(int i = 0; i < relation->count; i++)
    {
        elIsIn = 0;
        for(int j = 0; j < relation->count; j++)
        {
            if(strcmp(relation->elements[i][0], relation->elements[j][0]) == 0) 
            {
                if(strcmp(relation->elements[i][1], relation->elements[j][1]) == 0)
                {
                    elIsIn++;   
                    if(elIsIn >=2)
                    {
                        return true; 
                    }
                }
            }
        }
    }
    return false;
}




/**
 * @brief Kontroluje, jestli relace obsahuje pouze prvky ze setA
 *
 * @param setA
 * @param relation
 * @return int
 *  @retval 0 obsahuje prvky pouze ze setA
 *  @retval 0 obsahuje i jine prvky jak jsou v setA
 */
int RelationContainsOnlyElementsFromSetA(Set *setA, Relation *relation)
{
    for (int x = 0; x < relation->count; x++)
    {

        for (int j = 0; j < 2; j++)
        {
            int b = 0;
            for (int z = 0; z < setA->count; z++)
            {

                if (strcmp(setA->elements[z], relation->elements[x][j]) == 0)
                {
                    b = 1;
                }
            }
            if (b != 1)
            {
                return -1;
            }
        }
    }
    return 0;
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

    if (fp == NULL)
    {
        fprintf(stderr, "Chyba pri otevirani souboru.");
        data.err = 1;
        return data;
    }
    char c = 0;
    int dataSetBufferSize = 50;
    int dataRealtionBufferSize = 50;
    data.sets = (Set **)malloc(dataSetBufferSize * sizeof(Set *));
    data.relations = (Relation **)malloc(dataSetBufferSize * sizeof(Relation *));
    data.lines = (Line *)malloc((dataSetBufferSize + dataSetBufferSize + 1) * sizeof(Line)); //+1 protoze obsahuje i universum
    data.universum = (Set *)malloc(sizeof(Set));
    int line = 0;

    if (data.sets == NULL || data.universum == NULL || data.relations == NULL || data.lines == NULL)
    {
        fprintf(stderr,"Chyba pri allokaci.");
        data.err = true;
        return data;
    }
    //nacitani prvniho znaku na kazdym radku
    while ((c = fgetc(fp)) != EOF)
    {
        data.lines[line].typeOfLine = NONE;
        switch (c)
        {

        case 'U':
            if (loadSet(fp, data.universum))
            {
                fprintf(stderr, "Nepodarilo se nacist universum.");
                data.err = true;
                return data;
            }
            data.lines[line].line = data.universum;
            data.lines[line].typeOfLine = UNIVERSUM;
            printUniversum(*data.universum);
            printf("\n");
            break;
        case 'S':

            //pokud je treba nacist vice setu nez byl alokovan buffer, tak se zvetsi o velikost puvodniho bufferu
            if (data.setsCout >= dataSetBufferSize)
            {
                dataSetBufferSize += dataSetBufferSize;
                data.sets = (Set **)realloc(data.sets, dataSetBufferSize * sizeof(Set *));
                data.lines = (Line *)realloc(data.lines, (dataSetBufferSize + dataSetBufferSize + 1) * sizeof(Line));

                if (data.sets == NULL || data.lines == NULL)
                {
                    fprintf(stderr,"Chyba pri reallokaci.");
                    data.err = true;
                    return data;
                }
            }
            data.sets[data.setsCout] = (Set *)malloc(sizeof(Set));
            //pokud se alkoce nepoved, vrati data s err
            if (data.sets[data.setsCout] == NULL)
            {
                fprintf(stderr,"Chyba pri allokaci.");
                data.err = true;
                return data;
            }
            if (loadSet(fp, data.sets[data.setsCout]) == -1)
            {
                fprintf(stderr, "Nepodarilo se nacist mnozinu.");
                data.err = true;
                return data;
            }
            //Kontrola, jestli vsechny prvky patri do universa.
            if (SetContainsOnlyElementsFromSetA(data.universum, data.sets[data.setsCout]) == -1)
            {
                fprintf(stderr, "Prvek nepatri do univerza!!");
                data.err = true;
                return data;
            }
            data.sets[data.setsCout]->id = line;
            data.lines[line].line = data.sets[data.setsCout];
            data.lines[line].typeOfLine = SET;
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
                data.lines = (Line *)realloc(data.lines, (dataSetBufferSize + dataSetBufferSize + 1) * sizeof(Line));
                if (data.relations == NULL || data.lines == NULL)
                {
                    fprintf(stderr,"Chyba pri reallokaci.");
                    data.err = true;
                    return data;
                }
            }
            data.relations[data.relationsCount] = (Relation *)malloc(sizeof(Relation));
            //pokud se alkoce nepoved, vrati data s err
            if (data.relations[data.relationsCount] == NULL)
            {
                fprintf(stderr,"Chyba pri allokaci.");
                data.err = true;
                return data;
            }

            if (loadRelation(fp, data.relations[data.relationsCount]) == -1)
            {
                fprintf(stderr, "Nepodarilo se nacist mnozinu.");
                data.err = true;
                return data;
            }
            //Kontrola, jestli vsechny prvky patri do universa.
            if (RelationContainsOnlyElementsFromSetA(data.universum, data.relations[data.relationsCount]) == -1)
            {
                fprintf(stderr, "Prvek nepatri do univerza!!");
                data.err = true;
                return data;
            }

            //kontrola duplicitnich prvku
            if(DuplicitElementInRelation(data.relations[data.relationsCount]) == true)
            {
                fprintf(stderr, "Duplicitni prvek v relaci.");
                data.err = true;
                return data;
            }

            printRelation(*data.relations[data.relationsCount]);

            printf("\n");

            data.lines[line].line = data.relations[data.relationsCount];
            data.lines[line].typeOfLine = RELATION;
            data.relations[data.relationsCount]->id = line;
            data.relationsCount++;
            break;
        case 'C':
            if (doCommand(fp, data) != 0)
            {
                data.err = 1;
                return data;
            }
            break;
        default:
            if (doCommand(fp, data) != 0)
            {
                fprintf(stderr, "nezname znak na zacatku radku!");
                data.err = true;
                return data;
            }
            break;
        }
        line++;
    }
    data.lineCount = line;
    fclose(fp);
    return data;
}

int main(int argc, char **argv)
{
    //printData(Load("test.txt"));
    Data data;
    if (argc > 1)
    {
        data = Load(argv[1]);
    }
    else
    {
        fprintf(stderr, "Nezadana cesta k souboru!");
        return -1;
    }

    FreeData(data);
    // Load("test.txt");
    //printData(Load("test.txt"));



    return data.err;
}
