#!/usr/bin/python2.7
import os, sys, shutil, platform, time

OUTPUT = "bin/fsx"
COMPILER = "gcc"
INCLUDE = [  ]
SOURCE = [
  "src/*.c",
  "src/microtar/*.c",
]
FLAGS = [ "-Wall", "-Wextra", "--std=c99", "-fno-strict-aliasing", "-pedantic" ]
LINK = [ "m" ]
DEFINE = [ ]
EXTRA = ""

if platform.system() == "Windows":
  OUTPUT += ".exe"
  LINK += [ "mingw32" ]
  FLAGS += [ "-mwindows" ]

if platform.system() == "Linux":
  LINK += [  ]

if platform.system() == "Darwin":
  LINK += [  ]
  FLAGS += [  ]


def fmt(fmt, dic):
  for k in dic:
    fmt = fmt.replace("{" + k + "}", str(dic[k]))
  return fmt


def main():
  global FLAGS, SOURCE, LINK

  print "initing..."
  starttime = time.time()

  # Handle args
  build = "debug" if "debug" in sys.argv else "release"
  verbose = "verbose" in sys.argv

  # Handle build type
  if build == "debug":
    FLAGS += [ "-g" ]
  else:
    FLAGS += [ "-O3" ]

  print "building (" + build + ")..."

  # Create directories
  os.system("rm " + OUTPUT)
  outdir = os.path.dirname(OUTPUT)
  if not os.path.exists(outdir):
    os.makedirs(outdir)

  # Build
  cmd = fmt(
    "{compiler} -o {output} {flags} {source} {include} {link} {define} " +
    "{extra}",
    {
      "compiler"  : COMPILER,
      "output"    : OUTPUT,
      "source"    : " ".join(SOURCE),
      "include"   : " ".join(map(lambda x:"-I" + x, INCLUDE)),
      "link"      : " ".join(map(lambda x:"-l" + x, LINK)),
      "define"    : " ".join(map(lambda x:"-D" + x, DEFINE)),
      "flags"     : " ".join(FLAGS),
      "extra"     : EXTRA,
    })

  if verbose:
    print cmd

  print "compiling..."
  res = os.system(cmd)

  if build == "release":
    print "stripping..."
    os.system("strip %s" % OUTPUT)

  if res == 0:
    print "done (%.2fs)" % (time.time() - starttime)
  else:
    print "done with errors"
  sys.exit(res)


if __name__ == "__main__":
  main()
