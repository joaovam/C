//Strlen created for UTF-8, 16 and 32
#include<string.h>
#include <stdlib.h>
#include <stdio.h>
#include <uchar.h>


typedef struct{
    long num_chars;
    long num_bytes;
}unicode_len_t;

unicode_len_t strlen_ascii(char * str){
    unicode_len_t res;
    if(!str){
        res.num_bytes = 0;
        res.num_chars = 0;
        
    }else{
        res.num_bytes = strlen(str)+1;//considering the null character at the end of the string
        res.num_chars = strlen(str)+1;
    }
    return res;

}

unicode_len_t strlen_u8(char* str){
    unicode_len_t res;
    if(!str){
        res.num_bytes = 0;
        res.num_chars = 0;

    }else{
        res.num_bytes = 1;//considering the null character at the
        res.num_chars = 1;//end of the string
        while(*str){
            if((*str | 0x7f)==0x7f){
                res.num_bytes++;
                res.num_chars++;
                str++;
            }else if((*str | 0xc0)==0xc0){
                res.num_bytes+=2;
                res.num_chars++;
                str+=2;
            }else if((*str|0xe0)==0xe0){
                res.num_bytes+=3;
                res.num_chars++;
                str+=3;
            }else if((*str|0xf0)==0xf0){
                res.num_bytes+=4;
                res.num_chars++;
                str+=4;
            }else{
                fprintf(stderr,"UTF-8 string is not valid!\n");
                exit(1);
            }
            return res;
        }
    }
}

unicode_len_t strlen_u16(char16_t *str){

    unicode_len_t res;
    if(!str){
        res.num_bytes = 0;
        res.num_chars = 0;

    }else{
        res.num_bytes = 2;//considering the null character at the
        res.num_chars = 1;//end of the string
        while(*str){
            if(*str<0xdc00 || *str >0xdfff){
                res.num_chars++;
                res.num_bytes+=2;
                str++;
            }else{
                res.num_chars++;
                res.num_bytes+=4;
                str+=2;
            }
        }
    }
    return res;
}

unicode_len_t strlen_u32(char32_t *str){

    unicode_len_t res;

    if(!str){
        res.num_bytes = 0;
        res.num_chars = 0;
    }else{
        res.num_bytes = 4;//considering the null character at the
        res.num_chars = 1;//end of the string
        while(*str){
            res.num_bytes+=4;
            res.num_chars++;
            str++;
        }

    }
    return res;

}