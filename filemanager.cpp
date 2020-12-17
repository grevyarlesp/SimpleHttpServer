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

string FileManager::toHex(size_t s) {
    string ans;
    size_t tmp;
    while (s) {
        tmp = s % 16;
        if (tmp > 9) {
            ans = char(tmp - 10 + 'A') + ans;
        } else ans = char(tmp + '0') + ans;
        
        s /= 16;
    }
    if (ans.empty()) ans = "0";
    return ans;
}


void FileManager::writeChunk(QTcpSocket* socket, string& chunk, bool header) {
    string tmp = toHex(chunk.size()) + "\r\n";
    if (! header)
    chunk = chunk + "\r\n";
    /* cout << tmp << '\n'; */
    socket->write(tmp.c_str(), qint64(tmp.size()));
    socket->write(chunk.c_str(), qint64(chunk.size()));
    cout << tmp << chunk;
}

void FileManager::framing(QTcpSocket* socket, string &location, QFileInfo &info, string &ans) {
    /* <tr> */
    /*     <td><a href="./08. Networking Media.pptx">8. Networking Media.pptx</a></td> */
    /*     <td align="center">12/16/2020</td> */
    /*     <td align="center">3,071K</td> */
    /*     <td>&nbsp;</td> */
    /* </tr> */

    ans = "<tr>\n";
    writeChunk(socket, ans);
    if (! info.isDir()) {
        ans = "<td><a href=\"" + location  + info.fileName().toStdString() + "\">" + info.fileName().toStdString() + "</a></td>\n";
    }
    else {
        ans = "<td><a href=\"" + location  + info.fileName().toStdString() + "/\">" + info.fileName().toStdString() + "</a></td>\n";
    }
    writeChunk(socket, ans);
    ans = "<td align=\"center\">" + info.lastModified().toString("ddd MMMM d yyyy hh:mm:ss").toStdString() + "</td>\n";
    writeChunk(socket, ans);
    ans = "<td align=\"center\">" + QString::number(info.size()).toStdString() + " bytes </td>\n";
    writeChunk(socket, ans);
    ans = "<td>&nbsp;</td>\n";
    writeChunk(socket, ans);
    ans = "</tr>\n";
    writeChunk(socket, ans);
}

void FileManager::generate(string location, string &ans, QTcpSocket *socket) {
    QDir dir((root + location).c_str());
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    /* std::cout << "     Bytes Filename" << std::endl; */
    string tmp = HttpGenerator::header(200, 0, "text/html", 1);
    ans = prefix;
    writeChunk(socket, tmp, 1);
    writeChunk(socket, prefix);
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        framing(socket, location, fileInfo, tmp);
        /* if (socket == NULL) ans += tmp; */
    }
    writeChunk(socket, suffix);
    /* cout << suffix << '\n'; */
    /* cout << "== Ending =="; */
    ans.clear();
    writeChunk(socket, ans);

}

void FileManager::writeHtmlTo(string dir, string htmlDir) {
    string htmlAns; 
    generate(dir, htmlAns);
    ofstream ofs(root + htmlDir);
    ofs << htmlAns;
    /* cout << "html " + htmlAns << '\n'; */
    ofs.close();
}

