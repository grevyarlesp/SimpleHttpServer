#include "HttpProcessor.h"

map<string, string> HttpProcessor::data;
UserManager HttpProcessor::usm;
FileType HttpProcessor::ft;

string HttpProcessor::toHex(size_t s) {
    string ans;
    size_t tmp;
    while (s) {
        tmp = s % 16;
        if (tmp > 9) {
            ans = char(tmp - 10 + 'A') + ans;
        } else ans = char(tmp + '0') + ans;
        
        s /= 16;
    }
    return ans;
}


void HttpProcessor::process(QTcpSocket* socket, char *msg, qint64 sz, string& response) {
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
            response = HttpGenerator::htmlString(404, "./pages/404.html", "text/html");
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
            fm.generate("download/", tmp, socket);

            cout << "File view request - Transfer encoding chunked\n";
            /* content = "favicon.ico"; */
            /* content = "./pages" + content; */
            /* response = HttpGenerator::htmlString(200, content, ft.getFileType(content)); */
            response.clear();
            return;
        }
        if (content.back() == '/') {
            // You want a directory?
            // Send with transfer encoding

            cout << "Directory view request - Transfer encoding chunked\n";
            fm.generate(content, tmp, socket);

            /* content = "favicon.ico"; */
            /* content = "./pages" + content; */
            /* response = HttpGenerator::htmlString(200, content, ft.getFileType(content)); */
            response.clear();
            return;
        }
        // Not a directory then ...
        int tmp = -1;
        /* cout << content << '\n'; */
        for (int i = 1; i < (int) content.size(); ++i) {
            if (content[i] == '/') {
                tmp = i;
                break;
            }
        }
        cout << tmp << '\n';
        if (tmp != -1 && content.substr(0, tmp) == "/download") {
            // To Transfer-Encoding Chunked
            content = "./pages" + content;
            response = HttpGenerator::header(200, -1, ft.getFileType(content), 1);
            /* response.pop_back(); */
            /* response += "Content-Disposition: attachment\r\n\n"; */
            if (socket->isOpen()) socket->write(response.c_str(), (qint64) response.size());
            /* cout << response; */
            ifstream ifs(content);
            string tmp, tmp2;
            char c;
            /* string tmp, tmp2, tmp3; */
            /* bool first = true; */
            /* while (getline(ifs, tmp3)) { */
            /*     if (first) { */
            /*         tmp = tmp3; */
            /*         first = false; */
            /*         continue; */
            /*     } */
            /*     tmp.push_back('\n'); */
            /*     tmp2 = toHex(tmp.size()) + "\r\n"; */
            /*     socket->write(tmp2.c_str(), (qint64) tmp2.size()); */
            /*     /1* cout << tmp2; *1/ */
            /*     tmp.append("\r\n"); */
            /*     /1* cout << tmp; *1/ */
            /*     socket->write(tmp.c_str(), (qint64) tmp.size()); */
            /*     tmp = tmp3; */
            /* } */
            /* tmp2 = toHex(tmp.size()) + "\r\n"; */
            /* socket->write(tmp2.c_str(), (qint64) tmp2.size()); */
            /* /1* cout << tmp2; *1/ */
            /* tmp.append("\r\n"); */
            /* /1* cout << tmp; *1/ */
            /* socket->write(tmp.c_str(), (qint64) tmp.size()); */

            /* Send 1KB at a time */
            int counter = 0;
            while (ifs.get(c)) {
                ++counter;
                tmp.push_back(c);
                if (counter == 1024) {
                    counter = 0;
                    tmp2 = toHex(1024) + "\r\n";
                    socket->write(tmp2.c_str(), (qint64) tmp2.size());
                    /* cout << tmp2; */
                    tmp.append("\r\n");
                    /* cout << tmp; */
                    socket->write(tmp.c_str(), (qint64) tmp.size());
                    tmp.clear();
                }
            }
            if (counter) {
                tmp2 = toHex(tmp.size()) + "\r\n";
                socket->write(tmp2.c_str(), (qint64) tmp2.size());
                /* cout << tmp2; */
                tmp.append("\r\n");
                /* cout << tmp; */
                socket->write(tmp.c_str(), (qint64) tmp.size());
            }
            ifs.close();
            tmp2 = "0\r\n\r\n";
            /* cout << tmp2 << '\n'; */
            socket->write(tmp2.c_str(), (qint64) tmp2.size());
            cout << "Download request - Transfer encoding chunked\n";
            response.clear();
            return;
        }
        cout << "Send files with Content-Length\n";
        content = "./pages" + content;
        response = HttpGenerator::htmlString(200, content, ft.getFileType(content));
    }
}

