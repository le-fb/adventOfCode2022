f = readlines("input")

total_prio = 0
offset = Int(codepoint('A'))
for line in f
    substrings = [line[1:Int(length(line)/2)],line[Int(length(line)/2)+1:end]]                 # string has even length
    doubles = intersect(substrings[1], substrings[2])   # strings parsed into a sets, the twice-occuring string is in the intersection of the sets
    temp = Int(codepoint(doubles[1]))-offset+1          # convert to unicode-codepoint and parse into int a=97,z=122,A=65,Z=90
    if(temp>26)
        global total_prio += temp-(26+6)
    else 
        global total_prio += temp+26
    end
end

print("Priority sum is ", total_prio)