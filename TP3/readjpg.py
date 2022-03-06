import numpy as np
from PIL  import Image as img
import sys

if len(sys.argv) != 2:
    print(f"wrong argument number should be 1 is {len(sys.argv)-1}")
    exit(1)

filename = sys.argv[1]

data = []
with open(filename, "r") as f:
    for l in f.readlines():
        l = l.strip()
        if l != "":
            data.append(l)

if data[0] != "JPEG":
    print("invalid file type")
    exit(1)

width, height = data[1].split(" ")
width = int(width)
height = int(height)

pic = np.zeros((height, width))

actual_width = ((width % 8)+1) * 8
actual_height = ((height % 8)+1) * 8

to_skip = 0
data = np.array(data[2:])

positions = np.array((
    (0, 0),
    (0, 1),
    (1, 0),
    (2, 0),
    (1, 1),
    (0, 2),
    (0, 3),
    (1, 2),
    (2, 1),
    (3, 0),
    (4, 0),
    (3, 1),
    (2, 2),
    (1, 3),
    (0, 4),
    (0, 5),
    (1, 4),
    (2, 3),
    (3, 2),
    (4, 1),
    (5, 0),
    (6, 0),
    (5, 1),
    (4, 2),
    (3, 3),
    (2, 4),
    (1, 5),
    (0, 6),
    (0, 7),
    (1, 6),
    (2, 5),
    (3, 4),
    (4, 3),
    (5, 2),
    (6, 1),
    (7, 0),
    (7, 1),
    (6, 2),
    (5, 3),
    (4, 4),
    (3, 5),
    (2, 6),
    (1, 7),
    (2, 7),
    (3, 6),
    (4, 5),
    (5, 4),
    (6, 3),
    (7, 2),
    (7, 3),
    (6, 4),
    (5, 5),
    (4, 6),
    (3, 7),
    (4, 7),
    (5, 6),
    (6, 5),
    (7, 4),
    (7, 5),
    (6, 6),
    (5, 7),
    (6, 7),
    (7, 6),
    (7, 7)
))

for i in range(height / 8):
    for j in range(width / 8):
        x = 0
        y = 0
        for x, y in positions:
            if to_skip > 0:
                to_skip -= 1
                pic[i * 8 + x, j * 8 + y] = int(data[i*actual_width+j])
                
        

for i in range(height):
    for j in range(width):
        if to_skip > 0:
            to_skip -= 1
            continue
        if j < width:
            if data[i*actual_width+j][0] == "@":
                to_skip = int(data[i*actual_width+j][1:])
            else:
                pic[i, j] = int(data[i*actual_width+j])

pic += 127


img.fromarray(pic, 'L').show()