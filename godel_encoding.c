#include "godel_encoding.h"

unsigned long double_angle_encoding(const unsigned long x,
                                    const unsigned long y) {
  return (1 << x) * (2 * y + 1);
}

unsigned long single_angle_encoding(const unsigned long x,
                                    const unsigned long y) {
  return double_angle_encoding(x, y) - 1;
}

unsigned long encode_list(const argument_list list) {
  unsigned long encoded_list = 0;
  for (unsigned short i = 0; i < list.length; i++) {
    encoded_list = double_angle_encoding(list.contents[i], encoded_list);
  }
  return encoded_list;
}

/*
  A program is just a list of instructions, that have their own encoding.
  As such, we simply switch on OPCODE to determine the encoded instruction
  value, and then use a list encoding in the standard way.
*/
unsigned long encode_program(const program_t program) {
  unsigned long encoded_program = 0;
  unsigned long encoded_instruction;
  for (unsigned short i; i < program.num_instructions; i++) {
    switch (program.instructions[i].opcode) {
      case INCREMENT_REGISTER:
        encoded_instruction =
            double_angle_encoding(2 * program.instructions[i].register_index,
                                  program.instructions[i].label1_index);
        break;
      case DECREMENT_REGISTER_BRANCH:
        encoded_instruction = double_angle_encoding(
            2 * program.instructions[i].register_index + 1,
            single_angle_encoding(program.instructions[i].label1_index,
                                  program.instructions[i].label2_index));
        break;
      case HALT:
        encoded_instruction = 0;
        break;
    }
    encoded_program =
        double_angle_encoding(encoded_instruction, encoded_program);
  }
  return encoded_program;
}
