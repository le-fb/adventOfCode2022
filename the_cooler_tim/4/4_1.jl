f = readlines("input")

containing_pairs = 0

for line in f
    temp = split(line, ",")
    intervalls = [split(temp[1],"-"), split(temp[2],"-")]
    set1 = parse(Int, (intervalls[1][1])):parse(Int,(intervalls[1][2]))
    set2 = parse(Int,(intervalls[2][1])):parse(Int,(intervalls[2][2]))
    intersec = intersect(set1,set2)
    if(set1==intersec || set2==intersec) 
        global containing_pairs += 1
    end
end

print("There are ", containing_pairs, " pairs that overlapp completly")