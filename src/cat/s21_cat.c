#include "s21_cat.h"

int main(int argc, char *argv[]) {
    if (argc > 1)
        cat(argc, argv);
    else
        printf("cat: flags empty and file...");
    return 0;
}

void cat(int argc, char *argv[]) {
    int bflag = 0;
    int eflag = 0;
    int nflag = 0;
    int sflag = 0;
    int tflag = 0;
    int vflag = 0;
    int Tflag = 0;
    int opt;

    while ((opt = getopt_long(argc, argv, "beEnvstT?", long_options, NULL)) != -1) {
        if (opt == 'b') {
            bflag++;
        } else if (opt == 'e') {
            eflag++;
            vflag++;
        } else if (opt == 'E') {
            eflag++;
        } else if (opt == 'n') {
            nflag++;
        } else if (opt == 's') {
            sflag++;
        } else if (opt == 'T') {
            Tflag++;
        } else if (opt == 't') {
            tflag++;
            vflag++;
        } else if (opt == 'v') {
            vflag++;
        } else if (opt == '?') {
            printf("Используйте: s21_cat [-beEnsvtT] [file ...]\n");
            exit(1);
        }
    }
    FILE *file;
    char c[50] = {'\0'};
    char pred = '\n';
    char pred_pred = '\0';
    int current_file = optind;
    while (current_file <= argc) {
        if (current_file != argc) {
            file = fopen(argv[current_file], "r");
            if (file == NULL) {
                fprintf(stderr, "%s: %s: No such file or directory", argv[0], argv[current_file]);
                exit(1);
            }
        }
        int line_number_empty = 1;
        int line_number_all = 1;
        while ((*c = fgetc(file)) != EOF) {
            if (sflag) {
                if (option_sflag(*c, pred, pred_pred)) {
                continue;
                }
            }
            if (bflag) {
                option_bflag(*c, pred, &line_number_empty);
            } else if (nflag) {
                option_nflag(&line_number_all, pred);
            }
            if (eflag) {
                option_eflag(*c);
            }
            if (vflag) {
                option_vflag(c);
            }
            if (tflag || Tflag) {
                if (option_tflag(*c)) {
                    pred = *c;
                    continue;
                }
            }
            pred_pred = pred;
            pred = *c;
            if (*c == '\0') {
                fputc(*c, stdout);
            } else {
                fputs(c, stdout);
            }

            memset(c, '\0', 5);
        }
        current_file++;
    }
    fclose(file);
}

int is_special(unsigned char c) {
    return (c <= 8) || (c >= 11 && c <= 31) || (c >= 127 && c <= 159);
}
void option_bflag(char c, char pred, int *line_number_empty) {
    if (pred == '\n' && c != '\n') {
        printf("%6d\t", (*line_number_empty)++);
    }
}

void option_nflag(int *line_number_all, char pred) {
    if (pred == '\n') {
        printf("%6d\t", (*line_number_all)++);
    }
}

void option_eflag(char c) {
    if (c == '\n') {
        putc('$', stdout);
    }
}

int option_sflag(char c, char pred, char pred_pred) {
    return c == '\n' && pred == '\n' && pred_pred == '\n';
}

void option_vflag(char *c) {
    int uc = *c;
    if (is_special(uc)) {
        if (uc <= -1) {
            uc *= -1;
            uc = 256 - uc;
        }
        strcpy(c, s21_cat_spec_symbols[uc]);
    }
}

int option_tflag(char c) {
    int q = 0;
    if (c == '\t') {
        printf("^I");
         q = 1;
    }
    return q;
}
