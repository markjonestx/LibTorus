# Torus Data Type
A C++ data type influnced by doubly-linked lists. This data type is best
described as a Torus, where both the 'head' and 'tail' of the
linked list are tied to each other, producing a perfect circle, as well as
the head and tail of each column, forming a tube that is effectively connected
on the ends- like a Torus!

## Goal of this project
Ultimately, the goal of this datatype is to expand my skills in C++. Most
courses at my University use C++ as their language of choice, but none of
the true engineering course do. This means that I have to expand on my skills
as a C++ developer on my own time. This project is an attempt at me to do so,
or at least initially.

## Functions the Torus has
Currently, the Torus is an abstract data type functions much like a linked
list. The data type is reference counted, and supports being copied from
one variable to another, as well as compared to see if the Torus's match.

The software is built using Meson, tested using Catch2, and documented using
Doxygen.

## Further work that is required
Though the basic scaffolding is in place, there is still much more work to do.
As it stands right now, the unit testing only exists enough to determine if
Catch2 would be viable, much more unit testing is required. Also, there are
several features that I plan to impliment into the data type as well:

- Move support
- Iteration over all elements
- Iteration over a single row
- Iteration over a single column
- Adding and removing rows
- Adding and removing columns
- A more efficient `seek()` function

## Requirements

- Meson to build the project
- Ninja for the build engine
- Catch2 for Unit Testing
- a C++20 capable compiler
- Doxygen to produce the documentation


## Building and Testing
To build the project, run:

```
meson build
```

Which will produce a build folder named `build`, much like CMake if you are
familar with that build generator.

Next, change directories into the new build folder and run the following
command to produce the testing executable:

```
ninja
```

Last, to run the Unit Tests, run:

```
ninja test
```

## Generating Documentation with Doxygen
To generate the documentation for this project, produce the `build` folder
using the command above, change directory to the `build` directory, and
then run the following:

```
ninja docs
```

This will produce the Doxygen documentation in `docs` under the `build`
directory, and will produce both LaTeX files, as well as HTML that can be
browsed in your browser, or uploaded to a web server.
