#!/usr/bin/env python
import random, sys
from optparse import OptionParser

"""
Compares lines from two files and displays unique and shared lines
Copyright 2016 Manas Kumar

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License, but I'm not sure 
why you'd want to because I'm terrible with python and this script
is probably like super buggy.

This program is distributed in the hope that it will be useful (yeah right)
but WITHOUT ANY WARRANTY, yadda yadda yadda.

Please see <http://ww.gnu.org/licenses/for/my/buggy/code/>
for a copy of the license
"""

def maketabs(number):
    tabs = ""
    for i in range(number):
        tabs += "        "
    return tabs


class comm:
    def __init__(self, files, options, parser):
        self.options = options
        self.parser = parser
        self.files = files
        
    def compare(self):
        count1 = 0
        count2 = 0
        
        fileOneLines = self.files[0].readlines()
        fileTwoLines = self.files[1].readlines()

        isunsorted = not (fileOneLines == sorted(fileOneLines) and fileTwoLines == sorted(fileTwoLines))
        # true if files are unsorted
        
        if isunsorted and not self.options.unsort:
            self.parser.error("Input files are not sorted; please use -u option.")
            
        # If files are unsorted
        if self.options.unsort:
            for x in fileOneLines:
                for y in list(fileTwoLines):
                    if x == y and y in fileTwoLines:
                        col3 = True
                        sys.stdout.write("") if self.options.omit3 else sys.stdout.write(maketabs(2) + x)
                        fileTwoLines.remove(y)
                        break;
                else:
                    sys.stdout.write("") if self.options.omit1 else sys.stdout.write(maketabs(0) + x)

            for y in fileTwoLines:
                sys.stdout.write("") if self.options.omit2 else sys.stdout.write(maketabs(1) + y)
            return        
        
        # If files are not unsorted
        while count1 < len(fileOneLines) and count2 < len(fileTwoLines):
            numTabs = 0
            if fileOneLines[count1] < fileTwoLines[count2]:
                sys.stdout.write("") if self.options.omit1 else sys.stdout.write(maketabs(0) + fileOneLines[count1])
                count1+=1
                continue;
            numTabs+=1

            if fileOneLines[count1] > fileTwoLines[count2]:
                sys.stdout.write("") if self.options.omit2 else sys.stdout.write(maketabs(1) + fileTwoLines[count2])
                count2+=1
                continue;
            numTabs+=1

            if fileOneLines[count1] == fileTwoLines[count2]:
                sys.stdout.write("") if self.options.omit3 else sys.stdout.write(maketabs(2) + fileOneLines[count1])
                count1+=1
                count2+=1
                continue;
        
        if count1 == len(fileOneLines):
            while count2 < len(fileTwoLines):
                sys.stdout.write(maketabs(1) + fileTwoLines[count2])
                count2+=1
        else:
            while count1 < len(fileOneLines):
                sys.stdout.write(maketabs(0) + fileOneLines[count1])
                count1+=1

def main():
    # usage message:
    usage_msg = \
    """%prog [-123] [-u] file1 file2
    Compares text in input files.
	
    Options:
    -1 Suppress lines unique to file1
    -2 Suppress lines unique to file 2
    -3 Suppress lines found in both files
    -u Allows unsorted files to be comm'ed.
    """

    # add all the options
    parser = OptionParser(usage=usage_msg)
    parser.add_option("-1", action="store_true", dest="omit1", help="suppress lines unique to file1", default=False)
    parser.add_option("-2", action="store_true", dest="omit2", help="suppress lines unique to file1", default=False)
    parser.add_option("-3", action="store_true", dest="omit3", help="suppress lines found in both files", default=False)
    parser.add_option("-u", action="store_true", dest="unsort", help="Allows unsorted files to be comm'ed", default=False)

    options, args = parser.parse_args(sys.argv[1:])
    try: 
        f = [open(arg, 'r') if arg != '-' else sys.stdin for arg in args]
        if len(f) != 2:
            parser.error(usage_msg)
    except IOError as err:
        parser.error("I/O error: ".
                                     format(errno))

    # This is the actual part that does the thing
    ihatethisclass = comm(f, options, parser)
    ihatethisclass.compare()

# I have no idea what this is for    
if __name__== "__main__":
    main()
