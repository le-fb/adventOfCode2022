f = readlines("input")

boxes = Dict{Int, Vector{String}}()

inverted = false

for line in f
    if(occursin("move", line))          # move instructions-block
        if(!inverted)
            global inverted = true
            for i in 1:length(boxes)
            reverse!(boxes[i]) 
            end
        end
        numbers = SubString.(line, findall(Regex("[0-9]+"), line))
            
        c = (boxes[parse(Int,numbers[2])])[end-parse(Int,numbers[1])+1:end]
        for i in 1:parse(Int, numbers[1])
            pop!(boxes[parse(Int,numbers[2])])
        end
        append!(boxes[parse(Int,numbers[3])], c)

    elseif(occursin("[", line))         # initialize
        for i in 1:Int((length(line)+1)/4)
            crate = line[4*(i-1)+1:4*(i-1)+3]
            if(occursin("[", crate))
                if(haskey(boxes, i))
                    push!(boxes[i], string(crate[2]))
                else
                    global boxes[i] = [string(crate[2])]
                end
            end
        end
    end
end


println("Crates on top are:")
for i in 1:length(boxes)
    println(boxes[i][end])
end