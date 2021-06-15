import numpy as np
from PIL import Image
import sys


if len(sys.argv) <= 3:
    print("Usage: lab2.py [input] [output] [filterRadius]")
    exit(1)


filterRadius = int(sys.argv[3])

image = Image.open(sys.argv[1]).convert("L")
inputArr = np.asarray(image)

outputArr = np.copy(inputArr)

for y in range(inputArr.shape[1]):
    for x in range(inputArr.shape[0]):
        inputSlice = inputArr[max(0, x-filterRadius):min(inputArr.shape[0] - 1, x+filterRadius),
                              max(0, y-filterRadius):min(inputArr.shape[1] - 1, y+filterRadius)]
        outputArr[x, y] = np.median(inputSlice)

outputImage = Image.fromarray(outputArr, "L")
outputImage.show()
outputImage.save(sys.argv[2])
