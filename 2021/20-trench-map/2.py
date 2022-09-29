import numpy as np
from scipy.ndimage import convolve
from functools import cache
from datetime import datetime as dt

start = dt.now()
# Load data
with open('input.txt') as f:
    lines = [l.strip() for l in f.readlines()]

algo = tuple([1 if c == '#' else 0 for c in lines[0]])
img_arr = np.array([[1 if c=='#' else 0 for c in l] for l in lines[2:]])

# Create the vectorized + cached decode function
@np.vectorize
@cache
def decode(num): return algo[num]

# Here we create a convolution filter that maps bit values to each pixel
filter = np.array([2**i for i in range(9)]).reshape(3, 3)

# Padding the array to max expected size
img_filtered = np.pad(img_arr, 50)
# Iterating
for i in range(1, 51):
    img_filtered = decode(convolve(img_filtered, filter))
    if i in [2, 50]:
        print(f'iter {i:02,}; lit pixels: {img_filtered.sum().sum():6,} secs')

print(f'Total run time: {(dt.now() - start).total_seconds():.3f}')