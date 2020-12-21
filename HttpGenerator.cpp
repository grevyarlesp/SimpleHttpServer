#include "HttpGenerator.h"

map<int, string> statusCode = {
    {200, "OK"},
    {404, "Not found"},
};

string HttpGenerator::header(int code, size_t contentSize, string type, bool chunked) {
    string ans = "HTTP/1.1 " + to_string(code) + ' ' + statusCode[code] + '\n';
    /* ans.append("X-Content-Type-Options: nosniff\n"); */
    ans.append("X-Content-Type-Options: nosniff\n");
    ans.append("Content-Type: " + type + "\n");
    if (! chunked)
        ans.append("Content-Length: " + to_string(contentSize) + '\n');
    else ans.append("Transfer-Encoding: chunked\n");
    ans.push_back('\n');
    return ans;
}

string HttpGenerator::htmlString(int code, string fileDir, string type) {
    ifstream ifs;
    ifs.open(fileDir);
    string tmp, ans;
    string htmlContent;
    if (! ifs) {
        code = 404;
        ifs.open("./pages/404.html", ios::in);
    }
    while (getline(ifs, tmp)) {
        htmlContent += tmp + '\n';
    }
    ans += '\n';
    ifs.close();
    ans = header(code, htmlContent.size(), type) + htmlContent + '\n';
    return ans;
}

string HttpGenerator::redirection(string location, string fileDir) {
    ifstream ifs(fileDir);
    string tmp, ans;
    string htmlContent;
    while (getline(ifs, tmp)) {
        htmlContent += tmp + '\n';
    }
    ifs.close();
    return "HTTP/1.1 303 See Other\nLocation: " + location + '\n' + htmlContent + '\n';
}
