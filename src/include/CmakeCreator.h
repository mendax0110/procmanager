#ifndef CMAKE_CREATOR_H
#define CMAKE_CREATOR_H

#include <iostream>


namespace CmakeCreatorInternals
{
    class CmakeCreator
    {
    public:
        CmakeCreator();
        ~CmakeCreator();
        static bool CreateCmakeFile(const std::string& projectName, const std::string& projectPath);
        static int CreateBuildDirAndBuild(const std::string& projectPath);
        std::string DetectCmakeAndCompiler();
        static bool CreateGitIgnore(const std::string& projectPath);
        static bool CreateReadMe(const std::string& projectPath);

    private:

    protected:

    };
}


#endif // CMAKE_CREATOR_H