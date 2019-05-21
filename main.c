#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "inhandler.h"

/*
La empresa SUNOMBRE ha sido contratada para generar listados de
codificacion secreta enumeradas que activan la recarga de 
dispositivos de telecomunicacion portatiles.
    - Longitud de codificacion entre 4 y 6 digitos con la siguiente distribucion:
        .numeros desde 1 a 4 elementos A = [0 a 9]
        .consonantes desde 0 a 3 elementos  C = { b, k, m, p, s, t }
        .vocales 1 a 3 elementos {a, e, i, o, u}
    - Realizar 1 o 2 de las siguientes operaciones
    (intercepcion^, diferencia simetrica a$b, a\b)
    - Unir y mezclar los elementos definidos por el usuario en un arreglo S
    - Generar subconjuntos a partir del conjunto S y realizar la/s operaciones solicitadas
*/

struct operate
{
    int codifLen, vowlen, consonantslen, numlen, blocks, perBlock, blockSize;
} op;

// -------------------------------functions--------------------------------

void clearScreen(void)
{
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}

void range_read(int *toread, int minvalue, int maxvalue)
{
    while(1) {
        read_integer(toread);
        if (*toread < minvalue || *toread > maxvalue)
        {
            printf("---> ERROR!!! Use valores entre MIN(%d) y MAX(%d): ", minvalue, maxvalue);
        } else {
            break;
        }
    }
}

// (const char* to print || NULL, char* toRead)
int choiceReader(const char *inMsg, const char* choices, char* toRead)
{
    int choicesLen, loop = 1, x;
    if(inMsg != NULL) {
        printf("%s", inMsg);
    }

    choicesLen = strlen(choices);
    do {
        read_char(toRead, 2);
        for(x = 0; x < choicesLen; x++) {
            if(*toRead == choices[x]) {
                loop = 0;
                x = choicesLen;
                if(*toRead == 'N') {
                    return 0;
                } else {
                    return 1;
                }
            } else if((x + 1) == choicesLen) {
                printf("ERROR... ingrese [%s] solamente: ", choices);
            }
        }
    } while(loop);
}

int number_generator(int gen_range)
{
    return (rand() % gen_range); // may require repeated numbers handling
}

// message to the user when selecting ranges
void sel_message(const char *argmsg, int vmin, int vmax, int strchoice)
{
    if(strchoice == 1) {
        printf("\nCantidad de %s a generar?\n[MIN %d y MAX %d]: ", argmsg, vmin, vmax);
    } else if(strchoice == 2) {
        printf("\nCantidad de %s a leer? [MIN %d y MAX %d]: ", argmsg, vmin, vmax);
    }
}

int compLetters(char A, char* B, int lim){
    int x;
    for(x = 0; x < lim; x++) {
        if(A != B[x]) {
            continue;
        } else {
            return 0;
        }
    }
    return 1;
}

// (char ** Arrays, arrayslen)
void operationsMaker(char** Arrays,
int Arraylen,
int subSize,
char* results,
char* Operator,
int* lim) {
    int x, y, z, alpha;
    printf("\nOperator[0] = (%c)\nsubArray(RES)[A] = ", Operator[0]);
    z = 0;
    for(x = 0; x < subSize; x++) {
        alpha = 0;
        for(y = 0; y < subSize; y++) {
            printf("\nArrayA[%d] = (%c) | ArrayB[%d] = (%c)", x, Arrays[0][x], y, Arrays[1][y]);
            if(Operator[0] == '^') {
                if(Arrays[0][x] == Arrays[1][y]) {
                    if(compLetters(Arrays[0][x], results, (z + 1))) {
                        printf(" Coincide (%c) ->", Arrays[0][x]);
                        results[z] = Arrays[0][x];
                        *lim++;
                        z++;
                        printf(" Guardado");
                        break;
                    } else {
                        printf(" Repetido <- %c", Arrays[0][x]);
                        break;
                    }
                } else {
                    printf(" No Coincide");
                }
            } else if(Operator[0] == '\\') {
                if(compLetters(Arrays[0][x], Arrays[1], subSize)) {
                    printf(" Coincide (%c) ->", Arrays[0][x]);
                    if(compLetters(Arrays[0][x], results, (z + 1))) {
                        results[z] = Arrays[0][x];
                        *lim++;
                        z++;
                        printf(" Guardado");
                        break;
                    } else {
                        printf(" Repetido <- %c", Arrays[0][x]);
                        break;
                    }
                } else {
                    printf(" No Coincide");
                }
            } else if(Operator[0] == '$') {
                if(compLetters(Arrays[0][x], Arrays[1], subSize)) {
                    printf("\nArray A Saved");
                    if(compLetters(Arrays[0][x], results, (z + 1))) {
                        results[z] = Arrays[0][x];
                        *lim++;
                        z++;
                        break;
                    }
                } else if(compLetters(Arrays[1][x], Arrays[0], subSize)) {
                    printf("\nArray B Saved");
                    if(compLetters(Arrays[1][x], results, (z + 1))) {
                        results[z] = Arrays[1][x];
                        *lim++;
                        z++;
                        break;
                    }
                }
            }
        }
    }
    if(Arraylen > 2) {
        for(x = 0; x < subSize; x++) {
            alpha = 0;
            for(y = 0; y < subSize; y++) {
                printf("\nArrayA[%d] = (%c) | ArrayB[%d] = (%c)", x, results[x], y, Arrays[1][y]);
                if(Operator[1] == '^') {
                    if(results[x] == Arrays[2][y]) {
                        if(compLetters(Arrays[2][x], results, z+1)) {
                            printf(" Coincide (%c) ->", results[x]);
                            results[z] = Arrays[2][x];
                            //z++;
                            *lim++;
                            printf(" Guardado");
                            break;
                        } else {
                            printf(" Repetido <- %c", Arrays[2][x]);
                            break;
                        }
                    } else {
                        printf(" No Coincide");
                    }
                } else if(Operator[1] == '\\') {
                    if(compLetters(results[x], Arrays[2], subSize)) {
                        printf(" Coincide (%c) ->", Arrays[2][x]);
                        if(compLetters(Arrays[2][x], results, z+1)) {
                            results[z] = Arrays[2][x];
                            //z++;
                            *lim++;
                            printf(" Guardado");
                            break;
                        } else {
                            printf(" Repetido <- %c", Arrays[2][x]);
                            break;
                        }
                    } else {
                        printf(" No Coincide");
                    }
                } else if(Operator[1] == '$') {
                    if(compLetters(results[x], Arrays[2], subSize)) {
                        printf("\nArray C Saved");
                        if(compLetters(Arrays[2][x], results, (z + 1))) {
                            results[z] = Arrays[2][x];
                            //z++;
                            *lim++;
                            break;
                        }
                    } else if(compLetters(results[x], Arrays[2], subSize)) {
                        // printf("\nArray B Saved");
                        // if(compLetters(results[x], results, (z + 1))) {
                        //     results[z] = Arrays[1][x];
                        //     //z++;
                        //     break;
                        // }
                    }
                }
            }
        }
    }
}

// (char* array, int limit)
void printArray(char *ArrTopr, int argLimit)
{
    int x;
    for (x = 0; x < argLimit; x++)
    {
        printf("%c", ArrTopr[x]);
    }
}

void generateLine(char chac, int doTimes)
{
    int x;
    for (x = 0; x < doTimes; x++)
    {
        printf("%c", chac);
    }
}

/*main program*/
int main(void) {
    int doagain = 1, makeNewSettings = 1, x, y, z, LimitCounter;
    char vowels[] = {'a', 'e', 'i', 'o', 'u'},
     consonants[] = {'b', 'k', 'm', 'p', 's', 't'},
     checker;
    
    generateLine('*', 64);
    printf("\n|  BIENVENIDO A LA EMPRESA DE TELEFONIA MOVIL **MAS-RAPIDIN**  |\n");
    generateLine('*', 64);

    do {
        srand(time(NULL)); // rand seed initializer
        if (makeNewSettings) 
        {
            // quantity of digits to generate
            sel_message("DIGITOS", 4, 6, 1);
            range_read(&op.codifLen, 4, 6);
            LimitCounter = op.codifLen;
            
            // quantity of numbers to generate
            sel_message("NUMEROS", 1, 4, 1);
            range_read(&op.numlen, 1, 4);
            LimitCounter -= op.numlen;

            // quantity of vowels to generate
            if(LimitCounter > 0)
            {
                sel_message("VOCALES", 1, LimitCounter, 1);
                range_read(&op.vowlen, 1, LimitCounter);
                LimitCounter -= op.vowlen;
            }

            // quantity of consonants to generate
            if(LimitCounter > 0)
            {
                sel_message("CONSONANTES", 1, LimitCounter, 1);
                range_read(&op.consonantslen, 1, LimitCounter);
                LimitCounter -= op.vowlen;
            }
        }
        char subArrayUnchanged[3][op.codifLen];
        char S[op.codifLen];
        int sortRandom[op.codifLen];

        // load array with random numbers within op.codifLen lenght
        y = 0;
        z = 0;
        while (y < op.codifLen) {
            z = number_generator(op.codifLen);
            for (x = 0; x < op.codifLen; x++) {
                if (sortRandom[x] != z) {
                    //printf("\nLOL");
                    if (y == x) {
                        sortRandom[y] = z;
                        //printf("\n sorter[%d] = %d", y, z);
                        y++;
                        break;
                    }
                } else {
                    break;
                }
            }
        }
        // load of S[]
        for (x = 0; x < op.codifLen; x++)
        {
            if(op.numlen - x > 0) {
                S[sortRandom[x]] = '0' + number_generator(10);
            } else if ((op.numlen + op.vowlen) - x > 0) {
                S[sortRandom[x]] = vowels[number_generator(5)];
            } else if ((op.codifLen) - x > 0) {
                S[sortRandom[x]] = consonants[number_generator(6)];
            }
        }

        // subjoints generation
        for (y = 0; y < 3; y++)
        {
            for (x = 0; x < op.codifLen; x++)
            {
                subArrayUnchanged[y][x] = S[number_generator(op.codifLen)];
            }
        }

        printf("\n\n");
        generateLine('-', 24 + op.codifLen);
        printf("\n|  CONJUNTO[S] = { ");
        printArray(S, op.codifLen);
        printf(" }  |\n");
        generateLine('-', 24 + op.codifLen);

        for (x = 0; x < 3; x++)
        {
            printf("\n>> SUBCONJUNTO[%c] = { ", ('A' + x));
            printArray(subArrayUnchanged[x], op.codifLen);
            printf(" }");
        }

        if(makeNewSettings) {
            // operations selection
            printf("\n\nBLOQUES DE OPERACIONES A PROCESAR?\nNINGUNO(0) y MAX(9): ");
            range_read(&op.blocks, 0, 9);
        }

        op.blockSize = (op.perBlock * 2) + 1;
        struct blocks {
            char operations[op.blockSize], results[6];
            int reSize;
        } block[op.blocks];
        int repetitions, alpha, beta, gamma;
        char* blocksToProcess[3];

        if(op.blocks > 0) {
            // operations
            for(x = 0; x < op.blocks; x++) {
                if(makeNewSettings) {
                    printf("\n\n|  BLOQUE DE OPERACIONES #%d  |\n|  CANTIDAD DE OPERACIONES [MIN(1) y MAX(2)]?  |", x + 1);
                    range_read(&op.perBlock, 1, 2);
                    op.blockSize = (op.perBlock * 2) + 1;
                    printf("\n| OPERADORES >> (^ => INTERSECCION), (\\ => RELATIVO), ($ => DIFERENCIA SIMETRICA) |\n");
                    printf("\n| SUBCONJUNTOS DISPONIBLES >> (A), (B), (C) |");
                    generateLine('-', 78);
                    printf("\n");
                    y = 0;
                    while(1) {  // reader
                        if(y < op.blockSize) {
                            printf(">> ELEMENTOS CARGADOS(%d) = ", y);
                            for(z = 0; z < op.blockSize; z++) {
                                if(op.blockSize > 3 && z == 0) {
                                    printf("(");
                                }
                                if(z < y) {
                                    printf("[%c]", block[x].operations[z]);
                                } else {
                                    printf("[-]");
                                }
                                if(op.blockSize > 3 && (z + 1) % 3 == 0) {
                                    printf(")");
                                }
                            }
                            printf(": ");
                        
                            if(y%2 == 0) {
                                choiceReader(NULL, "ABC", &block[x].operations[y]);
                            } else if(y%2 > 0) {
                                choiceReader(NULL, "^\\$", &block[x].operations[y]);
                            }
                            // y adds up one unit
                            y++;
                            // checks operation submited with already saved ones for coincidences
                            if(y == op.blockSize) {
                                for(alpha = 0; alpha < x; alpha++) {
                                    repetitions = 0;
                                    for(beta = 0; beta < op.blockSize; beta++) {
                                        if(block[x].operations[beta] == block[alpha].operations[beta]) {
                                            repetitions++;
                                        }
                                        if(repetitions == op.blockSize) {
                                            printf("\n****BLOQUE DE OPERACIONES REPETIDO****\n");
                                            y = 0;
                                            break;
                                        }
                                    }
                                }
                            }
                        } else {
                            printf("---CARGA DE ELEMENTOS(%d) DEL BLOQUE #%d COMPLETA---", y, x + 1);
                            break;
                        }
                       
                    }
                }
                    beta = 0;
                    gamma = 0;
                    char OpBlock[2];
                    for(alpha = 0; alpha < op.blockSize; alpha++) 
                    {
                        if(block[x].operations[alpha] == 'A') 
                        {
                            blocksToProcess[beta] = subArrayUnchanged[0];
                            beta++;
                        } else if(block[x].operations[alpha] == 'B') 
                        {
                            blocksToProcess[beta] = subArrayUnchanged[1];
                            beta++;
                        } else if(block[x].operations[alpha] == 'C') 
                        {
                            blocksToProcess[beta] = subArrayUnchanged[2];
                            beta++;
                        } else if(block[x].operations[alpha] == '^') {
                            OpBlock[gamma] = '^';
                            gamma++;
                        } else if(block[x].operations[alpha] == '\\') {
                            OpBlock[gamma] = '\\';
                            gamma++;
                        } else if(block[x].operations[alpha] == '$') {
                            OpBlock[gamma] = '$';
                            gamma++;
                        }
                    }

                    // pass subarrays of S to operator function
                    if(op.perBlock == 1)
                    {
                        operationsMaker(blocksToProcess,
                        beta,
                        op.codifLen,
                        block[x].results,
                        OpBlock, block[x].reSize);
                    }
                }
            }
        
        //results printing
        for(x = 0; x < op.blocks; x++) {
            printf("\nRESULTADO BLOQUE %d = {", x + 1);
            printArray(block[x].results, strlen(block[x].results));
            printf("}");
        }

        // end main program loop
        doagain = choiceReader("\n\nREINICIAR PROGRAMA (y/n)? ", "YN", &checker);

        // settings management
        if (doagain) {
           makeNewSettings = choiceReader("CREAR AJUSTES NUEVOS (y/n)? ", "YN", &checker);
        }

    } while (doagain);
    printf("\n\nPROGRAMA FINALIZADO\n");
    return 0;
}