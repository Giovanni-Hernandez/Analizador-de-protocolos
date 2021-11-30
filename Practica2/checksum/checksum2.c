#include<stdio.h>

unsigned int getChecksum(unsigned char frame[] ){
    //unsigned int block[10];  
    unsigned char i = 0x00; 
    unsigned char aux = 0x00;
    unsigned int checksum = 0x00000000;  //  32 bits = 4 bytes
    
    // Arreglo con bloques de 16 bits = 2 bytes 
    for(i=0; i<20; i++){
      //block[aux]= (frame[i]<<8) | frame[++i]; // block[0] = frame[0]frame[1], block[1],..., block[9]
      //aux++;

        checksum += (frame[i]<<8) | frame[++i];
        aux = checksum>>1;
    }


    // Suma hexadecimal con cada elemento del arreglo
    /*
    for(i=0; i<10; i++){
        checksum += block[i];
        aux = (checksum - (checksum % 16))/16;
    }*/

    // Acarreo fuera de los 16 bits
    aux = checksum>>16;
    // Suma sin el acarreo fuera de los 16 bits
    checksum = checksum & 0x0ffff;  //add = add & mask; 
    // Suma hexadecimal con el acarreo
    checksum += aux;
    // Complementar a uno el resultado
    checksum = ~(checksum | 0xffff0000);
  
    return checksum;
}


void correctChecksum(unsigned char frame[]){
    unsigned short int checksum = 0x00000000;
    // obtener el checksum correspondiente a una trama
    checksum = getChecksum(frame);

    /* Si el checksum dado es igual al calculado entonces el checksum es correcto
        sino el checksum es incorrecto y se procede a calcular el checksum correcto
    */
     if((checksum >> 8) == frame[10] & (checksum & 0x00ff)== frame[11]){
        printf("\nChecksum correcto");
    }else{
        printf("\nChecksum incorrecto");
        frame[10]= 0x00;
        frame[11] = 0x00;
        checksum = getChecksum(frame);
        printf("\nEl checksum correcto es %x", checksum);
    }
}

int main(){
                                                                                    //frame1[10] frame1[11]
    unsigned char frame1[] = {0x45, 0x00, 0x01, 0xe2, 0xd7, 0xdb, 0x40, 0x00, 0x80, 0x06, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0x43, 0x94, 0xcc, 0x3a, 0xdd};
    unsigned char frame2[] = {0x45, 0x00, 0x01, 0xe2, 0xd7, 0xdb, 0x40, 0x00, 0x80, 0x06, 0x88, 0x9d, 0xc0, 0xa8, 0x01, 0x43, 0x94, 0xcc, 0x3a, 0xdd};
    unsigned short int checksum = 0x0000; // 2 bytes

    // Prueba 1
    checksum = getChecksum(frame1);
    
    // Prueba 2
    printf("\n----Checksum: %.4x-----",checksum);
    correctChecksum(frame2);

    return 0;
}