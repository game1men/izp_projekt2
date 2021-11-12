#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct {
    char **elements;
    int count;
} Set;

typedef struct {
    Set **sets;
    int setsCout;
    Set *universum;
} Data;//rename in future

void loadSet(FILE *file, Set *set){

    set->elements = (char**)malloc(100*sizeof(char*));
    char c =0;
    int j = 0;
    set->elements[0] = (char*)malloc(100*sizeof(char));

    for ( int i =0;(c =fgetc(file))!='\n'; i++)
    {
        if(c == EOF){
            break;
        }
        if(c==' '){
            set->elements[j][i] = 0;
             j++;
             i=-1;
             set->count = j+1;
            set->elements[j] = (char*)malloc(100*sizeof(char));
            continue;
        }

        set->elements[j][i] = c;

    }

    return;

}

// void AppendSet(Set set1, Set set2){

//     char* ptr;
//     ptr = (char**) malloc((set1.count + set2.count)*sizeOf(char*));

//     for (int i = 0; set1.count<i; i++)
//     {
//         int sizeOfString;
//         for(int j = 0; set1.elements[i] != 0; j++){

//         }
//        ptr[i]= (char**) malloc((set1.count + set2.count)*sizeOf(char*))set1.elements[i];
//     }

//     return set;

// }


Data Load(char file[]){
    Data data;
    data.setsCout=0;
     FILE *fp = fopen(file, "r");
    char c = 0;
    data.sets = (Set**)malloc(100*sizeof(Set*));
    data.universum = (Set*)malloc(sizeof(Set));
    while((c = fgetc(fp))!=EOF){
        switch(c){

            case 'U':
                  loadSet(fp, data.universum);
                break;
            case 'S':
                  data.sets[data.setsCout] = (Set*)malloc(sizeof(Set));
                 loadSet(fp,data.sets[data.setsCout]);
                 data.setsCout++;
                break;
            case 'R': //TODO:
                break;
            case 'C': //TODO:
                break;
        }
    }
//printing for debug
  printf("\n");
    for(int i =0;i<data.setsCout; i++){

         for(int j =0;j<data.sets[i]->count; j++){


                            printf(data.sets[i]->elements[j]);


             printf(" ");
           }
            printf("\n");

    }
 printf("\n universum\n ");
        for(int j =0;j<data.universum->count; j++){


                            printf(data.universum->elements[j]);

 printf(" ");

           }
 printf("\n");

    return data;

}



int main(void){

    Load("test.txt");
	return 0;
}
