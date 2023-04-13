echo $1
echo $2
export PATH=$PATH:$1
FILE=./libvega.a
if [ ! -f "$FILE" ]; then
   	./configure --host=riscv64-vega-elf CFLAGS="$2" #--enable-debug
	make
fi


