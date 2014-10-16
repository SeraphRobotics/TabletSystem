#!/usr/bin/env python

"""

"""

import math
import os
import re
import sys

class State:
	Moving = 1
	Extruding = 2

def rewrite(infile, outfile, verbose=False):
	
	closeLine = "M340 P0 S1650\n"
	openLine = "M340 P0 S2100\n"
	
	state = State.Moving
	
	
	for line in infile:
		xy_move_match = re.match(r'^G1 X([-\d\.]+) Y([-\d\.]+)($| F)*([-\d\.]+)',line.rstrip())
		z_move_match = re.match(r'^G1 Z([-\d\.]+)($| F)*([-\d\.]+)',line.rstrip())
		extrude_match = re.match(r'^G1 X([-\d\.]+) Y([-\d\.]+) E([-\d\.]+)($| F)*([-\d\.]+)',  ##$
						 line.rstrip())
		reset_match = re.match(r'^G92 E([-\d\.]+)',line.rstrip())
		retract_match = re.match(r'^G1 F([-\d\.]+) E([-\d\.]+)',line.rstrip())
		pre_extrude_match = re.match(r'^G1 E([-\d\.]+) F([-\d\.]+)',line.rstrip())
		
		tempchange_match = re.match(r'^M104 S([-\d\.]+)',line.rstrip())
		settemp_match = re.match(r'^M109 S([-\d\.]+)',line.rstrip())
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
			if (p2[3]==600):p2[3]=1800
			if p2[3]>0: speed = " F%f"%float(p2[3])
			newline = "G1 X%f Y%f%s\n"%(p2[0],p2[1],speed)

			if state == State.Moving:
				outfile.write(openLine)
				state = State.Extruding
			outfile.write(newline)
			
			
		elif xy_move_match:

			if verbose: 
				print "move: ", xy_move_match.groups()
			if state == State.Extruding:
				outfile.write(closeLine)
				state = State.Moving
			##outfile.write(line)
			
			m1 = [0]*3
			for j in range(0,3):
				group = xy_move_match.groups()[j]
				if not (type(group)==type(None)):
					if "f" in group.lower():
						m1[2]=xy_move_match.groups()[j+1]
						break
					else:
						m1[j]=float(group)

			
			if( ( (m1[2]<=600.1) and (m1[2]<=599.9) ) ): ##or m1[2]<0.01
				m1[2]=1800.00
			
			speed = ""
			if m1[2]>0: 
				speed = ""
				##speed = " F%f"%float(m1[2])
			newline = "G1 X%f Y%f%s\n"%(m1[0],m1[1],speed)
			outfile.write(newline)
		elif z_move_match:
			##p2 = [float(s) for s in match.groups()]
			if verbose: print "Z_move: ", z_move_match.groups()
			if state == State.Extruding:
				outfile.write(closeLine)
				state = State.Moving
			##outfile.write(line)
			z1 = [0]*2
			for j in range(0,2):
				group = z_move_match.groups()[j]
				if not (type(group)==type(None)):
					if "f" in group.lower():
						z1[1]=z_move_match.groups()[j+1]
						break
					else:
						z1[j]=float(group)		
			if (z1[1]>600 or z1[1]<0.01):z1[1]=600.00
			newline = "G1 Z%f F%f\n"%(z1[0],z1[1])
			outfile.write(newline)
		elif reset_match:
			if verbose: print "reset"
		elif retract_match:
			if verbose: print "retract"
		elif pre_extrude_match:
			if verbose: print "pre-extrude"
		elif tempchange_match:
		   if verbose:  print "set temp"
		elif settemp_match:
			if verbose:  print "temp"
		else:
			if verbose: print line
#			if state == State.Extruding:
				#outfile.write(closeLine)
				#state = State.Moving
			outfile.write(line)

if __name__ == '__main__':
	if len(sys.argv) < 2:
		sys.exit('usage: test.py <filename> [--verbose]')
	infilename = sys.argv[1]
	outfilename = '%s.extrude%s' % os.path.splitext(infilename)
	with open(infilename) as infile:
		with open(outfilename, 'w') as outfile:
			rewrite(infile, outfile, '--verbose' in sys.argv)
