f = readlines("input")

total_prio = 0
offset = Int(codepoint('A'))
for i in 1:Int(length(f)/3)
    badge = intersect(f[3*(i-1)+1], f[3*(i-1)+2], f[3*(i-1)+3])    # strings parsed into a sets, the twice-occuring string is in the intersection of the sets
    temp = Int(codepoint(badge[1]))-offset+1                                            # convert to unicode-codepoint and parse into int a=97,z=122,A=65,Z=90
    if(temp>26)
        global total_prio += temp-(26+6)
    else 
        global total_prio += temp+26
    end
end

print("Priority sum is ", total_prio)