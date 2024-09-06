/*
* Name: Richies Huynh, 5009307792, 1
* Description: .
* Input: .
* Output: .
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>

using namespace std;

/// These constant variables are self-explanatory. Each is assigned
/// a default value to silence CodeGrade's silly error detection.
const int SUCCESS = 0;
const int FAILURE = 0;
const int MAX_STR_LEN = 1024;
const int MAX_ARGS = 64;
const string USAGE = R"(
usage: git <command> [<args>]

These are common Git commands used in various situations:

start a working area (a directory)
   clone    Clone a repository into a new directory
   init     Create an empty Git repository

work on the current change
   add      Add a file contents to the index
   mv       Move or rename a file or directory
   rm       Remove files from the working directory

examine the history and state
   diff     Show changes between commits
   grep     Print lines matching a pattern

grow, mark and tweak your common history
   commit   Record changes to the repository

collaborate (see also: git help workflows)
   push     Update remote refs along with associated objects
)";

/// These declarations are generally necessary because of the
/// way C++ resolves and binds names to declarations.

/// A type that represents a file.
struct File
{
   string name;
   vector<string> lines;
};

/// A type that represents the changes made in a file.
struct Position
{
   int line;
   int column;
   int length;
   File file;
};

/// A type that represents a repository (i.e., a directory)
struct Directory
{
   string name;
   vector<File> files;
};

int parseCommand(const char* str, char** tokens);

bool makeDirectory(const char* name, vector<Directory>& directories);

int findDirectory(const char* name, vector<Directory>& directories);

int findFileInDirectory(const char* name, Directory& dir);

void printUsage();

bool stringCompare(const char* s1, const char* s2);

int strLength(const char* str);

bool addFileToDirectory(const char* name, Directory& dir);

void removeFileFromDirectory(vector<char*>& files, Directory& dir, bool& flag);

bool searchInFile(const char* str, Position& position, File& file);

bool replaceInFile(const char* str, Position& position, File& file);

///@fn int main(int, char**)
///@brief Entry point of our application.
///@param[in]argc the number of command-line argument passed to the program.
///@param[in]argv the vector of command-line arguments.
///@return SUCCESS if the program terminated successfully or FAILURE otherwise.
int main(int argc, char** argv) {
   // bailed out if we do not have enough arguments to process
   if (argc < 2) {
      cout << "ERROR: invalid number of command-line arguments." << endl;
      return FAILURE;
   }
   // Contains all repositories added via command-line. Don't forget
   // to close a previously opened directory!
   vector<Directory> directories;
   // Contains all changes made to files and before a commit command
   // is executed. Don't forget to move them to the commits vector
   // every time a commit command is executed
   vector<Position> stagingArea;
   // Contains all changes made to files which are ready to be pushed! (a.k.a supercommit)
   vector<Position> commits;
   // Contains the parsed command, that is, a sequence of tokens that
   // you should process and evaluate individually
   char* commands[MAX_ARGS];
   // <-- add your variables here
   // TODO: use some variables to keep track of the current working directory and current working file.
   // -->
   // We begin at 1 because 0 contains the name of the program, which
   // is not a command!
   for (int i = 1; i < argc; i++) {
      // <--
      // 1. Make sure to validate each command before executing each command
      //    that is, check for syntax!!
      // 2. Here is the list of commands that you must implement and use:
      //    - git OR git help
      //    - git init <dir>
      //    - git add <file_1> <file_2> ... <file_N> to <dir>
      //    - git rm <file_1> <file_2> ... <file_N> from <dir>
      //    - git mv <file> to <dir> <file>
      //    - git grep <pattern> in <file> OR git grep <pattern>
      //    - git diff <file1> <file2>
      //    - git commit
      //    - git push
      cout << argc - 1 << ' ';
//      int tokens = parseCommand(argv[i], commands);
      // TODO: Write your code here...
      // -->
   }
   return SUCCESS;
}

// TODO: write the implementation of each function here!
int parseCommand(const char* str, char** tokens) {
    char line[MAX_STR_LEN];
    int argc = 0;

    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == ' ') {
            line[i] = '\0';
            argc++;
        } else {
            line[i] = str[i];
        }

        i++;
    }

    return argc;
}

bool makeDirectory(const char* name, vector<Directory>& directories) {
    Directory directory = { .name = name};
    directories.push_back(directory);
    return true;
}

int findDirectory(const char* name, vector<Directory>& directories) {
    for (size_t i = 0; i < directories.size(); i++) {
        if (directories[i].name == name) {
            return i;
        }
    }

    return -1;
}

int findFileInDirectory(const char* name, Directory& dir) {
    vector<File> files = dir.files;
    for (size_t i = 0; i < files.size(); i++) {
        if (files[i].name == name) {
            return i;
        }
    }

    return -1;
}

void printUsage() {
    for (size_t i = 0; i < strLength(USAGE.c_str()); i++) {
        cout << USAGE[i];
    }
}

bool stringCompare(const char* s1, const char* s2) {
    if (strLength(s1) != strLength(s2)) {
        return false;
    }

    for (size_t i = 0; i < strLength(s1); i++) {
        if (s1[i] != s2[i]) {
            return false;
        }
    }
    return true;
}

int strLength(const char* str) {
    int len = 0;
    while (str[len] != '\0')
        len++;

    return len;
}

bool addFileToDirectory(const char* name, Directory& dir) {
    // file inside of project folder
    vector<string> lines;
    string line;
    ifstream projectFile;
    projectFile.open(name);

    if (!projectFile.is_open()) {
        return false;
    }

    while (!projectFile.eof()) {
        getline(projectFile, line);
        lines.push_back(line);
    }

    File file = {
        .name = name,
        .lines = lines
    };
    dir.files.push_back(file);

    projectFile.close();
    return true;
}

void removeFileFromDirectory(vector<char*>& files, Directory& dir, bool& flag) {
    flag = true;
    // iterate backwards for proper traversal while removing elements
    for (size_t i = files.size() - 1; i > 0; i--) {
        string fileToBeRemoved = files[i];
        for (size_t j = dir.files.size() - 1; j > 0; j--) {
            string curFile = dir.files[j].name;
            if (fileToBeRemoved == curFile) {
                // remove file
                files.erase(files.begin() + i);
                dir.files.erase(dir.files.begin() + j);
            }
        }
    }

    if (files.size() > 0) {
        flag = false;
    }
}

bool searchInFile(const char* str, Position& position, File& file) {
    for (size_t i = 0; i < file.lines.size(); i++) {
        if (file.lines[i].find(str) != string::npos) {
            position.line = i;
            position.column = file.lines[i].find(str);
            position.length = strLength(str);
            return true;
        }
    }

    return false;
}

bool replaceInFile(const char* str, Position& position, File& file) {
    string targetLine = file.lines[position.line];

    // can not replace a line with a negative index or an index higher than number of lines in file
    if (position.line < 0 || position.line > file.lines.size()) {
        return false;
    }

    // can not replace column with negative index or higher than number of characters in line
    if (position.column < 0 || position.column > strLength(file.lines[position.line].c_str())) {
        return false;
    }


    return true;
}