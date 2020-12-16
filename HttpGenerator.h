#ifndef HTTPGENERATOR_H
#define HTTPGENERATOR_H 

#include <QString>
#include <string>
#include <fstream>
#include <map>

using namespace std;

extern map<int, string> statusCode;

/* A helper class for generating http messages */

class HttpGenerator {
    private:
    public:
        /* Read and display html file */
        static string header(int code, size_t contentSize, string type, bool chunked = 0);
        static string htmlString(int code, string fileDir, string type);
        static string redirection(string location, string fileDir);
};

#endif /* HTTPGENERATOR_H */
