#!/usr/bin/env python3

import sys, os
import re

infile_name = sys.argv[1]
outfile_name = sys.argv[2]

with open(infile_name, 'rt') as infile:
    with open(outfile_name, 'wt') as outfile:

        last = infile.readline()
        outfile.write(last)
        curr = infile.readline()

        while curr:
            pattern = '^--- last message repeated ([0-9]+) time[s]? ---$'
            re_match = re.search(pattern, curr)
            if re_match:
                for _ in range(int(re_match.group(1))):
                    outfile.write(last)
            else:
                outfile.write(curr)

            last, curr = curr, infile.readline()

