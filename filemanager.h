#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <QDir>
#include <QDirIterator>
#include <QDateTime>

using namespace std;

class FileManager {
private:
  void framing(string &location, QFileInfo &info, string &ans);
  QDir dir;
  string location;
  static string root;
public:
    FileManager();
    static string prefix;
    static string suffix;
    /* Generate an HTML file */
    void generate(string location, string &ans);

    /* Write the html file */
    void writeHtmlTo(string dir, string htmlDir);
};

#endif // FILEMANAGER_H
