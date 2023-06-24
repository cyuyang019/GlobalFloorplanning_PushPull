make clean

make

echo "run" $1

./pprun $1 output.txt $2

echo "draw the result of" $1

python draw_circle_layout.py output.txt output.png