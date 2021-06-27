#include<stdio.h>
#include<string.h>
#include "operations.h"


typedef struct{
    country_t option1;
    country_t option2;
}options_t;

void help(){

printf("cConv a currency converter made in C\n");
printf("-------------------------------------------\n");
printf("Usage: cconv [ first option] [second option]\n\n");

printf("Options avaliable:\n");
printf("-b  Brazilian Real\n");
printf("-u  United States Dollar\n");
printf("-e  Euro\n");
printf("-h  Show help \n");

}


int main(int argc, char** argv){//if para a primeira opção,if para a segunda opção chamada da função de conversão
    options_t op;
    
    

    if(argc < 3 && 0 != strcmp(argv[1],"-h")){

        printf("Insufficient arguments, use -h to find help\n");

    }else if(argc > 3){
        printf("Too many arguments\n");
        printf("-h to find help\n");
    }else{
    
        if(0 == strcmp(argv[1],"-h")){

            help();

        }else{
            if(0 == strcmp(argv[1],"-b")){

                op.option1 = BR;

            }else if(0 == strcmp(argv[1],"-u")){

                op.option1 = US;

            }else if(0 == strcmp(argv[1],"-e")){

                op.option1 = EU;

            }


            if(0 == strcmp(argv[2],"-b")){

                op.option2 = BR;

            }else if(0 == strcmp(argv[2],"-u")){

                op.option2 = US;

            }else if(0 == strcmp(argv[2],"-e")){

                op.option2 = EU;

            }
            handleConversion(op.option1,op.option2);
        }
    }
    
}