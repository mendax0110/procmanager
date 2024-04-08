#ifndef ANALYZER_H
#define ANALYZER_H

#include <iostream>
#include <vector>
#include <cstdlib>

namespace AnalyzerInternals
{
    class Analyzer
    {
    public:
        Analyzer();
        ~Analyzer();
        static bool AnalyzeCreatedBinary(const std::string& binaryPath);
        static bool AnalyzeWholeProject(const std::string& projectPath);

    private:

    protected:

    };
}

#endif // ANALYZER_H