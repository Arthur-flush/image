# How to use
## Part 1
make part1 <br>
./main filename.ppm

### Alternative Compilation if make doesnt work
gcc part1.c -lm -o main

## Part 2
make part2 <br>
./main filename.ppm

### Alternative Compilation if make doesnt work
gcc part2.c -lm -o main

#### how to open generated file
python3 readjpg.py filename.jpeg

# Limitations
only works for files where the x and y is a multiple of 8 (doesnt need x == y)