#include<stdio.h>  //Hernández Rodríguez Armando Giovanni Practica 0

int main(){
    unsigned char input = 0;
    unsigned char i = 1;
    unsigned char count = 0;
    
    printf("Introduzca un caracter: ");
    scanf("%c", &input);

    for(i=1; i<i<<8; i<<=1){ //i = i<<1 
        if(input&i){count++;} // TRUE se ejecuta la acción
    }

    printf("\nEl caracter %c = 0x%.2X = %d tiene:  %d bits encendidos\n", input, input, input, count);

}