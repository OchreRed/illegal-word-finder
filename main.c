// Goal: create a program that has you type in words and tell you which words
// you couldn't type using the letters available to you on a hypothetical
// "broken keyboard." e.g. Letters g-k are illegal on Broken Keyboard

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//void space_reduce(char s[]);
char* get_input(void);
char** make_word_list(char* buffer);
char* get_word(char* pword);
int char_match(const char* restrict s1, const char* restrict s2);
void check_word_list(char** word_list, const char* restrict excl_chars);
void free_word_list(char** word_list);

int main (void) {
    char* sample_input = "Hello, my name is Theodore.";
    char* letters = "ghijk"; // illegal letters

    printf("\nSample input: \"%s\"\n", sample_input);
    printf("Illegal letters: %s (case sensitive)\n", letters);
    puts("Type your input below: ");

    char* buffer = get_input();
    char** word_list = make_word_list(buffer); // buffer is freed here
    check_word_list(word_list, letters);
    free_word_list(word_list); // frees word_list contents and word_list

    return EXIT_SUCCESS;
}

// Type in your input, and get_input returns a pointer to your input string.
char* get_input(void) {
    int ch;
    int len = 0;
    int size = 32;
    char* buffer = malloc(size * sizeof(char));

    while ( (ch = getchar() ) != '\n' && ch != EOF) {
        buffer[len++] = (char)ch;
        if (len == size) {
            size += 16;
            buffer = realloc(buffer, size * sizeof(char));
        }
    }
    buffer[len] = '\0';
    return buffer;
}

// Input pointer to space before word, or first letter. 
// Returns pointer to copy of that word.
char* get_word(char* pword) {
    
    // Increments pword until pword[0] points to a letter in input buffer.
    for (;;) {
        switch (pword[0]) {
            case 'A' ... 'Z':
            case 'a' ... 'z':
                goto start_read;
        }
        ++pword;
    }
    
    start_read: ; // Linter complained until I put this semicolon here.
    // Increments i from first letter to next space, newline, EOF, period, or comma.
    size_t i = 0;
    while (pword[i] != ' ' && 
           pword[i] != '.' && 
           pword[i] != '\n' && 
           pword[i] != '\0' &&
           pword[i] != ',')
        ++i;

    char* new_word = malloc((i+1) * sizeof(char));
    size_t n = 0;
    while (n < i) {
        new_word[n] = pword[n];
        ++n;
    }
    new_word[n] = '\0';

    return new_word;
}

// First index all the spots in the buffer where a word starts.
// Then make an array of strings from the input buffer.
char** make_word_list(char* buffer) {
    char** word_index = malloc(5 * sizeof(char*)); //array of pointers to buffer
    size_t wi_len = 0;
    size_t wi_size = 8;

    word_index[wi_len] = &buffer[0];

    wi_len = 1;
    size_t i = 1;
    while(buffer[i] != '\0') {
        if (buffer[i] == ' ') {
            word_index[wi_len] = &buffer[i];
            ++wi_len;
        }
        if (wi_len == wi_size) {
            wi_size += 8;
            word_index = realloc(word_index, wi_size * sizeof(char*));
        }
        ++i;
    }

    char** word_list = malloc( (wi_len+1) * sizeof(char*) );
    i = 0;
    while (i < wi_len) {
        char* word = get_word(word_index[i]);
        word_list[i] = word;
        ++i;
    }
    word_list[i] = ""; // Sentinel string to mark ending

    free(buffer); buffer = NULL;
    free(word_index); word_index = NULL;

    return word_list;
}

// Returns first location in string s1 where any character from string s2 occurs.
int char_match(const char* restrict s1, const char* restrict s2) {
    for (int i = 0 ; s1[i] != '\0' ; ++i)
        for (int j = 0 ; s2[j] != '\0' ; ++j)
            if (s1[i] == s2[j]) 
                return i;
    // If none of the characters in s2 match s1:
    return -1;
}


void check_word_list(char** word_list, const char* restrict excl_chars) {
    size_t i;
    for (i = 0 ; strcmp(word_list[i], "") != 0 ; ++i) { // Leave loop once sentinel string is reached.
        if (char_match(word_list[i], excl_chars) != -1) { // if illegal chars are present
            printf("Illegal word: %s\n", word_list[i]);
        }
    }
    return;
}

void free_word_list(char** word_list) {
    size_t i;
    for (i = 0 ; strcmp(word_list[i], "") != 0 ; ++i) {
        free(word_list[i]);
        word_list[i] = NULL;
    }

    free(word_list);
    word_list = NULL;

    return;
}


/* * * * * * * * * * * * * * * * * * * * *
 * UNUSED FUNCTION
 * Turns multiple spaces in a string into one space.
 * For some reason, it always segfaults. 
 * May be a problem of this code specifically, not the function.
 *
 * void space_reduce(char *s) {
 *    int i = 0;
 *    int j = 0;
 *    int spaces = 0;
 *
 *    while (s[i] != EOF) {
 *        switch (s[i]) {
 *            case '\n': // fallthrough
 *            case '\t':
 *            case  ' ':
 *                ++spaces;
 *                if (spaces < 2)
 *                    s[j++] = s[i];
 *                break;
 *            default:
 *                s[j++] = s[i];
 *                spaces = 0;
 *        }
 *        ++i;
 *    }
 *    s[j] = '\0';
 *    return;
 * }
 * * * * * * * * * * * * * * * * * * * * */