##    Copyright (C) 2010-2011, 2015  Renan Martins
##
##    This program is free software; you can redistribute it and/or
##    modify it under the terms of the GNU General Public License
##    as published by the Free Software Foundation; either version 2
##    of the License, or (at your option) any later version.
##
##    This program is distributed in the hope that it will be useful,
##    but WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##    GNU General Public License for more details.
##
##    You should have received a copy of the GNU General Public License
##    along with this program; if not, write to the Free Software
##    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

# Map Generator

TILESIZE = 48
filename = "../data/maps/map.txt"
tile_id = 24

map_width = int(input("Enter map width: "))
map_height = int(input("Enter map height: "))

# Note: use integer division; otherwise, the result will be float
tiles_across = map_width // TILESIZE
tiles_down = map_height // TILESIZE

print("Generating a ", map_width, " by ", map_height, " map. . .")

fh = open(filename, 'w')

fh.write("{} {}\n".format(map_width, map_height))

for i in range(0, tiles_down):
    for j in range(0, tiles_across):
        if j != tiles_across - 1:
            # Note: cannot concatenate int and str in Python
            # Note: do not use keywords for var names
            myStr = str(tile_id) + " "
            fh.write(myStr)
        else:
            myStr = str(tile_id) + "\n"
            fh.write(myStr)

# Note: You cannot concatenate a str with an int in Python. Use .format or
# call print() with a comma-separated list of arguments
print("Map ", filename, " generated.")

fh.close()
