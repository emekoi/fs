#include <stdio.h>
#include "util.h"
#include "fs.h"

int main(int argc, const char **argv) {
  ((void) argc);
  ((void) argv);

  fs_error(fs_mount(argv[1])) ;

  fs_FileListNode *list = fs_listDir(".");
  fs_FileListNode *n = list;
  while (n) {
    puts(n->name);
    n = n->next;
  }
  fs_freeFileList(list);
  fs_deinit();
  return 0;
}
