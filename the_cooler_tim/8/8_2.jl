function convertSplit(list,IsLeft)
    if(length(list)>1 && sum(list)>0)
        if(IsLeft)
            for i in 1:length(list)
                if(i==length(list))
                    return length(list)
                elseif(list[end-i+1]==0)
                    return length(list[end-i+1:end])
                end
            end
        else
            for i in 1:length(list)
                if(i == length(list))
                    return length(list)
                elseif(list[i] == 0)
                    return length(list[1:i])
                end
            end
        end
    else
        return 1
    end
end

f = readlines("input")

w = length(f[1])
l = length(f)
tree_mat = Array{Integer}(undef,l,0)

for line in f
    row = [parse(Int, i) for i in line]
    global tree_mat = hcat(tree_mat, row)
end

coords = []
trees = 0

println(tree_mat)

for i in 2:l-1
    for j in 2:w-1
        element = tree_mat[i,j]
        l = tree_mat[i,1:j-1] .< element
        r = tree_mat[i,j+1:end] .< element
        u = tree_mat[1:i-1,j] .< element
        d = tree_mat[i+1:end,j] .< element
        dummy = convertSplit(l,true)*convertSplit(r,false)*convertSplit(u,true)*convertSplit(d,false)
        if(trees < dummy)
            global trees = dummy
        end
    end
end

println(coords)
println("Number of tallest trees: ", trees)