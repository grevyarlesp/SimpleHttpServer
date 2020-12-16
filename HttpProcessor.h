#ifndef HTTPPROCESSOR_H
#define HTTPPROCESSOR_H

#include "HttpGenerator.h"
#include "filetype.h"
#include "usermanager.h"
#include <QMetaType>
#include <cstring>
#include <iostream>
#include <string>
#include "filemanager.h"

using namespace std;

/* A helper class for processing http requests */

class HttpProcessor {
    private: 
        FileManager fm;
        static FileType ft;
        static map<string, string> data;
        static UserManager usm;
    public:
        void process(char* msg, qint64 sz, string& response);
};

#endif /* HTTPPROCESSOR_H */
