#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>


#define ABORT(...)                              \
    fprintf(stderr, __VA_ARGS__);               \
    abort()


void _print_debug_char(char c) {
    if (c == '"') {
        printf("\\\"");
    } else if (c == '\\') {
        printf("\\\\");
    } else if (c == '\0') {
        printf("\\0");
    } else if (c == '\n') {
        printf("\\n");
    } else if (c == '\r') {
        printf("\\r");
    } else if (c == '\t') {
        printf("\\t");
    } else if (c == '\v') {
        printf("\\v");
    } else if (c == '\f') {
        printf("\\f");
    } else if (c == '\b') {
        printf("\\b");
    } else if (c == '\a') {
        printf("\\a");
    } else if (iscntrl(c)) {
        printf("\\%03o", c);
    } else {
        fputc(c, stdout);
    }
}

void print_debug_char(char c) {
    fputc('\'', stdout);
    _print_debug_char(c);
    fputc('\'', stdout);
}


typedef char* string;

void print_string(const char* str) {
    printf("%s", str);
}

void print_debug_string(const char* str) {
    print_string("\"");
    while (*str) {
        _print_debug_char(*str);
        str++;
    }
    print_string("\"");
}

string get_string() {
    while (true) {
#define SIZ 100
        char *line = malloc(SIZ);
        size_t len = SIZ;
#undef SIZ
        errno = 0;
        ssize_t n = getline(&line, &len, stdin);
        if (n < 0) {
            ABORT("Could not get a line from stdin: %s.\n",
                  errno == 0 ? "EOF" : strerror(errno));
        }
        size_t l = strlen(line);
        // Always must have either a '\n' at the end or some other
        // text, since the EOF case is handled above.
        assert(l >= 1);
        if (line[l - 1] == '\n') {
            line[l - 1] = '\0';
            l--;
        }
        if (l > 0) {
            return line;
        }
        free(line);
        print_string("Your answer is empty. Please enter a string: ");
    }
}

int get_int() {
    while (true) {
        string s = get_string();
        char *tail;
        errno = 0;
        long n = strtol(s, &tail, 10);
        if (errno == 0) {
            if (*tail == '\0') {
                if (n >= INT_MIN && n <= INT_MAX) {
                    free(s);
                    return n;
                } else {
                    print_string("Your answer is not within the range of "
                                 "numbers of the `int` type.");
                }
            } else {
                print_string("Please enter only a number with nothing "
                             "after it.");
            }
        } else {
            printf("Your answer is not an integer number in range possible "
                   "for the `long` type: %s.",
                   strerror(errno));
        }
        free(s);
        print_string(" Please enter an integer number: ");
    }
}

void print_int(int n) {
    printf("%i", n);
}


typedef int nat;

int get_nat() {
    while (true) {
        int i = get_int();
        if (i > 0) {
            return i;
        }
        print_string("Your answer is not above zero.");
        print_string(" Please enter a natural number: ");
    }
}

void print_nat(int n) {
    if (n > 0) {
        printf("%i", n);
    } else {
        ABORT("error: print_nat(%i): argument out of range\n", n);
    }
}


typedef int nat0;

int get_nat0() {
    while (true) {
        int i = get_int();
        if (i >= 0) {
            return i;
        }
        print_string("Your answer is negative.");
        print_string(" Please enter a natural number or zero: ");
    }
}

void print_nat0(int n) {
    if (n >= 0) {
        printf("%i", n);
    } else {
        ABORT("error: print_nat0(%i): argument out of range\n", n);
    }
}


void print_float(float x) {
    printf("%g", x);
}

// largely copy-paste of get_int
float get_float() {
    while (true) {
        string s = get_string();
        char *tail;
        errno = 0;
        float x = strtof(s, &tail);
        if (errno == 0) {
            if (*tail == '\0') {
                free(s);
                return x;
            } else {
                print_string("Please enter only a number with nothing "
                             "after it.");
            }
        } else {
            printf("Your answer is not a floating point number in the "
                   "possible range for the `float` type: %s.",
                   strerror(errno));
        }
        free(s);
        print_string(" Please enter a floating point number: ");
    }
}


void* new_array_of_type_and_len(const char* typename,
                                size_t typesize,
                                size_t len) {
    void* p = calloc(len, typesize);
    if (p) {
        return p;
    } else {
        ABORT("Could not allocate enough memory for an array "
              "of %ld %s elements",
              len, typename);
    }
}

char* new_chars(size_t len) {
    return new_array_of_type_and_len("char", sizeof(char), len);
}
string new_string(size_t len) {
    return new_array_of_type_and_len("char", sizeof(char), len);
}

string* new_strings(size_t len) {
    return new_array_of_type_and_len("string", sizeof(string), len);
}
void free_strings(string* ary, size_t len) {
    for (size_t i = 0; i < len; i++) {
        string s = ary[i];
        if (s) {
            free(s);
        }
    }
    free(ary);
}

int* new_ints(size_t len) {
    return new_array_of_type_and_len("int", sizeof(int), len);
}
nat* new_nats(size_t len) {
    return new_array_of_type_and_len("nat", sizeof(nat), len);
}
nat0* new_nat0s(size_t len) {
    return new_array_of_type_and_len("nat0", sizeof(nat0), len);
}

float* new_floats(size_t len) {
    return new_array_of_type_and_len("float", sizeof(float), len);
}

/*
  does this work?
  #define INITIALIZE_ARRAY(ary, members) *ary = members
  */

#define PRINT_ARRAY(print_typ, ary, len)        \
    print_string("[");                          \
    for (size_t i = 0; i < len; i++) {          \
        if (i > 0) {                            \
            print_string(", ");                 \
        }                                       \
        print_typ(ary[i]);                      \
    }                                           \
    print_string("]");

void print_debug_chars(const char* ary, size_t len) {
    PRINT_ARRAY(print_debug_char, ary, len);
}

void print_debug_strings(const string* ary, size_t len) {
    PRINT_ARRAY(print_debug_string, ary, len);
}

void print_debug_ints(const int* ary, size_t len) {
    PRINT_ARRAY(print_int, ary, len);
}
void print_debug_nats(const nat* ary, size_t len) {
    PRINT_ARRAY(print_nat, ary, len);
}
void print_debug_nat0s(const nat0* ary, size_t len) {
    PRINT_ARRAY(print_nat0, ary, len);
}

void print_debug_floats(const float* ary, size_t len) {
    PRINT_ARRAY(print_float, ary, len);
}

#undef PRINT_ARRAY
