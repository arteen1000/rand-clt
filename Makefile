# Make x86-64 random byte generators.

# Copyright 2015, 2020, 2021 Paul Eggert

# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

# Optimization level.  Change this -O2 to -Og or -O0 or whatever.
OPTIMIZE = -O3

# The C compiler and its options.
CC = gcc
CFLAGS = $(OPTIMIZE) -g3 -Wall -Wextra -fanalyzer \
  -march=native -mtune=native -mrdrnd

# The archiver command, its options and filename extension.
TAR = tar
TARFLAGS = --gzip --transform 's,^,randall/,'
TAREXT = tgz

# bytes to check for

CHECK_BYTES = 1000

# compilations

default: randall

# rand64-hw.o: rand64-hw.c rand64-hw.h

# rand64-sw.o: rand64-sw.c rand64-sw.h 

# options.o: options.c options.h 

# output.o: output.c output.h 

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

randall: randall.c rand64-hw.o rand64-sw.o options.o output.o
	$(CC) $(CFLAGS) $^ -o $@

check: randall
	[ $$(./randall $(CHECK_BYTES) | wc -c) -eq $(CHECK_BYTES) ]
	[ $$(./randall $(CHECK_BYTES) -i /dev/random | wc -c) -eq $(CHECK_BYTES) ]
	[ $$(./randall $(CHECK_BYTES) -i mrand48_r | wc -c) -eq $(CHECK_BYTES) ]
	[ $$(./randall $(CHECK_BYTES) -i rdrand | wc -c) -eq $(CHECK_BYTES) ]
	[ $$(./randall $(CHECK_BYTES) -o stdio | wc -c) -eq $(CHECK_BYTES) ]
	[ $$(./randall $(CHECK_BYTES) -o 1 | wc -c) -eq $(CHECK_BYTES) ]
	[ $$(./randall $(CHECK_BYTES) -o 7 | wc -c) -eq $(CHECK_BYTES) ]
	[ $$(./randall $(CHECK_BYTES) -o 67 | wc -c) -eq $(CHECK_BYTES) ]
	[ $$(./randall $(CHECK_BYTES) -o $$(( $(CHECK_BYTES) + 7 ))  | wc -c) -eq $(CHECK_BYTES) ]
	[ $$(./randall $(CHECK_BYTES) -o 1 -i /dev/random | wc -c) -eq $(CHECK_BYTES) ]
	[ $$(./randall $(CHECK_BYTES) -o 7 -i mrand48_r | wc -c) -eq $(CHECK_BYTES) ]
	[ $$(./randall $(CHECK_BYTES) -o 67 -i rdrand | wc -c) -eq $(CHECK_BYTES) ]
	[ $$(./randall $(CHECK_BYTES) -o 1007 -i mrand48_r | wc -c) -eq $(CHECK_BYTES) ]
# final check avoids using $$(( $(CHECK_BYTES) + 7 )) due to the length limit

# assignment related

assignment: randall-assignment.$(TAREXT)
assignment-files = COPYING Makefile randall.c options.c options.h output.c output.h \
rand64-hw.c rand64-hw.h rand64-sw.c rand64-sw.h
randall-assignment.$(TAREXT): $(assignment-files)
	$(TAR) $(TARFLAGS) -cf $@ $(assignment-files)

submission-tarball: randall-submission.$(TAREXT)
submission-files = $(assignment-files) \
  notes.txt # More files should be listed here, as needed.
randall-submission.$(TAREXT): $(submission-files)
	$(TAR) $(TARFLAGS) -cf $@ $(submission-files)

repository-tarball:
	$(TAR) -czf randall-git.tgz .git .gitignore

.PHONY: default clean assignment submission-tarball repository-tarball

clean:
	rm -f *.o *.$(TAREXT) randall
