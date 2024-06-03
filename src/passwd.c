#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "utils.h"

const char uppercase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char lowercase[] = "abcdefghijklmnopqrstuvwxyz";
const char numbers[] = "0123456789";
const char symbols[] = "!@#$%^&*()_-=+{[]}\\|;:'\",<.>/?";

typedef struct {
    char av_chars[256];
    int total_chars;
    int password_length;
    int use_clipboard;
    int display_password;
    int file_index;
    int has_custom_chars;
    int display_seed;
    int use_custom_seed;
} Passwd;

void init_passwd(Passwd* passwd) {
    passwd->av_chars[0] = '\0';
    passwd->total_chars = 0;
    passwd->password_length = 8;
    passwd->use_clipboard = 0;
    passwd->display_password = 1;
    passwd->file_index = 0;
    passwd->has_custom_chars = 0;
    passwd->display_seed = 0;
    passwd->use_custom_seed = 0;
}

void parse_args(int argc, char** argv, Passwd* passwd) {
    // handle arguments
    for (int i = 1; i < argc; ++i) {
        // adds uppercase letters
        if (strcmp(argv[i], "-upper") == 0) {
            strncat(passwd->av_chars, uppercase, sizeof(passwd->av_chars) - strlen(passwd->av_chars) - 1);
            passwd->total_chars += strlen(uppercase);
        }
        // adds lowercase letters
        else if (strcmp(argv[i], "-lower") == 0) {
            strncat(passwd->av_chars, lowercase, sizeof(passwd->av_chars) - strlen(passwd->av_chars) - 1);
            passwd->total_chars += strlen(lowercase);
        }
        // adds numbers
        else if (strcmp(argv[i], "-num") == 0) {
            strncat(passwd->av_chars, numbers, sizeof(passwd->av_chars) - strlen(passwd->av_chars) - 1);
            passwd->total_chars += strlen(numbers);
        }
        // add symbols
        else if (strcmp(argv[i], "-sym") == 0) {
            strncat(passwd->av_chars, symbols, sizeof(passwd->av_chars) - strlen(passwd->av_chars) - 1);
            passwd->total_chars += strlen(symbols);
        }
        // sets custom length
        else if (strcmp(argv[i], "-len") == 0) {
            if (argc > i + 1) {
                passwd->password_length = atoi(argv[++i]);
            }
        }
        // sets to copy to clipboard
        else if (strcmp(argv[i], "-cb") == 0) {
            passwd->use_clipboard = 1;
        }
        // sets to refrain from printing
        else if (strcmp(argv[i], "-np") == 0) {
            passwd->display_password = 0;
        }
        // specify character to be used 
        else if (strcmp(argv[i], "-ch") == 0) {
            if (argc > i + 1) {
                ++i;
                strncat(passwd->av_chars, argv[i], sizeof(passwd->av_chars) - strlen(passwd->av_chars) - 1);
                passwd->has_custom_chars = 1;
                passwd->total_chars += strlen(argv[i]);
            }
        }
        else if (strcmp(argv[i], "-seed") == 0) {
            if (argc > i + 1) {
                ++i;
                passwd->use_custom_seed = atoi(argv[i]);
            }
        }
        // sets to copy to textfile
        else if (strcmp(argv[i], "-o") == 0) {
            if (argc > i + 1) {
                ++i;
                passwd->file_index = i;
            }
        }
        else if (strcmp(argv[i], "-h") == 0) {
            pr_help();
            return;
        }
    }

    if (passwd->password_length == 0) {
        fprintf(stderr, "Length cannot be 0\n");
        return;
    }
}

void generate_password(int argc, char** argv) {
    Passwd passwd;
    init_passwd(&passwd);

    if (argc == 1)
        goto generate;
    else 



generate:
    /* if no characters are specified, use all character attributes */
    if (passwd.total_chars == 0) {
        strncat(passwd.av_chars, uppercase, sizeof(passwd.av_chars) - strlen(passwd.av_chars) - 1);
        strncat(passwd.av_chars, lowercase, sizeof(passwd.av_chars) - strlen(passwd.av_chars) - 1);
        strncat(passwd.av_chars, numbers, sizeof(passwd.av_chars) - strlen(passwd.av_chars) - 1);
        strncat(passwd.av_chars, symbols, sizeof(passwd.av_chars) - strlen(passwd.av_chars) - 1);
        passwd.total_chars = strlen(passwd.av_chars);
    }

    // Set seed to either custom or unix-time
    if (passwd.use_custom_seed > 0) {
        srand(passwd.use_custom_seed);
    } else {
        srand(time(0));
    }

    /* Generate password */
    char password[passwd.password_length + 1];
    for (int i = 0; i < passwd.password_length; ++i) {
        password[i] = passwd.av_chars[rand() % passwd.total_chars];
    }

    password[passwd.password_length] = '\0';

    if (passwd.use_clipboard > 0) {
        to_clipboard(password);
    }

    // Output password in filename specified in argv[file_index]
    if (passwd.file_index > 0) {
        to_file(password, argv[passwd.file_index]);
    }

    if (passwd.display_password == 1) {
        printf("%s\n", password);
    }
}
