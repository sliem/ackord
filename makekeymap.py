#!/usr/bin/env python

import sys

modifiers = ('KEY_CTRL',
             'KEY_GUI',
             'KEY_ALT',
             'KEY_SHIFT')

def valid(l):
    return l != '' # yeah, should write a proper (in)validator...

def main():
    keys = {}
    for line in sys.stdin:

        if not valid(line.strip()):
            continue

        s = line.split('\t')
        keys[int(s[0],2)] = s[1].strip()

    print('#include "ackord.h"\n\nconst prog_uint8_t keymap[128] PROGMEM = {') 

    for i in range(128):
        sys.stdout.write("  ")

        if i in keys:
            sys.stdout.write(keys[i])

            if keys[i] in modifiers:
                sys.stdout.write(' + MODIFIER_OFFSET')

        else:
            sys.stdout.write('0')

        if i != 127:
            sys.stdout.write(',')
        
        print

    print('};')

if __name__ == '__main__':
    main()
