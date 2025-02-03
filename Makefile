#
#   FILE          : Makefile
#   PROJECT       : encodeInput - A2
#   PROGRAMMER    : Valentyn, Juan Jose, Warren
#   FIRST VERSION : 01/31/2025
#   DESCRIPTION   :
#      TTis is the Makefile which puts all the source codes together
#		and compile it into the executable file
#

# FINAL BINARY Target
./bin/encodeInput : ./obj/encodeInput.o ./obj/SRecord.o ./obj/Assembly.o
	cc ./obj/encodeInput.o ./obj/SRecord.o ./obj/Assembly.o -o ./bin/encodeInput

# =======================================================
#                     Dependencies
# =======================================================
./obj/encodeInput.o : ./src/encodeInput.c ./inc/encodeInput.h
	cc -c ./src/encodeInput.c -o ./obj/encodeInput.o 

./obj/SRecord.o : ./src/SRecord.c ./inc/encodeInput.h
	cc -c ./src/SRecord.c -o ./obj/SRecord.o 

./obj/Assembly.o : ./src/Assembly.c ./inc/encodeInput.h
	cc -c ./src/Assembly.c -o ./obj/Assembly.o 

# =======================================================
# Other targets
# =======================================================
all : ./bin/encodeInput

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o