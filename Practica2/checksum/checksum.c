// Elaborado por: Hernández Rodríguez Armando Giovanni
#include<stdio.h>

unsigned short int checksum = 0x0000; // 2 bytes = 16 bits

unsigned short int getChecksum(unsigned char frame[] ){
    unsigned char i = 0x00; 
    unsigned char aux = 0x00;
    checksum = 0x0000;  //  16 bits = 2 bytes
    
    // Suma hexadecimal con bloques de 2bytes
    for(i=0; i<20; i++){
        // Suma la concatenacion de 1 byte y el byte siguiente de una trama
        checksum += (frame[i]<<8) | frame[++i];
        //Acarreo fuera de los 16 bits
        if(i==17){
            aux = checksum>>12;
        }
    } 
    // Suma hexadecimal con el acarreo
    checksum += aux;
    // Complementar a uno el resultado
    checksum = ~(checksum);
  
    return checksum;
}


void correctChecksum(unsigned char frame[]){
    checksum = 0x0000;
    // Obtener el checksum correspondiente a una trama para ello se asigna un valor de 0 al checksum 
    frame[10]= 0x00;
    frame[11] = 0x00;
    checksum = getChecksum(frame);

    /* Si el checksum dado es igual al calculado entonces el checksum es correcto
        sino el checksum es incorrecto y se indica el valor del checksum correcto.
    */
     if((checksum >> 8) == frame[10] & (checksum & 0x00ff)== frame[11]){
        printf("\nChecksum correcto");
    }else{
        printf("\nChecksum incorrecto");
        printf("\nEl checksum correcto es: %.4x", checksum);
    }
}

int main(){
                                                                                    
    unsigned char frame1[] = {0x45, 0x00, 0x01, 0xe2, 0xd7, 0xdb, 0x40, 0x00, 0x80, 0x06, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0x43, 0x94, 0xcc, 0x3a, 0xdd};
    unsigned char frame2[] = {0x45, 0x00, 0x01, 0x9c, 0xd7, 0xde, 0x40, 0x00, 0x80, 0x06, 0x88, 0x9d, 0xc0, 0xa8, 0x01, 0x43, 0x94, 0xcc, 0x3a, 0xdd};
    checksum = 0x0000; 

    // Prueba 1
    checksum = getChecksum(frame1);
    printf("\nPrueba1\nChecksum de la trama1: %.4x\n",checksum);

    // Prueba 2
    printf("\nPrueba2");
    correctChecksum(frame2);
    

    return 0;
}


