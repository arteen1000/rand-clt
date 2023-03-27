# rand-clt

`./randall: usage: ./randall NBYTES [ -i [ rdrand | mrand48_r | /absolute/file/path ] ] [ -o [ stdio | N ] ]`

Compile using `make`, `make check` verifies correct program functionality.

`-i input` to specify:
- `rdrand` - hardware random-number generation supported by x86-64 processors, error if unavailable
- `mdrand48_r` - [mdrand48_r](https://www.gnu.org/software/libc/manual/html_node/SVID-Random.html#index-mrand48_005fr) of GNU C library
- `/F` - uses file `/F` as source of random data

`-o output`:
- `stdio` - use stdio output
- `N` - output `N` bytes at a time, using `write` system call. Will write `NBYTES/N` output bytes in first `NBYTES/N` iterations,
will write `NBYTES%N` bytes on the last, if applicable.

Options parsing implemented manually, no modules used.

Assignment can be found at: [here](https://web.cs.ucla.edu/classes/winter23/cs35L/assign/assign5.html).
