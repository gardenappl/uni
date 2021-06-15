import numpy as np
from PIL import Image
import sys


if len(sys.argv) <= 3:
    print("Usage: lab2.py [input] [output] [filterRadius]")
    exit(1)


# Configurable radius
filterRadius = int(sys.argv[3])

image = Image.open(sys.argv[1]).convert("L")
inputArr = np.asarray(image)

outputArr = np.empty_like(inputArr)

for y in range(inputArr.shape[1]):
    for x in range(inputArr.shape[0]):
        aperture = []
        for i in range(max(0, x-filterRadius), min(inputArr.shape[0], x+filterRadius+1)):
            aperture.append(inputArr[i, y])

        for i in range(max(0, y-filterRadius), min(inputArr.shape[1], y+filterRadius+1)):
            if i != y:
                aperture.append(inputArr[x, i])
        
        outputArr[x, y] = np.median(np.array(aperture))

Image.fromarray(outputArr, "L").save(sys.argv[2])
