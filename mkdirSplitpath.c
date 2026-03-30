#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    //
    // YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW
    // Lecture Code (Mar 18, 2025) 

    struct NODE* parent;
    struct NODE* child;
    char *baseName;
    char *dirName;
    struct NODE* newDir;

    // checks pathname to see if it's empty
    if(pathName[0] == '\0'){
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    baseName = malloc(2000);
    dirName = malloc(2000);

    parent = splitPath(pathName, baseName, dirName);

    // added a null check so I don't seg fault
    if (parent == NULL){
        free(baseName);
        free(dirName);
        return;
    }

    child = parent->childPtr;

    // checks if existing directory already exists
    while (child != NULL){
        if(strcmp(child->name, baseName) == 0){
            printf("MKDIR ERROR: directory %s already exists \n", baseName);
            free(baseName);
            free(dirName);
            return;
        }
        child = child->siblingPtr;
    }

    newDir = malloc(sizeof(struct NODE));
    strcpy(newDir->name, baseName);
    newDir->fileType = 'D';
    newDir->parentPtr = parent;
    newDir->siblingPtr = NULL;
    newDir->childPtr = NULL;

    // inserting new dir
    if(child == NULL){
        parent->childPtr = newDir;
    }
    else {
        while (child->siblingPtr != NULL){
            child = child->siblingPtr;
        }
        child->siblingPtr = newDir;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", baseName);
    
    free(baseName);
    free(dirName);

    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    // TO BE IMPLEMENTED
    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED SOLUTION EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

    //my code starts here :3

    // find last slash in path
    char *lastSlash = strrchr(pathName, '/');

    // if no slash, the base name is path name
    if (lastSlash == NULL){
        strcpy(dirName, "");    // no directory
        strcpy(baseName, pathName); // base name = path name
        return cwd;
    }

    // last slash at start 
    if (lastSlash == pathName){
        strcpy(dirName, "/"); // directory portion is just the root
        strcpy(baseName, lastSlash + 1);  // copy everything after the last Slash
    }

    // slash somewhere in the middle
    else {
        // copy chars before last slash into dirName
        strncpy(dirName, pathName, lastSlash - pathName);
        // null-terminate cause strncpy won't always do it for some reason
        dirName[lastSlash - pathName] = '\0'; 
        // copy chars after last slash into baseName
        strcpy(baseName, lastSlash + 1);
    }   

    // finding parent node

    struct NODE* current;

    // if we have an absolute path, we start at root dir
    if(dirName[0] == '/'){
        current = root;
    }
    // otherwise it's relative
    else {
        current = cwd;
    }
    
    // tokenize dirName by '/'
    // strtok() on the first call returns the first token
    // each call using strtok(NULL, "/") returns the next one
    char *token = strtok(dirName, "/"); 


    // searches the children of current node, then searches the siblings of each child node until it's found a match
    while (token != NULL){

        struct NODE* child = current->childPtr;

        while (child != NULL){
            if(strcmp(child->name, token) == 0 && child->fileType == 'D'){
                break; // found matching dir
            }
            child = child->siblingPtr;
        }
         // if we didn't find a matching dir, the path doesn't exist
        if (child == NULL){
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }
        // if we did find it, move into that directory and repeat
        current = child;
        token = strtok(NULL, "/");
    }
    
    // current now points to the parent node for the new entry
    return current;

}
