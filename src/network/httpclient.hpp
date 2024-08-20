#pragma once

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <spdlog/spdlog.h>
namespace Entropy
{
    namespace Network
    {
        class HttpClient
        {
        public:
            HttpClient();

            int Get(std::string url);

        private:

        };


    }
}
