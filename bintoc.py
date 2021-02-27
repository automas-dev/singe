#!/usr/bin/env python3

import os
import sys
import io


if __name__ == '__main__':
    args = sys.argv
    if len(args) < 4:
        #print('Usage: {} <binary_file> <c_file> <h_file> <struct_name>'.format(args[0]))
        print('Usage: {} <binary_file> <h_file> <struct_name>'.format(args[0]))
        binfile = './demo/res/Questrial_Regular.ttf'
        #cfile = './src/default_font.c'
        hfile = './include/default_font.h'
        structname = 'default_font'
    else:
        binfile = args[1]
        #cfile = args[2]
        hfile = args[2]
        structname = args[3]

    hname = os.path.basename(hfile)

    with open(binfile, 'rb') as fin, open(hfile, 'w') as fh:
        buff = fin.read()

        
        #fc.write('#include "{}"\n'.format(hfile))
        #fc.write('unsigned char *__{}_start = "'.format(structname))
        #for byte in buff:
        #    fc.write('\\x{:02X}'.format(byte))
        #fc.write('";\n')

        #fc.write('const size_t __{}_size = {};\n'.format(structname, size))
        
        ifndef = hname.upper().replace('.', '_')
        fh.write('#ifndef {}\n'.format(ifndef))
        fh.write('#define {}\n'.format(ifndef))
        fh.write('#define __{}_size  {}\n'.format(structname, len(buff)))
        fh.write('#define __{}_start "'.format(structname))
        for byte in buff:
            fh.write('\\x{:02X}'.format(byte))
        fh.write('"\n')
        fh.write('#endif // {}\n'.format(ifndef))

        #fh.write('font.loadFromMemory("')
        #for byte in buff:
        #    fh.write('\\x{:02X}'.format(byte))
        #fh.write('", {});\n'.format(size))

