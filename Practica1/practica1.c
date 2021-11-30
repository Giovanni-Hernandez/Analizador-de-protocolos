#include<stdio.h>

int main(){
    unsigned char MR[4] = {0, 0, 0, 0};
    unsigned char ip[4] = {0X0A, 0XFA, 0X01, 0X01}; // 10.250.1.1
    //unsigned char ip[4] = {0X96, 0X0A, 0X0F, 0X00}; // 150.10.15.0
    //unsigned char ip[4] = {0XC0, 0X0E, 0X02, 0X00}; // 192.14.2.0
    //unsigned char ip[4] = {0X94, 0X11, 0X09, 0X01}; // 148.17.9.1
    //unsigned char ip[4] = {0XC1, 0X00, 0X00, 0XFF}; // 193.0.0.255
    //unsigned char ip[4] = {0XDC, 0XC8, 0X17, 0X01}; // 220.200.23.1
    //unsigned char ip[4] = {0XB1, 0X64, 0X12, 0X04}; // 177.100.18.4
    //unsigned char ip[4] = {0X5F, 0X00, 0X15, 0X00}; // 95.0.21.0
    //unsigned char ip[4] = {0X6F, 0X6F, 0X6F, 0X6F}; // 111.111.111.111
    //unsigned char ip[4] = {0XDB, 0X1A, 0X33, 0X00}; // 219.26.51.0
    //unsigned char ip[4] = {0XA7, 0X00, 0X00, 0X00}; // 167.0.0.0
    //unsigned char ip[4] = {0XF0, 0X01, 0X01, 0X01}; // 240.1.1.1
    //unsigned char ip[4] = {0X0A, 0X01, 0X01, 0X01}; // 10.1.1.1
    //unsigned char ip[4] = {0XE1, 0X01, 0X01, 0X01}; // 225.1.1.1
    
    if(ip[0] & 128){ //Resultado es difente de cero 1
        if(ip[0] & 64){ // 11
            if(ip[0] & 32){ // 111
                if(ip[0] & 16){ // 1111
                    printf("\nClase E");
                    printf("\nIP:%d.%d.%d.%d\nSin mascara de red", ip[0], ip[1], 
                    ip[2], ip[3]);
                    return 0;
                }else{ // 1110
                    printf("\nClase D");
                    printf("\nIP:%d.%d.%d.%d\nSin mascara de red", ip[0], ip[1], 
                    ip[2], ip[3]);
                    return 0;
                }
            }else{ // 110
                MR[0] = 255;
                MR[1] = 255;
                MR[2] = 255;
                
                printf("\nClase C");
            }
        }else{ // 10
            MR[0] = 255;
            MR[1] = 255;
            printf("\nClase B");
        }
    }else{// Resultado es cero 0
        MR[0] = 255;
        printf("\nClase A"); 
    }
    
    printf("\nIP:%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    printf("\nIP Broadcast: %d.%d.%d.%d", ip[0]| (unsigned char)~MR[0], 
    ip[1]| (unsigned char) ~MR[1], ip[2]| (unsigned char)~MR[2], 
    ip[3]| (unsigned char)~MR[3]);
    printf("\nIP Red: %d.%d.%d.%d\n", ip[0] & MR[0], ip[1] & MR[1], ip[2] & MR[2], 
    ip[3] & MR[3]);  
    return 0;
}