#!/usr/bin/env python

"""
Gcode cleaner to work around some bugs in KISSlicer.

This eliminates stuttering caused by too many extra G1 commands on
straight lines or very short segments. It also reduces Gcode file
size by up to 20%, without reducing print quality or resolution.

To use this script automatically in KISSlicer, enter the following
in the Printer -> Firmware tab under Post-Process:

/full/path/to/clean_gcode.py "<FILE>"

KISSlicer will replace the "<FILE>" part with the actual
filename.gcode and this script will create a new file called
filename.clean.gcode in the same folder.

If you want to see an explanation for each removed G1 line:

python clean_gcode.py filename.gcode --verbose
colordiff -u filename.gcode filename.clean.gcode
"""

import math
import os
import re
import sys

def translate(in_line_list, out_line_list, delta, verbose=False):
    
    
    for line in in_line_list:
        xy_move_match = re.match(r'^G1 X([-\d\.]+) Y([-\d\.]+)($| F)*([-\d\.]+)',line.rstrip())
        z_move_match = re.match(r'^G1 Z([-\d\.]+)($| F)*([-\d\.]+)',line.rstrip())
        extrude_match = re.match(r'^G1 X([-\d\.]+) Y([-\d\.]+) E([-\d\.]+)($| F)*([-\d\.]+)',  ##$
                         line.rstrip())

        if extrude_match:

            if verbose: print "extrude: ", extrude_match.groups()
            p2 =[0]*4
            
            for j in range(0,4):
                group = extrude_match.groups()[j]
                if not (type(group)==type(None)):
                    if "f" in group.lower():
                        p2[3]=extrude_match.groups()[j+1]
                        break
                    else:
                        p2[j]=float(group)
            speed=""
            if p2[3]>0:
                speed = " F%f"%float(p2[3])
            p2[0] = delta[0] + p2[0]
            p2[1] = delta[1] + p2[1]
            
            newline = "G1 X%f Y%f E%f%s\n"%(p2[0],p2[1],p2[2],speed)
            out_line_list.append(newline)
            
            
        elif xy_move_match:
            if verbose: 
                print "move: ", xy_move_match.groups()
            
            m1 = [0]*3
            for j in range(0,3):
                group = xy_move_match.groups()[j]
                if not (type(group)==type(None)):
                    if "f" in group.lower():
                        m1[2]=xy_move_match.groups()[j+1]
                        break
                    else:
                        m1[j]=float(group)
            
            speed = ""
            if m1[2]>0: 
                speed = " F%f"%float(m1[2])
            m1[0] = delta[0] + m1[0]
            m1[1] = delta[1] + m1[1]
            newline = "G1 X%f Y%f%s\n"%(m1[0],m1[1],speed)
            out_line_list.append(newline)
            
        elif z_move_match:
            if verbose: print "Z_move: ", z_move_match.groups()

            z1 = [0]*2
            for j in range(0,2):
                group = z_move_match.groups()[j]
                if not (type(group)==type(None)):
                    if "f" in group.lower():
                        z1[1]=float(z_move_match.groups()[j+1])
                        break
                    else:
                        z1[j]=float(group)        

            z1[0]= z1[0] + delta[2]
            newline = "G1 Z%f F%f\n"%(z1[0],z1[1])
            out_line_list.append(newline)
            
        else:
            if verbose: print line
            out_line_list.append(line)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.exit('usage: test.py <filename> <x> <y> <z> [--verbose]')
    infilename = sys.argv[1]
    delta= [float(sys.argv[2]), float(sys.argv[3]), float(sys.argv[4]) ]
    outfilename = '%s.translate%s' % os.path.splitext(infilename)
    with open(infilename) as infile:
        with open(outfilename, 'w') as outfile:
            out_line_list=[]
            translate(infile, out_line_list, delta, '--verbose' in sys.argv)
            for line in out_line_list:
                outfile.write(line)
