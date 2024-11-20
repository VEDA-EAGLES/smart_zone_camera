
#include "httpClient.h"

HTTPClient::HTTPClient() {}
HTTPClient::~HTTPClient() {}

void HTTPClient::startHTTPClient(httplib::Client& cli)
{  
    mtx.lock();
    auto res = cli.Post("/", "periodic http", "text/plain");
    if (res) {  
    } else {
        std::cout << "Error: " << res.error() << std::endl;
    }
    mtx.unlock();
}
