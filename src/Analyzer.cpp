#include "./include/Analyzer.h"

#ifdef _WIN32
#include <windows.h>
#include <TlHelp32.h>
#include <winnt.h>
#include <wintrust.h>
#pragma comment(lib, "wintrust.lib")
#endif

#ifdef __LINUX
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#endif

#ifdef __APPLE__
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <signal.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;
using namespace AnalyzerInternals;

Analyzer::Analyzer()
{
}

Analyzer::~Analyzer()
{
}

bool Analyzer::AnalyzeCreatedBinary(const string& binaryPath)
{
    #ifdef _WIN32
    // analyze binary on Windows
    if (binaryPath.empty())
    {
        cerr << "Error: Binary path is empty" << endl;
        return false;
    }

    // check name and size of the binary
    WIN32_FIND_DATA findData;

    wchar_t* wtext = new wchar_t[binaryPath.size() + 1];
    copy(binaryPath.begin(), binaryPath.end(), wtext);
    wtext[binaryPath.size()] = L'\0';


    HANDLE hFind = FindFirstFile(binaryPath.c_str()/*wtext*/, &findData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        cerr << "Error: FindFirstFile failed" << endl;
        return false;
    }

    std::cout << "Binary name: " << findData.cFileName << endl;
    std::cout << "Binary size: " << findData.nFileSizeLow << " bytes" << endl;

    // check if binary is executable
    if (!(findData.dwFileAttributes & /*FILE_ATTRIBUTE_EXECUTABLE*/ FILE_ATTRIBUTE_VIRTUAL))
    {
        cerr << "Error: Binary is not executable" << endl;
        return false;
    }

    // check if binary is signed
    WINTRUST_FILE_INFO fileData;
    memset(&fileData, 0, sizeof(fileData));
    const GUID GUID_VERIFICATION_ACTION = { 0x12345678, 0x1234, 0x1234, { 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12 } };

    fileData.cbStruct = sizeof(fileData);
    fileData.pcwszFilePath = reinterpret_cast<LPCWSTR>(binaryPath.c_str());
    fileData.hFile = NULL;
    fileData.pgKnownSubject = NULL;

    GUID guidAction = GUID_VERIFICATION_ACTION;
    WINTRUST_DATA trustData;
    memset(&trustData, 0, sizeof(trustData));

    trustData.cbStruct = sizeof(trustData);
    trustData.pPolicyCallbackData = NULL;
    trustData.pSIPClientData = NULL;
    trustData.dwUIChoice = WTD_UI_NONE;
    trustData.fdwRevocationChecks = WTD_REVOKE_NONE;
    trustData.dwUnionChoice = WTD_CHOICE_FILE;
    trustData.dwStateAction = WTD_STATEACTION_VERIFY;
    trustData.hWVTStateData = NULL;
    trustData.pwszURLReference = NULL;
    trustData.dwProvFlags = WTD_SAFER_FLAG;
    
    trustData.pFile = &fileData;

    LONG result = WinVerifyTrust(NULL, &guidAction, NULL);
    if (result != ERROR_SUCCESS)
    {
        cerr << "Error: Binary is not signed" << endl;
        return false;
    }
    std::cout << "Binary is signed" << endl;
    return true;
    #endif

    #ifdef __LINUX
    // analyze binary on Linux
    if (binaryPath.empty())
    {
        cerr << "Error: Binary path is empty" << endl;
        return false;
    }

    // check name and size of the binary
    struct stat fileStat;
    if (stat(binaryPath.c_str(), &fileStat) != 0)
    {
        cerr << "Error: stat failed" << endl;
        return false;
    }

    cout << "Binary name: " << binaryPath << endl;
    cout << "Binary size: " << fileStat.st_size << " bytes" << endl;

    // check if binary is executable
    if (!(fileStat.st_mode & S_IXUSR))
    {
        cerr << "Error: Binary is not executable" << endl;
        return false;
    }

    // check if binary is signed
    string command = "codesign -v " + binaryPath;
    int status = system(command.c_str());

    if (status != 0)
    {
        cerr << "Error: Binary is not signed" << endl;
        return false;
    }
    cout << "Binary is signed" << endl;
    return true;
    #endif

    #ifdef __APPLE__
    // analyze binary on macOS
    if (binaryPath.empty())
    {
        cerr << "Error: Binary path is empty" << endl;
        return false;
    }

    // check name and size of the binary on macOS not using stat
    std::string binaryName = binaryPath.substr(binaryPath.find_last_of("/\\") + 1);
    cout << "Binary name: " << binaryName << endl;

    FILE* file = fopen(binaryPath.c_str(), "rb");
    if (file == NULL)
    {
        cerr << "Error: fopen failed" << endl;
        return false;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    cout << "Binary size: " << fileSize << " bytes" << endl;
    fclose(file);

    // check if binary is executable
    if (access(binaryPath.c_str(), X_OK) != 0)
    {
        cerr << "Error: Binary is not executable" << endl;
        return false;
    }

    // check if binary is signed
    string command = "codesign -v " + binaryPath;
    int status = system(command.c_str());

    if (status != 0)
    {
        cerr << "Error: Binary is not signed" << endl;
        return false;
    }
    cout << "Binary is signed" << endl;
    return true;
    #endif
}

bool Analyzer::AnalyzeWholeProject(const string& projectPath)
{
    return false;
}