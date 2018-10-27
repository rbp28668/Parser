# Pascal PARSER

Written to provide an embeddable interpreter that could be extended with application specific functions.  This grew out of writing a domain specific language for image processing which over time grew towards a general purpose language.  In a lot of ways it's easier to start with a general purpose language and wrap domain specific concepts into it.  This has been used "in anger" to control a pipetting robot in a lab environment.

The basic process is to compile the pascal down to virtual machine instructions.  The VM then executes an instruction every call it its tick() function until it completes.  This allows good integration with state machine based hardware without the need for threading etc.

Currently this compiles with MS Visual C++ V6.  The library is written entirely in C, primarily for portability.

## Folders are:

* lib - contains a compiled library for the interpreter.
* TEST - Pascal test scripts to test different parts of the interpreter.
* DOCS - Documentation - of the implemented language, how to use the library and the demo application.
* PAS2BIN - Code that allows limited pre-compilation of source.
* Windows - a Windows demo application for exercising the library.
* DOS - Ye Olde command line demo application.
* SOURCE - source for the interpreter.
* Compile - helper application for compiling text files into binary resources - used for the interpreter error file.
* DEMODISK - demonstration code & copy of key docs.
