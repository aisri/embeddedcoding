// Write a function that takes in a str as an argument and returns true if that string is a real number

// Good Numebrs:  1, -2, 1.5, 90.3, 0.1, .7, 1.3e-10
// Bad numebrs: hkhd, ..9, 7 8, 9.,
// esp32 chip

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    INIT,
    INTEGER,
    FLOAT,
    FLOAT_EXP,
    INVALID
};

bool is_valid_number(char *str, int len) {
    enum state st = INIT;
    if (!str || !len) {
        return false;
    }

    int i = 0;
    bool check_integer_only = false;
    while (i < len) {
        if (str[i] == 0)
            return false;
        // printf("str[i]=%c b-st=%u", str[i], st);
        switch (st) {
        case INIT:
            if(str[i] == ' ') {
            }
            if (str[i] == '-' || str[i] == '+') {
                st = INTEGER;
            } else if (str[i] == '.') {
                if (check_integer_only)
                    return false;
                st = FLOAT;
            } else if (isdigit(str[i])) {
                st = INTEGER;
            }
            i++;
            break;
        case INTEGER:
            if (isdigit(str[i])) {
                st = INTEGER;
            } else if (str[i] == '.') {
                if (check_integer_only)
                    return false;
                st = FLOAT;
            } else {
                st = INVALID;
            }
            i++;
            break;
        case FLOAT:
            if (isdigit(str[i])) {
                st = FLOAT;
            } else if (str[i] == 'e' || str[i] == 'E') {
                st = FLOAT_EXP;
            } else {
                st = INVALID;
            }
            i++;
            break;
        case FLOAT_EXP:
            if (str[i] == '-' || str[i] == '+' || isdigit(str[i])) {
                st = INTEGER;
                check_integer_only = true;
            }
            i++;
            break;
        case INVALID:
            return false;
        }
        // printf(" a-st=%u\n", st);
    }
    return (st == FLOAT && str[len-1] != '.') || st == INTEGER;
}

int main() {
    printf("1 is valid =%u\n", is_valid_number(" 1", 2));
    printf("-2 is valid =%u\n", is_valid_number("-2", 2));
    printf("1.5 is valid =%u\n", is_valid_number(" 1.5", 4));
    printf("90.3 is valid =%u\n", is_valid_number("90.3", 4));
    printf("0.1 is valid =%u\n", is_valid_number("0.1", 3));
    printf(".7 is valid =%u\n", is_valid_number(".7", 2));
    printf("1.3e-10 is valid =%u\n", is_valid_number("1.3e-10", 7));

    printf("hkhd is valid =%u\n", is_valid_number("hkhd", 4));
    printf("..9 is valid =%u\n", is_valid_number("..9", 3));
    printf("7 8 is valid =%u\n", is_valid_number("7 8", 3));
    printf("9. is valid =%u\n", is_valid_number("9.", 2));
    return 0;
}
