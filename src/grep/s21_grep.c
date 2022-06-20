#include "s21_grep.h"

int main(int argc, char *argv[]) {
    char pattern[SIZE] = {0};
    Flags flags;
    init_flags(&flags);
    int init_flags(Flags *flags);
    if (argc > 1) {
        parse_option(argc, argv, &flags, pattern);
        open_file(argc, argv, flags, pattern);
    } else {
        printf("grep: flags empty and file...");
    }
    return 0;
}

int init_flags(Flags *flags) {
    flags->e = 0;
    flags->i = 0;
    flags->v = 0;
    flags->c = 0;
    flags->l = 0;
    flags->n = 0;
    flags->h = 0;
    flags->s = 0;
    flags->str_empty = 0;
    return 0;
}

void file_count(Flags *flags, int *count_e_file, char *pattern) {
    if (*count_e_file) {
        strcat(pattern, "|");
    }
    if (!*optarg) {
        optarg = ".";
        flags->str_empty += 1;
    }
    strcat(pattern, optarg);
    *count_e_file += 1;
}

void parse_option(int argc, char *argv[], Flags *flags, char *pattern) {
    int opt;
    int count_e_file = 0;
    flags->opt_count = 0;
    while ((opt = getopt(argc, argv, "+e:ivclnhs?")) != -1) {
        if (opt == 'e') {
            flags->e = 1;
            file_count(flags, &count_e_file, pattern);
        } else if (opt == 'i') {
            flags->i = 1;
        } else if (opt == 'v') {
            flags->v = 1;
        } else if (opt == 'c') {
            flags->c = 1;
        } else if (opt == 'l') {
            flags->l = 1;
        } else if (opt == 'n') {
            flags->n = 1;
        } else if (opt == 'h') {
            flags->h = 1;
        } else if (opt == 's') {
            flags->s = 1;
        } else if (opt == '?') {
            printf("Используйте: s21_grep [-eivclnhs] [regex] [file ...]\n");
            exit(1);
        }
        flags->opt_count++;
    }
    if (!flags->e) {
        if (!*argv[optind]) argv[optind] = ".";
        strcat(pattern, argv[optind]);
        optind += 1;
    }
}

void open_file(int argc, char *argv[], Flags flags, char *pattern) {
        FILE *file = NULL;
        char *buff = NULL;
        int count_n = 1;
        int current_file = optind;
        int count_c = 0;
        int count_vc = 0;
        while (current_file < argc) {
            buff = calloc(SIZE, sizeof(char));
            file = fopen(argv[current_file], "r");
            if (file == NULL) {
                if (!flags.s) {
                    fprintf(stderr, "%s: %s: No such file or directory", argv[0], argv[current_file]);
                }
                exit(1);
            }
            while (fgets(buff, SIZE, file)) {
                regex_str(argc, argv, current_file, buff, &flags, count_n, &count_c, &count_vc, pattern);
                count_n++;
            }
            if (argc - optind > 1 && ((flags.c && !flags.v) || (flags.c && flags.v))) {
                printf("%s:", argv[current_file]);
            }
              if (flags.c && !flags.v) {
                printf("%d\n", count_c);
            }
             if (flags.c && flags.v) {
                printf("%d\n", count_vc);
            }
            if (flags.l && !flags.v && count_c > 0) {
                printf("%s\n", argv[current_file]);
            } else {
                if (flags.l && flags.v && count_vc > 0) {
                    printf("%s\n", argv[current_file]);
                }
            }
            fclose(file);
            free(buff);
            current_file++;
            count_n = 1;
            count_c = 0;
            count_vc = 0;
        }
}

void regex_str(int argc, char *argv[], int current_file, char *buff, Flags *flags,
              int count_n, int *count_c, int *count_vc, char *pattern) {
    regex_t regex;
    int value_re;
    int match;
    int regex_flag = REG_EXTENDED;
    if (flags->i) {
        regex_flag = REG_EXTENDED|REG_ICASE;
    }
    value_re = regcomp(&regex, pattern, regex_flag);
    if (value_re == 0) {
        match = regexec(&regex, buff, 0, NULL, 0);
    }
    if (match == 0) {
        *count_c += 1;
        if (argc - optind > 1 && !flags->l && !flags->h && !flags->v && !flags->c)
            printf("%s:", argv[current_file]);
        if (flags->n && !flags->l && !flags->c && !flags->v)
            printf("%d:", count_n);
        if (!flags->v && !flags->l && !flags->c) {
            if (buff[strlen(buff) - 1] != '\n') {
                printf("%s\n", buff);
            } else {
                 printf("%s", buff);
            }
        }
    } else {
        if (argc - optind > 1 && flags->v && !flags->c && !flags->l && !flags->h && !flags->e) {
            printf("%s:", argv[current_file]);
        }
        if (flags->n && flags->v && !flags->l && !flags->c) {
            printf("%d:", count_n);
        }
        if (flags->v && !flags->c && !flags->l) {
            if (buff[strlen(buff) - 1] != '\n') {
                printf("%s\n", buff);
            } else {
                 printf("%s", buff);
            }
        }
        *count_vc += 1;
    }
    regfree(&regex);
}

