#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "map/map.h"
#include "util.h"
#include "fs.h"

typedef map_t(unsigned) unsigned_map_t;
unsigned_map_t filemap;
fs_FileListNode *list;

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

void list_dir(fs_FileListNode *list, char *root, unsigned_map_t *files) {
  fs_FileListNode *n = list;
  for (int i = 0; n; i++) {
    char *dir = concat(root, "/", n->name, NULL);

    unsigned *tmp, last, time = 0;
    fs_error(fs_modified(dir, &time));

    tmp = map_get(files, dir);
    last = (tmp ? *tmp : time);
    if (!tmp) map_set(files, dir, time);
    
    if (time != last) {
      map_set(files, dir, time);
      printf("%s has been modified\n", dir);
    }
 

    if (fs_isDir(dir)) {
      fs_FileListNode *l = fs_listDir(dir);
      list_dir(l, dir, files);
      fs_freeFileList(l);
    } 
    free(dir);
    n = n->next;
  }
}

void _wait(double n) {
  clock_t target = clock() / CLOCKS_PER_SEC + n;
  while (clock() / CLOCKS_PER_SEC != target);
}

void shutdown() {
  map_deinit(&filemap);
  fs_freeFileList(list);
  fs_deinit();
}

int main(int argc, const char **argv) {
  atexit(shutdown);

  /* Enable single line buffering for Windows */
  #if _WIN32
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
  #endif
 
  fs_error(fs_mount(argv[1]));
  double interval;

  char *trash;
  interval = (argc > 2 ?  strtod(argv[2], &trash) : 0.5);
  
  list = fs_listDir(".");
  map_init(&filemap);
  while (1) {
    list_dir(list, (char *)argv[1], &filemap);
    _wait(interval);
  }

  return 0;
}
