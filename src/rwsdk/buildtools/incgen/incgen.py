
import sys
import os
import time

if len(sys.argv) < 2:
    print('No header name specified.')
    exit(1)

current_path = os.getcwd() + '\\'
header_name = sys.argv[1]

system_headers = [
'stdarg.h',
'stddef.h',
'stdio.h',
'stdlib.h',
]

header_list = []
with open(current_path + header_name + '.lst', 'r') as f:
    for line in f.readlines():
        if line.startswith('#'):
            continue
        header_list.append(line.strip())

filename = current_path + 'include\\d3d8\\' + header_name + '.h'
with open(filename, 'w') as fw:
    fmt =  '/******************************************/\n'
    fmt += '/*                                        */\n'
    fmt += '/*    RenderWare(TM) Graphics Library     */\n'
    fmt += '/*                                        */\n'
    fmt += '/******************************************/\n'
    fmt += '\n'
    fmt += '/*\n'
    fmt += ' * This file is a product of Criterion Software Ltd.\n'
    fmt += ' *\n'
    fmt += ' * This file is provided as is with no warranties of any kind and is\n'
    fmt += ' * provided without any obligation on Criterion Software Ltd.\n'
    fmt += ' * or Canon Inc. to assist in its use or modification.\n'
    fmt += ' *\n'
    fmt += ' * Criterion Software Ltd. and Canon Inc. will not, under any\n'
    fmt += ' * circumstances, be liable for any lost revenue or other damages\n'
    fmt += ' * arising from the use of this file.\n'
    fmt += ' *\n'
    fmt += ' * Copyright (c) 1999. Criterion Software Ltd.\n'
    fmt += ' * All Rights Reserved.\n'
    fmt += ' */\n'
    fmt += '\n'
    fw.write(fmt)

    fmt =  '/*************************************************************************\n'
    fmt += ' *\n'
    if filename:
        fmt += ' * Filename: <%s>\n' % filename
    else:
        fmt += ' * Filename: <unknown>\n'
    fmt += ' * Automatically Generated on: %s\n' % time.ctime()
    fmt += ' *\n'
    fmt += ' ************************************************************************/\n'
    fmt += '\n'
    fw.write(fmt)

    if header_name:
        base = header_name.upper()
        fmt =  '#ifndef %s_H\n' % base
        fmt += '#define %s_H\n' % base
        fmt += '\n'
        fw.write(fmt)

    fmt = '/*--- System Header Files ---*/\n'
    for header in system_headers:
        fmt += '#include <%s>\n' % header
    fmt += '\n'
    fw.write(fmt)

    for header in header_list:
        print('Parsing: ', header)
        fullpath = current_path + header
        try:
            with open(fullpath, 'r') as fr:
                fw.write('\n/*--- Automatically derived from: %s ---*/\n' % fullpath)

                is_public = False
                public_start = False
                cur_line = 0
                for line in fr.readlines():
                    cur_line += 1
                    if 'RWPUBLIC' in line:
                        if 'RWPUBLICEND' in line:
                            is_public = False
                        else:
                            if is_public:
                                print('Mismatched RWPUBLIC - RWPUBLICEND at line', cur_line)
                                exit(1)
                            public_start = True
                    if is_public:
                        fw.write(line)
                    if public_start:
                        is_public = True
                        public_start = False
            fw.write('\n')
        except FileNotFoundError as err:
            print(err)

    if header_name:
        base = header_name.upper()
        fw.write('#endif /* %s_H */\n' % base)

print('Completed.')
