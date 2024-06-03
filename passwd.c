#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "utils.h"

const char uppercase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char lowercase[] = "abcdefghijklmnopqrstuvwxyz";
const char numbers[] = "0123456789";
const char symbols[] = "!@#$%^&*()_-=+{[]}\\|;:'\",<.>/?";

void generate_password(int argc, char** argv) {
    srand(time(0));
    int length = 8;
    char chars[128] = "";
    int chars_length = 0;
    int clipboard = 0;
    int noprint = 0;
    int fout = 0;
    int fname = 0;
    int custom_chars = 0;

    if (argc == 1) {
        goto generate;
    }

    // handle arguments
    for (int i = 1; i < argc; ++i) {
        // adds uppercase letters
        if (strcmp(argv[i], "-upper") == 0) {
            strncat(chars, uppercase, sizeof(chars) - strlen(chars) - 1);
            chars_length += strlen(uppercase);
        }
        // adds lowercase letters
        else if (strcmp(argv[i], "-lower") == 0) {
            strncat(chars, lowercase, sizeof(chars) - strlen(chars) - 1);
            chars_length += strlen(lowercase);
        }
        // adds numbers
        else if (strcmp(argv[i], "-num") == 0) {
            strncat(chars, numbers, sizeof(chars) - strlen(chars) - 1);
            chars_length += strlen(numbers);
        }
        // add symbols
        else if (strcmp(argv[i], "-sym") == 0) {
            strncat(chars, symbols, sizeof(chars) - strlen(chars) - 1);
            chars_length += strlen(symbols);
        }
        // sets custom length
        else if (strcmp(argv[i], "-len") == 0) {
            if (argc > i + 1)
                length = atoi(argv[++i]);
        }
        // sets to copy to clipboard
        else if (strcmp(argv[i], "-cb") == 0) {
            clipboard = 1;
        }
        // sets to refrain from printing
        else if (strcmp(argv[i], "-np") == 0) {
            noprint = 1;
        }
        else if (strcmp(argv[i], "-ch") == 0) {
            if (argc > i + 1) {
                ++i;
                strncat(chars, argv[i], sizeof(chars) - strlen(chars) - 1);
                custom_chars = 1;
                chars_length += strlen(argv[i]);
            }
        }
        // sets to copy to textfile
        else if (strcmp(argv[i], "-o") == 0) {
            if (argc > i + 1) {
               ++i;
               fname = i;
               fout = 1;
            }
        }
        else if (strcmp(argv[i], "-h") == 0) {
            pr_help();
            return;
        }
    }

    if (length == 0) {
        fprintf(stderr, "Length cannot be 0");
        return;
    }

generate:
    
    /* if length is 0, all char attributes are added to the password */
    if (chars_length + custom_chars == 0) {
        strcat(chars, uppercase);
        strcat(chars, lowercase);
        strcat(chars, numbers);
        strcat(chars, symbols);
        chars_length = strlen(uppercase) + strlen(lowercase) + strlen(numbers) + strlen(symbols);
    }

    char password[length + 1];
    for (int i = 0; i < length; ++i) {
        password[i] = chars[(rand() % chars_length)];
    }

    password[length] = '\0';
    if (clipboard > 0)
        to_clipboard(password);

    if (fout > 0)
        to_file(password, argv[fname]);

    if (noprint == 0)
        printf("%s\n", password);
}
