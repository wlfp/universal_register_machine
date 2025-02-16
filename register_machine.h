#include "program_types.h"

/*
  Modifies the registers in-place according to the given instructions.
*/
void interpret_instructions(const program_t program,
                            unsigned long *const registers);
