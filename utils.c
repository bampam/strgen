#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
