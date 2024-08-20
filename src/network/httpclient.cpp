#include "httpclient.hpp"

using namespace Entropy::Network;

HttpClient::HttpClient()
{

}

int HttpClient::Get(std::string url)
{
	CURL *curl = curl_easy_init();
	if(curl) {
	  CURLcode res;
	  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	  res = curl_easy_perform(curl);

	  spdlog::info(res);
	  curl_easy_cleanup(curl);
	}

	return 0;

}
