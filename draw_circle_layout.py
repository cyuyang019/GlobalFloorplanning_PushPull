import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import math
import time


def draw_block(ax, x, y, width, height):
    color = "#BBB"
    ax.add_patch(
        patches.Rectangle(
            (x, y),
            width,
            height,
            fill=True,
            edgecolor="#000",
            facecolor=color,
            alpha=1.0  # 0.3 original
        )
    )


def draw_circle(ax, x, y, radius):
    color = "#FCC"
    ax.add_patch(
        patches.Circle(
            (x, y),
            radius,
            fill=True,
            edgecolor="#000",
            facecolor=color,
            alpha=1.0  # 0.3 original
        )
    )


png_size = (16, 12)
txt_name = sys.argv[1]
png_name = sys.argv[2]
fread = open(txt_name, 'r')
f = fread.read().split("\n")


total_block_number = int(f[0].split(" ")[1])
total_connection_number = int(f[0].split(" ")[3])
window_width = int(f[1].split(" ")[0])
window_height = int(f[1].split(" ")[1])

fig = plt.figure(figsize=png_size)

ax = fig.add_subplot(111)
ax.set_xbound(0, window_width)
ax.set_ybound(0, window_height)

i = 2

ax.add_patch(
    patches.Rectangle(
        (0, 0),
        window_width,
        window_height,
        fill=False,
        edgecolor="#000",
        facecolor="#FFF",
        alpha=1.0  # 0.3 original
    )
)

name2pos = {}

for block in range(total_block_number):
    ss = f[i].split(" ")
    if ss[1] == "SOFT":
        draw_circle(ax, float(ss[2]), float(ss[3]), float(ss[4]))
        plt.text(float(ss[2])-20, float(ss[3])-20, ss[0])
        name2pos[ss[0]] = (float(ss[2]), float(ss[3]))
    else:
        draw_block(ax, float(ss[2]), float(ss[3]),
                   float(ss[4]), float(ss[5]))
        plt.text(float(ss[2])+float(ss[4])/2-20,
                 float(ss[3])+float(ss[5])/2-20, ss[0])
        name2pos[ss[0]] = (float(ss[2])+float(ss[4])/2,
                           float(ss[3])+float(ss[5])/2)
    i += 1

j = i
max_value = 1
min_value = 1e10
for connection in range(total_connection_number):
    ss = f[j].split(" ")
    value = int(ss[2])
    if value > max_value:
        max_value = value
    if value < min_value:
        min_value = value
    j += 1

for connection in range(total_connection_number):
    ss = f[i].split(" ")
    x_values = [name2pos[ss[0]][0], name2pos[ss[1]][0]]
    y_values = [name2pos[ss[0]][1], name2pos[ss[1]][1]]
    value = float(ss[2])
    width = (value - min_value) / (max_value - min_value) * 14 + 1
    plt.plot(x_values, y_values, color="blue",
             linestyle="-", linewidth=width, alpha=0.5)
    i += 1

# plt.savefig(str(sys.argv[1])[:-4]+".png")

plt.savefig(png_name)
