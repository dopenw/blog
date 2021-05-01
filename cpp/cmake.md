# CMake

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [CMake](#cmake)
  - [CMake Tutorial](#cmake-tutorial)
    - [Introduction](#introduction)
    - [A Basic Starting Point (Step 1)](#a-basic-starting-point-step-1)
      - [Adding a Version Number and Configured Header File](#adding-a-version-number-and-configured-header-file)
      - [Specify the C++ Standard](#specify-the-c-standard)
      - [Build and Test](#build-and-test)
      - [Full code](#full-code)
    - [Adding a Library (Step 2)](#adding-a-library-step-2)
      - [Full code](#full-code-1)
    - [Adding Usage Requirements for Library (Step 3)](#adding-usage-requirements-for-library-step-3)
      - [Full code](#full-code-2)
    - [Installing and Testing (Step 4)](#installing-and-testing-step-4)
      - [Install Rules](#install-rules)
      - [Testing Support](#testing-support)
      - [Full code](#full-code-3)
    - [Adding System Introspection (Step 5)](#adding-system-introspection-step-5)
      - [Full code](#full-code-4)

<!-- /code_chunk_output -->

注：该文档是参考 CMake release 3.20.2 相关文档而写的学习记录，官方链接 [cmake-tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)

## CMake Tutorial

### Introduction

The CMake tutorial provides a step-by-step guide that covers common build system issues that CMake helps address. The tutorial documentation and source code for examples can be found in the `Help/guide/tutorial` directory of the CMake source code tree.

### A Basic Starting Point (Step 1)

The most basic project is an executable built from source code files. For simple projects, a three line `CMakeLists.txt` file is all that is required. This will be the starting point for our tutorial. Create a `CMakeLists.txt` file in the `Step1` directory that looks like:

```cmake
cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial)

# add the executable
add_executable(Tutorial tutorial.cxx)
```

Note that this example uses lower case commands in the `CMakeLists.txt` file. Upper, lower, and mixed case commands are supported by CMake. The source code for `tutorial.cxx` is provided in the `Step1` directory and can be used to compute the square root of a number.

#### Adding a Version Number and Configured Header File

The first feature we will add is to provide our executable and project with a version number. While we could do this exclusively in the source code, using `CMakeLists.txt` provides more flexibility.

First, modify the `CMakeLists.txt` file to use the [project()](https://cmake.org/cmake/help/latest/command/project.html#command:project) command to set the project name and version number.

```cmake
cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Tutorial VERSION 1.0)
```

Then, configure a header file to pass the version number to the source code:

```cmake
configure_file(TutorialConfig.h.in TutorialConfig.h)
```

Since the configured file will be written into the binary tree, we must add that directory to the list of paths to search for include files. Add the following lines to the end of the CMakeLists.txt file:

```cmake
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
```

Using your favorite editor, create `TutorialConfig.h.in` in the source directory with the following contents:

```cmake
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
```

When CMake configures this header file the values for `@Tutorial_VERSION_MAJOR@` and`@Tutorial_VERSION_MINOR@` will be replaced.

Next modify tutorial.cxx to include the configured header file, `TutorialConfig.h`.

Finally, let's print out the executable name and version number by updating `tutorial.cxx` as follows:

```cmake
  if (argc < 2) {
    // report version
    std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
              << Tutorial_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    return 1;
  }
```

#### Specify the C++ Standard

Next let's add some C++11 features to our project by replacing `atof` with `std::stod` in `tutorial.cxx`. At the same time, remove `#include <cstdlib>`.

```c++
  const double inputValue = std::stod(argv[1]);
```

We will need to explicitly state in the CMake code that it should use the correct flags. The easiest way to enable support for a specific C++ standard in CMake is by using the [CMAKE_CXX_STANDARD](https://cmake.org/cmake/help/latest/variable/CMAKE_CXX_STANDARD.html#variable:CMAKE_CXX_STANDARD) variable. For this tutorial, set the `CMAKE_CXX_STANDARD` variable in the `CMakeLists.txt` file to 11 and [CMAKE_CXX_STANDARD_REQUIRED](https://cmake.org/cmake/help/latest/variable/CMAKE_CXX_STANDARD_REQUIRED.html#variable:CMAKE_CXX_STANDARD_REQUIRED) to True. Make sure to add the `CMAKE_CXX_STANDARD` declarations above the call to `add_executable`.

```cmake
cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Tutorial VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

#### Build and Test

Run the [cmake](https://cmake.org/cmake/help/latest/manual/cmake.1.html#manual:cmake(1)) executable or the [cmake-gui](https://cmake.org/cmake/help/latest/manual/cmake-gui.1.html#manual:cmake-gui(1)) to configure the project and then build it with your chosen build tool.

For example, from the command line we could navigate to the `Help/guide/tutorial` directory of the CMake source code tree and create a build directory:

```shell
mkdir Step1_build
```

Next, navigate to the build directory and run `CMake` to configure the project and generate a native build system:

```shell
cd Step1_build
cmake ../Step1
```

Then call that build system to actually compile/link the project:

```shell
cmake --build .
```

Finally, try to use the newly built Tutorial with these commands:

```shell
Tutorial 4294967296
Tutorial 10
Tutorial
```

#### Full code

- [CMakeLists.txt full code](../any/cmake/tutorial/Step1/CMakeLists.txt)
- [TutorialConfig.h.in full code](../any/cmake/tutorial/Step1/TutorialConfig.h.in)
- [tutorial.cxx full code](../any/cmake/tutorial/Step1/tutorial.cxx)

### Adding a Library (Step 2)

Now we will add a library to our project. This library will contain our own implementation for computing the square root of a number. The executable can then use this library instead of the standard square root function provided by the compiler.

For this tutorial we will put the library into a subdirectory called `MathFunctions`. This directory already contains a header file, `MathFunctions.h`, and a source file `mysqrt.cxx`. The source file has one function called `mysqrt` that provides similar functionality to the compiler's `sqrt` function.

Add the following one line `CMakeLists.txt` file to the `MathFunctions` directory:

```cmake
add_library(MathFunctions mysqrt.cxx)
```

To make use of the new library we will add an [add_subdirectory()](https://cmake.org/cmake/help/latest/command/add_subdirectory.html#command:add_subdirectory) call in the top-level `CMakeLists.txt` file so that the library will get built. We add the new library to the executable, and add `MathFunctions` as an include directory so that the `mysqrt.h` header file can be found. The last few lines of the top-level `CMakeLists.txt` file should now look like:

```cmake
# add the MathFunctions library
add_subdirectory(MathFunctions)

# add the executable
add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC MathFunctions)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/MathFunctions"
                          )
```

Now let us make the MathFunctions library optional. While for the tutorial there really isn't any need to do so, for larger projects this is a common occurrence. The first step is to add an option to the top-level `CMakeLists.txt` file.

```cmake
option(USE_MYMATH "Use tutorial provided math implementation" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)
```

This option will be displayed in the [cmake-gui](https://cmake.org/cmake/help/latest/manual/cmake-gui.1.html#manual:cmake-gui(1)) and [ccmake](https://cmake.org/cmake/help/latest/manual/ccmake.1.html#manual:ccmake(1)) with a default value of ON that can be changed by the user. This setting will be stored in the cache so that the user does not need to set the value each time they run CMake on a build directory.

The next change is to make building and linking the MathFunctions library conditional. To do this we change the end of the top-level `CMakeLists.txt` file to look like the following:

```cmake
if(USE_MYMATH)
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
  list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif()

# add the executable
add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ${EXTRA_INCLUDES}
                           )
```

Note the use of the variable `EXTRA_LIBS` to collect up any optional libraries to later be linked into the executable. The variable `EXTRA_INCLUDES` is used similarly for optional header files. This is a classic approach when dealing with many optional components, we will cover the modern approach in the next step.

The corresponding changes to the source code are fairly straightforward. First, in `tutorial.cxx`, include the `MathFunctions.h` header if we need it:

```c++
#ifdef USE_MYMATH
#  include "MathFunctions.h"
#endif
```

Then, in the same file, make `USE_MYMATH` control which square root function is used:

```c++
#ifdef USE_MYMATH
  const double outputValue = mysqrt(inputValue);
#else
  const double outputValue = sqrt(inputValue);
#endif
```

Since the source code now requires `USE_MYMATH` we can add it to `TutorialConfig.h.in` with the following line:

```cmake
#cmakedefine USE_MYMATH
```

Exercise: Why is it important that we configure TutorialConfig.h.in after the option for USE_MYMATH? What would happen if we inverted the two?

Run the `cmake` executable or the `cmake-gui` to configure the project and then build it with your chosen build tool. Then run the built Tutorial executable.

Now let's update the value of `USE_MYMATH`. The easiest way is to use the `cmake-gui` or `ccmake` if you're in the terminal. Or, alternatively, if you want to change the option from the command-line, try:

```shell
cmake ../Step2 -DUSE_MYMATH=OFF
```

Rebuild and run the tutorial again.
Which function gives better results, sqrt or mysqrt?

#### Full code

- [main CMakeLists.txt full code](../any/cmake/tutorial/Step2/CMakeLists.txt)
- [TutorialConfig.h.in full code](../any/cmake/tutorial/Step2/TutorialConfig.h.in)
- [tutorial.cxx full code](../any/cmake/tutorial/Step2/tutorial.cxx)
- [sub CMakeLists.txt full code](../any/cmake/tutorial/Step2/MathFunctions/CMakeLists.txt)
- more ...

### Adding Usage Requirements for Library (Step 3)

Usage requirements allow for far better control over a library or executable's link and include line while also giving more control over the transitive property of targets inside CMake. The primary commands that leverage usage requirements are:

- [target_compile_definitions()](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html#command:target_compile_definitions)

- [target_compile_options()](https://cmake.org/cmake/help/latest/command/target_compile_options.html#command:target_compile_options)

- [target_include_directories()](https://cmake.org/cmake/help/latest/command/target_include_directories.html#command:target_include_directories)

- [target_link_libraries()](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries)

Let's refactor our code from [Adding a Library (Step 2)](#adding-a-library-step-2) to use the modern CMake approach of usage requirements. We first state that anybody linking to MathFunctions needs to include the current source directory, while MathFunctions itself doesn't. So this can become an `INTERFACE` usage requirement.

Remember `INTERFACE` means things that consumers require but the producer doesn't. Add the following lines to the end of `MathFunctions/CMakeLists.txt`:

```cmake
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )
```

Now that we've specified usage requirements for MathFunctions we can safely remove our uses of the `EXTRA_INCLUDES` variable from the top-level CMakeLists.txt, here:

```cmake
if(USE_MYMATH)
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
endif()
```

And here:

```cmake
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
```

Once this is done, run the `cmake` executable or the `cmake-gui` to configure the project and then build it with your chosen build tool or by using `cmake --build .` from the build directory.

#### Full code

- [main CMakeLists.txt full code](../any/cmake/tutorial/Step3/CMakeLists.txt)
- [sub CMakeLists.txt full code](../any/cmake/tutorial/Step3/MathFunctions/CMakeLists.txt)
- more ...

### Installing and Testing (Step 4)

Now we can start adding install rules and testing support to our project.

#### Install Rules

The install rules are fairly simple: for MathFunctions we want to install the library and header file and for the application we want to install the executable and configured header.

So to the end of MathFunctions/CMakeLists.txt we add:

```cmake
install(TARGETS MathFunctions DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)
```

And to the end of the top-level `CMakeLists.txt` we add:

```cmake
install(TARGETS Tutorial DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  DESTINATION include
  )
```

That is all that is needed to create a basic local install of the tutorial.

Now run the `cmake` executable or the `cmake-gui` to configure the project and then build it with your chosen build tool.

Then run the install step by using the `install` option of the `cmake` command (introduced in 3.15, older versions of CMake must use `make install`) from the command line. For multi-configuration tools, don't forget to use the `--config` argument to specify the configuration. If using an IDE, simply build the `INSTALL` target. This step will install the appropriate header files, libraries, and executables. For example:

```shell
cmake --install .
```

The CMake variable `CMAKE_INSTALL_PREFIX` is used to determine the root of where the files will be installed. If using the `cmake --install` command, the installation prefix can be overridden via the `--prefix` argument. For example:

```cmake
cmake --install . --prefix "/home/myuser/installdir"
```

Navigate to the install directory and verify that the installed Tutorial runs.

#### Testing Support

Next let's test our application. At the end of the top-level `CMakeLists.txt` file we can enable testing and then add a number of basic tests to verify that the application is working correctly.

```cmake
enable_testing()

# does the application run
add_test(NAME Runs COMMAND Tutorial 25)

# does the usage message work?
add_test(NAME Usage COMMAND Tutorial)
set_tests_properties(Usage
  PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number"
  )

# define a function to simplify adding tests
function(do_test target arg result)
  add_test(NAME Comp${arg} COMMAND ${target} ${arg})
  set_tests_properties(Comp${arg}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result}
    )
endfunction(do_test)

# do a bunch of result based tests
do_test(Tutorial 4 "4 is 2")
do_test(Tutorial 9 "9 is 3")
do_test(Tutorial 5 "5 is 2.236")
do_test(Tutorial 7 "7 is 2.645")
do_test(Tutorial 25 "25 is 5")
do_test(Tutorial -25 "-25 is [-nan|nan|0]")
do_test(Tutorial 0.0001 "0.0001 is 0.01")
```

The first test simply verifies that the application runs, does not segfault or otherwise crash, and has a zero return value. This is the basic form of a CTest test.

The next test makes use of the [PASS_REGULAR_EXPRESSION](https://cmake.org/cmake/help/latest/prop_test/PASS_REGULAR_EXPRESSION.html#prop_test:PASS_REGULAR_EXPRESSION) test property to verify that the output of the test contains certain strings. In this case, verifying that the usage message is printed when an incorrect number of arguments are provided.

Lastly, we have a function called `do_test` that runs the application and verifies that the computed square root is correct for given input. For each invocation of `do_test`, another test is added to the project with a name, input, and expected results based on the passed arguments.

Rebuild the application and then cd to the binary directory and run the [ctest](https://cmake.org/cmake/help/latest/manual/ctest.1.html#manual:ctest(1)) executable: `ctest -N` and `ctest -VV`. For multi-config generators (e.g. Visual Studio), the configuration type must be specified. To run tests in Debug mode, for example, use `ctest -C Debug -VV` from the build directory (not the Debug subdirectory!). Alternatively, build the `RUN_TESTS` target from the IDE.

#### Full code

- [main CMakeLists.txt full code](../any/cmake/tutorial/Step4/CMakeLists.txt)
- [sub CMakeLists.txt full code](../any/cmake/tutorial/Step4/MathFunctions/CMakeLists.txt)
- more ...

### Adding System Introspection (Step 5)

Let us consider adding some code to our project that depends on features the target platform may not have. For this example, we will add some code that depends on whether or not the target platform has the `log` and `exp` functions. Of course almost every platform has these functions but for this tutorial assume that they are not common.

If the platform has `log` and `exp` then we will use them to compute the square root in the `mysqrt` function. We first test for the availability of these functions using the [CheckSymbolExists](https://cmake.org/cmake/help/latest/module/CheckSymbolExists.html#module:CheckSymbolExists) module in `MathFunctions/CMakeLists.txt`. On some platforms, we will need to link to the m library. If `log` and `exp` are not initially found, require the m library and try again.

```cmake
include(CheckSymbolExists)
check_symbol_exists(log "math.h" HAVE_LOG)
check_symbol_exists(exp "math.h" HAVE_EXP)
if(NOT (HAVE_LOG AND HAVE_EXP))
  unset(HAVE_LOG CACHE)
  unset(HAVE_EXP CACHE)
  set(CMAKE_REQUIRED_LIBRARIES "m")
  check_symbol_exists(log "math.h" HAVE_LOG)
  check_symbol_exists(exp "math.h" HAVE_EXP)
  if(HAVE_LOG AND HAVE_EXP)
    target_link_libraries(MathFunctions PRIVATE m)
  endif()
endif()
```

If available, use [target_compile_definitions()](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html#command:target_compile_definitions) to specify `HAVE_LOG` and `HAVE_EXP` as `PRIVATE` compile definitions.

```cmake
if(HAVE_LOG AND HAVE_EXP)
  target_compile_definitions(MathFunctions
                             PRIVATE "HAVE_LOG" "HAVE_EXP")
endif()
```

If `log` and `exp` are available on the system, then we will use them to compute the square root in the `mysqrt` function. Add the following code to the `mysqrt` function in `MathFunctions/mysqrt.cxx` (don't forget the #endif before returning the result!):

```c++
#if defined(HAVE_LOG) && defined(HAVE_EXP)
  double result = exp(log(x) * 0.5);
  std::cout << "Computing sqrt of " << x << " to be " << result
            << " using log and exp" << std::endl;
#else
  double result = x;
```

We will also need to modify mysqrt.cxx to include cmath.

```c++
#include <cmath>
```

Run the `cmake` executable or the `cmake-gui` to configure the project and then build it with your chosen build tool and run the Tutorial executable.

Which function gives better results now, sqrt or mysqrt?

#### Full code

- [sub CMakeLists.txt full code](../any/cmake/tutorial/Step5/MathFunctions/CMakeLists.txt)
- [tutorial.cxx full code](../any/cmake/tutorial/Step5/tutorial.cxx)
- more ...

---
- [上一级](README.md)
- 上一篇 -> [Clock 和 Timer](chrono.md)
- 下一篇 -> [判断浮点数是否相等](compareDoubleFloat.md)
