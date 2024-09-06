#include <curl/curl.h>
#include <string>
#include <iostream>
#include "http_client.hpp"

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string *data)
{
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}

std::string httpGet(const std::string &url)
{
    CURL *curl = curl_easy_init(); // Initialize a CURL handle
    std::string response_string;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());             // Set the URL for the request
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction); // Set the callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);  // Pass the string to store the response

        CURLcode res = curl_easy_perform(curl); // Perform the request
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl); // Clean up the CURL handle
    }

    return response_string; // Return the response string
}
