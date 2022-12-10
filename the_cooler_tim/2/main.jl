# The ordering is always "Rock, Paper, Siccor" in each dict
# Possible points, depending on "strategic choice", e.g. Rock on Rock will yield 3 (draw)
PointMap = Dict(
    "A" => [3,6,0], 
    "B" => [0,3,6], 
    "C" => [6,0,3])
# possible strategic choices (r,p,s) and their "shape value"
XYZ_Map = Dict(
    "X" => [[1,0,0],1], 
    "Y" => [[0,1,0],2], 
    "Z" => [[0,0,1],3])
    
f= readlines("strategy.dat")
    
total_points = 0
for line in f 
    next_move = split(line, " ")
    x = XYZ_Map[next_move[2]]
    # points per turn is the dot product + the shape value
    global total_points += (PointMap[next_move[1]]'x[1]+x[2])
end

print("The first strategy will yield ", total_points, " points.")