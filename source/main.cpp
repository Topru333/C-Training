#include <iostream>
#include <switch.h>
#include <Swurl.hpp>

using namespace swurl;
using namespace std;

void swurlProgressUpdate(WebRequest * request, double progress);
void swurlOnCompleted(WebRequest * request);
void swurlOnError(WebRequest * request, string error);

int main(int argc, char **argv) {
    SessionManager::initialize();
    consoleInit(NULL);

    // Global Settings
    SessionManager::userAgent = "SwurlExample/1.0.0";
    SessionManager::requestHeaders.insert(
        pair<string, string>(
            "Cache-Control",
            "no-cache"
        )
    );

    // Callbacks
    SessionManager::onProgressChanged = swurlProgressUpdate;
    SessionManager::onCompleted = swurlOnCompleted;
    SessionManager::onError = swurlOnError;

    // Draw the progress.
    cout << "Downloading:   0%\n";
    consoleUpdate(NULL);

    // Make the request.
    WebRequest * kosmosReleases = new WebRequest("https://api.gfycat.com/v1/gfycats/search?search_text=lol");
    SessionManager::makeRequest(kosmosReleases);

    int state = 0;
    while(appletMainLoop())
    {
        hidScanInput();
        if (hidKeysDown(CONTROLLER_P1_AUTO)) {
            consoleClear();
            cout << "\x1b[0;0H";

            if (state == 0) {
                map<string, string>::iterator reqIt;
                for (auto it = kosmosReleases->response.headers.begin(); it != kosmosReleases->response.headers.end(); it++) {
                    cout << it->first << ": " << it->second << "\n";
                }
                cout << "\n" << "Press any key to view a preview of the response body.\n";
            }
            else if (state == 1) {
                consoleClear();
                cout << "\x1b[0;0HRresponse body:\n\n" << kosmosReleases->response.rawResponseBody << "\n";
                cout << "\n" << "Press any key to close.\n";
            }
            else {
                break;
            }

            state++;
        }

        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    SessionManager::dealloc();
    delete kosmosReleases;
    return 0;
}

void swurlProgressUpdate(WebRequest * request, double progress) {
    int progressPercentage = (progress < 1) ? progress * 100 : 100;
    if (progressPercentage < 10) {
        cout << "\x1b[1;14H  " << progressPercentage;
    } else if (progressPercentage < 100) {
        cout << "\x1b[1;14H " << progressPercentage;
    } else {
        cout << "\x1b[1;14H" << progressPercentage;
    }
    consoleUpdate(NULL);
}

void swurlOnCompleted(WebRequest * request) {
    cout << "\x1b[2;0HDownload Completed with status code " << request->response.statusCode << "!\n\n";
    cout << "Press any key to view response headers.\n";
    consoleUpdate(NULL);
}

void swurlOnError(WebRequest * request, string error) {
    cout << "\x1b[2;0HError: " << error << "\n\n";
    cout << "Press any key to close.\n";
    consoleUpdate(NULL);
}
