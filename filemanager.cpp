#include "filemanager.h"

string FileManager::root = "./pages/";

string FileManager::prefix = 
"<!DOCTYPE html>"
"<html lang=\"en\">"
"<head>"
    "<title>FILES</title>"
    "<body>"
        "<h1>Computer Network - Files</h1>"
        "<table>"
            "<tbody>"
                "<tr>"
                    "<th><a href="">Name</a></th>"
                    "<th><a href="">Last modified</a></th>"
                    "<th><a href="">Size</a></th>"
                    "<th><a href="">Description</a></th>"
                "</tr>"
                "<tr>"
                    "<th colspan=\"5\">"
                        "<hr>" 
                    "</th>"
                "</tr>";

string FileManager::suffix = 
                "<tr>"
                    "<th colspan=\"5\">"
                        "<hr>"
                    "</th>"
                "</tr>"
            "</tbody>"
        "</table>"
        "<link"
            "href=\"https://fonts.googleapis.com/css?family=Montserrat:400,500,700|Noto+Sans+JP:400,500,700|Noto+Sans+KR:400,500,700|Roboto:400,500,700&amp;display=swap\" rel=\"stylesheet\">"
    "</body>"
"</head>"
"</html>";

FileManager::FileManager() {

}


void FileManager::framing(string &location, QFileInfo &info, string &ans) {
    /* <tr> */
    /*     <td><a href="./08. Networking Media.pptx">8. Networking Media.pptx</a></td> */
    /*     <td align="center">12/16/2020</td> */
    /*     <td align="center">3,071K</td> */
    /*     <td>&nbsp;</td> */
    /* </tr> */

    /* std::cout << qPrintable(QString("%1 %2 %3").arg(fileInfo.size(), 10) */
    /*                                         .arg(fileInfo.fileName()).arg(fileInfo.lastModified().toString("ddd MMMM d yyyy hh:mm:ss"))); */
    /* std::cout << std::endl; */

    ans = "";
    ans += "<tr>\n";
    if (! info.isDir())
        ans += "<td><a href=\"" + location  + info.fileName().toStdString() + "\">" + info.fileName().toStdString() + "</a></td>\n";
    else 
        ans += "<td><a href=\"" + location  + info.fileName().toStdString() + "/\">" + info.fileName().toStdString() + "</a></td>\n";
    ans += "<td align=\"center\">" + info.lastModified().toString("ddd MMMM d yyyy hh:mm:ss").toStdString() + "</td>\n";
    ans += "<td align=\"center\">" + QString::number(info.size()).toStdString() + " bytes </td>\n";
    ans += "<td>&nbsp;</td>\n";
    ans += "</tr>\n";
}

void FileManager::generate(string location, string &ans) {
    QDir dir((root + location).c_str());
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    /* std::cout << "     Bytes Filename" << std::endl; */
    string tmp;
    ans = prefix;
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        framing(location, fileInfo, tmp);
        ans += tmp;
    }
}

void FileManager::writeHtmlTo(string dir, string htmlDir) {
    string htmlAns; 
    generate(dir, htmlAns);
    ofstream ofs(root + htmlDir);
    ofs << htmlAns;
    /* cout << "html " + htmlAns << '\n'; */
    ofs.close();
}

