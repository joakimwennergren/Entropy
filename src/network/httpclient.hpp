#pragma once

#include <curl/curl.h>
#include <spdlog/spdlog.h>
#include <string>

namespace Entropy::Network {
  class HttpClient {
  public:
    HttpClient();

    static int Get(const std::string &url);
  };
}
