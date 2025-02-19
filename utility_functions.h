#define LEN_NON_DESTRUCTIVE_COPY 7
#define LEN_PUSH_TO_LIST 7
#define LEN_POP_FROM_LIST 10
#define TEMP_REG REGISTER_Z_SCRATCH
#define DUMMY_REG 999

/*
  PRECONDITION: src_reg = a natural number x, dest_reg = a natural number y.
  POSTCONDITION: src_reg = x, dest_reg = x.
*/
#define NON_DESTRUCTIVE_COPY_MACRO(base, src_reg, dest_reg)                    \
  instructions[(base) + 0] = (instruction_t){                                  \
      DECREMENT_REGISTER_BRANCH, TEMP_REG, (base) + 0, (base) + 1};            \
  instructions[(base) + 1] = (instruction_t){                                  \
      DECREMENT_REGISTER_BRANCH, dest_reg, (base) + 1, (base) + 2};            \
  instructions[(base) + 2] = (instruction_t){DECREMENT_REGISTER_BRANCH,        \
                                             src_reg, (base) + 3, (base) + 5}; \
  instructions[(base) + 3] =                                                   \
      (instruction_t){INCREMENT_REGISTER, dest_reg, (base) + 4, DUMMY_REG};    \
  instructions[(base) + 4] =                                                   \
      (instruction_t){INCREMENT_REGISTER, TEMP_REG, (base) + 2, DUMMY_REG};    \
  instructions[(base) + 5] = (instruction_t){                                  \
      DECREMENT_REGISTER_BRANCH, TEMP_REG, (base) + 6, (base) + 7};            \
  instructions[(base) + 6] =                                                   \
      (instruction_t){INCREMENT_REGISTER, src_reg, (base) + 5, DUMMY_REG};

/*
  PRECONDITION: item_reg = a natural number x, list_reg = an encoded list.
  POSTCONDITION: item_reg = 0, list_reg = <<x, encoded_list>>.
*/
#define PUSH_TO_LIST_MACRO(base, item_reg, list_reg)                        \
  instructions[(base) + 0] = (instruction_t){                               \
      DECREMENT_REGISTER_BRANCH, TEMP_REG, (base) + 0, (base) + 1};         \
  instructions[(base) + 1] =                                                \
      (instruction_t){INCREMENT_REGISTER, TEMP_REG, (base) + 2, DUMMY_REG}; \
  instructions[(base) + 2] = (instruction_t){                               \
      DECREMENT_REGISTER_BRANCH, list_reg, (base) + 3, (base) + 4};         \
  instructions[(base) + 3] =                                                \
      (instruction_t){INCREMENT_REGISTER, TEMP_REG, (base) + 1, DUMMY_REG}; \
  instructions[(base) + 4] = (instruction_t){                               \
      DECREMENT_REGISTER_BRANCH, TEMP_REG, (base) + 5, (base) + 6};         \
  instructions[(base) + 5] =                                                \
      (instruction_t){INCREMENT_REGISTER, list_reg, (base) + 4, DUMMY_REG}; \
  instructions[(base) + 6] = (instruction_t){                               \
      DECREMENT_REGISTER_BRANCH, item_reg, (base) + 2, (base) + 7};

/*
  PRECONDITION: list_reg empty (jump to exit_dest) or encodedlist <<x, l>>.
  POSTCONDITION: list_reg = the encoded list l, dest_reg = x.
*/
#define POP_FROM_LIST_MACRO(base, list_reg, dest_reg, exit_dest)               \
  instructions[(base) + 0] = (instruction_t){                                  \
      DECREMENT_REGISTER_BRANCH, dest_reg, (base) + 0, (base) + 1};            \
  instructions[(base) + 1] = (instruction_t){                                  \
      DECREMENT_REGISTER_BRANCH, TEMP_REG, (base) + 1, (base) + 2};            \
  instructions[(base) + 2] = (instruction_t){DECREMENT_REGISTER_BRANCH,        \
                                             list_reg, (base) + 3, exit_dest}; \
  instructions[(base) + 3] =                                                   \
      (instruction_t){INCREMENT_REGISTER, list_reg, (base) + 4, DUMMY_REG};    \
  instructions[(base) + 4] = (instruction_t){                                  \
      DECREMENT_REGISTER_BRANCH, list_reg, (base) + 5, (base) + 6};            \
  instructions[(base) + 5] =                                                   \
      (instruction_t){INCREMENT_REGISTER, TEMP_REG, (base) + 4, DUMMY_REG};    \
  instructions[(base) + 6] = (instruction_t){                                  \
      DECREMENT_REGISTER_BRANCH, TEMP_REG, (base) + 7, (base) + 9};            \
  instructions[(base) + 7] = (instruction_t){                                  \
      DECREMENT_REGISTER_BRANCH, TEMP_REG, (base) + 8, (base) + 10};           \
  instructions[(base) + 8] =                                                   \
      (instruction_t){INCREMENT_REGISTER, list_reg, (base) + 9, DUMMY_REG};    \
  instructions[(base) + 9] =                                                   \
      (instruction_t){INCREMENT_REGISTER, dest_reg, (base) + 4, DUMMY_REG};
