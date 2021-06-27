#ifndef OPERATIONS_H

#define OPERATIONS_H

#define BR_c "R$"
#define US_c "$"
#define EU_c "€"

typedef enum {
    BR,
    US,
    EU
}country_t;
void handleConversion(country_t,country_t);
char* defineSymbol(country_t c);
void defineValues();
double converter();

#endif