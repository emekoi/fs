#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "util.h"
#include "fs/fs.h"

int main(int argc, const char **argv) {
  fs_error(fs_mount("test0")) ;
  fs_error(fs_mount("test1")) ;

  fs_FileListNode *list = fs_listDir("dir1");
  int i = 1;
  fs_FileListNode *n = list;
  while (n) {
    printf("%s\n", n->name);
    i++;
    n = n->next;
  }
  fs_freeFileList(list);

  fs_deinit();
  return 0;
}
