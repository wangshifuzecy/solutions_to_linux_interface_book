# 16-1
Write a program that can be used to create or modify a user EA for a file (i.e., a simple version of `setfattr(1)`). The filename and the EA name and value should be supplied as command-line arguments to the program.

## answer
see `setfattr.c`, use `getfattr -d files` to check.
only implement `setfattr -n name [-v value] files...` 
