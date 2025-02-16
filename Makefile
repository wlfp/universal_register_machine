universal_register_machine: universal_register_machine.c register_machine.c parser.c godel_encoding.c
	clang parser.c -c -Wall -pedantic -std=c17
	clang register_machine.c -c -Wall -pedantic -std=c17
	clang godel_encoding.c -c -Wall -pedantic -std=c17
	clang universal_register_machine.c -o build/universal_register_machine -Wall -pedantic -std=c17 parser.o register_machine.o godel_encoding.o
