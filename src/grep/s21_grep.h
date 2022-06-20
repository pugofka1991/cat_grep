#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <regex.h>
#define SIZE 8000

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int str_empty;
  int files_count;
  int opt_count;
} Flags;

void parse_option(int argc, char *argv[], Flags *flags, char *pattern);
void open_file(int argc, char *argv[], Flags flags, char *pattern);
void file_count(Flags *flags, int *count_e_file, char *pattern);
void file_proc(int *count_e_file, char *pattern, Flags *flags);
void regex_str(int argc, char *argv[], int current_file, char *buff, Flags *flags,
              int count_n, int *count_c, int *count_vc, char *pattern);
int init_flags(Flags *flags);

#endif  // SRC_GREP_S21_GREP_H_
