#include "godel_encoding.h"

unsigned long encode_list(const argument_list list) {
  unsigned long coded_list = 0;
  for (unsigned short i = 0; i < list.length; i++) {
    unsigned int value_to_push = list.contents[i];
    coded_list = (1 << value_to_push) * (2 * coded_list + 1);
  }
  return coded_list;
}

/*
  A program is just a list of instructions, that have their own encoding.
  As such, we simply switch on OPCODE to determine the encoded instruction
  value, and then use a list encoding in the standard way.
*/
unsigned long encode_program(const program_t program) {
  unsigned long encoded_program = 0;
  unsigned long encoded_instruction = 0;
  for (unsigned short i; i < program.num_instructions; i++) {
    switch (program.instructions[i].opcode) {
      case INCREMENT_REGISTER:
        encoded_instruction =
            (1 << (2 * program.instructions[i].register_index)) *
            (2 * program.instructions[i].label1_index + 1);
        break;
      case DECREMENT_REGISTER_BRANCH:
        encoded_instruction =
            (1 << (2 * program.instructions[i].register_index + 1)) *
            ((1 << program.instructions[i].label1_index) *
                 (2 * program.instructions[i].label2_index + 1) -
             1);
        break;
      case HALT:
        encoded_instruction = 0;
        break;
    }
    encoded_program =
        (1 << encoded_instruction) * (2 * encoded_instruction + 1);
  }
  return encoded_program;
}
