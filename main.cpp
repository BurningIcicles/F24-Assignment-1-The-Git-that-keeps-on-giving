/*
* Name: Richies Huynh, 5009307792, 1
* Description: Pass in a Git command and can create files and directories
               using commit, push, etc.
* Input: Git command
* Output: The corresponding success / error message in terminal and
          completes action
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>

using namespace std;

/// These constant variables are self-explanatory. Each is assigned
/// a default value to silence CodeGrade's silly error detection.
const char* TOKEN_GIT = "git";
const char* TOKEN_HELP = "help";
const char* TOKEN_CLONE = "clone";
const char* TOKEN_INIT = "init";
const char* TOKEN_ADD = "add";
const char* TOKEN_MV = "mv";
const char* TOKEN_RM = "rm";
const char* TOKEN_DIFF = "diff";
const char* TOKEN_GREP = "grep";
const char* TOKEN_COMMIT = "commit";
const char* TOKEN_PUSH = "push";
const char* TOKEN_TO = "to";

const char* TOKENS[12] = {
        TOKEN_GIT,
        TOKEN_HELP,
        TOKEN_CLONE,
        TOKEN_INIT,
        TOKEN_ADD,
        TOKEN_MV,
        TOKEN_RM,
        TOKEN_DIFF,
        TOKEN_GREP,
        TOKEN_COMMIT,
        TOKEN_PUSH,
        TOKEN_TO
};

const int TOKENS_SIZE = size(TOKENS);

const char* WARNING_GENERAL_DIRECTORY = "WARNING: make sure a working "
                                        "directory is already opened.";

const char* WARNING_ADD_SYNTAX = "WARNING: possible syntax error found "
                                 "in git add.";

const char* WARNING_ADD_MISSING_TO = "WARNING: missing 'to' in command, "
                                     "found 'add' instead.";

const char* ERROR_GENERAL_INVALID = "ERROR: invalid command";

const char* ERROR_ADD_REPOSITORY = "ERROR: cannot add a file because "
                                   "repository does not exist.";

const char* ERROR_ADD_INVALID_NUM_CLI = "ERROR: invalid number of "
                                        "command-line arguments for"
                                        " git add.";

const char* ERROR_ADD_MISSING_DIRECTORY = "ERROR: syntax error found "
                                          "in git add. Missing name "
                                          "of directory.";



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

void removeFileFromDirectory(vector<char*>& files, Directory& dir,
                             bool& flag);

bool searchInFile(const char* str, Position& position, File& file);

bool replaceInFile(const char* str, Position& position, File& file);

bool inArray(const char** haystack, const char* needle, int size);

void printArray(char* arr[], int size);


///@fn int main(int, char**)
///@brief Entry point of our application.
///@param[in]argc the number of command-line argument passed to
/// the program.
///@param[in]argv the vector of command-line arguments.
///@return SUCCESS if the program terminated successfully
/// or FAILURE otherwise.
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
    // Contains all changes made to files which are ready to be pushed!
    // (a.k.a supercommit)
    vector<Position> commits;
    // Contains the parsed command, that is, a sequence of tokens that
    // you should process and evaluate individually
    char* commands[MAX_ARGS];
    // <-- add your variables here
    // TODO: use some variables to keep track of
    //  the current working directory and current working file.
    // -->
    Directory workingDirectory;
    File workingFile;
    // We begin at 1 because 0 contains the name of the program, which
    // is not a command!
    for (int i = 1; i < argc; i++) {
        // <--
        // 1. Make sure to validate each command before executing
        // each command
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

        int tokens = parseCommand(argv[i], commands);
        printArray(commands, size(commands));
        // TODO: Write your code here...
        // -->

        int tokenToIndex = -1;
        if (!stringCompare(commands[0], TOKEN_GIT) || (tokens > 1
            && !inArray(TOKENS,commands[1],
                        TOKENS_SIZE))) {
            cout << ERROR_GENERAL_INVALID << endl;
            continue;
        }

        // if "git" or "git help" is passed, print usage
        if (commands[1][0] == '\0' || stringCompare(commands[1],
                                                    TOKEN_HELP)) {
            printUsage();
            continue;
        }

        // runs if "git add" is passed
        if (stringCompare(commands[1], TOKEN_ADD)) {
            // no argument provided after "git add"
            if (commands[2][0] == '\0') {
                cout << WARNING_ADD_SYNTAX << endl;
                cout << WARNING_ADD_MISSING_TO << endl;
                cout << WARNING_GENERAL_DIRECTORY << endl;
                cout << ERROR_ADD_REPOSITORY << endl;
                continue;
            }

            // start at 3rd token to look through files until
            // TOKEN_TO is reached
            for (int j = 2; j < tokens; j++) {
                // if TOKEN_TO does exist
                if (stringCompare(commands[j],
                                  TOKEN_TO)) {
                    tokenToIndex = j;
                    break;
                }
            }

            // if TOKEN_TO does not exist
            if (tokenToIndex == -1) {
                cout << WARNING_ADD_SYNTAX << endl;
                cout << WARNING_ADD_MISSING_TO << endl;
                cout << WARNING_GENERAL_DIRECTORY << endl;
                cout << ERROR_ADD_REPOSITORY << endl;
                continue;
            }

            // if directory is not provided
            cout << "STarting" << endl;
            if (commands[tokenToIndex + 1][0] == '\0')
                cout << "if: /0" << endl;
            else
                cout << "else: " << commands[tokenToIndex + 1][0] << endl;

            if (commands[tokenToIndex + 1][0] == '\0') {
                cout << "WSG" << endl;
                cout << ERROR_ADD_INVALID_NUM_CLI << endl;
                cout << ERROR_ADD_MISSING_DIRECTORY << endl;
                continue;
            }
        }
    }
    return SUCCESS;
}

// TODO: write the implementation of each function here!
int parseCommand(const char* str, char** tokens) {
    int argc = 0;

    int i = 0;
    int charIndex = 0;
    tokens[charIndex] = new char[MAX_STR_LEN];
    while (str[i] != '\0') {
        const char cur[] = {str[i], '\0'};

        if (stringCompare(cur, " ")) {
            tokens[charIndex][i] = '\0';
            argc++;
            charIndex = 0;
        } else {
            tokens[charIndex][i] = str[i];
            charIndex++;
        }
        tokens[charIndex] = new char[MAX_STR_LEN];
        i++;
    }
    return argc;
}

bool makeDirectory(const char* name, vector<Directory>& directories) {
    Directory directory = { .name = name};
    directories.push_back(directory);
    return true;
}

int findDirectory(const char* name, const vector<Directory>& directories) {
    for (int i = 0; i < directories.size(); i++) {
        if (directories[i].name == name) {
            return i;
        }
    }

    return -1;
}

int findFileInDirectory(const char* name, const Directory& dir) {
    const vector<File> files = dir.files;
    for (int i = 0; i < files.size(); i++) {
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
    // Compare lengths first (quick check)
    int len1 = strLength(s1);
    int len2 = strLength(s2);

    if (len1 != len2) {
        return false;
    }

    // Compare characters one by one
    for (int i = 0; i < len1; i++) {
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

void removeFileFromDirectory(vector<char*>& files, Directory& dir,
                             bool& flag) {
    flag = true;
    // iterate backwards for proper traversal while removing elements
    for (size_t i = files.size() - 1; i > 0; i--) {
        const char* fileToBeRemoved = files[i];
        for (size_t j = dir.files.size() - 1; j > 0; j--) {
            const char* curFile = dir.files[j].name.c_str();
            if (fileToBeRemoved == curFile) {
                // remove file
                files.erase(files.begin() + static_cast<long>(i));
                dir.files.erase(dir.files.begin() + static_cast<long>(j));
                std::remove(fileToBeRemoved);
            }
        }
    }

    if (!files.empty()) {
        flag = false;
    }
}

bool searchInFile(const char* str, Position& position, File& file) {
    for (int i = 0; i < file.lines.size(); i++) {
        if (file.lines[i].find(str) != string::npos) {
            position.line = i;
            position.column = static_cast<int>(file.lines[i].find(str));
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

bool inArray(const char** haystack, const char* needle, const int size) {
    for (int i = 0; i < size; i++) {
        if (stringCompare(haystack[i], needle)) {
            return true;
        }
    }
    return false;
}

void printArray(char* arr[], const int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < strLength(arr[i]); j++) {
            const char cur = arr[i][j];
            if (cur == '\0') {
                cout << "/0 " << endl;
            } else {
                cout << cur << ' ' << endl;
            }
        }
    }
}