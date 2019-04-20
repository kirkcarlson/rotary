flash : rotary.bin
	particle flash rotary1 rotary.bin

rotary.bin : src/rotary.ino src/*.h
	particle compile photon . --saveTo rotary.bin

print : src/rotary.ino
	vim -c 'hardcopy > output.ps'  -c quit src/rotary.ino && ps2pdf output.ps >output.pdf
