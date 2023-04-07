# Module name                           :  textToImage.py
# Name of Author                        :  Anoop Varghese, C-DAC
# Email ID  (Report any bugs)           :  vanoop@cdac.in
# Module Description                    :  Python3 script to convert raw image data in text file to image.

#  Usage : python3 textToImage.py (provided img.txt contains valid Grascale image data).

import numpy as np
import matplotlib.pyplot as plt
from ctypes import *

rows = 144
cols = 174
imageFile = "img.txt"
colorData = []
pixelArray = []
with open(imageFile, 'r') as file:
    for line in file:
        colorData = [int(x) for x in line.split()]
        length = len(colorData)
        x = 0
        while(x < length):
            pixelArray.append(colorData[x]) #red
            pixelArray.append(colorData[x]) #green
            pixelArray.append(colorData[x]) #blue
            x = x + 1
imgArray = np.asarray(pixelArray)
rgbArray = imgArray.reshape(rows, cols, 3)
imgplot = plt.imshow(rgbArray)
plt.savefig('OV7670_Grayscale.jpg')
plt.show()

