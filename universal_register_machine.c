#include <stdio.h>
#include <stdlib.h>

#include "godel_encoding.h"
#include "parser.h"
#include "program_types.h"
#include "register_machine.h"

/*
  Remember that, although not in the variable name, these are the indices for
  the registers, not the registers themselves.
  This change makes the code more human readable when writing programs in this
  way.
*/
void non_destructive_copy(const unsigned int source_register,
                          const unsigned int destination_index,
                          const unsigned int temporary_register,
                          unsigned long *const register_file) {
  instruction instructions[8];
  program_t program = {instructions, 8};
  instructions[0] =
      (instruction){DECREMENT_REGISTER_BRANCH, temporary_register, 0, 1},
  instructions[1] =
      (instruction){DECREMENT_REGISTER_BRANCH, destination_index, 1, 2},
  instructions[2] =
      (instruction){DECREMENT_REGISTER_BRANCH, source_register, 3, 5},
  instructions[3] =
      (instruction){INCREMENT_REGISTER, destination_index, 4, 999},
  instructions[4] =
      (instruction){INCREMENT_REGISTER, temporary_register, 2, 999},
  instructions[5] =
      (instruction){DECREMENT_REGISTER_BRANCH, temporary_register, 6, 7},
  instructions[6] = (instruction){INCREMENT_REGISTER, source_register, 5, 999},
  instructions[7] = (instruction){HALT, 999, 999, 999};
  interpret_instructions(program, register_file);
  return;
}

void push_to_list(const unsigned int item_register,
                  const unsigned int list_register,
                  const unsigned int temporary_register,
                  unsigned long *const register_file) {
  instruction instructions[8];
  program_t program = {instructions, 8};
  instructions[0] =
      (instruction){DECREMENT_REGISTER_BRANCH, temporary_register, 0, 1},
  instructions[1] =
      (instruction){INCREMENT_REGISTER, temporary_register, 2, 999},
  instructions[2] =
      (instruction){DECREMENT_REGISTER_BRANCH, list_register, 3, 4},
  instructions[3] =
      (instruction){INCREMENT_REGISTER, temporary_register, 1, 999},
  instructions[4] =
      (instruction){DECREMENT_REGISTER_BRANCH, temporary_register, 5, 6},
  instructions[5] = (instruction){INCREMENT_REGISTER, list_register, 4, 999},
  instructions[6] =
      (instruction){DECREMENT_REGISTER_BRANCH, item_register, 2, 7},
  instructions[7] = (instruction){HALT, 999, 999, 999};
}

void pop_from_list(const unsigned int list_register,
                   const unsigned int destination_register,
                   const unsigned int temporary_register,
                   unsigned long *const register_file) {
  instruction instructions[11];
  program_t program = {instructions, 11};
  instructions[0] =
      (instruction){DECREMENT_REGISTER_BRANCH, destination_register, 0,
                    1},  // Zero the dest.
      instructions[1] =
          (instruction){DECREMENT_REGISTER_BRANCH, temporary_register, 1,
                        2},  // Zero the scratch.
      instructions[2] =
          (instruction){DECREMENT_REGISTER_BRANCH, list_register, 3,
                        10},  // This is the case of popping from an empty list,
                              // could add a warning here.
      instructions[3] =
          (instruction){INCREMENT_REGISTER, list_register, 4, 999},
  instructions[4] =
      (instruction){DECREMENT_REGISTER_BRANCH, list_register, 5, 6},
  instructions[5] =
      (instruction){INCREMENT_REGISTER, temporary_register, 4, 999},
  instructions[6] =
      (instruction){DECREMENT_REGISTER_BRANCH, temporary_register, 7, 9},
  instructions[7] =
      (instruction){DECREMENT_REGISTER_BRANCH, temporary_register, 8, 10},
  instructions[8] = (instruction){INCREMENT_REGISTER, list_register, 9, 999},
  instructions[9] =
      (instruction){INCREMENT_REGISTER, destination_register, 4, 999},
  instructions[10] = (instruction){HALT, 999, 999, 999};
  return;
}

unsigned long simulate_machine(unsigned long program, unsigned long arguments) {
  unsigned long *const registers = calloc(10, sizeof(unsigned long));
  if (registers == NULL) {
    perror(
        "Calloc failed to allocate space for the universal machine's "
        "registers.");
    return EXIT_FAILURE;
  }
  registers[1] = program,
  registers[2] = arguments;  // Load in details of machine to sim.

  printf("The output of the machine is %lu", registers[0]);
  free(registers);
  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    perror(
        "Please supply <program.txt> and <arguments.txt> according to the "
        "formats in README.md.");
    return EXIT_FAILURE;
  }

  program_t program = parse_program_from_txt_file(argv[1]);
  argument_list arguments = parse_arguments_from_txt_file(argv[2]);
  unsigned long encoded_program = encode_program(program),
                encoded_arguments = encode_list(arguments);
  simulate_machine(encoded_program, encoded_arguments);

  return EXIT_SUCCESS;
}
