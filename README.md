# CCCCounter

C and C++ Code Counter

## Presentation

CCCCounter is a free software that performs analysis and reports metrics on source code written in C, C++ and Java. It is a fork of CCCC, written by Tim Littlefair and others, and whose source is on [sourceforge](http://cccc.sourceforge.net).

CCCCounter is a command line tool, implemented for Linux and other POSIX-style platforms. Files to be analyzed are listed (typically on the command line, although support does exist for the list of files to be read from standard input. The program analyzes the files and generates a one or more reports in HTML or XML format on the content.

This fork is based on a merge of several projects to keep the good ideas of each, the authors of which will be granted in `git log`.

## Install

### Requirements

The Purdue Compiler Construction Toolset (PCCTS) is required to compile CCCCounter. A version of this program can be found [here](https://github.com/thaddeus002/pccts).

To install it :

    $ git clone https://github.com/thaddeus002/pccts
    $ cd pccts
    $ make
    $ su
    # make install

### get and build CCCCounter

    $ git clone https://github.com/thaddeus002/CCCCounter
    $ cd CCCCounter
    $ make

### optionnaly run tests

    $ make test

This should end with the following message :

    ================
    All tests passed
    ================

### install CCCCounter

    $ su
    # make install

### quick start

    $ cccc source_file ...

## Documentation

To start using CCCCounter, please read the [User Guide](https://thaddeus002.github.io/CCCCounter/CCCCounter_User_Guide.html).

## License

CCCCounter is released under the GNU General Public License V2 or newer. See [LICENSE](LICENSE).

## Notes from the original author of CCCC

CCCC was originally developed as a proof-of-concept tool for some ideas relating to software metrics which I was investigating as an academic research project. I have now completed the project and been awarded my PhD.

With this release, I am choosing to declare the project in a dormant state. I have no plans to do further releases, either to add new features or to correct existing defects. I am happy to provide advice to users of CCCC via email, and I encourage all users to exercise the rights granted them under the GPL to tinker with the code if this helps the software to meet their needs better.

I am grateful to a number of people for assistance on this project, including Terence Parr and his co-workers, and Tom Moog for the development and ongoing maintenance of the excellent PCCTS tool; my academic supervisor, Dr Thomas O'Neill; a number of developers who have contributed source patches; and many people who have mailed me with constructive suggestions or merely to confirm that they were using the tool. Feedback on the value or otherwise of CCCC will always be welcome.

Tim Littlefair (tim_littlefair _At_ hotmail.com) updated January 2005
