/** 
 * @file WebServer.cpp
 * @brief WebServer class implementation
 * 
 * Contains the implementation of the WebServer class methods for processing
 * requests and managing server state transitions.
 */

#include "WebServer.h"

// WebServer class implementation will go here

WebServer::WebServer() : busy(false), time_left(0), curr_request() {
    // Initialize as not busy with no time left and empty request
}

bool WebServer::isbusy() const {
    return busy;
}

int WebServer::gettimeleft() const {
    return time_left;
}

Request WebServer::getcurr() const {
    return curr_request;
}

void WebServer::assignrequest(const Request& r) {
    if (!busy) {
        curr_request = r;
        time_left = r.gettime();
        busy = true;
    }
}

void WebServer::tick() {
    if (busy && time_left > 0) {
        time_left--;
        if (time_left == 0) {
            busy = false;
            curr_request = Request();
        }
    }
}
