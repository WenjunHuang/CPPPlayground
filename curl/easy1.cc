//
// Created by rick on 2021/4/28.
//
#include <QtCore/QtCore>
#include <curl/curl.h>

size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp)
{
    auto realSize = size * nmemb;
    auto userBuffer = reinterpret_cast<QBuffer*>(userp);
    userBuffer->write((char*)buffer, realSize);
    return realSize;
}

int main()
{
    auto easyhandle = curl_easy_init();
    auto userBuffer = new QBuffer {};
    userBuffer->open(QBuffer::ReadWrite);
    curl_easy_setopt(easyhandle, CURLOPT_URL, "https://www.youtube.com");
    curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, userBuffer);

    // 设置代理
    curl_easy_setopt(easyhandle, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
    curl_easy_setopt(easyhandle, CURLOPT_PROXY, "127.0.0.1:9797");

    // 开始干活
    auto code = curl_easy_perform(easyhandle);
    curl_easy_cleanup(easyhandle);
    if (code != CURLE_OK) {
        qInfo() << "Failed";
    } else {
        QString content { userBuffer->data() };
        qInfo() << content;
    }
}