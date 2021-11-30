// Elaborado por: Hernández Rodríguez Armando Giovanni
// Analizador de Tramas Versión 3
#include<stdio.h>

unsigned char i = 0x00; // 1 byte
unsigned char j = 0x00; // 1 byte
unsigned short int tot = 0x0000; // 2 bytes
unsigned int checksum = 0x00000000; // 4 bytes
unsigned char IHL = 0x00; // 1 byte

void analizaLLC(unsigned char T[]){
    unsigned char SS[][5] = {"RR", "RNR", "REJ", "SREJ"};
    unsigned char UC[][6] = {"UI", "SIM", "-", "SARM", "UP", "-", "-", "SABM", "DISC", "-", "-", "SARME", "-", "-", "-", "SABME", "SNRM", "-", "-", "RSET", "-", "-", "-", "XID", "-", "-", "-", "SNRME"}; // comandos - p
    unsigned char UR[][6] = {"UI", "RIM", "-", "DM", "-", "-", "-", "-", "RD", "-", "-", "-", "UA", "-", "-", "-", "-", "FRMR", "-", "-", "-", "-", "-", "XID"}; // respuestas - f
    printf("\n\n..:::Cabecera LLC:::.."); //En LLC solo hay SABME T-U 1byte , T-S y T-I 2bytes
    switch(T[16] & 3){ //0000 0011 -> 3  xxxx xxxx & 0000 0011 -> {0, 1, 2, 3}={00-T-I, 01-T-S, 10 -T-I, 11-TU}
        case 0:
        case 2: //T-I
         printf("\nT-I, N(s)=%d, N(r)=%d",T[16]>>1, T[17]>>1);
        if(T[17]&1){
            if(T[15]&1){ printf(", -f\n"); }//LSB SAPo
            else{ printf(", -p\n"); }
        }
        break;

        case 1: //T-S
        printf("\nT-S: %s, N(r)=%d", SS[(T[16]>>2) & 3], T[17]>>1);
        if(T[17]&1){ // p/f encendido?
            if(T[15]&1){ printf(", -f\n"); } //LSB SAPo
            else{ printf(", -p\n"); }
        }
        break;

        case 3: //T-U   MMMx MM11 T[16]
        if(T[16]&16){// p-f = 1?
            if(T[15]&1){ printf("\nT-U: %s -f\n", UR[((T[16] >> 2) & 3) | ((T[16] >> 3) & 28)] ); }//LSB SAPo
            else{ printf("\nT-U: %s -p\n", UC[((T[16] >> 2) & 3) | ((T[16] >> 3) & 28)] ); }
        }
        break;

    }
}

void analizaARP(unsigned char t[]){
	printf("\n\n..:::Cabecera ARP:::..");

	printf("\n\nTipo de direccion de HW: ");
	if(t[15] == 1){printf("Ethernet");}
	else if(t[15] == 6){printf("IEEE 802");}
	else if(t[15] == 15){printf("Frame Relay");}
	else if(t[15] == 16){printf("ATM");}
	else{printf("0x%.2x 0x%.2x", t[14], t[15]);}
	
	//Protocol Address Type
	printf("\nTipo de direccion de Protocolo:");
	if(t[16] == 8){
		printf(" IP");
	}else{
	printf(" 0x%.2x 0x%.2x", t[16], t[17]);
	}
	//HW Add Len
	printf("\nLongitud direccion HW: %d bytes", t[18]);

	//Protocol Add Len
	printf("\nLongitud direccion de Protocolo: %d bytes", t[19]);

	//OPCODE
	printf("\nCodigo de operacion: ");
	if(t[21] == 1){printf("ARP Request");}
	else if(t[21] == 2){printf("ARP Reply");}
	else if(t[21] == 3 || t[21] == 8){printf("Inverse ARP Request");}
	else if(t[21] == 9 || t[21] == 4){printf("Inverse ARP Reply");}
	else{printf("%d", (t[20] | t[21]));}
	
	//MAC Origen
	printf("\nMAC Origen: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x", t[22], t[23], t[24], t[25], t[26], t[27]);

	//IP Origen
	printf("\nIP Origen: %d.%d.%d.%d", t[28], t[29], t[30], t[31]);

	//MAC Destino
	printf("\nMAC Destino: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x", t[32], t[33], t[34], t[35], t[36], t[37]);

	//IP Destino
	printf("\nIP Destino: %d.%d.%d.%d\n", t[38], t[39], t[40], t[41]);
}

unsigned short int getChecksum(unsigned char frame[], unsigned char IHL){
    j = 0x00; 
    checksum = 0x00000000; 
    
    // Suma hexadecimal con bloques de 2bytes
    for(j=0; j<IHL; j++){
        // Suma la concatenacion de 1 byte y el byte siguiente de una trama
        checksum += (frame[j]<<8) | frame[++j];
    } 
    // Suma hexadecimal con el acarreo
    checksum += checksum>>16;
    // Complementar a uno el resultado
    checksum = ~(checksum);
  
    return checksum;
}

void correctChecksum(unsigned char frame[], unsigned char IHL){
    checksum = 0x00000000;
 	checksum = getChecksum(frame, IHL);

    /* Si el checksum dado es igual a 0  entonces el checksum es correcto
        sino el checksum es incorrecto y se procede a calcular el valor del checksum correcto,
		poniendo en 0 el apartado de chechksum (t[10], t[11])
    */
     if((checksum == 0)){
        printf("\nChecksum correcto :)\n");
    }else{
		// Obtener el checksum correspondiente a una trama para ello se asigna un valor de 0 al checksum 
		frame[10]= 0x00;
		frame[11] = 0x00;
		checksum = getChecksum(frame, IHL);
        printf("\nChecksum incorrecto :(");
        printf("\nEl checksum correcto es: %.4x\n", checksum);
    }
}

void analizaIP(unsigned char t[]){
	IHL = (t[14]&15)*4; // Tamaño cabecera min ->20 bytes (5 palabras), max ->60 bytes(15 palabras)
	unsigned char frame[IHL-1];
	printf("\n\n..:::Cabecera IP:::..\n");
	//printf("\nIHL: %d bytes", IHL);
	//1.
	// Cabecera IP tiene opciones
	if(IHL>20){
		// Protocolo sig ICMP
		if(t[23] == 1){
			printf("\nOpciones: ");
			for(j=34; j<14+IHL; j++){
				printf("%.2x ", t[j]);
			}
		}
	}
	// 2. Trama de costo mínimo 'c'
	if(t[15]&2){printf("\nTTL: %d saltos", t[22]);} //tiempo de vida -> TTL máximo número de enrutadores que un paquete puede atravesar.
    
	//4.
	// Cabecera IP no tiene opciones
	if(IHL == 20){
		// Protocolo sig UDP
		if(t[23] == 17){
			// Se puede tener hasta (2^13)*8 = 65536 bytes de desplazamiento
			printf("\nOffset(desplazamiento): %d bytes", (((t[20]&31)<<8)|t[21])*8);
		}
	}

	//3. Verificacion del checksum
	for(j=0; j<IHL; j++){
		// t[14] a t[14+IHL-1] considerando las opciones
		frame[j] = t[14+j];
		//printf("\nframe[%d]=%.2x\n", j, t[14+j]);
	}
	correctChecksum(frame, IHL);
}

void analizaTrama(unsigned char t[]){
	    printf("\n-------------------------------------------------------------------------------------");
        printf("\n\n\tTrama: %d\n", i+1);
        printf("\n..:::Cabecera Ethernet:::..\n");
        printf("\nMAC Destino: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x", t[0], t[1], t[2], t[3], t[4], t[5]);
        printf("\nMAC Origen: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x", t[6], t[7], t[8], t[9], t[10], t[11]);

		// tot -> Tamaño o tipo
        tot = (t[12]<<8) | t[13]; // 2bytes

		// tot < 05dc (tamaño de cabecer LLC)
        if(tot<1500){
            printf("\nTama%co de cabecera LLC: %d bytes\n", 164, tot);
            analizaLLC(t);
        }
        else{// tot = 0x0800 (tipo IP)
            if(tot == 2048){
                printf("\nTipo IP\n");
				analizaIP(t);
            }//tot = 0x0806 (tipo ARP)
            else if(tot == 2054){
                printf("\nTipo ARP\n");
				analizaARP(t);
            }
            else{
                printf("\nTipo: %.2x.%.2x\n", t[12], t[13]);
            }
        }

}

int main(){

    printf("\n\t<<<Escuela Superior de C%cmputo>>>\nElaborado por: Hern%cndez Rodr%cguez Armando Giovanni\n", 162, 160, 161);
    
    unsigned char t[][256]=
	{   // 16 columnas x fila
		{0x00,0x1f,0x45,0x9d,0x1e,0xa2,0x00,0x23,0x8b,0x46,0xe9,0xad,0x08,0x00,0x46,0x04/**/, // TRAMA IPV4, IHL = 6(24 bytes, 4 bytes opc) fiabilidad máxima, TTL = 0x80(128 bytes), ICMP protocol, checksum = 6bf0
		 0x80,0x42,0x04,0x55,0x34,0x11,0x80/**/,0x01/**/,0x6b,0xf0/**/,0x94,0xcc,0x39,0xcb,0x94,0xcc, 
		 0x67,0x02,/**/0xee,0xde,0xca,0xdb/**/,0x04,0x0c,0x00,0x35,0x00,0x2e,0x85,0x7c,0xe2,0x1a,
		 0x01,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x77,0x77,0x77,0x03,0x69,
		 0x73,0x63,0x05,0x65,0x73,0x63,0x6f,0x6d,0x03,0x69,0x70,0x6e,0x02,0x6d,0x78,0x00,
		 0x00,0x1c,0x00,0x01},	
		
		 {0x00,0x1f,0x45,0x9d,0x1e,0xa2,0x00,0x23,0x8b,0x46,0xe9,0xad,0x08,0x00,0x47/**/,0x06/**/, // TRAMA IPV4, IHL = 7(28 bytes, 8 bytes opc), costo  minimo y fiabilidad maxima,flag= M, offset = 0x1411 (5137*8 = 41096 bytes), TTL = 0x80(128bytes),UDP protocol, checksum = a392
		 0x80,0x42,0x04,0x55,0x34,0x11,0x80/**/,0x11/**/,0xa3,0x92,0x94,0xcc,0x39,0xcb,0x94,0xcc, 
		 0x67,0x02,0xaa,0xbb,0xcc,0xdd,0xee,0xff,0xab,0xac,0x04,0x0c,0x00,0x35,0x00,0x2e,
		 0x85,0x7c,0xe2,0x1a,0x01,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x77,
		 0x77,0x77,0x03,0x69,0x73,0x63,0x05,0x65,0x73,0x63,0x6f,0x6d,0x03,0x69,0x70,0x6e,
		 0x02,0x6d,0x78,0x00,0x00,0x1c,0x00,0x01},

		{0x00,0x1f,0x45,0x9d,0x1e,0xa2,0x00,0x23,0x8b,0x46,0xe9,0xad,0x08,0x00,0x45/**/,0x08/**/, // TRAMA IPV4, IHL = 5(20 bytes, 0 bytes opc), máximo rendimiento, flag= M, offset = 0x1411 (5137*8 = 41096 bytes), TTL = 0x80(128bytes), UDP protocl, checksum = 95a4 
		 0x80,0x42,0x04,0x55,0x34,0x11,0x80/**/,0x11/**/,0x95,0xa4,0x94,0xcc,0x39,0xcb,0x94,0xcc, 
		 0x67,0x02,0x04,0x0c,0x00,0x35,0x00,0x2e,0x85,0x7c,0xe2,0x1a,0x01,0x00,0x00,0x01,
		 0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x77,0x77,0x77,0x03,0x69,0x73,0x63,0x05,0x65,
		 0x73,0x63,0x6f,0x6d,0x03,0x69,0x70,0x6e,0x02,0x6d,0x78,0x00,0x00,0x1c,0x00,0x01},

		 {0x00,0x1f,0x45,0x9d,0x1e,0xa2,0x00,0x23,0x8b,0x46,0xe9,0xad,0x08,0x00,0x48,0x02/**/, // TRAMA IPV4, IHL = 8(32 bytes, 12 bytes opc) costo mínimo, TTL = 0x88(136 bytes), ICMP protocol, checksum = 3f53
		 0x80,0x42,0x04,0x55,0x34,0x11,0x88/**/,0x01/**/,0x3f,0x53/**/,0x94,0xcc,0x39,0xcb,0x94,0xcc, 
		 0x67,0x02,/**/0xaa,0xbb,0xcc,0xdd,0xae,0xbe,0xcc,0xae,0xeb,0xec,0xde,0xcd/**/,0x04,0x0c,
		 0x00,0x35,0x00,0x2e,0x85,0x7c,0xe2,0x1a,0x01,0x00,0x00,0x01,0x00,0x00,0x00,0x00,
		 0x00,0x00,0x03,0x77,0x77,0x77,0x03,0x69,0x73,0x63,0x05,0x65,0x73,0x63,0x6f,0x6d,
		 0x03,0x69,0x70,0x6e,0x02,0x6d,0x78,0x00,0x00,0x1c,0x00,0x01},

		 {0x00,0x1f,0x45,0x9d,0x1e,0xa2,0x00,0x23,0x8b,0x46,0xe9,0xad,0x08,0x00,0x45/**/,0x02/**/, // TRAMA IPV4, IHL = 5(20 bytes, 0 bytes opc), costo mínimo, flag= M, offset = 0x1413 (5139*8 = 41112 bytes), TTL = 0xa0(160bytes), UDP protocl, checksum = 97bc
		 0x80,0x42,0x04,0x55,0x34,0x13,0xa0/**/,0x11/**/,0x97,0xbc,0x94,0xcc,0x39,0xcb,0x94,0xcc, 
		 0x67,0x02,0x04,0x0c,0x00,0x35,0x00,0x2e,0x85,0x7c,0xe2,0x1a,0x01,0x00,0x00,0x01,
		 0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x77,0x77,0x77,0x03,0x69,0x73,0x63,0x05,0x65,
		 0x73,0x63,0x6f,0x6d,0x03,0x69,0x70,0x6e,0x02,0x6d,0x78,0x00,0x00,0x1c,0x00,0x01}

	};
    
    for(i=0; i<5; i++){
		analizaTrama(t[i]);
    }
    return 0;
}
