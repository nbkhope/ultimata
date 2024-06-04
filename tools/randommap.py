#!/usr/bin/env python
#
# Random Map Generator
# for Ultimata
#
#

# Copyright (C) 2011  nbkhope
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#
# Map format: each line represents a coordinate of the map with values
#	      tileid [item ids]
#
#	      the first line of file has the width and height of the map
#

import random
import sys

tile_width = 32
tile_height = 32

itemgen = False

try:
	fout = open("randommap.txt", "w")
except IOError as (errno, strerror):
	print "I/O error({0}): {1}".format(errno, strerror)
	sys.exit(errno)
except:
	print "Unexpected error:", sys.exc_info()[0]
	raise


while True:
	try:
		map_width = int(raw_input("Enter map width: "))
		break
	except ValueError:
  		print "Input is not a valid integer number."

while True:
	try:
		map_height = int(raw_input("Enter map height: "))
		break
	except ValueError:
  		print "Input is not a valid integer number."


if map_width % tile_width != 0 or map_height % tile_height != 0:
	print "Invalid map size: dimensions must be a multiple of tile size"
	sys.exit(3) 


print "Generating random map..."

i = 0
#mapsize = 1024 // 32 * 1024 // 32 

fout.write(str(map_width) + " " + str(map_height) + "\n")

while i < map_width // tile_width * map_height // tile_height:
	tileid = str(random.randrange(0, 64)) # between 0 and 63
	itemcount = random.randrange(0, 6)

	fout.write(tileid)
	
	# some items
	if i % 7 == 0 and itemgen:
		for n in range(itemcount):
			itemid = str(random.randrange(0, 256))
			fout.write(" " + itemid)

	fout.write("\n")
	i = i + 1	

fout.close()

print "Done."
