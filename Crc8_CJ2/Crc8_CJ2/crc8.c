#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

//prototypes
void decToBina(uint8_t crcDec);
char* makeCRC(char* BitString);
uint8_t crc8(uint32_t input);
void finalCrc(int length);
void appendFinalCrc(char data[]);

//global variable
uint8_t byteToBitArr[8];
uint8_t crcFinal[8];

int main()
{
    uint8_t crc8bit;
    uint32_t temp2 = 0x36BD6EF00;                           //34 bit length     36BD6EF00
    char bs[] = "1101101011110101101110111100000000";     //34 bit string index 0 --> 1101101011110101101110111100000000 <-- index 33
    char sb[] = "0000000011110111011010111101011011";     //4 bit string inverted 
    int lent = strlen(bs);
    int k = 0;

    //init
    for (k; k < 8; k++) {
        byteToBitArr[k] = 0;
        crcFinal[k] = 0;
    }
    k = 0;

    //print data content
    printf("data with appended 0's: \n");
    while (k < lent) {
        printf("%c, ", bs[k] & '1');
        k++;
    }
    printf("\n");
    k = 0;

    //perform crc8
    crc8bit = crc8(temp2); 

    //convert crc8 result to binary //converts and writes the value to byteToBitArr[]
    decToBina(crc8bit);  

    //copy only the 8 required bits from the decToBina to crcFinal
    finalCrc(sizeof(byteToBitArr) / sizeof(byteToBitArr[0]));

    //printf("\ncrc8bit: %d\n", crc8bit);
    
    printf("\nFinal crc: ");  
    while (k < sizeof(crcFinal) / sizeof(crcFinal[0])) {
        printf("%d", crcFinal[k]);
        k++;
    } 
    k = 0;  
    printf("\n");

    //updated data
    appendFinalCrc(bs);
         
    printf("\ncrc appended: \n");
    while (k < lent) {
        printf("%c, ", bs[k] & '1');
        k++;
    }
    
    printf("\n");
    return 0;
}

void finalCrc(int length) {
    int k = 0, n = 0;

    while (length > 0) {
        if (k > 0 && k < sizeof(byteToBitArr) / sizeof(byteToBitArr[0])) {
            crcFinal[n++] = byteToBitArr[k++];
        }
        else k++;
        length--;
    }
}

void decToBina(uint8_t crcDec) {
    int i = 0;
    for (i = 7; i >= 0; i--) {
        if ((crcDec & (1 << i)) != 0) {
            //printf("1");
            byteToBitArr[i] = 1;
        }
        else {
           // printf("0");
            byteToBitArr[i] = 0;
        }
    }
}

uint8_t crc8(uint32_t input) //uint16_t to uint32_t
{
    uint8_t crc[9] = { 1,1,1,1,1,1,1,1,1};
    uint8_t i;
    uint8_t inv;
    uint8_t output = 0;

    for (i = 0; i < 32; i++)    //16 to 32
    {
        inv = ((((input >> i) & 1) ^ crc[7]) & 1);

        crc[7] = (crc[6] & 1);
        crc[6] = (crc[5] & 1);
        crc[5] = (crc[4] ^ inv & 1);
        crc[4] = (crc[3] ^ inv & 1);
        crc[3] = (crc[2] & 1);
        crc[2] = (crc[1] & 1);
        crc[1] = (crc[0] & 1);
        crc[0] = (inv & 1);
    }

    for (i = 0; i < 8; i++)
    {
        output |= ((crc[i] << i) & (1 << i));
    }

    return output;
}

/*Take the crc from finalCrc variable and replace the crc part of the packet*/
void appendFinalCrc(char data[]) {
    int length = strlen(data);      //length should be 34, 43 - 26 = 8 i.e LSB 0 -7
    int index = 26;
    int crcFinalIndex = 0;

    while (crcFinalIndex < 8) {
        data[index++] |= (char)(*&crcFinal[crcFinalIndex]);    //replace the appended 0's with the crc
        crcFinalIndex++;
    }

}

char* makeCRC(char* BitString)
{
    static char Res[9];                                 // CRC Result
    char CRC[8];
    int  i;
    char DoInvert;

    for (i = 0; i < 8; ++i)
        CRC[i] = 0;                    // Init before calculation

    int length = strlen(BitString);

    for (i = 0; i < length; ++i)
    {
        DoInvert = ('1' == BitString[i]) ^ CRC[7];         // XOR required?

        CRC[7] = CRC[6];
        CRC[6] = CRC[5];
        CRC[5] = CRC[4] ^ DoInvert;
        CRC[4] = CRC[3] ^ DoInvert;
        CRC[3] = CRC[2];
        CRC[2] = CRC[1];
        CRC[1] = CRC[0];
        CRC[0] = DoInvert;
    }

    for (i = 0; i < 8; ++i) {
        Res[7 - i] = CRC[i] ? '1' : '0'; // Convert binary to ASCII
    }
    Res[8] = 0;                          // Set string terminator

    return(Res);
}

