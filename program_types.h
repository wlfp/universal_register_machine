#ifndef PROGRAM_TYPES_H
#define PROGRAM_TYPES_H

/*
  Used to define instruction types, following convention that increments are
  even and decrements are odd, with HALT as zero.
*/
typedef enum OPCODE {
  HALT = 0,
  DECREMENT_REGISTER_BRANCH,
  INCREMENT_REGISTER
} OPCODE;

/*
  A struct for register machine instructions.
  Unused labels on instructions will be ignored.
*/
typedef struct instruction {
  OPCODE opcode;
  unsigned int register_index;
  unsigned int label1_index;
  unsigned int label2_index;
} instruction;

typedef struct program {
  instruction *instructions;
  unsigned int num_instructions;
} program_t;

typedef struct argument_list {
  unsigned long *contents;
  unsigned short length;
} argument_list;

#endif
