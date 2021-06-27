#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operations.h"

double conversion;
double value;
void handleConversion(country_t c1,country_t c2){
    
    char symbol1[3];
    strcpy(symbol1,defineSymbol(c1));

    char symbol2[3];
    strcpy(symbol2,defineSymbol(c2));
    
    defineValues();
    double final_value =  converter();
    printf("%s %.2f is %s %.2f\n",symbol1,value,symbol2,final_value);
    
}

char* defineSymbol(country_t c){
    char *symbol = (char*) malloc(sizeof(char) * 3);

    switch (c){
    case BR:
        strcpy(symbol,BR_c);
        break;
    case EU:
        strcpy(symbol,EU_c);
        break;
    case US:
        strcpy(symbol,US_c);
        break;
    default:
        break;
    }
    return symbol;
}

void defineValues(){
    printf("Conversion rate: ");
    scanf("%lf",&conversion);
    printf("Value: ");
    
    scanf("%lf",&value);

}

double converter(){
    return value * conversion;
}