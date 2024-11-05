#pragma once

#include <curl/curl.h>
#include <spdlog/spdlog.h>
#include <string>

namespace Entropy {
namespace Network {
class HttpClient {

public:
  HttpClient();
  int Get(std::string url);

private:
};

} // namespace Network
} // namespace Entropy
