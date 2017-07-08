# README for CCCC version 3.1.5

## Foreword

The Purdue Compiler Construction Toolset (PCCTS) is required to compile CCCC. A version of this program can be found [here](https://github.com/thaddeus002/pccts).

## Presentation

This is a maintenance update of the last (upstream) release of the program CCCC (C and C++ Code Counter).  This software performs analysis and reports metrics on source code written in C, C++ and Java.  The languages Ada83 and Ada95, which were supported in previous versions are presently not supported (stay tuned for updates).  The original source is still on sourceforge; the new site for CCCC development is: https://github.com/thaddeus002/cccc and for current documentation see: http://thadddeus002.github.io/cccc/

* [User Guide](http://thaddeus002.github.io/cccc/CCCC_User_Guide.html) - Upstream user guide (may not be current).
* [Software API docs](http://thaddeus002.github.io/cccc/api/html/index.html) - Software API documentation with design diagrams, etc.
* [Complexity Metrics](http://thaddeus002.github.io/cccc/metrics/cccc.html) - Source code metrics for a source code metrics tool... ;)

CCCC is a command line tool, originally implemented for Linux and other POSIX-style platforms, but also buildable on the Win32 platform. Files to be analyzed are listed (typically on the command line, although support does exist for the list of files to be read from standard input. The program analyzes the files and generates a one or more reports in HTML or XML format on the content.  

This distribution is intended to be a single package containing everything which is required to compile and install CCCC on a POSIX platform. Whereas previous distributions contained only a single directory, containing the source code of CCCC, this distribution is divided into a number of subdirectories, with the following contents:

 * The source of a new version of CCCC, which I hope will turn out to be more efficient, accurate and useful than previous versions, although it is very similar in spirit and interface.
 * A directory of minimal test cases for the command line CCCC tool.
 * A makefile to support building, testing and installing the software under POSIX.

Current versions should be built and installes using the makefile, however, in earlier versions (3.pre-*) the recommended way of building the software was by using a shell script in the top directory of the distribution.

Previous versions of CCCC have been released like PCCTS into the public domain with no restrictions, other than the pious hope that should anyone base a derivative work on it I will be given due credit. The current version is released under the GNU Public License.

The upcoming 3.1.5 release will be a maintenance release to clean up the build and makefile config (enabling parallel make) and fix a build error with recent gcc.

SLA: Among other things, I'm grateful that Tim chose such a cool thesis, and 
also produced some useful open source code that rarely seem to exist outside
of (expensive) commercial engineering tools.  It has a simple CLI and other
support like Jenkins plugins.  Use it to find out your own complexity metrics.
You might be surprised...

The above updates by Stephen L Arnold.


## Notes from the original author

CCCC was originally developed as a proof-of-concept tool for some ideas relating to software metrics which I was investigating as an academic research project. I have now completed the project and been awarded my PhD.  

With this release, I am choosing to declare the project in a dormant state. I have no plans to do further releases, either to add new features or to correct existing defects. I am happy to provide advice to users of CCCC via email, and I encourage all users to exercise the rights granted them under the GPL to tinker with the code if this helps the software to meet their needs better.

I am grateful to a number of people for assistance on this project, including Terence Parr and his co-workers, and Tom Moog for the development and ongoing maintenance of the excellent PCCTS tool; my academic supervisor, Dr Thomas O'Neill; a number of developers who have contributed source patches; and many people who have mailed me with constructive suggestions or merely to confirm that they were using the tool. Feedback on the value or otherwise of CCCC will always be welcome.

Tim Littlefair (tim_littlefair _At_ hotmail.com) updated January 2005
