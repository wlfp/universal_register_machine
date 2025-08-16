#include <stdio.h>
#include <stdlib.h>

#include "godel_encoding.h"
#include "program_types.h"
#include "register_machine.h"
#include "utility_functions.h"

#define REGISTER_ZERO 0
#define REGISTER_PROGRAM 1
#define REGISTER_ARGS 2
#define REGISTER_PROGRAM_COUNTER 3
#define REGISTER_N_INSTRBODY 4
#define REGISTER_C_OPCODE 5
#define REGISTER_R_SIM_REG_VALUE 6
#define REGISTER_S_SCRATCH 7
#define REGISTER_T_SCRATCH 8
#define REGISTER_Z_SCRATCH 9

#define NUM_INSTRS_IN_URM 108

/*
 * Takes a Godel encoded program and arguments and simulates them according to
 * the URM I designed. This URM is built of mini programs that I built as macros
 * so that I could see the full instruction list of the program in one go, as it
 * were.
 */
void simulate_machine(unsigned long program_to_simulate,
                      unsigned long arguments) {
  unsigned long registers[10] = {0};
  registers[REGISTER_PROGRAM] = program_to_simulate,
  registers[REGISTER_ARGS] = arguments;  // Load in details of machine to sim.

  instruction_t instructions[NUM_INSTRS_IN_URM];
  PUSH_TO_LIST_MACRO(0, REGISTER_ZERO, REGISTER_ARGS)                  // +7.
  NON_DESTRUCTIVE_COPY_MACRO(7, REGISTER_PROGRAM, REGISTER_T_SCRATCH)  // +7.
  POP_FROM_LIST_MACRO(14, REGISTER_T_SCRATCH, REGISTER_N_INSTRBODY,
                      97)  // +10.
  instructions[24] = (instruction_t){DECREMENT_REGISTER_BRANCH,
                                     REGISTER_PROGRAM_COUNTER, 14, 25};
  POP_FROM_LIST_MACRO(25, REGISTER_N_INSTRBODY, REGISTER_C_OPCODE,
                      97)                                               // +10.
  POP_FROM_LIST_MACRO(35, REGISTER_ARGS, REGISTER_R_SIM_REG_VALUE, 45)  // +10.
  instructions[45] =
      (instruction_t){DECREMENT_REGISTER_BRANCH, REGISTER_C_OPCODE, 46, 94};
  instructions[46] =
      (instruction_t){DECREMENT_REGISTER_BRANCH, REGISTER_C_OPCODE, 47, 56};
  PUSH_TO_LIST_MACRO(47, REGISTER_R_SIM_REG_VALUE, REGISTER_S_SCRATCH)  // +7.
  // Need to create a loop here that jumps back to POPA (35), so INC then DEC to
  // jump without changing.
  instructions[54] =
      (instruction_t){INCREMENT_REGISTER, REGISTER_Z_SCRATCH, 55, 55};
  instructions[55] =
      (instruction_t){DECREMENT_REGISTER_BRANCH, REGISTER_Z_SCRATCH, 35, 35};
  instructions[56] =
      (instruction_t){INCREMENT_REGISTER, REGISTER_N_INSTRBODY, 57, 57};
  POP_FROM_LIST_MACRO(57, REGISTER_N_INSTRBODY, REGISTER_PROGRAM_COUNTER,
                      67)  // +10. Exit no special case here.
  instructions[67] = (instruction_t){DECREMENT_REGISTER_BRANCH,
                                     REGISTER_R_SIM_REG_VALUE, 75, 68};
  NON_DESTRUCTIVE_COPY_MACRO(68, REGISTER_N_INSTRBODY,
                             REGISTER_PROGRAM_COUNTER)             // +7.
  PUSH_TO_LIST_MACRO(75, REGISTER_R_SIM_REG_VALUE, REGISTER_ARGS)  // +7.
  POP_FROM_LIST_MACRO(82, REGISTER_S_SCRATCH, REGISTER_R_SIM_REG_VALUE,
                      7)  // +10.
  // Another loop case to get back to push R to A.
  instructions[92] =
      (instruction_t){INCREMENT_REGISTER, REGISTER_Z_SCRATCH, 93, 93};
  instructions[93] =
      (instruction_t){DECREMENT_REGISTER_BRANCH, REGISTER_Z_SCRATCH, 75, 75};
  // This is a case then branch back to PC::=N for R+.
  instructions[94] =
      (instruction_t){INCREMENT_REGISTER, REGISTER_R_SIM_REG_VALUE, 95, 95};
  instructions[95] =
      (instruction_t){INCREMENT_REGISTER, REGISTER_Z_SCRATCH, 96, 96};
  instructions[96] =
      (instruction_t){DECREMENT_REGISTER_BRANCH, REGISTER_Z_SCRATCH, 68, 68};
  POP_FROM_LIST_MACRO(97, REGISTER_ARGS, REGISTER_ZERO, 107)  // +10.
  instructions[107] = (instruction_t){HALT, 107, 107, 107};

  program_t simulator = {instructions, NUM_INSTRS_IN_URM};
  interpret_instructions(simulator, registers);
  printf("The output of simulating the machine is %lu.\n",
         registers[REGISTER_ZERO]);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    perror("Please supply a Godel encoded program and arguments list.\n");
    return EXIT_FAILURE;
  }

  unsigned long encoded_program = strtol(argv[1], NULL, 10),
                encoded_arguments = strtol(argv[2], NULL, 10);
  simulate_machine(encoded_program, encoded_arguments);

  return EXIT_SUCCESS;
}
