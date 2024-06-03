#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char uppercase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char lowercase[] = "abcdefghijklmnopqrstuvwxyz";
const char numbers[] = "0123456789";
const char symbols[] = "!@#$%^&*()_-=+{[]}\\|;:'\",<.>/?";

void pr_help() {
    printf("-upper.......upperacase\n");
    printf("-lower.......lowercase\n");
    printf("-num.........numbers\n");
    printf("-sym.........symbols\n");
    printf("-len <len>...length\n");
    printf("-np..........no print\n");
    printf("-cb..........clipboard\n");
    printf("-o <file>....file\n");
}

/* copies password to file */
void to_file(const char* password, const char* fname) {
    FILE* fp = fopen(fname, "w");
    if (fp == NULL) {
        perror("could not write to file");
        exit(1);
    }

    fprintf(fp, "%s", password);
    fclose(fp);
}

/* copies password to xclip clipboard */
void to_clipboard(const char* password) {
    
    FILE* fp = popen("xclip -selection clipboard", "w");
    if (fp == NULL) {
        perror("could not access xclip clipboard");
        exit(1);
    }

    size_t len = strlen(password);
    if (fwrite(password, sizeof(char), len, fp) != len) {
        perror("could not write");
        exit(1);
    }

    if (pclose(fp) == -1) {
        perror("pclose");
        exit(1);
    }
}

int main(int argc, char** argv) {

    srand(time(0));
    int length = 8;
    char chars[128] = "";
    int chars_length = 0;
    int clipboard = 0;
    int noprint = 0;
    int fout = 0;
    int fname = 0;

    if (argc == 1) {
        goto generate;
    }

    // handle arguments
    for (int i = 1; i < argc; ++i) {
        // adds uppercase letters
        if (strcmp(argv[i], "-upper") == 0) {
            strcat(chars, uppercase);
            chars_length += strlen(uppercase);
        }
        // adds lowercase letters
        else if (strcmp(argv[i], "-lower") == 0) {
            strcat(chars, lowercase);
            chars_length += strlen(lowercase);
        }
        // adds numbers
        else if (strcmp(argv[i], "-num") == 0) {
            strcat(chars, numbers);
            chars_length += strlen(numbers);
        }
        // add symbols
        else if (strcmp(argv[i], "-sym") == 0) {
            strcat(chars, symbols);
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
            return 1;
        }
    }

    if (length == 0) {
        fprintf(stderr, "Length cannot be 0");
        return 1;
    }

generate:
    
    /* if length is 0, all char attributes are added to the password */
    if (chars_length == 0) {
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

    return 0;
}
