/*
Projekt 2 - Prace s datovymi strukturami

github repozitar:   https://github.com/game1men/izp_projekt2

vytvorili:
    Rene Ceska          xceska06    https://github.com/game1men
    Krystof Andrysek    xandry12    https://github.com/ArgonOutPlay
    Ivan Mahut          xmahut01    https://github.com/fasen92
    Dusan Sluka         xsluka00    https://github.com/Dusko-8
    Samuel Barak        xbarak08    https://github.com/SamB4rak
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    char **elements; //elementy setu [prvek][znak]
    int count;       //pocet elementu setu
    int id;          //radek na kterem je set
} Set;

typedef struct
{
    char ***elements; //elementy relace [prvek][leva strana = 0, prava strana = 1][znak]
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
    Set **sets;           //vsechny nactene sety
    int setsCout;         //pocet setu
    Relation **relations; //vsechny nactene relace
    int relationsCount;   //pocet relaci
    Set *universum;       //universum
    bool err;             //nastal error
    int lineCount;        //pocet nahranych radku
} Data;

const char *COMMANDS[] = {"empty", "card", "complement", "union", "intersect", "minus", "subseteq",
                          "subset", "equals", "reflexive", "symmetric", "antisymmetric", "transitive", "function", "domain",
                          "codomain", "injective", "surjective", "bijective", "closure_ref", "closure_sym", "closure_trans",
                          "select", "true", "false"};

const int COMMAND_COUNT = 25;

/**
 * @brief uvolní paměť od všeho co bylo dynamicky alokováno v structu Relation
 *
 * @param relation
 */
void FreeRelation(Relation *relation)
{

    for (int i = 0; i < relation->count; i++)
    {
        if (relation->elements[i][0] != NULL)
            free(relation->elements[i][0]);
        if (relation->elements[i][1] != NULL)
            free(relation->elements[i][1]);
        if (relation->elements[i] != NULL)
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
        if (set->elements[i] != NULL)
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
        if (data.relations[i] != NULL)
            FreeRelation(data.relations[i]);
        free(data.relations[i]);
    }

    for (int i = 0; i < data.setsCout; i++)
    {
        if (data.sets[i] != NULL)
            FreeSet(data.sets[i]);
        free(data.sets[i]);
    }

    if (data.universum != NULL)
    {
        FreeSet(data.universum);
        free(data.universum);
    }
    if (data.universum != NULL)
        free(data.lines);
    if (data.universum != NULL)
        free(data.relations);
    if (data.universum != NULL)
        free(data.sets);
}

void printSet(Set set, char c);

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
 * @brief zjistuje jestli je relace injektivni a jestli jeji prvky patri do mnozin A a B, podle toho vraci bool
 *
 * @param relation
 * @param setA
 * @param setB
 */
bool injective(Relation *relation, Set *setA, Set *setB)
{
    for (int i = 0; i < relation->count; i++)
    {
        if ((isInSet(relation->elements[i][0], setA) && isInSet(relation->elements[i][1], setB)) == false)
        {
            return false;
        }
    }

    if (relation->count != setA->count)
    {
        return false;
    }

    if (setA->count > setB->count)
    {
        return false;
    }

    int a, b;
    for (int i = 0; i < relation->count; i++)
    {
        a = 0;
        b = 0;
        for (int j = 0; j < relation->count; j++)
        {
            if (strcmp(relation->elements[i][0], relation->elements[j][0]) == 0)
            {
                a++;
            }
            if (a >= 2)
            {
                return false;
            }
            if (strcmp(relation->elements[i][1], relation->elements[j][1]) == 0)
            {
                b++;
            }
            if (b >= 2)
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief zjistuje jestli je relace surjektivni a jestli jeji prvky patri do mnozin A a B, podle toho vraci bool
 *
 * @param relation
 * @param setA
 * @param setB
 */
bool surjective(Relation *relation, Set *setA, Set *setB)
{
    for (int i = 0; i < relation->count; i++)
    {
        if ((isInSet(relation->elements[i][0], setA) && isInSet(relation->elements[i][1], setB)) == false)
        {
            return false;
        }
    }

    //relaci musi byt vzdy vice nebo stejne jak prvku v mnozine B
    if (relation->count < setB->count)
    {
        return false;
    }

    //kontrola, zda jsem v relaci pouzil vsechny prvky z mnoziny B
    bool isIn;
    for (int i = 0; i < setB->count; i++)
    {
        isIn = false;
        for (int j = 0; j < relation->count; j++)
        {
            if (strcmp(relation->elements[j][1], setB->elements[i]) == 0)
            {
                isIn = true;
            }
        }
        if (isIn == false)
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
            if (a >= 2)
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief zjistuje jestli je relace bijektivni(surjektivni i injektivni) a jestli jeji prvky patri do mnozin A a B, podle toho vraci bool
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
void dom_cod(Relation *relation, int identification)
{
    int usedElements[relation->count + 1];
    for (int i = 0; i < relation->count + 1; i++)
    {
        usedElements[i] = -1;
    }
    int j = 0;
    bool con;
    printf("S");
    for (int i = 0; i < relation->count; i++)
    {
        con = false;
        for (int x = 0; x < j; x++)
        {

            if (strcmp(relation->elements[i][identification], relation->elements[(usedElements[x])][identification]) == 0)
            {
                con = true;
                break;
            }
        }

        if (con == false)
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
 * @brief tiskne pocet prvku v mnozine A (definovane na radku A).
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
 * @brief vypisuje true alebo false, ak mnozina A je podmnozinou mnoziny B
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
 * @brief vypisuje true alebo false, ak je mnozina A vlastnou podmnozinou mnoziny B
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
 * @brief Ziska vsehny id z daneho radku.
 *
 * @param file ukazatel na soubor
 * @param ids bere pole ids, do ktereho ulozi ziskana id
 * @param parsed pocet id, ktera byl nactena
 */
int getIds(FILE *file, int ids[4], int *parsed)
{

    char c[100] = {0}; //buffer znaku
    int x = 0;
    //cte radek, dokud nenarazi na konec radku
    for (; (c[x] = fgetc(file)) != EOF && c[x] != '\n'; x++)
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

    //buffer 32 znaků bude stacit, jelikoz predem zname vsechny mozne prikazi
    char cmd[32];

    //fscanf umi brat stringy podle mezer
    fscanf(file, "%31s", cmd);
    //nastavime pole id do -1, aby se dalo poznat ktera id byla nalezena, a ktera ne (tam kde je -1 nebylo ulozeno zadne id)
    int ids[4] = {-1, -1, -1, -1};

    //ziskame veskera id
    int parsed;
    if (getIds(file, ids, &parsed) == -1)
    {
        fprintf(stderr, "Prilis mnoho argumentu\n");
        return -1;
    }

    //pokud nebyla zadana zadna id, tak vratime error.
    if (ids[0] == -1)
    {
        fprintf(stderr, "Nebyly zadany relace/mnoziny se kterymi se ma pracovat\n");
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
                fprintf(stderr, "Nelze provest prikaz mezi mnouzinou a relaci\n");
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
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            empty((Set *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "card") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            card((Set *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "complement") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            complement((Set *)(data.lines[ids[0] - 1].line), (Set *)(data.universum));
        }
        else if (strcmp(cmd, "union") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[2] != -1 || ids[0] == -1 || ids[1] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            unionAB((Set *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line));
        }
        else if (strcmp(cmd, "intersect") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[2] != -1 || ids[0] == -1 || ids[1] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            intersect((Set *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line));
        }
        else if (strcmp(cmd, "minus") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[2] != -1 || ids[0] == -1 || ids[1] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            minus((Set *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line));
        }
        else if (strcmp(cmd, "subseteq") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[2] != -1 || ids[0] == -1 || ids[1] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            subseteq((Set *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line));
        }
        else if (strcmp(cmd, "subset") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[2] != -1 || ids[0] == -1 || ids[1] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            subset((Set *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line));
        }
        else if (strcmp(cmd, "equals") == 0)
        {
            //pokud bylo zadano vice nebo mene argumentu, nez bylo treba vyhod chybu
            if (ids[2] != -1 || ids[0] == -1 || ids[1] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            equals((Set *)(data.lines[ids[0] - 1].line), (Set *)(data.lines[ids[1] - 1].line));
        }
        else
        {
            fprintf(stderr, "Neplatny prikaz, nebo prikaz nelze provest na mnozine\n");
            return -1;
        }
    }
    else if (data.lines[ids[0] - 1].typeOfLine == RELATION) //do relacnich prikazu pustime jenom relace
    {
        if (strcmp(cmd, "reflexive") == 0)
        {
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            reflexive((Relation *)(data.lines[ids[0] - 1].line), (Set *)(data.universum));
        }
        else if (strcmp(cmd, "symmetric") == 0)
        {
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            symmetric((Relation *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "antisymmetric") == 0)
        {
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            antisymmetric((Relation *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "transitive") == 0)
        {
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            transitive((Relation *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "function") == 0)
        {
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            function((Relation *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "domain") == 0)
        {
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
                return -1;
            }
            domain((Relation *)(data.lines[ids[0] - 1].line));
        }
        else if (strcmp(cmd, "codomain") == 0)
        {
            if (ids[1] != -1 || ids[0] == -1)
            {
                fprintf(stderr, "Nespravny pocet argumentu\n");
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
            fprintf(stderr, "Neplatny prikaz, nebo prikaz nelze provest na relaci\n");
            return -1;
        }
    }
    else //pokud se nejedna o prikaz na mnozine ani o prikaz na relaci, tak se jedna o chybu
    {
        fprintf(stderr, "Prikaz nelze provest na tomto radku\n");
        return -1;
    }
    return 0;
}

/**
 * @brief vraci bool podle toho, jestli se jedna o nevalidni prvek
 *
 * @param str element
 */
bool isCommand(char *str)
{
    for (int i = 0; i < COMMAND_COUNT; i++)
    {
        if (strcmp(str, COMMANDS[i]) == 0)
        {
            return true;
        }
    }
    return false;
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
    int charArrayBufferSize = 60;
    set->count = 0;

    int j = 0;

    set->elements = (char **)malloc(elementsBufferSize * sizeof(char *));
    if (set->elements == NULL)
    {
        fprintf(stderr, "Chyba pri allokaci.\n");
        free(set->elements);
        return -1;
    }
    set->elements[0] = (char *)malloc(charArrayBufferSize * sizeof(char));

    //pokud se nepodvedla alokace
    if (set->elements[0] == NULL)
    {
        fprintf(stderr, "Chyba pri allokaci.\n");
        free(set->elements[0]);
        return -1;
    }

    char c = 0;

    if ((c = fgetc(file)) == '\n')
    { //preskoci prvni mezeru, pokud se jedna o znak noveho radku ukonci funkci
        free(set->elements[0]);
        return 0;
    }
    if (c != ' ')
    { //pokud prvni znak nebyl mezera ukonci funkci a vrati error
        fprintf(stderr, "Chybi mezera za prvnim znakem radku!\n");
        free(set->elements[0]);
        return -1;
    }

    //nacita znaky dokud nenarazi na konec radku
    int i = 0;
    for (; (c = fgetc(file)) != EOF && c != '\n'; i++)
    {
        //pokud obsahuje prvky co nepatri do nazvu prvku/relace vyhod chybu
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '(' || c == ')'))
        {
            fprintf(stderr, "Neplatny znak v nazvu prvku!!\n");
            set->count = j + 1;
            set->elements[j][i] = 0;

            return -1;
        }
        if (i >= charArrayBufferSize)
        {
            fprintf(stderr, "Prvek je delsi nez maximalni povolena delka!!\n");
            set->count = j + 1;
            set->elements[j][i - 1] = 0;

            return -1;
        }
        //rozdeli jednotlive prvky podle mezer
        if (c == ' ')
        {
            //ukonci retezec 0
            set->elements[j][i] = 0;

            j++;
            //pokud je treba nacist vice prvku nez byl alokovan buffer, tak se zvetsi o velikost puvodniho bufferu
            if (j >= elementsBufferSize)
            {
                elementsBufferSize += elementsBufferSize;
                set->elements = (char **)realloc(set->elements, elementsBufferSize * sizeof(char *));
                if (set->elements == NULL)
                {
                    fprintf(stderr, "Chyba pri reallokaci.\n");
                    return -1;
                }
            }

            //alokace mista pro dalsi prvek
            set->elements[j] = (char *)malloc(charArrayBufferSize * sizeof(char));
            //pokud se nepodvedla alokace
            if (set->elements[j] == NULL)
            {
                fprintf(stderr, "Chyba pri allokaci.\n");
                return -1;
            }
            //vynulovani i (-1 aby po inkrementaci bylo 0)
            i = -1;
            continue;
        }
        set->elements[j][i] = c;
    }

    //da 0 na konci stringu
    set->elements[j][i] = 0;
    //pocet prvku je o 1 vetsi jak index
    set->count = j + 1;

    //kontrola duplicitnich prvku
    int elInSet = 0;
    for (int i = 0; i < set->count; i++)
    {
        if (set->elements[i][0] == '(')
            break;
        elInSet = 0;
        for (int j = 0; j < set->count; j++)
        {
            if (strcmp(set->elements[j], set->elements[i]) == 0)
            {
                elInSet++;
            }
            if (elInSet >= 2)
            {
                fprintf(stderr, "Duplicitni prvek v mnozine.\n");
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
    relation->elements = NULL;
    Set set;
    //int relationsBufferSize = 20;
    //nacteni prvku ze radku oddelenych mezerou
    if (loadSet(file, &set) == -1)
    {
        FreeSet(&set);
        return -1;
    }

    //alokace
    relation->elements = (char ***)malloc((set.count / 2) * sizeof(char **));

    if (relation->elements == NULL)
    {
        fprintf(stderr, "Chyba pri allokaci.\n");
        FreeSet(&set);
        return -1;
    }

    for (int i = 0; i < set.count / 2; i++)
    {
        relation->elements[i] = (char **)malloc(2 * sizeof(char *));

        if (relation->elements[i] == NULL)
        {
            fprintf(stderr, "Chyba pri allokaci.\n");
            FreeSet(&set);
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
            fprintf(stderr, "Chyba pri allokaci.");
            FreeSet(&set);
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
 * @param c - znak co se ma vypsat na zacatek radku
 */
void printSet(Set set, char c)
{
    printf("%c", c);
    for (int j = 0; j < set.count; j++)
    {
        printf(" %s", set.elements[j]);
    }
}

/**
 * @brief Kontroluje, jestli mnozina obsahuje pouze prvky ze setA
 *
 * @param setA
 * @param relation
 * @return int
 *  @retval 0 obsahuje prvky pouze ze setA
 *  @retval -1 obsahuje i jine prvky jak jsou v setA
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
    for (int i = 0; i < relation->count; i++)
    {
        elIsIn = 0;
        for (int j = 0; j < relation->count; j++)
        {
            if (strcmp(relation->elements[i][0], relation->elements[j][0]) == 0)
            {
                if (strcmp(relation->elements[i][1], relation->elements[j][1]) == 0)
                {
                    elIsIn++;
                    if (elIsIn >= 2)
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
 *  @retval -1 obsahuje i jine prvky jak jsou v setA
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
 * @brief Nacte data ze souboru, a vrati je ve forme structu Data
 *
 * @param file cesta k souboru
 * @return Data
 */
Data Load(char file[])
{
    //inicializace
    Data data;
    data.universum = NULL;
    data.relations = NULL;
    data.sets = NULL;
    data.setsCout = 0;
    data.relationsCount = 0;
    data.err = false;

    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Chyba pri otevirani souboru.\n");
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
    data.universum->elements = NULL;
    data.universum->count = 0;
    int line = 0;
    bool universumLoaded = 0;
    if (data.sets == NULL || data.universum == NULL || data.relations == NULL || data.lines == NULL)
    {
        fprintf(stderr, "Chyba pri allokaci.\n");
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

            if (universumLoaded)
            {
                fprintf(stderr, "Universum zadano vice nez 1\n");
                data.err = true;
                fclose(fp);
                return data;
            }
            if (loadSet(fp, data.universum))
            {
                fprintf(stderr, "Nepodarilo se nacist universum.\n");
                data.err = true;
            }
             //ulozeni ukazatele na spravny radek
            data.lines[line].line = data.universum;
            data.lines[line].typeOfLine = UNIVERSUM;

            //kontrola nevalidniho prvku
            for (int i = 0; i < data.universum->count; i++)
            {
                if (isCommand(data.universum->elements[i]) == true)
                {
                    fprintf(stderr, "Jedna se o nevalidni prvek.\n");
                    data.err = true;
                }
            }

            printSet(*data.universum, 'U');
            printf("\n");
            if (data.err == 1)
            {
                fclose(fp);
                return data;
            }
            universumLoaded = 1;
            break;
        case 'S':
            if (!universumLoaded)
            {

                fprintf(stderr, "Nezadano universum\n");
                data.err = true;
                fclose(fp);
                return data;
            }
            //pokud je treba nacist vice setu nez byl alokovan buffer, tak se zvetsi o velikost puvodniho bufferu
            if (data.setsCout >= dataSetBufferSize)
            {
                dataSetBufferSize += dataSetBufferSize;
                data.sets = (Set **)realloc(data.sets, dataSetBufferSize * sizeof(Set *));
                data.lines = (Line *)realloc(data.lines, (dataSetBufferSize + dataSetBufferSize + 1) * sizeof(Line));

                if (data.sets == NULL || data.lines == NULL)
                {
                    fprintf(stderr, "Chyba pri reallokaci.\n");
                    data.err = true;
                }
            }
            data.sets[data.setsCout] = (Set *)malloc(sizeof(Set));

            //pokud se alkoce nepovede, vrati data s err
            if (data.sets[data.setsCout] == NULL)
            {
                fprintf(stderr, "Chyba pri allokaci.\n");
                data.err = true;
            }
            if (loadSet(fp, data.sets[data.setsCout]) == -1)
            {
                fprintf(stderr, "Nepodarilo se nacist mnozinu.\n");
                data.err = true;
            }
            //Kontrola, jestli vsechny prvky patri do universa.
            if (SetContainsOnlyElementsFromSetA(data.universum, data.sets[data.setsCout]) == -1)
            {
                fprintf(stderr, "Prvek nepatri do univerza!!\n");
                data.err = true;
            }

            //kontrola nevalidniho prvku
            for (int i = 0; i < data.sets[data.setsCout]->count; i++)
            {
                if (isCommand(data.sets[data.setsCout]->elements[i]) == true)
                {
                    fprintf(stderr, "Jedna se o nevalidni prvek.\n");
                    data.err = true;
                }
            }
             //ulozeni ukazatele na spravny radek
            data.sets[data.setsCout]->id = line;
            data.lines[line].line = data.sets[data.setsCout];
            data.lines[line].typeOfLine = SET;
            printSet(*data.sets[data.setsCout], 'S');
            printf("\n");
            data.setsCout++;
            if (data.err == 1)
            {
                fclose(fp);
                return data;
            }
            break;
        case 'R':
            //pokud je treba nacist vice setu nez byl alokovan buffer, tak se zvetsi o velikost puvodniho bufferu

            if (!universumLoaded)
            {
                fprintf(stderr, "Nezadano universum\n");
                data.err = true;
                fclose(fp);
                return data;
            }

            if (data.relationsCount >= dataRealtionBufferSize)
            {
                dataRealtionBufferSize += dataRealtionBufferSize;
                data.relations = (Relation **)realloc(data.relations, dataRealtionBufferSize * sizeof(Relation *));
                data.lines = (Line *)realloc(data.lines, (dataSetBufferSize + dataSetBufferSize + 1) * sizeof(Line));
                if (data.relations == NULL || data.lines == NULL)
                {
                    fprintf(stderr, "Chyba pri reallokaci.\n");
                    data.err = true;
                    fclose(fp);
                }
            }
            data.relations[data.relationsCount] = (Relation *)malloc(sizeof(Relation));
            //pokud se alkoce nepovede, vrati data s err
            if (data.relations[data.relationsCount] == NULL)
            {
                fprintf(stderr, "Chyba pri allokaci.\n");
                data.err = true;
            }

            if (loadRelation(fp, data.relations[data.relationsCount]) == -1)
            {
                fprintf(stderr, "Nepodarilo se nacist mnozinu.\n");
                data.err = true;
            }
            //Kontrola, jestli vsechny prvky patri do universa.
            if (RelationContainsOnlyElementsFromSetA(data.universum, data.relations[data.relationsCount]) == -1)
            {
                fprintf(stderr, "Prvek nepatri do univerza!!\n");
                data.err = true;
            }

            //kontrola duplicitnich prvku
            if (DuplicitElementInRelation(data.relations[data.relationsCount]) == true)
            {
                fprintf(stderr, "Duplicitni prvek v relaci.\n");
                data.err = true;
            }

            //kontrola nevalidniho prvku
            for (int i = 0; i < data.relations[data.relationsCount]->count; i++)
            {
                if ((isCommand(data.relations[data.relationsCount]->elements[i][0]) == true) ||
                    (isCommand(data.relations[data.relationsCount]->elements[i][0]) == true))
                {
                    fprintf(stderr, "Jedna se o nevalidni prvek.\n");
                    data.err = true;
                }
            }

            printRelation(*data.relations[data.relationsCount]);

            printf("\n");

            //ulozeni ukazatele na spravny radek
            data.lines[line].line = data.relations[data.relationsCount];
            data.lines[line].typeOfLine = RELATION;
            data.relations[data.relationsCount]->id = line;
            data.relationsCount++;

            if (data.err == 1)
            {
                fclose(fp);
                return data;
            }
            break;
        case 'C':

            if (data.relationsCount == 0 && data.setsCout == 0)
            {
                fprintf(stderr, "Nezadana zadna relace ani mnozina.\n");
                data.err = true;
            }
            if (doCommand(fp, data) != 0)
            {
                data.err = 1;
                fclose(fp);
                return data;
            }
            break;
        default:
            fprintf(stderr, "Neznamy znak na zacatku radku!\n");
            data.err = true;
            fclose(fp);
            return data;
            break;
        }
        line++;
        if (line > 1000)
        {
            fprintf(stderr, "Vic jak 1000 radku.\n");
            data.err = true;
            fclose(fp);
            return data;
        }
    }
    data.lineCount = line;
    fclose(fp);
    return data;
}

int main(int argc, char **argv)
{
    Data data;
    if (argc > 1)
    {
        data = Load(argv[1]);
    }
    else
    {
        fprintf(stderr, "Nezadana cesta k souboru!\n");
        return -1;
    }
    FreeData(data);
    return data.err;
}
