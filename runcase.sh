#!/bin/bash

make

txtFile=$1'.txt'
pngFile=$1'.png'

echo "run ${txtFile}"

./pprun "${txtFile}" output.txt

echo "draw the result to ${pngFile}"

python draw_circle_layout.py output.txt "${pngFile}"