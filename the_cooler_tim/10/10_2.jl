f = readlines("input")

x = 1
cycle = 1
display = fill(".", 7, 40)

displayline = "........................................"

for line in f
    str = split(line, " ")
    for i in 1:length(str)
        # start of cycle 
        temp = displayline
        if(x>=2)
            temp = temp[1:x-1]*"###"*temp[x+3:end]
        elseif(x == 1)
            temp = "###"*temp[4:end]
        elseif(x == 0)
            temp = "##"*temp[3:end]
        elseif(x == -1)
            temp = "#"*temp[2:end]            
        end
        # during cycle
        if(cycle%40==0)
            global display[div(cycle,40)+1,cycle%40+1] = string(temp[cycle%40+1])
        else
            global display[div(cycle,40)+1,cycle%40] = string(temp[cycle%40])
        end
        # end cycle
        if(i == 2)
            global x += parse(Int, str[2])
        end
        println(cycle, "|",x, ":    ", temp)
        global cycle += 1
    end
end

for i in 1:7
    t = ""
    for j in 1:length(display[i,:])
        t*=display[i,j]
    end
    println(t)
end