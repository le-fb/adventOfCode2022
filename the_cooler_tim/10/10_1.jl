f = readlines("input")

x = 1
cycle = 1
d = Dict{Integer, Integer}()

for line in f
    global cycle += 1
    if(occursin("addx", line))
        str = split(line, " ")
        d[cycle] = cycle*x
        global x += parse(Int, str[2])
        global cycle += 1
    end
    global d[cycle] = x*cycle
end

println(d[20]+d[60]+d[100]+d[140]+d[180]+d[220])
println(241 in keys(d))