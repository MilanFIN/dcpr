"""
simple helper script to create lookup tables 

Adapted from a script made by Ethan Crooks: https://www.ethancrooks.com/blog/gba3
"""

# TODO: ADD LUT's for other predefined values as well

import math, os

fixed_point_size = 8


def reciprocal(index):
    divisor = index - 350
    if (divisor == 0):
        return 256
    return  1 / ((index - 350) / 256)

def cameraX(index):
    f = 2*index / 120 - 1
    return f

def textureStep(index):
    if (index == 0):
        return 16
    return 16 / index

def addReciprocal(f):
    count = 700

    table_name = "RECIPROCALLUT"

    f.write(f"const FIXED {table_name}[{count}] = {{\n")

    column_count = 0

    for i in range(0, count):
        calc = reciprocal(i)

        fixedpoint = int(calc * 256)

        if column_count == 0:
            f.write("    ")

        f.write(f"{hex(fixedpoint)}, ")

        if column_count == 7:
            column_count = 0
            f.write("\n")
        else:
            column_count+=1

    f.write("};\n\n") 

def addCameraX(f):
    count = 120

    table_name = "CAMERAXLUT"

    f.write(f"const FIXED {table_name}[{count}] = {{\n")
    column_count = 0

    for i in range(0, count):
        calc = cameraX(i)
        fixedpoint = int(calc * 256)

        if column_count == 0:
            f.write("    ")

        f.write(f"{hex(fixedpoint)}, ")

        if column_count == 7:
            column_count = 0
            f.write("\n")
        else:
            column_count+=1
    f.write("};\n\n") 

    
def addTextureStepLu(f):
    count = 160

    table_name = "TEXTURESTEPLUT"

    f.write(f"const FIXED {table_name}[{count}] = {{\n")
    column_count = 0

    for i in range(0, count):
        calc = textureStep(i)
        fixedpoint = int(calc * 256)

        if column_count == 0:
            f.write("    ")

        f.write(f"{hex(fixedpoint)}, ")

        if column_count == 7:
            column_count = 0
            f.write("\n")
        else:
            column_count+=1
    f.write("};\n\n") 


output_filename = "out.c"

os.remove(output_filename)

with open(output_filename, "a") as f:
    addReciprocal(f)
    addCameraX(f)
    addTextureStepLu(f)


