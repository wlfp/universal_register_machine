#include "register_machine.h"

unsigned int interpret(const program_t program,
                       unsigned int *restrict const registers) {
  unsigned int current_instruction = 0;
  while (current_instruction < program.num_instructions) {
    switch (program.instructions[current_instruction].opcode) {
      case INCREMENT_REGISTER:
        registers[program.instructions[current_instruction].register_index] +=
            1;
        current_instruction =
            program.instructions[current_instruction].label1_index;
        break;
      case DECREMENT_REGISTER_BRANCH:
        if (registers[program.instructions[current_instruction]
                          .register_index] == 0) {
          current_instruction =
              program.instructions[current_instruction].label2_index;
        } else {
          registers[program.instructions[current_instruction].register_index] -=
              1;
          current_instruction =
              program.instructions[current_instruction].label1_index;
        }
        break;
      case HALT:
        return registers[0];
        break;
    }
  }
  return registers[0];  // Erroneous halts are caught here.
};
