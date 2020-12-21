#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <QDir>
#include <QDirIterator>
#include <map>
#include <QDateTime>
#include <QTcpSocket>
#include "HttpGenerator.h"

using namespace std;

class FileManager {
private:
    string fileIcon(string name);
    string toHex(size_t s);
    void writeChunk(QTcpSocket*socket, string &chunk, bool header=0);
    void framing(QTcpSocket* socket, string &location, QFileInfo &info, string &ans);
    QDir dir;

    string location;

    static string root;
    static string prefix;
    static string suffix;
    static map<string, string> iconMap;
public:
    FileManager();
    /* Generate an HTML file */
    void generate(string location, string &ans, QTcpSocket *socket = NULL);

    /* Write the html file */
    void writeHtmlTo(string dir, string htmlDir);
};

#endif // FILEMANAGER_H
