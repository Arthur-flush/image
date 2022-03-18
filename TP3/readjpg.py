import numpy as np
from PIL  import Image as img
import sys
from scipy.fftpack import idct #thank you scipy

def idct2(block):
    return idct(idct(block.T, norm='ortho').T, norm='ortho')

Q = np.array([
    [16, 11, 10, 16, 24, 40, 51, 61],
    [12, 12, 14, 19, 26, 58, 60, 55],
    [14, 13, 16, 24, 40, 57, 69, 56],
    [14, 17, 22, 29, 51, 87, 80, 62],
    [18, 22, 37, 56, 68, 109, 103, 77],
    [24, 35, 55, 64, 81, 104, 113, 92],
    [49, 64, 78, 87, 103, 121, 120, 101],
    [72, 92, 95, 98, 112, 100, 103, 99]
], np.int32)

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

def reverse_zigzagextract(data, height, width):
    pic = np.zeros((height, width))
    for i in range(int(height / 8)):
        for j in range(int(width / 8)):
            for p in range(64):
                x, y = positions[p]
                v = data[i*(64 * int(width / 8))+j*64 + p]
                pic[i * 8 + x, j * 8 + y] = int(v)
    return pic

l = []
for v in data:
    if v[0] == "@":
        for i in range(int(v[1:])):
            l.append(0)
    else:
        l.append(int(v))


pic = reverse_zigzagextract(l, height, width)


for i in range(int(height / 8)):
    for j in range(int(width / 8)):
        block = pic[(i*8):(i*8)+8, (j*8):(j*8)+8]
        block *= Q
        pic[(i*8):(i*8)+8, (j*8):(j*8)+8] = idct2(block)





img.fromarray(pic.astype(np.int8), 'L').resize((np.array(pic.shape)*10)).show()