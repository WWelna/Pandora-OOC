## Pandora-OOC
An Object-Oriented ANSI C Framework to obfuscate and prevent static and/or live executable analysis

### The Story
This is one of my old research and development projects into preventing heuristic analysis performed
by Anti-Virus engines. To this day, the methods and concepts employed here still work. Besides for that,
the code looks confusingly cool, and it simplifies many tasks. 

This relies on:
* The fact that static analysis is impossible on pointers in lookup (Class) tables that have not been initialized.
* Dynamic initialization of the lookup (Class) tables in execution prevents full overview of what code is being executed.
* Magick, with a K

Coincidentally, I was working on this project before the discovery and attempted analysis of Duqu framework. When
I saw the pieces of code asking the community for help identifying information how Duqu was coded, I recognized
it instantly, as it looked very similar to my own code when decompiled. This was an attempt at streamlining previous
OOC methods I used on other proof of concepts concerning Anti-Virus evasion, and is not in any way based upon Duqu,
and has nothing in common as far as I know besides for using an OOC framework.

### TODO
* Get it working in Linux and have Linux specific Class wrappers.
* Remove customized build enviroment dependancies (tcc, nmake, nasm, openssl binaries).
* Modernization.

### Build Instructions

Lots of prayer at present until this is ported to a more modern build enviroment.

### Interesting Links
* A good introduction -> http://www.planetpdf.com/codecuts/pdfs/ooc.pdf
* Identifying the Duqu framework -> https://seclists.org/fulldisclosure/2012/Mar/225

### License
 
Copyright (C) 2011-2021 William Welna (wwelna@occultusterra.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
