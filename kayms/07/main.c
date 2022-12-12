#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utils.h>

#define INPUT_FILE_NAME "../../kayms/07/input"
#define MAX_SUBFOLDERS 1000
#define MAX_FILES_PER_FOLDER 1000

#define DISK_SIZE 70000000

typedef struct folder folder;
typedef struct file file;

struct file
{
    char *filename;
    int fileSize;
};

struct folder
{
    char *folderName;
    int folderSize;

    folder *rootFolder;

    int folderIndex;
    folder **subFolders;

    int fileIndex;
    file **files;
};

folder *makeFolder(char *name)
{
    folder *newFolder = malloc(sizeof(folder));
    assert(newFolder != NULL);
    newFolder->folderName = strdup(name);
    newFolder->folderSize = 0;
    newFolder->rootFolder = NULL;
    newFolder->subFolders = malloc(MAX_SUBFOLDERS * sizeof(void *));
    newFolder->folderIndex = 0;
    newFolder->files = malloc(MAX_FILES_PER_FOLDER * sizeof(void *));
    newFolder->fileIndex = 0;
    return newFolder;
}

void addDir(folder *currentFolder, folder *newFolder)
{
    assert(currentFolder->folderIndex < (MAX_SUBFOLDERS - 1));

    currentFolder->subFolders[currentFolder->folderIndex] = newFolder;
    currentFolder->folderIndex++;

    newFolder->rootFolder = currentFolder;
}

file *makeFile(char *name, int size)
{
    file *newFile = malloc(sizeof(file));
    newFile->filename = strdup(name);
    newFile->fileSize = size;
    return newFile;
}

void addFileToFolder(folder *currFolder, file *currfile)
{
    assert(currFolder->fileIndex < (MAX_FILES_PER_FOLDER - 1));

    currFolder->files[currFolder->fileIndex] = currfile;
    currFolder->fileIndex++;

    // currFolder->folderSize += currfile->fileSize;
}

int getFileSizeOfFolder(folder *currFolder)
{
    int sum = 0;
    // get sum from files
    for (int i = 0; i < currFolder->fileIndex; i++)
    {
        sum += currFolder->files[i]->fileSize;
    }
    return sum;
}

int makeSizes(folder *currFolder)
{

    int sum = getFileSizeOfFolder(currFolder);

    // get sum from dirs
    for (int i = 0; i < currFolder->folderIndex; i++)
    {
        sum += makeSizes(currFolder->subFolders[i]);
    }
    currFolder->folderSize = sum;

    return sum;
}

int getLowSizeDirsSum(folder *currFolder, int *folderCount)
{
    int sum = 0;

    for (int i = 0; i < currFolder->folderIndex; i++)
    {

        if (currFolder->subFolders[i]->folderSize <= 100000)
        {
            (*folderCount)++;
            sum += currFolder->subFolders[i]->folderSize;
        }
        sum += getLowSizeDirsSum(currFolder->subFolders[i], folderCount);
    }

    return sum;
}

void searchDir(int size, folder **candidate, folder *currFolder)
{
    if (*candidate == NULL)
    {
        *candidate = currFolder;
    }
    if ((currFolder->folderSize - size) > 0 && (currFolder->folderSize - size) < ((*candidate)->folderSize - size))
    {
        *candidate = currFolder;
    }

    for (int i = 0; i < currFolder->folderIndex; i++)
    {
        searchDir(size, candidate, currFolder->subFolders[i]);
    }
}

void printFileStruct(folder *currFolder, int currentLevel);

void printFileStruct(folder *currFolder, int currentLevel)
{
    for (int i = 0; i < currentLevel; i++)
    {
        printf("   ");
    }
    printf("- %s (dir) size: %i\n", currFolder->folderName, currFolder->folderSize);
    for (int i = 0; i < currFolder->fileIndex; i++)
    {
        for (int j = 0; j < currentLevel; j++)
        {
            printf("   ");
        }
        printf(" - %s (file, size: %i)\n", currFolder->files[i]->filename, currFolder->files[i]->fileSize);
    }
    int nextLevel = currentLevel + 1;
    for (int i = 0; i < currFolder->folderIndex; i++)
    {
        printFileStruct(currFolder->subFolders[i], nextLevel);
    }
    return;
}

int main()
{
    char **content;
    int lines = 0;

    enum mode
    {
        N = 0,
        LIST = 1
    };

    enum mode m;

    int r = readFile(INPUT_FILE_NAME, &content, &lines);

    char buffer[1000];

    m = N;
    folder *rootFolder = NULL;

    folder *currFolder = NULL;
    folder *lastListedFolder = NULL;

    for (int i = 0; i < lines; i++)
    {
        int currIndex = 0;
        char *currLine = strdup(content[i]);
        char *split = strtok(currLine, " ");
        if (split == NULL)
        {
            continue;
        }

        if (strcmp(split, "$") == 0)
        {
            m = N;
        }
        else
        {
            m = LIST;
        }

        if (m == N)
        {
            assert(strcmp(split, "$") == 0);

            split = strtok(NULL, " ");
            if (strcmp(split, "cd") == 0)
            {
                // switch dir
                split = strtok(NULL, " ");
                assert(split != NULL);

                if (strcasecmp(split, "/") == 0)
                {
                    // create root dir
                    char *fileName = strdup(split);
                    currFolder = makeFolder(fileName);
                }
                else if (strcasecmp(split, "..") == 0)
                {
                    assert(currFolder->rootFolder != NULL);
                    // go one folder back
                    currFolder = currFolder->rootFolder;
                }
                else
                {
                    // go into dir
                    int found = 0;
                    for (int j = 0; j < currFolder->folderIndex; j++)
                    {
                        if (strcmp(split, currFolder->subFolders[j]->folderName) == 0)
                        {
                            currFolder = currFolder->subFolders[j];
                            found = 1;
                            break;
                        }
                    }
                    assert(found == 1);
                }
            }
            else
            {
                assert(strcmp(split, "ls") == 0);
                // list dir (create files and dirs)
                m = LIST;
            }
        }
        else
        {
            // listen mode
            if (strcmp(split, "dir") == 0)
            {
                // create dir
                split = strtok(NULL, " ");
                char *folderName = strdup(split);
                folder *nf = makeFolder(folderName);
                // add to currfolder
                addDir(currFolder, nf);
            }
            else
            {
                assert(currFolder != NULL);

                char *temp = strdup(split);
                int fileSize = atoi(temp);
                split = strtok(NULL, " ");
                char *fileName = strdup(split);
                // create file
                file *nFile = makeFile(fileName, fileSize);
                // add file to folder
                addFileToFolder(currFolder, nFile);
            }
        }
    }
    // go back to root
    while (currFolder->rootFolder != NULL)
    {
        currFolder = currFolder->rootFolder;
    }

    rootFolder = currFolder;

    makeSizes(rootFolder);
    printFileStruct(rootFolder, 0);
    int test = 0;
    printf("solution part 1: %i\n", getLowSizeDirsSum(rootFolder, &test));

    folder *candidate = NULL;

    int searchGoal = 30000000 - (DISK_SIZE - rootFolder->folderSize);
    printf("search goal : %i\n", searchGoal);
    searchDir(searchGoal, &candidate, rootFolder);
    printf("solution part 2: %i\n", candidate->folderSize);

    return 0;
}