#!/bin/sh
if test -z "$1"
then
  aclocal
  autoconf
  automake -a
else
  if test "$1" = "clean"
  then
    make clean
    make maintainer-clean-am
    rm -fr src/Makefile src/Makefile.in \
          Makefile Makefile.in \
          aclocal.m4 mkinstalldirs install-sh \
         missing config.status configure \
         src/.deps
  fi
fi

