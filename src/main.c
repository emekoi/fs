#include <stdio.h>
#include "util.h"
#include "fs.h"

int main(int argc, const char **argv) {
  ((void) argc);
  ((void) argv);

  fs_error(fs_mount(argv[1])) ;

  /*
  http://www.theasciicode.com.ar/extended-ascii-code/angle-quotes-guillemets-right-pointing-double-angle-french-quotation-marks-ascii-code-174.html
  └
  ├
  */

  fs_FileListNode *list = fs_listDir(".");
  fs_FileListNode *n = list;
  while (n) {
    printf("├ %s\n", n->name);
    if
    n = n->next;
  }
  fs_freeFileList(list);
  fs_deinit();
  return 0;
}
