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

pos_h = [0,0]
pos_t = [0,0]
diff = [0,0]

head_mat = [[0,0]]
tail_mat = [[0,0]]

for line in f 
    str = split(line, " ")
    for i in 1:parse(Int, str[2])
        global diff = pos_h - pos_t
        if(diff == movements[str[1]])                                   # moving along the axis 
            global pos_h += movements[str[1]]
            global pos_t += movements[str[1]]
        elseif(((pos_h+movements[str[1]])-pos_t) in values(movements)   # h-t is diagonal and touching again (non-diagonal) after the move
            || sum_squared(((pos_h+movements[str[1]])-pos_t))==2        # h-t is non-diagonal and h is moving into the diagonal state
            || ((pos_h+movements[str[1]]) == pos_t))                    # h-t is diagonal and h moves to the position of t
            global pos_h+=movements[str[1]]
        else                                                            
            global pos_t = pos_h                                        # t needs to catch up over the diagonal
            global pos_h +=movements[str[1]]
        end
        if(!(pos_t in tail_mat))
            push!(tail_mat, pos_t)
        end
        push!(head_mat, pos_h)
    end
end

println(length(tail_mat))