#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "util.h"
#include "fs/fs.h"

char *concat(char *str, ...) {
  va_list args;
	char *s;
  /* Get len */
  int len = strlen(str);
  va_start(args, str);
  while ((s = va_arg(args, char*))) {
    len += strlen(s);
  }
  va_end(args);
  /* Build string */
  char *res = malloc(len + 1);
  if (!res) return NULL;
  strcpy(res, str);
  va_start(args, str);
  while ((s = va_arg(args, char*))) {
    strcat(res, s);
  }
  va_end(args);
  return res;
}

void list_dir(fs_FileListNode *list, char *sp, char *root, int f) {
  fs_FileListNode *n = list;
  int i = 0;
  while (n) {
    char *delim = "├";
    if (i == 0 && !f) delim = "┌";
    else if (!n->next) delim = "└";
    // printf("%s%s─ %s\n", sp, delim, n->name);
    char *dir = concat(root, "/", n->name, NULL);
    printf("%s\n", dir);
    if (fs_isDir(dir)) {
      fs_FileListNode *l = fs_listDir(dir);
      list_dir(l, concat(sp, "  ", NULL), dir, 1);
      fs_freeFileList(l);
    }
    free(dir);
    n = n->next;
    i++;
  }
}

int main(int argc, const char **argv) {
  ((void) argc);
  ((void) argv);

  fs_error(fs_mount(argv[1])) ;

  fs_FileListNode *list = fs_listDir(".");
  list_dir(list, " ", (char *)argv[1], 0);
  fs_freeFileList(list);
  fs_deinit();
  return 0;
}
