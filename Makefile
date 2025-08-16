CFLAGS = -Wall -pedantic -std=c17

universal_register_machine: universal_register_machine.c register_machine.c godel_encoding.c
	clang register_machine.c -c $(CFLAGS) -o build/register_machine.o
	clang godel_encoding.c -c $(CFLAGS) -o build/godel_encoding.o
	clang universal_register_machine.c -o build/universal_register_machine $(CFLAGS) build/register_machine.o build/godel_encoding.o

debug: universal_register_machine.c register_machine.c godel_encoding.c
	clang register_machine.c -c $(CFLAGS) -g -DDEBUG -o build/register_machine.o
	clang godel_encoding.c -c $(CFLAGS) -g -DDEBUG -o build/godel_encoding.o
	clang universal_register_machine.c -o build/universal_register_machine_debug $(CFLAGS) -g -DDEBUG build/register_machine.o build/godel_encoding.o

clean:
	rm -f build/*.o build/universal_register_machine build/universal_register_machine_debug
	rm -rf build/universal_register_machine.dYSM

.PHONY: debug clean
