#include "HttpProcessor.h"

map<string, string> HttpProcessor::data;
UserManager HttpProcessor::usm;
FileType HttpProcessor::ft;

void HttpProcessor::process(char *msg, qint64 sz, string& response) {
    string request;
    string content;
    int i = 0;
    for (; i < sz && (msg[i] == ' ' || msg[i] == '\n'); ++i);
    for (; i < sz; ++i) {
        if (msg[i] == ' ') 
            break;
        request += msg[i];
    }
    ++i;
    for (; i < sz; ++i) {
        if (msg[i] == ' ') 
            break;
        content += msg[i];
    }
    string tmp;

    for (; i < sz; ++i) {
        if (msg[i] == ':') {
            i += 2;
            string tmp2;
            while (msg[i] != ',' && msg[i] != '\n') {
                tmp2 += msg[i++];
            }
            data[tmp] = tmp2;
            /* if (tmp == "Accept") cerr << tmp2 << '\n'; */
            tmp.clear();
            continue;
        }
        if (msg[i] == '\n') {
            tmp.clear();
        }
        tmp += msg[i];
    }

    if (request == "POST") {
        string usr, pwd;
        int i = 0;
        cout << "Login in...\n";
        for (; i < (int) tmp.size() && tmp[i] != '&'; ++i);
        usr = tmp.substr(0, i);
        usr = usr.substr(7);
        int j = i + 1;
        for (; j < (int) tmp.size() && tmp[j] != '&'; ++j);
        i += 1;
        pwd = tmp.substr(i, j - i);
        pwd = pwd.substr(4);
        string reformated;
        int ii;
        char ch;
        for (int i = 0; i < (int) pwd.size(); ++i) {
            if (pwd[i] == '%') {
                sscanf(pwd.substr(i + 1, 2).c_str(), "%x", &ii);
                ch = static_cast<char>(ii);
                reformated += ch;
                i = i + 2;
            }
            else reformated += pwd[i];
        }
        pwd = reformated;
        cout << usr << ' ' << pwd << '\n';
        if (! usm.check(usr, pwd)) {
            response = HttpGenerator::htmlString(404, "./pages/404/index.html", "text/html");
        }
        else {
            response = HttpGenerator::redirection("/Information/index.html", "./pages/Information/index.html");
        }
        return; 
    }
    
    if (request == "GET") {
        if (content == "/") {
            response = HttpGenerator::redirection("/Login/index.html", "./pages/Login/index.html");
            return;
        }
        string reformated;
        int ii;
        char ch;
        for (int i = 0; i < (int) content.size(); ++i) {
            if (content[i] == '%') {
                sscanf(content.substr(i+1,2).c_str(), "%x", &ii);
                ch = static_cast<char>(ii);
                reformated += ch;
                i = i + 2;
            }
            else reformated += content[i];
        }
        content = reformated;
        if (content == "/files.html") {
            string tmp;
            fm.generate("download/", tmp);
            response = HttpGenerator::header(200, tmp.size(), "text/html");
            response += tmp;
            return;
        }
        if (content.back() == '/') {
            // You want a directory?
            string tmp;
            fm.generate(content, tmp);
            response = HttpGenerator::header(200, tmp.size(), "text/html");
            response += tmp;
            return;
        }
        // Not a directory then ... 
        int tmp = content.find('/');
        if (content.substr(0, tmp) == "download") {
            // To Transfer-Encoding Chunked
            content = "./pages" + content;
            return;
        }
        content = "./pages" + content;
        response = HttpGenerator::htmlString(200, content, ft.getFileType(content));
    }
}

