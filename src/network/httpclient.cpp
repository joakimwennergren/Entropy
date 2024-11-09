#include "httpclient.hpp"

using namespace Entropy::Network;

HttpClient::HttpClient() = default;

int HttpClient::Get(const std::string &url) {
  if (CURL *curl = curl_easy_init()) {
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
  return 0;
}
