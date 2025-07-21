/**
 * @file main.cpp
 * @brief Main entry point for the Load Balancer simulation application
 * 
 * This program simulates a load balancer that distributes network requests across
 * multiple web servers. The simulation includes dynamic server scaling, IP-based
 * request blocking, and comprehensive logging of all activities.
 */

#include <cstdio>
#include <iostream>

#include "LoadBalancer.h"
#include "Request.h"
#include "WebServer.h"


/**
 * @brief Main entry point for the Load Balancer application
 * 
 * Prompts the user for simulation parameters and runs the load balancer simulation.
 * The program collects user input for:
 * - Number of servers to manage
 * - Number of simulation cycles to run
 * - Initial queue size (or -1 for automatic sizing)
 * 
 * @return 0 on successful completion
 */
int main() {
	
    int servers;
    int cycles;
    int initialQueue;

    
    std::cout << "Enter number of servers: ";
    std::cin >> servers;
    std::cout << "Enter number of simulation cycles: ";
    std::cin >> cycles;
    std::cout << "Enter initial queue size (or -1 for servers*100): ";
    std::cin >> initialQueue;

    // Create loadbalancer object (automatically loads blocked IPs)
    LoadBalancer lb(servers, initialQueue);
    
    // Run simulation
    lb.run(cycles);
	return 0;
}
