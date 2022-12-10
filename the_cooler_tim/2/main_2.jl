# The ordering is always "Rock, Paper, Siccor" in each dict
# possible choices of the opponent(r,p,s)
PointMap = Dict("A"=>[1,0,0], "B"=>[0,1,0], "C"=>[0,0,1])
# possible strategic choices (w,d,l) depending on opponents choice and their "shape value"
XYZ_Map = Dict(
    "X" => [[3,1,2],0], 
    "Y" => [[1,2,3],3], 
    "Z" => [[2,3,1],6])
    
f= readlines("strategy.dat")
    
total_points = 0
for line in f 
    next_move = split(line, " ")
    x = XYZ_Map[next_move[2]]
    # points per turn is the dot product + the shape value
    global total_points += (x[1]'PointMap[next_move[1]]+x[2])
end

print("The second strategy will yield ", total_points, " points.")