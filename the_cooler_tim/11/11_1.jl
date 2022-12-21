f = readlines("input")

mutable struct Monkey
    name::String
    items::Vector{Int64}
    operation::Function
    test::Function
    action::Function
    activeness::Int64
end

monkeys = Dict{Int64, Monkey}()

for i in 0:div(length(f)+1, 7)-1
    m = Monkey(
        split(f[1+i*7][1:end-1], "y ")[2],
        parse.(Int, split(split(f[2+i*7], ": ")[2], ", ")),
        x -> if(occursin("*", f[3+i*7]) && split(f[3+i*7], "* ")[2] != "old")
                x*parse(Int, split(f[3+i*7], "* ")[2])
            elseif(occursin("+", f[3+i*7])) 
                x+parse(Int, split(f[3+i*7], "+ ")[2]) 
            else
                x^2
            end,
        x -> x%parse(Int, split(f[4+i*7], "by ")[2])==0,
        x -> if(x) 
                return parse(Int, split(f[5+i*7], "y ")[2]) 
            else 
                return parse(Int, split(f[6+i*7], "y ")[2]) 
            end,
        0
    )
    monkeys[parse(Int, m.name)] = m
end

rounds = 2

println("begin rounds")
for i in 1:rounds
    for m in 0:length(monkeys)-1
        current = monkeys[m]
        for item in current.items
            current.activeness += 1
            worrylevel = current.operation(item)
            worrylevel = div(worrylevel,3)
            target = current.action(current.test(worrylevel))
            push!(monkeys[target].items, worrylevel)
        end
        current.items = []
    end
end

actives = []
for i in 0:length(monkeys)-1
    push!(actives, monkeys[i].activeness)
end

for i in 0:length(monkeys)-1
    println("========ITEMS========")
    println(monkeys[i].items)
end

business = actives[argmax(actives)] * actives[argmax(deleteat!(actives, argmax(actives)))]
println(business)