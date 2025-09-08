# Universal Register Machine

This is a C implementation I build as an extension to the [CST IB Computation Theory course](https://www.cl.cam.ac.uk/teaching/2425/CompTheory/) to test my understanding.
Some of the design decisions are unconventional, such as the extensive use of macros in `universal_register_machine.c`, as I was trying to test my understanding of the lambda calculus whilst writing this.

## Usage

First, build with `make build` and then run `./build/universal_register_machine [program] [arguments]`.
The program is provided as a list of instructions and the arguments as a list of integers for the initial values of the registers corresponding to the list index (starting from index 1).
These lists are represented as integers using the simple bijection $[] := 0$, and $h::t := 2^h(2t+1)$.
The instructions are defined as HALT $:= 0$, increment register $i$ and jump to location $j$ $:= 2^{2i}(2j+1)$, and decrementing register $i$ and conditionally jumping to either register $j$ or $k$ depending on if $i$ previously held the value zero is $:= 2^{2i + 1}(2(2^j(2k + 1) - 1) + 1)$.
