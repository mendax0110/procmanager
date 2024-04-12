#include "./include/CmakeCreator.h"

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#endif

#ifdef __LINUX
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

#ifdef __APPLE__
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
using namespace CmakeCreatorInternals;

CmakeCreator::CmakeCreator()
{
}

CmakeCreator::~CmakeCreator()
{
}

bool CmakeCreator::CreateCmakeFile(const string& projectName, const string& projectPath)
{
    string cmakeFile = projectPath + "/CMakeLists.txt";
    ofstream file(cmakeFile);

    if (!file.is_open())
    {
        cerr << "Error: Failed to create CMakeLists.txt" << endl;
        return false;
    }

    file << "cmake_minimum_required(VERSION 3.10)" << endl;
    file << "project(" << projectName << ")" << endl;
    file << endl;
    file << "set(CMAKE_CXX_STANDARD 17)" << endl;
    file << endl;
    
    // find all the header files
    vector<string> headerFiles;
    #ifdef _WIN32
    string searchPath = projectPath + "\\include";
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile((searchPath + "\\*").c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                continue;
            }

            string fileName(findData.cFileName);
            string fileExtension = fileName.substr(fileName.find_last_of(".") + 1);

            if (fileExtension == "h" || fileExtension == "hpp")
            {
                headerFiles.push_back(fileName);
            }
        } while (FindNextFile(hFind, &findData) != 0);

        FindClose(hFind);
    }
    #endif

    #ifdef __LINUX
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir((projectPath + "/include").c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            string fileName(ent->d_name);
            string fileExtension = fileName.substr(fileName.find_last_of(".") + 1);

            if (fileExtension == "h" || fileExtension == "hpp")
            {
                headerFiles.push_back(fileName);
            }
        }

        closedir(dir);
    }
    #endif

    #ifdef __APPLE__
    DIR* dir;
    struct dirent* ent;

    if ((dir = opendir((projectPath + "/include").c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            string fileName(ent->d_name);
            string fileExtension = fileName.substr(fileName.find_last_of(".") + 1);

            if (fileExtension == "h" || fileExtension == "hpp")
            {
                headerFiles.push_back(fileName);
            }
        }

        closedir(dir);
    }
    #endif

    // find all the source files
    vector<string> sourceFiles;

    #ifdef _WIN32
    searchPath = projectPath + "\\src";
    hFind = FindFirstFile((searchPath + "\\*").c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                continue;
            }

            string fileName(findData.cFileName);
            string fileExtension = fileName.substr(fileName.find_last_of(".") + 1);

            if (fileExtension == "cpp")
            {
                sourceFiles.push_back(fileName);
            }
        } while (FindNextFile(hFind, &findData) != 0);

        FindClose(hFind);
    }
    #endif

    #ifdef __LINUX
    string searchPath = projectPath + "/src";

    if ((dir = opendir(searchPath.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            string fileName(ent->d_name);
            string fileExtension = fileName.substr(fileName.find_last_of(".") + 1);

            if (fileExtension == "cpp")
            {
                sourceFiles.push_back(fileName);
            }
        }

        closedir(dir);
    }
    #endif

    #ifdef __APPLE__
    string searchPath = projectPath + "/src";

    if ((dir = opendir(searchPath.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            string fileName(ent->d_name);
            string fileExtension = fileName.substr(fileName.find_last_of(".") + 1);

            if (fileExtension == "cpp")
            {
                sourceFiles.push_back(fileName);
            }
        }

        closedir(dir);
    }
    #endif

    // add the header files to the CMakeLists.txt
    if (!headerFiles.empty())
    {
        file << "include_directories(include)" << endl;
        file << "set(HEADER_FILES" << endl;

        for (const string& headerFile : headerFiles)
        {
            file << "    include/" << headerFile << endl;
        }

        file << ")" << endl;
        file << endl;
    }

    // add the source files to the CMakeLists.txt
    if (!sourceFiles.empty())
    {
        file << "set(SOURCE_FILES" << endl;

        for (const string& sourceFile : sourceFiles)
        {
            file << "    src/" << sourceFile << endl;
        }

        file << ")" << endl;
        file << endl;
    }

    // add the executable to the CMakeLists.txt
    file << "add_executable(" << projectName << " ${SOURCE_FILES} ${HEADER_FILES})" << endl;
    file.close();

    return true;
}

int CmakeCreator::CreateBuildDirAndBuild(const string& projectPath)
{
    try
    {
        #ifdef _WIN32
        string buildDir = projectPath + "\\build";
        _mkdir(buildDir.c_str());
        #endif

        #ifdef __LINUX
        string buildDir = projectPath + "/build";
        mkdir(buildDir.c_str(), 0777);
        #endif

        #ifdef __APPLE__
        string buildDir = projectPath + "/build";
        mkdir(buildDir.c_str(), 0777);
        #endif

        #ifdef _WIN32
        string command = "cd " + buildDir + " && cmake .. && cmake --build .";
        return system(command.c_str());
        #endif

        #ifdef __LINUX
        string command = "cd " + buildDir + " && cmake .. && make";
        return system(command.c_str());
        #endif

        #ifdef __APPLE__
        string command = "cd " + buildDir + " && cmake .. && cmake --build .";
        return system(command.c_str());
        #endif
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    return 0;
}

std::string CmakeCreator::DetectCmakeAndCompiler()
{
    #ifdef _WIN32
    string cmakePath = "cmake";
    string compilerPath = "cl";
    #endif

    #ifdef __LINUX
    string cmakePath = "cmake";
    string compilerPath = "g++";
    #endif

    #ifdef __APPLE__
    string cmakePath = "cmake";
    string compilerPath = "clang++";
    #endif

    return cmakePath + " " + compilerPath;
}

bool CmakeCreator::CreateGitIgnore(const string& projectPath)
{
    string gitIgnoreFile = projectPath + "/.gitignore";
    ofstream file(gitIgnoreFile);

    if (!file.is_open())
    {
        cerr << "Error: Failed to create .gitignore" << endl;
        return false;
    }

    file << "build/" << endl;
    file << ".vscode/" << endl;
    file << ".idea/" << endl;
    file << "*.suo" << endl;
    file << "*.sdf" << endl;
    file << "*.user" << endl;
    file << "*.vcxproj" << endl;
    file << "*.vcxproj.filters" << endl;
    file << "*.vcxproj.user" << endl;
    file << "*.cmake" << endl;
    file << "*.out" << endl;
    file << "*.exe" << endl;
    file << "*.dll" << endl;
    file << "*.so" << endl;
    file << "*.dylib" << endl;
    file << "*.a" << endl;
    file << "*.lib" << endl;
    file << "*.pdb" << endl;
    file << "*.ipch" << endl;
    file << "*.log" << endl;
    file << "*.txt" << endl;
    file << "*.log" << endl;
    file << "*.zip" << endl;
    file << "*.7z" << endl;
    file << "*.rar" << endl;
    file << "*.tar.gz" << endl;
    file << "*.tar.xz" << endl;
    file << "*.tar.bz2" << endl;
    file << "*.tar" << endl;
    file << "*.gz" << endl;
    file << "*.xz" << endl;
    file << "*.bz2" << endl;
    file << "*.pdf" << endl;
    file << "*.doc" << endl;
    file << "*.docx" << endl;
    file << "*.ppt" << endl;
    file << "*.pptx" << endl;
    file << "*.xls" << endl;
    file << "*.xlsx" << endl;
    file << "*.jpg" << endl;
    file << "*.jpeg" << endl;
    file << "*.png" << endl;
    file << "*.gif" << endl;
    file << "*.bmp" << endl;
    file << "*.tiff" << endl;
    file << "*.tif" << endl;
    file << ".DS_Store" << endl;
    file << ".vscode/" << endl;

    file.close();

    return true;
}

bool CmakeCreator::CreateReadMe(const string& projectPath)
{
    string readMeFile = projectPath + "/README.md";
    ofstream file(readMeFile);

    if (!file.is_open())
    {
        cerr << "Error: Failed to create README.md" << endl;
        return false;
    }

    file << "# " << projectPath.substr(projectPath.find_last_of("/") + 1) << endl;
    file << endl;
    file << "## Description" << endl;
    file << endl;
    file << "## Build Instructions" << endl;
    file << endl;
    file << "## Run Instructions" << endl;
    file << endl;
    file << "## License" << endl;
    file << endl;
    file << "## Author" << endl;

    file.close();

    return true;
}

