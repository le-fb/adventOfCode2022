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
    # for subdir in dirDict[dir.Containing].SubDirs
    #     if(subdir.DirectoryName == dir.DirectoryName)
    #         push!(subdir.Files, File(filename, filesize))
    #     end
    # end
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

f = readlines("input")

currentDir = Directory("/", 0, nothing, Vector{File}(), Vector{Directory}())
root = currentDir
r = Regex("[0-9]+")

for line in f
    if(occursin("cd", line))
        if(occursin("..", line))
            global currentDir = currentDir.Containing
        else
            dname = split(line, "cd ")[end]
            #if(!haskey(dirs, dname))#nein nein nein, die ficker machen das nicht unique
            global currentDir = AddSubDir(currentDir, dname)
            #end
        end
    elseif(!occursin("dir", line) && !occursin("\$", line))     # file
        size = match(r, line).match
        AddFile(currentDir, split(line, size)[end], parse(Int, size))
    end
end

PrintTree(root, "")


println("Total sum of all sub 100k is: ", GetSize!(root, 100000))