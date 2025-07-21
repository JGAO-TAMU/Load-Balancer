/**
 * @file LoadBalancer.h
 * @brief LoadBalancer class header file
 */
#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "WebServer.h"
#include "Request.h"
#include <vector>
#include <queue>
#include <random>
#include <unordered_set> 
#include <string>
#include <fstream>

/**
 * @brief A load balancer that distributes incoming requests across multiple web servers
 * 
 * The LoadBalancer class manages a pool of web servers and distributes incoming requests
 * among them. It supports dynamic scaling (adding/removing servers based on load),
 * IP-based request blocking for security, and comprehensive logging of all activities.
 * The load balancer operates in discrete time ticks, processing requests and managing
 * server load at each time step.
 */
class LoadBalancer {
private:
    std::vector<WebServer*> servers;                     ///< Pool of web servers managed by the load balancer
    std::queue<Request> requestQueue;                    ///< Queue of pending requests waiting to be processed
    std::unordered_set<std::string> blockedIPs;         ///< Set of blocked IP addresses for security filtering
    int current_time;                                    ///< Current simulation time (tick counter)
    int max_servers;                                     ///< Maximum number of servers allowed in the pool
    int active_servers;                                  ///< Number of currently active servers

public:
    /**
     * @brief Constructs a LoadBalancer with the specified configuration
     * @param numServers Maximum number of servers to manage
     * @param initialQueueSize Number of initial requests to generate (-1 for default: numServers*100)
     * @param blockedIPsFile Path to file containing blocked IP addresses (default: "blocked_ips.txt")
     */
    LoadBalancer(int numServers, int initialQueueSize = -1, const std::string& blockedIPsFile = "blocked_ips.txt");
    
    /**
     * @brief Destructor that cleans up allocated WebServer objects
     */
    ~LoadBalancer();

    /**
     * @brief Randomly generates and enqueues new requests based on probability
     * 
     * Each tick has a 30% chance of adding 1 request, and a 1% chance of adding
     * a burst of 5 requests to simulate traffic spikes.
     */
    void addRandomRequest();
    
    /**
     * @brief Simulates one tick of time in the load balancer
     * 
     * Performs the following operations in sequence:
     * 1. Possibly adds new random requests
     * 2. Manages server scaling based on current load
     * 3. Updates all servers (processes current requests, assigns new ones)
     * 4. Reports current system status
     */
    void tick();
    
    /**
     * @brief Runs the main simulation loop for the specified duration
     * @param totalTime Number of time ticks to simulate
     */
    void run(int totalTime);
    
    // Dynamic server management
    /**
     * @brief Adds a new server to the pool if under the maximum limit
     */
    void scaleUp();
    
    /**
     * @brief Removes an idle server from the pool to reduce overhead
     */
    void scaleDown();
    
    /**
     * @brief Automatically scales the server pool based on current load metrics
     * 
     * Scales up when queue size exceeds idle server count.
     * Scales down when queue is empty and there are many idle servers.
     */
    void manageServerLoad();
    
    // IP Blocking functionality
    /**
     * @brief Loads blocked IP addresses from a file
     * @param filepath Path to the file containing blocked IP addresses (one per line)
     */
    void loadBlockedIPs(const std::string& filepath);
    
    /**
     * @brief Checks if an IP address is in the blocked list
     * @param ip IP address to check
     * @return true if the IP is blocked, false otherwise
     */
    bool isBlocked(const std::string& ip) const;
    
    /**
     * @brief Logs a blocked request to the firewall log file
     * @param ip IP address that was blocked
     */
    void logBlockedRequest(const std::string& ip) const;
    
    /**
     * @brief Logs general output to both console and simulation log file
     * @param message Message to log with timestamp
     */
    void logOutput(const std::string& message) const;
    
private:
    /**
     * @brief Checks if any server is currently processing a request
     * @return true if at least one server is busy, false if all servers are idle
     */
    bool hasActiveTasks() const;
    
    /**
     * @brief Counts the number of idle (not busy) servers
     * @return Number of servers that are currently idle
     */
    int getIdleServerCount() const;
    
    /**
     * @brief Calculates the average queue size per active server
     * @return Average number of queued requests per active server
     */
    double getAverageQueueSize() const;
};
#endif // LOADBALANCER_H
