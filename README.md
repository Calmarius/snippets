# snippets

Small reusable code snippets without dependencies.

## Rules

- All snippets must be self contained and must contain the fewest  algorithms reasonable.
- They are written in C and must be compilable using a freestanding compiler. So no standard library use, except in unit tests.
- If the algorithm has dependencies (like memory allocation or I/O), these dependencies should be provided as a parameter.
- All algorithms should contain a main function which is used for unit testing using assert statements.
- Structs should be avoided to avoid binding a certain naming scheme. Instead of structs, use macros to refer to a field (see binarytree.h)
