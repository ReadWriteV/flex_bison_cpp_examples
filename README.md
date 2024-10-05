# Flex and Bison Examples in cpp

This repo contains some examples using flex and bison in c++ interface.

## Example Introduction

### wc

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

### calc

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

### inc_finder

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

### concordance

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

### cross_ref

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

### sql_analyzer

A simple program that translate SQL statements into Reverse Polish Notation (RPN).

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

cmake --build out/build/default
```

## Using Code Examples

> This book is here to help you get your job done. In general, you may use the code in
> this book in your programs and documentation. You do not need to contact us for
> permission unless you’re reproducing a significant portion of the code. For example,
> writing a program that uses several chunks of code from this book does not require
> permission. Selling or distributing a CD-ROM of examples from O’Reilly books does
> require permission. Answering a question by citing this book and quoting example
> code does not require permission. Incorporating a significant amount of example code
> from this book into your product’s documentation does require permission.
> *<p align="right">flex & bison</p>*

## Acknowledgement

The code is based on the examples in the book [flex & bison](https://www.oreilly.com/library/view/flex-bison/9780596805418/).

## Reference

flex & bison by John R. Levine. Copyright © 2009 John Levine. All rights reserved. ISBN: 978-0-596-15597-1.
