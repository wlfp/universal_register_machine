universal_register_machine: universal_register_machine.c register_machine.c parser.c godel_encoding.c
	clang parser.c -c -Wall -pedantic -std=c17 -o build/parser.o
	clang register_machine.c -c -Wall -pedantic -std=c17 -o build/register_machine.o
	clang godel_encoding.c -c -Wall -pedantic -std=c17 -o build/godel_encoding.o
	clang universal_register_machine.c -o build/universal_register_machine -g -Wall -pedantic -std=c17 build/parser.o build/register_machine.o build/godel_encoding.o
