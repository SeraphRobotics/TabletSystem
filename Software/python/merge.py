#!/usr/bin/env python

import math
import os
import re
import sys


class Layer:
    displacement = 1
    valve = 2
    
    def __init__(self,z):
        self.z=z
        self.cmds=[]
        self.type=Layer.valve
    
def merge(inlists, outlist, verbose=False):
    ## a list of lists of gcodes is loaded in inlists
    # the lists can be files or []
    # the files are then sorted by  changes in Z height into layers and the layers are listed in a command group for each input list
    # the command group is put into one list and then sorted by Z height and integrated together
    ## Sort files into layers. The first layer is the startup code\
    cmd_groups=[]
    for infile in inlists:
        cmd_group =[]
        newlayer=Layer(0)
        for line in infile:
            z_move_match = re.match(r'^G1 Z([-\d\.]+)($| F)*([-\d\.]+)',line.rstrip())
            extrude_match = re.match(r'^G1 X([-\d\.]+) Y([-\d\.]+) E([-\d\.]+)($| F)*([-\d\.]+)',  ##$
                         line.rstrip())
            if z_move_match:
                #print "layer type %i"%newlayer.type
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
            if extrude_match:
                newlayer.type=Layer.displacement
            newlayer.cmds.append(line)
        #print "layer type %i"%newlayer.type
        cmd_group.append(newlayer)
        cmd_groups.append(cmd_group)
    
    #write first files startup code to file dumps the rest of the startup code
    for cmd in cmd_groups[0][0].cmds:
        outfile.write(cmd)
    for cmd_group in cmd_groups:
        cmd_group.pop(0)
    
    newlist = []
    for cmd_group in cmd_groups:
        for layer in cmd_group:
            newlist.append(layer)
            #print layer.z
    newlist.sort(key=lambda x: x.z)
    
    previous_layer_type = 0
    lowercmds="G4 P2\nM340 P1 S1000\n"
    raisecmds="G4 P2\nM340 P1 S5000\n"
    
    for cmd_layer in newlist:
        if (previous_layer_type != cmd_layer.type):
            if (previous_layer_type != Layer.displacement):
                outlist.append(lowercmds)
#                print "lower"
            elif (previous_layer_type == Layer.displacement):
#                print "Raise"
                outlist.append(raisecmds)
            else:
#                print "type: %i"%cmd_layer.type
                outlist.append(raisecmds)
        
#        print "layer type: %i"%cmd_layer.type
        previous_layer_type = cmd_layer.type
        for cmd in cmd_layer.cmds:
            outlist.append(cmd)
    
    

if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.exit('usage: test.py <filename> <filename> [--verbose]')
    infilename1 = sys.argv[1]
    infilename2 = sys.argv[2]
    parts = os.path.splitext(infilename1)
    outfilename = '%s.%s' % (parts[0],infilename2)
    with open(infilename1) as infile1:
        with open(infilename2) as infile2:
            with open(outfilename, 'w') as outfile:
                inlists = [infile1,infile2]
                outlist = []
                merge(inlists, outlist, '--verbose' in sys.argv)
                for line in outlist:
                    outfile.write(line)