as86 -0 -a -o $1.o $1.s
ld86 -0 -s -o $1 $1.o
dd bs=1 if=$1 of=Image skip=32
cp ./Image ../Image

