# Flex and Bison Examples in cpp

This repo contains some examples using flex and bison in their C++ interface.

## Flex C++ interface

To generate a C++ scanner, you can use `%option c++` to tell Flex that the scanner should be generated in C++ language. In the flex installation path, there is a FlexLexer.h file that contains class FlexLexer, an abstract class which specifies the external interface provided to flex C++ lexer objects, and class yyFlexLexer, which defines a particular lexer class.

``` c++
// FlexLexer.h

class FlexLexer
{
   // ... getter and setter for yytext, yyleng, yylineno and yy_flex_debug

   virtual int yylex() = 0;

   // ... interface functions like `virtual void yyrestart( std::istream* s ) = 0;`
};

class yyFlexLexer : public FlexLexer
{
   // ... override interface functions, definitions will be added by flex in the generated scanner.cpp

   virtual int yylex();

   // ... extra functions like `int LexerInput( char* buf, int max_size );`, definitions will be added by flex in the generated scanner.cpp
};
```

You can directly use yyFlexLexer as the scanner class, but you cannot add any additional member variables or functions to the scanner if so. To add additional member variables or functions to the scanner, you can inherit yyFlexLexer and then use `%option yyclass="Scanner"` to tell Flex to generate yylex functions for class Scanner. Then you can access the additional member variables or functions in the rules section.

``` c++
// scanner.hpp

class Scanner : public FlexLexer
{
   // ...

   int yylex() override;
   // or custom lex function name, return type and function signature
   std::variant<int, std::string> scan();

   // ... extra member functions like `void report_total();`
   // ... extra member variables like `std::size_t chars{0};`
};
```

Flex will generate definitions for member functions of yyFlexLexer, including yylex, but what is generated is not lex code, but error output indicating that this function should not be called. Flex will only generate the definition of yylex() for the Scanner class, where is the lex code, which allows you to access member variables and functions in the Scanner class in the rules section. The return type of yylex can be specified by defining macro YY_DECL.

``` c++
// scanner.cpp, generated by flex

int yyFlexLexer::yylex()
{
	LexerError( "yyFlexLexer::yylex invoked but %option yyclass used" );
	return 0;
}

// definitions of interface functions from class FlexLexer and extra functions from class yyFlexLexer
void yyFlexLexer::yyrestart(std::istream* s) { /* code generated by flex */ }

#define YY_DECL int Scanner::yylex()
// or explicitly define YYDECL if the return type or function name is customized
#define YY_DECL std::variant<int, std::string> Scanner::scan()

YY_DECL
{
   // code generated by flex
}
```

## Bison C++ interface

Different from Flex, Bison can produce a C++ parser directly. option `%language "c++"` can be used to declare a C++ parser should be generated. The generated C++ parser looks like:

```c++
// parser.hpp

class Parser
{
   // ...
   Parser();

   // ...
   virtual int parse ();

   // ...
}

// parser.cpp

int
Parser::parse ()
{
   // ...

   {
     symbol_type yylookahead (yylex ());
     yyla.move (yylookahead);
   }

   // ...
}
```

For more information about the Bison parser in C++, you can access this website [C++ Parsers](https://www.gnu.org/software/bison/manual/bison.html#C_002b_002b-Parsers).

## Bridge Flex and Bison in C++ interface

The generated C++ parser will call yylex() in the parse member function. This is inconsistent with the C++ interface scanner generated by Flex. To bridge Flex and Bison in C++ interface, you can define the function yylex which forwards return value from the scanner. However this requires yylex to have access to the scanner instance. In order to access the scanner in yylex, you can use the option `%param {Scanner &scanner}` which will change the function signature of yylex and constructor of parser to accept an additional parameter *Scanner*.

``` c++
// parser.hpp
class Parser
{
   // ...

   Parser (Scanner &scanner_yyarg);

   // ...
   virtual int parse ();

   // ...

   // User arguments.
   Scanner &scanner;
};

// parser.cpp
yy::Parser::symbol_type yylex(Scanner &scanner) {
    return scanner.scan();
}

/// Build a parser object.
Parser::Parser (yy::Scanner &scanner_yyarg)
    :
      scanner (scanner_yyarg)
  {}

int
Parser::parse ()
{
   // ...

   {
     symbol_type yylookahead (yylex (scanner));
     yyla.move (yylookahead);
   }

   // ...
}
```

## Example Introduction

### wc (flex used)

This small tool reads through console or files and reports the number of lines, words, and characters in them.

``` bash
# read from console
wc
hello world!
flex and bison
^Z
       3       5      28

# read from files
wc .gitignore CMakeLists.txt
       6       9      57 .gitignore
      19      23     405 CMakeLists.txt
      25      32     462
```

### calc (flex and bison used)

A simple console calculator that supports arithmetic calculation, variable definition, branches, loops and functions.

```bash
calc
> let max(x,y) = if x >= y then x; else y;;
Defined max
>  max(4+5,6+7)
= 13.0000
> 1+2*4/8
= 2.0000
>
```

### inc_finder (flex used)

This simple program handles nested include files and prints them out.

```bash
inc_finder .\inc_finder\scanner.hpp
enter file: .\inc_finder\scanner.hpp
   1 #pragma once
   2
   3 #if !defined(yyFlexLexerOnce)
enter file: FlexLexer.h
open file FlexLexer.h failed
   5 #endif
   6
enter file: print
open file print failed
enter file: stack
open file stack failed
enter file: string
open file string failed
enter file: fstream
open file fstream failed
enter file: string_view
open file string_view failed
...
```

### concordance (flex used)

A simple tool makes a *concordance*, which is a list of the line numbers where each word in the input appears.

```bash
concordance .gitignore CMakeLists.txt
BISON           CMakeLists.txt:11
Build           .gitignore:4
CMAKE           CMakeLists.txt:7 8
CTest           CMakeLists.txt:4
CXX             CMakeLists.txt:2 7 8
Directory       .gitignore:4
Editor          .gitignore:1
FLEX            CMakeLists.txt:10
LANGUAGES       CMakeLists.txt:2
REQUIRED        CMakeLists.txt:8 10 11
STANDARD        CMakeLists.txt:7 8
Specific        .gitignore:1
VERSION         CMakeLists.txt:1 2
...
```

### cross_ref (flex used)

cross_ref reads C source and lists all the reference for each definition in the source.

```bash
cross_ref .\cross_ref\main.cpp
enter file: .\cross_ref\main.cpp
enter file: scanner.hpp
open file scanner.hpp failed
enter file: print
open file print failed
exit file
Scanner         .\cross_ref\main.cpp:9
argc            .\cross_ref\main.cpp:5* 12 18
argv            .\cross_ref\main.cpp:5* 20
cross_referencer .\cross_ref\main.cpp:9 10 20 22 26
debug_flag      .\cross_ref\main.cpp:7* 10
enter_file      .\cross_ref\main.cpp:20
...
```

### sql_analyzer (flex and bison used)

A simple program that translates SQL statements into Reverse Polish Notation (RPN).

``` bash
sql_analyzer
SELECT * FROM a JOIN b on a.foo=b.bar;
rpn: SELECTALL
rpn: TABLE a
rpn: TABLE b
rpn: FIELDNAME a.foo
rpn: FIELDNAME b.bar
rpn: CMP 4
rpn: ONEXPR
rpn: JOIN 100
rpn: SELECT 0 1 1
rpn: STMT
```

## Getting Started

### Prerequisites

The following tools are required to build the examples.

* c++23 compatible compiler (used feature: *std::print*, *std::println*, *std::format_string*, can be easily ported to c++20)
* cmake
* flex
* bison

### Build

```bash
cd flex_bison_cpp_examples

cmake -S . -B build

cmake --build build
```

## Copyright Notice (from the book)

> This book is here to help you get your job done. In general, you may use the code in
> this book in your programs and documentation. You do not need to contact us for
> permission unless you’re reproducing a significant portion of the code. For example,
> writing a program that uses several chunks of code from this book does not require
> permission. Selling or distributing a CD-ROM of examples from O’Reilly books does
> require permission. Answering a question by citing this book and quoting example
> code does not require permission. Incorporating a significant amount of example code
> from this book into your product’s documentation does require permission.
> *<p align="right">flex & bison -- Using Code Examples</p>*

## Acknowledgement

The code is based on the examples in the book [flex & bison](https://www.oreilly.com/library/view/flex-bison/9780596805418/).

## Reference

flex & bison by John R. Levine. Copyright © 2009 John Levine. All rights reserved. ISBN: 978-0-596-15597-1.
