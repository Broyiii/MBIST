import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import random
import numpy as np
import sys

def generate_random_hex(length):
    hex_digits = "0123456789ABCDEF"
    random_hex = ''.join(random.choice(hex_digits) for _ in range(length))
    return random_hex

def generate_shape(length):
    shape_type = "o*.sp834<>v^x+hP21H"
    random_shape = ''.join(random.choice(shape_type) for _ in range(length))
    return random_shape

def main(fileName: str):
    iris = pd.read_csv(fileName)
    Species = iris.contronler.unique()
    Hard = iris.hard.unique()

    Species_map: dict[str:int] = {}
    Hard_map: dict[str:int] = {}
    index = 0
    for ctrler in Species:
        Species_map[ctrler] = index
        index = index + 1
    index = 0
    for har in Hard:
        Hard_map[har] = index
        index = index + 1

    colors = list()
    markers = list()
    for i in range(len(Species)):
        ran = '#' + generate_random_hex(6)
        colors.append(ran)

    for j in range(len(Hard)):
        shape_ran = generate_shape(1)
        markers.append(shape_ran)

    #set color and shape
    for cellIndex in range(len(iris.hard)):
        i = Species_map[iris.contronler[cellIndex]]
        n = Hard_map[iris.hard[cellIndex]]
        # print(f"{i} : {n}")
        plt.scatter(iris.loc[iris.contronler == Species[i], 'UP'], iris.loc[iris.contronler == Species[i],'Down'],
                    s = 35, c = colors[i], label = Species[i],marker = markers[n])

    plt.title('Memory distribution')
    plt.xlabel('Up Bound')
    plt.ylabel('Down Bound')
    plt.show()

if __name__ == '__main__' :
    if (len(sys.argv) != 2):
        print("Usage:\n\tpython3 plt.py <input csv file>")
    else:
        print("Start M3CPlot !")
        main(sys.argv[1])