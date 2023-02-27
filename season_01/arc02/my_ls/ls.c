#include <stdio.h>      // printf
#include <dirent.h>     // dirent, readdir, opendir, closedir
#include <string.h>     // strcmp, strcpy
#include <stdlib.h>     // malloc, qsort
#include <sys/stat.h>   // stat

// We will process up to 100 entries by default
#define ENTRIES_COUNT 100
// default entry name length will be 50
#define ENTRY_NAME_LEN 50

// Sets up the flags for -a and -t
void setup_flags(char** argv, int argc, short *a, short *t) {
  for (int i = 1; i < argc; i++) {   
    char* arg = argv[i];

    if (arg[0] == '-') { // handle flag
      for (size_t j = 0; j < strlen(arg); j++)
        switch (arg[j]) {
            case 'a': *a = 1; break;
            case 't': *t = 1; break;
            default: break;
        }
    } else continue;
  }
}

// A predicate to compare two directory entry (dirent) names 
int _compare_names(const void* a, const void* b) {
  return strcmp(*(const char**)a, *(const char**)b);
}

// Sorts the dirent(s) alphabetically
void sort_alphabetically(char** names, int count){
  qsort(names, count, sizeof(char*), _compare_names);
}

// Compares the last time two given dirent(s) were modified
int _compare_time(const void* a, const void* b) {
  struct stat first = {0}, second = {0};
  const char* dir1 = (*(const char**)a);
  const char* dir2 = (*(const char**)b);

  stat(dir1, &first);
  stat(dir2, &second);

  return (first.st_mtime < second.st_mtime);
}

// Sorts the dirent(s) by the last time modified
void sort_by_time(char** names, int count) {
  qsort(names, count, sizeof(char*), _compare_time);
}

// Read, parse and memorize the folder, given by name
int parse_folder(char* name, char** names, int* count, int all) {
  struct dirent *pDirent;
  DIR *pDir;  
  
  // Ensure we can open directory.
  pDir = opendir(name);
  if (pDir == NULL) {
    printf ("Cannot open directory '%s'\n", name);
    return 1;
  }

  // Process each entry.
  while ((pDirent = readdir(pDir)) != NULL) {
    if (all == 0 && pDirent->d_name[0] == '.')
      continue;
      
    strcpy(names[*count], pDirent->d_name);
    (*count)++;
  }

  // Close directory and exit.
  closedir (pDir);

  return 0;
}

int main (int argc, char *argv[]) {
  short all = 0, time = 0, total_args_parsed = 0;
  setup_flags(argv, argc, &all, &time);

  char** names = (char**)malloc(ENTRIES_COUNT * sizeof(char*));
  for (int i = 0; i < ENTRIES_COUNT; i++)
    names[i] = (char*)malloc(ENTRY_NAME_LEN * sizeof(char));
  int names_count = 0;

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      char* arg = argv[i];

      if (arg[0] == '-') continue;
    
      parse_folder(arg, names, &names_count, all);
      total_args_parsed++;
    }
  }
  if (argc == 0 || total_args_parsed == 0)
    parse_folder(".", names, &names_count, all);

  sort_alphabetically(names, names_count);

  if (time == 1)
    sort_by_time(names, names_count);

  for (int i = 0; i < names_count; i++){
    printf("%s\n", names[i]);
  }
  
  for (int i = 0; i < ENTRIES_COUNT; i++)
    free(names[i]);
  free(names);
  
  return 0;
}
