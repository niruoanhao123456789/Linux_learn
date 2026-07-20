#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <functional>
#include <unordered_map>
#include "HttpProtocol.hpp"
#include "TcpServer.hpp"
#include "Logger.hpp"

using namespace LogModule;

using route_t = std::function<void(const HttpRequest &req, HttpResponse &resp)>;

class HttpServer
{
public:
    HttpServer(uint16_t port)
        : _port(port),
          _tsvr(std::make_unique<TcpServer>(port))
    {
    }
    // 1. 分析收到的字节流是否有完整的HTTP报文
    // 2. http request -> http response
    std::string HandlerHttpRequest(std::string &streamstr)
    {
        std::cout << "HandlerHttpRequest: \r\n"
                  << streamstr << std::endl;

        // 1. 报文完整性 - 略
        // 2. 反序列化
        HttpRequest httpreq;
        httpreq.Deserialize(streamstr);
        HttpResponse httpresp;

        if (IsNeedRoute(httpreq["path"])) // POST: /login GET: /login   ?username=xx&passwd=yy
        {
            // 处理动态资源
            _route[httpreq["path"]](httpreq, httpresp);
        }
        else
        {
            // 3. 处理静态资源请求，构建应答httpreq->httpresp
            std::string filecontent = GetFileContentHelper(httpreq["path"]);

            std::string suffix = httpreq["suffix"];
            if (filecontent.empty())
            {
                httpresp.SetCode(301);
                httpresp.SetHeader("Location", "/404.html");
            }
            else
            {
                httpresp.SetCode(200);
                httpresp.SetHeader("Content-Length", filecontent.size());
                httpresp.SetHeader("Content-Type", Suffix2Type(suffix));
                httpresp.SetHeader("Connection", "close");
                httpresp.SetBody(filecontent);
            }
        }

        // 4. 应答序列化
        std::string httprespstr;
        httpresp.Serialize(&httprespstr);

        // 5. 返回
        return httprespstr;
    }
    void Run()
    {
        _tsvr->Run([this](std::string &streamstr) -> std::string
                   { return this->HandlerHttpRequest(streamstr); });
    }
    void Register(std::string uri, route_t handler)
    {
        std::string key = webroot + uri; // wwwroot/login
        _route[key] = handler;
    }
    ~HttpServer() {}

private:
    bool IsNeedRoute(const std::string &key)
    {
        return _route.find(key) != _route.end();
    }
    std::string GetFileContentHelper(const std::string &filename)
    {
        std::ifstream in(filename);
        if (!in.is_open())
        {
            return std::string();
        }
        in.seekg(0, in.end);
        int filesize = in.tellg();
        in.seekg(0, in.beg);

        std::string content;
        content.resize(filesize);
        in.read((char *)content.c_str(), filesize);
        in.close();

        return content;
    }
    // suffix: .html
    // return: text/html
    std::string Suffix2Type(const std::string &suffix)
    {
        // 使用静态映射表提高效率，避免每次调用都重新创建
        static const std::unordered_map<std::string, std::string> mime_map = {
            // 文本类型
            {".html", "text/html"},
            {".htm", "text/html"},
            {".css", "text/css"},
            {".js", "application/javascript"},
            {".mjs", "application/javascript"},
            {".json", "application/json"},
            {".xml", "application/xml"},
            {".txt", "text/plain"},
            {".csv", "text/csv"},
            {".md", "text/markdown"},

            // 图片类型
            {".jpg", "image/jpeg"},
            {".jpeg", "image/jpeg"},
            {".png", "image/png"},
            {".gif", "image/gif"},
            {".bmp", "image/bmp"},
            {".webp", "image/webp"},
            {".svg", "image/svg+xml"},
            {".ico", "image/x-icon"},
            {".tiff", "image/tiff"},
            {".tif", "image/tiff"},

            // 字体类型
            {".woff", "font/woff"},
            {".woff2", "font/woff2"},
            {".ttf", "font/ttf"},
            {".otf", "font/otf"},

            // 视频类型
            {".mp4", "video/mp4"},
            {".webm", "video/webm"},
            {".ogv", "video/ogg"},
            {".avi", "video/x-msvideo"},
            {".mov", "video/quicktime"},
            {".mpeg", "video/mpeg"},
            {".mpg", "video/mpeg"},

            // 音频类型
            {".mp3", "audio/mpeg"},
            {".wav", "audio/wav"},
            {".ogg", "audio/ogg"},
            {".flac", "audio/flac"},
            {".m4a", "audio/mp4"},
            {".aac", "audio/aac"},

            // 应用类型
            {".pdf", "application/pdf"},
            {".zip", "application/zip"},
            {".rar", "application/vnd.rar"},
            {".7z", "application/x-7z-compressed"},
            {".tar", "application/x-tar"},
            {".gz", "application/gzip"},
            {".exe", "application/vnd.microsoft.portable-executable"},
            {".dll", "application/x-msdownload"},
            {".msi", "application/x-msi"},
            {".doc", "application/msword"},
            {".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
            {".xls", "application/vnd.ms-excel"},
            {".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
            {".ppt", "application/vnd.ms-powerpoint"},
            {".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
            {".wasm", "application/wasm"},

            // 其他常见类型
            {".jsonld", "application/ld+json"},
            {".rss", "application/rss+xml"},
            {".atom", "application/atom+xml"},
            {".manifest", "text/cache-manifest"},
            {".map", "application/json"}, // source maps
            {".ts", "video/mp2t"},        // MPEG transport stream
            {".m3u8", "application/vnd.apple.mpegurl"}};

        auto it = mime_map.find(suffix);
        if (it != mime_map.end())
        {
            return it->second;
        }

        // 默认返回二进制流，或根据需求返回其他默认值
        return "application/octet-stream";
    }

private:
    uint16_t _port;
    std::unique_ptr<TcpServer> _tsvr;
    std::unordered_map<std::string, route_t> _route; // 注册服务的容器
};