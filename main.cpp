#include <emscripten/fetch.h>

#include <cstdio>
#include <cstring>
#include <vector>
#include <string>

const std::size_t NUM = 2048;
const std::string baseURL = "http://localhost/data/";

void fetchData(std::string url, int fileIdx);

void downloadSucceeded(emscripten_fetch_t *fetch)
{
    printf("Received '%s'\n", fetch->url);

    int fileIdx = int(fetch->userData) + 1;
    emscripten_fetch_close(fetch); // Free data associated with the fetch.

    if(fileIdx < NUM)
        fetchData(baseURL + std::to_string(fileIdx) + ".bin", fileIdx);
}

void downloadFailed(emscripten_fetch_t *fetch)
{
  printf("[ERROR]: %s FAILED!!!\n", fetch->url);
  emscripten_fetch_close(fetch);
}


void fetchData(std::string url, int fileIdx)
{
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    std::strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = downloadSucceeded;
    attr.onerror = downloadFailed;
    attr.userData = (void*)fileIdx;
    emscripten_fetch(&attr, url.c_str());
}

int main()
{
    printf("Start...\n");
    fetchData(baseURL + "1.bin", 1);

    return 0;
}
