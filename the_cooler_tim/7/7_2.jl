mutable struct File
    Filename::String
    Filesize::Integer
end

mutable struct Directory
    DirectoryName::String
    DirSize::Integer
    Containing::Union{Directory, Nothing}
    Files::Vector{File}
    SubDirs::Vector{Directory}
end

function UpdateDirSize(dir, newsize)
    dir.DirSize += newsize
    if(dir.DirectoryName != "/")
        UpdateDirSize(dir.Containing, newsize)
    end
end

function AddFile(dir, filename, filesize)
    push!(dir.Files, File(filename, filesize))
    UpdateDirSize(dir, filesize)
end

function PrintTree(dir, offset)
    println(offset, dir.DirectoryName, " ", dir.DirSize)
    for file in dir.Files
        println(offset, " - ", file.Filename, " ", file.Filesize)
    end
    for subdir in dir.SubDirs
        PrintTree(subdir, offset*"   ")
    end
end

function AddSubDir(dir, subdirname)
    d = Directory(subdirname, 0, dir, Vector{File}(), Vector{Directory}())
    push!(dir.SubDirs, d)
    return d
end

function GetSize!(dir, threshhold)
    totalsize = 0
    if(threshhold >= dir.DirSize)
        totalsize+=dir.DirSize 
    end
    for subdir in dir.SubDirs
        if(length(subdir.SubDirs) > 0 || length(subdir.Files) > 0)
            totalsize += GetSize!(subdir, threshhold)
        end
    end
    return totalsize
end

function GetDirToDelete(startDir, threshhold, buffer)
    dir = startDir
    for subDir in startDir.SubDirs
        if(subDir.DirSize > threshhold)
            append!(buffer,GetDirToDelete(subDir, threshhold, buffer).DirSize)
        end
    end
    return dir
end

f = readlines("input")

currentDir = Directory("/", 0, nothing, Vector{File}(), Vector{Directory}())
root = currentDir
r = Regex("[0-9]+")

for line in f
    if(occursin("\$ cd", line))
        if(occursin("..", line))
            global currentDir = currentDir.Containing
        else
            dname = split(line, "cd ")[end]
            global currentDir = AddSubDir(currentDir, dname)
        end
    elseif(!occursin("dir", line) && !occursin("\$", line))     # file
        size = match(r, line).match
        AddFile(currentDir, split(line, size)[end], parse(Int, size))
    end
end

freespace = 30000000-(70000000 - root.SubDirs[1].DirSize)
println(root.SubDirs[1].DirSize)
arr = []
toDelete = GetDirToDelete(root.SubDirs[1], freespace, arr)
println(arr)
println("The dir to delete is ", minimum(arr))
println("freespace ", freespace)