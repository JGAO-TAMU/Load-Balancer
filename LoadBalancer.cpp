/** 
 * @file LoadBalancer.cpp
 * @brief LoadBalancer class implementation
 * 
 * Contains the complete implementation of the LoadBalancer class, including
 * request distribution, dynamic server scaling, IP blocking, and logging functionality.
 */

#include "LoadBalancer.h"
#include <iostream>
#include <sstream>
#include <random>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>

LoadBalancer::LoadBalancer(int numServers, int initialQueueSize, const std::string& blockedIPsFile) : current_time(0), max_servers(numServers), active_servers(0) {
    // Load blocked IPs first, before generating initial requests
    loadBlockedIPs(blockedIPsFile);
    
    // Start with all servers active
    servers.reserve(max_servers);
    
    // Add all servers initially
    for (int i = 0; i < max_servers; ++i) {
        servers.push_back(new WebServer());
        active_servers++;
    }
    
    // Add initial requests - use parameter or default
    int queueSize = (initialQueueSize == -1) ? (max_servers * 100) : initialQueueSize;
    
    // Random number generators for initial requests
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> ip_part(0, 254);  // Include 0 to match blocked_ips.txt
    std::uniform_int_distribution<> process_time(1, 10);
    
    for (int i = 0; i < queueSize; ++i) { 
        // Generate random IP addresses
        std::ostringstream ip_in, ip_out;
        ip_in << "192.168.1." << ip_part(gen);
        ip_out << "192.168.1." << ip_part(gen);
        
        Request initialRequest(ip_in.str(), ip_out.str(), process_time(gen));
        
        // Check if the request should be blocked
        if (isBlocked(initialRequest.getin())) {
            logBlockedRequest(initialRequest.getin());
            continue; // Skip adding this request to the queue
        }
        
        requestQueue.push(initialRequest);
    }
    
    logOutput("LoadBalancer initialized with " + std::to_string(max_servers) + "/" + std::to_string(max_servers) 
              + " servers and " + std::to_string(queueSize) + " initial requests");
}

LoadBalancer::~LoadBalancer() {
    // Clean up all allocated WebServers
    for (WebServer* server : servers) {
        delete server;
    }
    servers.clear();
}

void LoadBalancer::addRandomRequest() {
    // Random number generator
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> chance(1, 100);
    static std::uniform_int_distribution<> ip_part(0, 254);  // Include 0 to match blocked_ips.txt
    static std::uniform_int_distribution<> process_time(1, 10);
    static std::uniform_int_distribution<> burst_chance(1, 100);
    
    // 30% chance to add new request(s) each tick
    if (chance(gen) <= 30) {
        int requestsToAdd = 1;
        
        // burst of 5 requests
        if (burst_chance(gen) <= 1) {
            requestsToAdd = 5;
            logOutput("Time " + std::to_string(current_time) + ": TRAFFIC SURGE! Adding 5 requests");
        }
        
        for (int r = 0; r < requestsToAdd; ++r) {
            // Generate random IP addresses in 192.168.1.xxx
            std::ostringstream ip_in, ip_out;
            ip_in << "192.168.1." << ip_part(gen);
            ip_out << "192.168.1." << ip_part(gen);
            
            // Create and enqueue new request
            Request newRequest(ip_in.str(), ip_out.str(), process_time(gen));
            
            // Check if the request should be blocked
            if (isBlocked(newRequest.getin())) {
                logBlockedRequest(newRequest.getin());
                continue; // Skip adding this request to the queue
            }
            
            requestQueue.push(newRequest);
            
            if (requestsToAdd == 1) {
                // Normal single request - show details
                logOutput("Time " + std::to_string(current_time) + ": New request added (" 
                          + ip_in.str() + " -> " + ip_out.str() 
                          + ", " + std::to_string(newRequest.gettime()) + " cycles)");
            }
        }
        
        if (requestsToAdd > 1) {
            logOutput("Time " + std::to_string(current_time) + ": Added " + std::to_string(requestsToAdd) 
                      + " requests to queue");
        }
    }
}

void LoadBalancer::tick() {
    current_time++;
    logOutput("\n--- Time " + std::to_string(current_time) + " ---");
    
    // 1. Possibly add a new Request (random chance)
    addRandomRequest();
    
    // 2. Manage server load (dynamic scaling)
    manageServerLoad();
    
    // 3. For each WebServer:
    for (size_t i = 0; i < servers.size(); ++i) {
        WebServer* server = servers[i];
        
        if (server->isbusy()) {
            // Get current request info before ticking
            Request currentRequest = server->getcurr();
            
            // a. If it's busy: tick down time_remaining
            server->tick();
            
            // Check if server just finished
            if (!server->isbusy()) {
                logOutput("Server " + std::to_string(i) + ": Completed request (" 
                          + currentRequest.getin() + " -> " + currentRequest.getout() 
                          + ")!");
            } else {
                logOutput("Server " + std::to_string(i) + ": Processing request (" 
                          + currentRequest.getin() + " -> " + currentRequest.getout() 
                          + "), " + std::to_string(server->gettimeleft()) + " cycles remaining");
            }
        } else {
            // b. If it's free: assign it the next request in the queue
            if (!requestQueue.empty()) {
                Request nextRequest = requestQueue.front();
                requestQueue.pop();
                server->assignrequest(nextRequest);
                
                logOutput("Server " + std::to_string(i) + ": Assigned new request (" 
                          + nextRequest.getin() + " -> " + nextRequest.getout() 
                          + ", " + std::to_string(nextRequest.gettime()) + " cycles)");
            } else {
                logOutput("Server " + std::to_string(i) + ": Idle");
            }
        }
    }
    
    int idle_servers = getIdleServerCount();
    logOutput("Queue size: " + std::to_string(requestQueue.size()) 
              + " | Active servers: " + std::to_string(active_servers) + "/" + std::to_string(max_servers)
              + " | Idle servers: " + std::to_string(idle_servers));
}

void LoadBalancer::run(int totalTime) {
    logOutput("Starting Load Balancer simulation with " + std::to_string(servers.size()) 
              + " servers for " + std::to_string(totalTime) + " ticks.\n");
    
    for (int t = 0; t < totalTime; ++t) {
        tick();
    }

    // std::cout << "\n=== Processing remaining requests ===" << std::endl;
    // while (!requestQueue.empty() || hasActiveTasks()) {
    //     tick();
    // }
        
    logOutput("\nSimulation complete!");
    logOutput("Requests remaining in queue: " + std::to_string(requestQueue.size()));
    
    int busyCount = 0;
    for (const WebServer* server : servers) {
        if (server->isbusy()) busyCount++;
    }
    logOutput("Servers still busy: " + std::to_string(busyCount) + "/" + std::to_string(servers.size()));
}

bool LoadBalancer::hasActiveTasks() const {
    for (const WebServer* server : servers) {
        if (server->isbusy()) {
            return true;
        }
    }
    return false;
}

void LoadBalancer::scaleUp() {
    if (active_servers < max_servers) {
        servers.push_back(new WebServer());
        active_servers++;
        logOutput(">> SCALED UP: Added server " + std::to_string(active_servers - 1) 
                  + " (" + std::to_string(active_servers) + "/" + std::to_string(max_servers) + ")");
    }
}

void LoadBalancer::scaleDown() {
    if (active_servers > 1) { // Keep at least 1 server
        // Find and remove an idle server
        for (auto it = servers.begin(); it != servers.end(); ++it) {
            if (!(*it)->isbusy()) {
                logOutput(">> SCALED DOWN: Removed idle server " 
                          + std::to_string(it - servers.begin()));
                delete *it;
                servers.erase(it);
                active_servers--;
                break;
            }
        }
    }
}

void LoadBalancer::manageServerLoad() {
    int queue_size = requestQueue.size();
    int idle_servers = getIdleServerCount();
    
    // Scale up if queue is building up and we have capacity
    if (queue_size > idle_servers && active_servers < max_servers) {
        scaleUp();
    }
    // Scale down ONLY if queue is empty AND we have many idle servers
    else if (queue_size == 0 && idle_servers > 2 && active_servers > 1) {
        scaleDown();
    }
}

int LoadBalancer::getIdleServerCount() const {
    int idle_count = 0;
    for (const WebServer* server : servers) {
        if (!server->isbusy()) {
            idle_count++;
        }
    }
    return idle_count;
}

double LoadBalancer::getAverageQueueSize() const {
    return static_cast<double>(requestQueue.size()) / active_servers;
}

// IP Blocking functionality
void LoadBalancer::loadBlockedIPs(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cout << "Warning: Could not open blocked IPs file: " << filepath << std::endl;
        std::cout << "Continuing without IP blocking..." << std::endl;
        return;
    }
    
    std::string ip;
    int count = 0;
    while (std::getline(file, ip)) {
        // Remove any whitespace
        if (!ip.empty() && ip.back() == '\r') {
            ip.pop_back(); // Remove carriage return if present
        }
        
        if (!ip.empty()) {
            blockedIPs.insert(ip);
            count++;
        }
    }
    
    file.close();
    logOutput("Loaded " + std::to_string(count) + " blocked IP addresses from " + filepath);
}

bool LoadBalancer::isBlocked(const std::string& ip) const {
    return blockedIPs.find(ip) != blockedIPs.end();
}

void LoadBalancer::logBlockedRequest(const std::string& ip) const {
    std::ofstream logFile("firewall_log.txt", std::ios::app);
    if (logFile.is_open()) {
        // Get current timestamp
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        logFile << "[" << std::ctime(&time_t) << "] "
                << "BLOCKED: Request from IP " << ip 
                << " at simulation time " << current_time << std::endl;
        
        logFile.close();
    }
    
    // Also log to console and simulation log for visibility
    std::string message = "FIREWALL: Blocked request from IP " + ip;
    std::cout << message << std::endl;
    logOutput("FIREWALL: Blocked request from IP " + ip);
}

void LoadBalancer::logOutput(const std::string& message) const {
    std::ofstream logFile("simulation_log.txt", std::ios::app);
    if (logFile.is_open()) {
        // Get current timestamp
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        logFile << "[" << std::ctime(&time_t) << "] " << message << std::endl;
        logFile.close();
    }
    
    // Also output to console
    std::cout << message << std::endl;
}
