#!/bin/sh

#autoreconf --force --install -I config -I m4
aclocal \
&& automake --add-missing \
&& autoconf
