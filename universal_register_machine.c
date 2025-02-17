#include <stdio.h>
#include <stdlib.h>

#include "godel_encoding.h"
#include "parser.h"
#include "program_types.h"
#include "register_machine.h"
#include "utility_functions.h"

#define REGISTER_ZERO registers[0]
#define REGISTER_PROGRAM registers[1]
#define REGISTER_ARGS registers[2]
#define REGISTER_PROGRAM_COUNTER registers[3]
#define REGISTER_N_INSTRBODY registers[4]
#define REGISTER_C_OPCODE registers[5]
#define REGISTER_R_SIM_REG_VALUE registers[6]
#define REGISTER_S_SCRATCH registers[7]
#define REGISTER_T_SCRATCH registers[8]
#define REGISTER_Z_SCRATCH registers[9]

#define NUM_INSTRS_IN_URM 119

void simulate_machine(unsigned long program_to_simulate,
                      unsigned long arguments) {
  unsigned long registers[10] = {0};
  REGISTER_PROGRAM = program_to_simulate,
  REGISTER_ARGS = arguments;  // Load in details of machine to sim.

  instruction_t instructions[NUM_INSTRS_IN_URM];
  PUSH_TO_LIST_MACRO(0, REGISTER_ZERO, REGISTER_ARGS)                  // +8.
  NON_DESTRUCTIVE_COPY_MACRO(8, REGISTER_PROGRAM, REGISTER_T_SCRATCH)  // +8.
  POP_FROM_LIST_MACRO(16, REGISTER_T_SCRATCH, REGISTER_N_INSTRBODY,
                      118)  // +11.
  instructions[27] = (instruction_t){DECREMENT_REGISTER_BRANCH,
                                     REGISTER_PROGRAM_COUNTER, 16, 28};
  POP_FROM_LIST_MACRO(28, REGISTER_N_INSTRBODY, REGISTER_C_OPCODE,
                      118)                                              // +11.
  POP_FROM_LIST_MACRO(39, REGISTER_ARGS, REGISTER_R_SIM_REG_VALUE, 50)  // +11.
  instructions[50] =
      (instruction_t){DECREMENT_REGISTER_BRANCH, REGISTER_C_OPCODE, 51, 104};
  instructions[51] =
      (instruction_t){DECREMENT_REGISTER_BRANCH, REGISTER_C_OPCODE, 52, 62};
  PUSH_TO_LIST_MACRO(52, REGISTER_R_SIM_REG_VALUE, REGISTER_S_SCRATCH)  // +8.
  // Need to create a loop here that jumps back to POPA (39), so INC then DEC to
  // jump without changing.
  instructions[60] =
      (instruction_t){INCREMENT_REGISTER, REGISTER_Z_SCRATCH, 61, 61};
  instructions[61] =
      (instruction_t){DECREMENT_REGISTER_BRANCH, REGISTER_Z_SCRATCH, 39, 39};
  // This is hmm2
  instructions[62] =
      (instruction_t){INCREMENT_REGISTER, REGISTER_N_INSTRBODY, 63, 63};
  POP_FROM_LIST_MACRO(63, REGISTER_N_INSTRBODY, REGISTER_PROGRAM_COUNTER,
                      74)  // +11. Exit no special case here.
  instructions[74] = (instruction_t){DECREMENT_REGISTER_BRANCH,
                                     REGISTER_R_SIM_REG_VALUE, 83, 75};
  NON_DESTRUCTIVE_COPY_MACRO(75, REGISTER_N_INSTRBODY,
                             REGISTER_PROGRAM_COUNTER)             // +8.
  PUSH_TO_LIST_MACRO(83, REGISTER_R_SIM_REG_VALUE, REGISTER_ARGS)  // +8.
  POP_FROM_LIST_MACRO(91, REGISTER_S_SCRATCH, REGISTER_R_SIM_REG_VALUE,
                      8)  // +11.
  // Another loop case to get back to push R to A.
  instructions[102] =
      (instruction_t){INCREMENT_REGISTER, REGISTER_Z_SCRATCH, 103, 103};
  instructions[103] =
      (instruction_t){DECREMENT_REGISTER_BRANCH, REGISTER_Z_SCRATCH, 83, 83};
  // This is a case then branch back to PC::=N for R+.
  instructions[104] =
      (instruction_t){INCREMENT_REGISTER, REGISTER_R_SIM_REG_VALUE, 105, 105};
  instructions[105] =
      (instruction_t){INCREMENT_REGISTER, REGISTER_Z_SCRATCH, 106, 106};
  instructions[106] =
      (instruction_t){DECREMENT_REGISTER_BRANCH, REGISTER_Z_SCRATCH, 75, 75};
  POP_FROM_LIST_MACRO(107, REGISTER_ARGS, REGISTER_ZERO, 118)  // +11.
  instructions[118] = (instruction_t){HALT, 999, 999, 999};

  program_t simulator = {instructions, NUM_INSTRS_IN_URM};
  interpret_instructions(simulator, registers);
  printf("The output of simulating the machine is %lu.\n", REGISTER_ZERO);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    perror(
        "Please supply <program.txt> and <arguments.txt> according to the "
        "formats in README.md.\n");
    return EXIT_FAILURE;
  }

  // program_t program = parse_program_from_txt_file(argv[1]);
  // if (program.instructions == NULL) {
  //   perror("Failed to parse the program txtfile.");
  //   return EXIT_FAILURE;
  // }
  // argument_list arguments = parse_arguments_from_txt_file(argv[2]);
  // if (arguments.contents == NULL) {
  //   perror("Failed to parse the arguments txtfile.");
  //   return EXIT_FAILURE;
  // }
  // unsigned long encoded_program = encode_program(program),
  //               encoded_arguments = encode_list(arguments);
  unsigned long encoded_program = strtol(argv[1], NULL, 10),
                encoded_arguments = strtol(argv[2], NULL, 10);
  simulate_machine(encoded_program, encoded_arguments);

  return EXIT_SUCCESS;
}
