#include <ctype.h>
#ifndef inhandler_h
char dump;

void buffer_clearer(void) {
    while(dump = getchar() != '\n');
}

void err_message(const char* msg) {
    printf("Dato no valido, ingrese un %s: ", msg);
}

void read_integer(int* int_number) {
    while(scanf(" %d", int_number) != 1) {
        err_message("NUMERO ENTERO");
        buffer_clearer();
    }
    buffer_clearer();
}

void read_float(float* float_number) {
    while(scanf(" %f", float_number) != 1) {
        err_message("NUMERO REAL");
        buffer_clearer();
    }
    buffer_clearer();
}

// (char* to read, tolower 1 || toupper 2 || 0 as is)
void read_char(char* character, int sel) {
    char toRead;

    while(1) {
        *character = getchar();
        if(*character == '\n' || *character == '\0') {
            err_message("CARACTER VALIDO: ");
        } else {
            buffer_clearer();
            break;
        }
    }
    if(sel == 1) {
        *character = tolower(*character);
    } else if(sel == 2) {
        *character = toupper(*character);
    }
}

#endif