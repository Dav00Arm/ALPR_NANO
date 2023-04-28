#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_map>
#include <iterator>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "string.h"
#include "curl/curl.h"
#include <json/json.h>
// #undef FALSE
#define FALSE 0
// #undef TRUE
#define TRUE 1

/*
    This file's functions are for precossing and sending data 
    to server.
*/
// convert data from unordered map to json.
Json::Value ConvertToJson(std::unordered_map<std::string, std::string> map)
{
    Json::Value json;

    for(auto& element : map)
    {
        json[element.first] = element.second;

    }
    return json;
}

// Send data about free spots to server.
int PostJSON(std::unordered_map<std::string, std::string> frees,std::unordered_map<std::string, std::string> frees_spot)
{
  FILE *wfd = fopen("foo.txt", "w");

  int retcode = FALSE;
  CURL *curl = NULL;
  CURLcode res = CURLE_FAILED_INIT;
  char errbuf[CURL_ERROR_SIZE] = { 0, };
  struct curl_slist *headers = NULL;
  char agent[1024] = { 0, };

////////////////////////////// 
  Json::Value root;
  Json::Value leaf;
  constexpr bool shouldUseOldWay = false;

  // root["mac_address"]= "2C:54:91:88:C9:E3";
  // root["camera_id"] = 1;
  // root["spot_id"] = 1;
  // root["license_number"] = "01AI201";
  // root["confidence"] = 97;
  // root["time"] = "10-01-2003 16:20:30";

  root = ConvertToJson(frees);
  leaf = ConvertToJson(frees_spot);
  root["spot"] = leaf;
  ///
  std::string json_file ;
  if (shouldUseOldWay) 
  {
    Json::FastWriter writer;
    json_file = writer.write(root);
    std::cout << json_file << "still json   \n"<< std::endl;
  } else 
  {
    Json::StreamWriterBuilder builder;
    json_file = Json::writeString(builder, root);
    std::cout << json_file << std::endl;
  }

  
  // char arr[json_file.length() + 1]; 
  // strcpy(arr, json_file.c_str()); 
  // char* f = arr;
  const char* data;
  curl = curl_easy_init();
  if(!curl) 
  {
    fprintf(stderr, "Error: curl_easy_init failed.\n");
    goto cleanup;
  }
    
  // headers = curl_slist_append(headers, "POST /api/camera_info HTTP/1.1");
  // headers = curl_slist_append(headers, "Host: gaz.locator.am/api/camera_info");
  headers = curl_slist_append(headers, "Accept: application/json");
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, "charset: utf-8");

  // headers = curl_slist_append(headers, "Expect:");
  // headers = curl_slist_append(headers, "Content-Type: application/json");
  
  curl_easy_setopt(curl, CURLOPT_URL,"https://gaz.locator.am/api/camera_info");
  
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  // curl_easy_setopt(curl, CURLOPT_HEADER,1L);
  
  data = json_file.c_str();
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);//post data
  
curl_easy_setopt(curl, CURLOPT_WRITEDATA, wfd);
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);


  // curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, json_file.length() +1);//info about size of post

  //   curl_easy_setopt(curl, CURLOPT_READDATA, *data);
  // curl_easy_setopt(curl, CURLOPT_POST, 1L);
  res = curl_easy_perform(curl);
  if(res != CURLE_OK) {
    size_t len = strlen(errbuf);
    fprintf(stderr, "\nlibcurl: (%d) ", res);/////////////////////
    if(len)
      fprintf(stderr, "%s%s", errbuf, ((errbuf[len - 1] != '\n') ? "\n" : ""));
    fprintf(stderr, "%s\n\n", curl_easy_strerror(res));
    goto cleanup;
  }

  // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);//this line is for getting additional helpful info
  // curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);//this line provides extra info about errors

  
  

  retcode = TRUE;
cleanup:
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  // cJSON_Delete(root);
  // free(json);
  return retcode;
}

// Send data about new cars's license plate to server.
int PostJSON(std::unordered_map<std::string, std::string> exm)
{
  FILE *wfd = fopen("foo.txt", "w");

  int retcode = FALSE;
  CURL *curl = NULL;
  CURLcode res = CURLE_FAILED_INIT;
  char errbuf[CURL_ERROR_SIZE] = { 0, };
  struct curl_slist *headers = NULL;
  char agent[1024] = { 0, };

//////////////////////////////
  Json::Value root;
  constexpr bool shouldUseOldWay = false;

  // root["mac_address"]= "2C:54:91:88:C9:E3";
  // root["camera_id"] = 1;
  // root["spot_id"] = 1;
  // root["license_number"] = "01AI201";
  // root["confidence"] = 97;
  // root["time"] = "10-01-2003 16:20:30";

  root = ConvertToJson(exm);
  ///
  std::string json_file ;
  if (shouldUseOldWay) 
  {
    Json::FastWriter writer;
    json_file = writer.write(root);
    std::cout << json_file << "still json   \n"<< std::endl;
  } else 
  {
    Json::StreamWriterBuilder builder;
    json_file = Json::writeString(builder, root);
    std::cout << json_file << std::endl;
  }

  
  // char arr[json_file.length() + 1]; 
  // strcpy(arr, json_file.c_str()); 
  // char* f = arr;

  const char* data;
  curl = curl_easy_init();
  if(!curl) 
  {
    fprintf(stderr, "Error: curl_easy_init failed.\n");
    goto cleanup;
  }
    
  headers = curl_slist_append(headers, "Accept: application/json");
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, "charset: utf-8");

  
  curl_easy_setopt(curl, CURLOPT_URL,"https://gaz.locator.am/api/camera_info");
  
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  // curl_easy_setopt(curl, CURLOPT_HEADER,1L);
  
  data = json_file.c_str();
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);//post data
  
curl_easy_setopt(curl, CURLOPT_WRITEDATA, wfd);
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
  res = curl_easy_perform(curl); //1 is here

  if(res != CURLE_OK) {
    size_t len = strlen(errbuf);
    fprintf(stderr, "\nlibcurl: (%d) ", res);/////////////////////
    if(len)
      fprintf(stderr, "%s%s", errbuf, ((errbuf[len - 1] != '\n') ? "\n" : ""));
    fprintf(stderr, "%s\n\n", curl_easy_strerror(res));
    goto cleanup;
  }

  // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);//this line is for getting additional helpful info
  // curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);//this line provides extra info about errors

  
  

  retcode = TRUE;
  cleanup:
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  // cJSON_Delete(root);
  // free(json);
  return retcode;
}

void OpenGate(int state){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Failed to create socket\n";
        return ;
    }

    std::string ip = "192.168.2.214";
    int port = 80;

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip.c_str());
    server_address.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) != 0) {
        std::cerr << "Failed to connect to server\n";
        return ;
    }

    char message[100];
    if(state == 1){
      sprintf(message, "GET %s HTTP/1.1\r\n\r\n", "/api/ForceOpen");
    }
    else if(state == 0){
      sprintf(message, "GET %s HTTP/1.1\r\n\r\n", "/api/ForceClose");

    }

    if (send(sock, message, strlen(message), 0) == -1) {
        std::cerr << "Failed to send data\n";
        return ;
    }
    

    char buffer[1024] = {0};
    if (recv(sock, buffer, 1024, 0) == -1) {
        std::cerr << "Failed to receive data\n";
        return ;
    }

    if(strstr(buffer, "Barrier Force Close") != 0)
    {
        std::cout << "Barrier is Close" << std::endl;
    }

    else if(strstr(buffer, "Barrier Force Open") != 0)
    {
        std::cout << "Barrier is Open" << std::endl;
    }

    close(sock);

}