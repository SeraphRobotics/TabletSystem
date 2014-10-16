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


class Layer:
    def __init__(self,z):
        self.z=z
        self.cmds=[]
    
def merge(infiles, outfile, verbose=False):

    ## Sort files into layers. The first layer is the startup code
    cmd_groups=[]
    for infile in infiles:
        print infile
        cmd_group =[]
        newlayer=Layer(0)
        for line in infile:
            z_move_match = re.match(r'^G1 Z([-\d\.]+)($| F)*([-\d\.]+)',line.rstrip())
            if z_move_match:
                cmd_group.append(newlayer)
                if verbose: print "Z_move: ", z_move_match.groups()
                z1 = [0]*2
                for j in range(0,2):
                   group = z_move_match.groups()[j]
                   if not (type(group)==type(None)):
                        if "f" in group.lower():
                            z1[1]=z_move_match.groups()[j+1]
                            break
                        else:
                            z1[j]=float(group)
                newlayer=Layer(z1[0])
                
            newlayer.cmds.append(line)
        cmd_group.append(newlayer)
        cmd_groups.append(cmd_group)
    
    #write first files startup code to file
    for cmd in cmd_groups[0][0].cmds:
        outfile.write(cmd)
    for cmd_group in cmd_groups:
        cmd_group.pop(0)
    
    newlist = []
    for cmd_group in cmd_groups:
        for layer in cmd_group:
            newlist.append(layer)
            print layer.z
    newlist.sort(key=lambda x: x.z)
    for layer in newlist:
        for cmd in layer.cmds:
            outfile.write(cmd)
    
    

if __name__ == '__main__':
    if len(sys.argv) < 3:
        sys.exit('usage: test.py <output file name> <input file names> [--verbose]')
    
    outfilename = sys.argv[1] 
    print outfilename;
    
    i=2;
    infiles=[]
    for i in range(2,len(sys.argv) ):
        if(sys.argv[i] != '--verbose'):
            infiles.append( open(sys.argv[i]) )
            print "opened ",sys.argv[i] 
    
    with open(outfilename, 'w') as outfile:
        merge(infiles, outfile, '--verbose' in sys.argv)
            
    
