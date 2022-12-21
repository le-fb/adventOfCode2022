using Plots

gr()

f = readlines("input")

function sum_squared(arr)
    return sum(arr.^2)
end

movements = Dict(
    "R" => [1,0],
    "L" => [-1,0],
    "U" => [0,1],
    "D" => [0,-1]
    )
l = 10
rope = fill([0,0],l)
diff = [0,0]

head_mat = [[0,0]]
tail_mat = [[0,0]]

for line in f 
    str = split(line, " ")
    for i in 1:parse(Int, str[2])
        rope[1] +=movements[str[1]]
        for j in 2:l
            global diff = rope[j-1] - rope[j]
            if(sum_squared(diff) == 4)                                      # moving along the axis 
                global rope[j] += [sign(diff[1]), sign(diff[2])]
            elseif(sum_squared(diff)==5                                     # leading piece moved away, catch up and move diagonal
                || sum_squared(diff)==8)                                    # leading piece moved away diagonally, catch up and move diagonal
                global rope[j] += [sign(diff[1]),sign(diff[2])]             
            end
        end
        push!(head_mat, rope[1])
        if(!(rope[end] in tail_mat))
            push!(tail_mat, rope[end])
        end
    end
end


println(length(tail_mat))
