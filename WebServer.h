/**
 * @file WebServer.h
 * @brief WebServer class header file
 */
#ifndef WEBSERVER_H
#define WEBSERVER_H
#include "Request.h" 
#include <queue>

/**
 * @brief A web server that processes requests over time
 * 
 * The WebServer class represents a single server that can process one request at a time.
 * Each request takes a specified number of time cycles to complete. The server maintains
 * its state (busy/idle) and tracks the remaining processing time for the current request.
 */
class WebServer {
private:
    bool busy;                  ///< Flag indicating if the server is currently processing a request
    int time_left;              ///< Number of time cycles remaining to complete the current request
    Request curr_request;       ///< The request currently being processed by this server

public:
    /**
     * @brief Default constructor that initializes the server as idle
     */
    WebServer();

    /**
     * @brief Checks if the server is currently busy processing a request
     * @return true if the server is busy, false if idle
     */
    bool isbusy() const;
    
    /**
     * @brief Gets the remaining processing time for the current request
     * @return Number of time cycles left to complete the current request
     */
    int gettimeleft() const;
    
    /**
     * @brief Gets a copy of the current request being processed
     * @return Copy of the current Request object
     */
    Request getcurr() const;
    
    /**
     * @brief Assigns a new request to this server if it's not busy
     * @param r The request to assign to this server
     */
    void assignrequest(const Request& r);
    
    /**
     * @brief Advances the server by one time tick
     * 
     * Decrements the remaining processing time for the current request.
     * If the request is completed (time_left reaches 0), the server becomes idle.
     */
    void tick();

};
#endif // WEBSERVER_H
