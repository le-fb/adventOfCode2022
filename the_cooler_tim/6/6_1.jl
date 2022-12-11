f = readlines("input")

buffer = []
str = ""
index = 0
packetLength = 4 

for line in f 
    global str *= line
end

for i in 1:length(str)
    # create a set from the buffer since each element is unique per definition
    if(length(Set(buffer)) == packetLength && index == 0) # check if the sets cardinality equals the packet lenght (4)
        global index = i-1
    end
    if(length(buffer) < packetLength) # fill the buffer
        push!(buffer, str[i])
    elseif(length(buffer) == packetLength && index == 0) # move the buffer by one 
        popfirst!(buffer)
        push!(buffer, str[i])
    end
end

print("First marker after: ", index)