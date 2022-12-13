f = readlines("input")

w = length(f[1])
l = length(f)
tree_mat = Array{Integer}(undef,l,0)

for line in f
    row = [parse(Int, i) for i in line]
    global tree_mat = hcat(tree_mat, row)
end

coords = []
trees = 2*(l+w)-4

println(tree_mat)

for i in 2:l-1
    for j in 2:w-1
        element = tree_mat[i,j]
        l = tree_mat[i,1:j-1] .>= element
        r = tree_mat[i,j+1:end] .>= element
        u = tree_mat[1:i-1,j] .>= element
        d = tree_mat[i+1:end,j] .>= element
        if(sum(l) == 0 || sum(r) == 0 || sum(u) == 0 || sum(d) == 0)
            global trees += 1
        end
    end
end

println(coords)
println("Number of tallest trees: ", trees)