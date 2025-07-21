# Load Balancer Simulation

## Overview
This project implements a C++ simulation of a load balancer that distributes network requests across multiple web servers. The simulation includes dynamic server scaling, IP-based request blocking for security, and comprehensive logging of all activities.

## Features
- **Dynamic Server Scaling**: Automatically adds/removes servers based on current load
- **IP Blocking**: Security feature to block requests from specified IP addresses
- **Comprehensive Logging**: Dual logging system for both firewall events and general simulation activities
- **Random Request Generation**: Simulates realistic traffic patterns with occasional traffic surges
- **Time-based Simulation**: Discrete time simulation with configurable duration

## Classes
- **LoadBalancer**: Main class that manages the server pool and request distribution
- **WebServer**: Represents individual servers that process requests over time
- **Request**: Encapsulates network requests with source/destination IPs and processing time

## Building and Running
```bash
# Compile the project
g++ -o main.exe main.cpp LoadBalancer.cpp WebServer.cpp Request.cpp

# Run the simulation
./main.exe
```

## Generating Documentation
This project includes comprehensive Doxygen documentation. To generate HTML documentation:

```bash
# Install Doxygen (if not already installed)
# On Windows: Download from http://www.doxygen.nl/download.html
# On Ubuntu/Debian: sudo apt-get install doxygen
# On macOS: brew install doxygen

# Generate documentation
doxygen Doxyfile

# View documentation
# Open docs/html/index.html in your web browser
```

The generated documentation will include:
- Class hierarchy and relationships
- Detailed API documentation for all methods
- Parameter and return value descriptions
- Source code browsing
- Alphabetical index of all symbols

## Configuration Files
- `blocked_ips.txt`: Contains IP addresses to block (one per line)
- `Doxyfile`: Doxygen configuration for documentation generation

## Output Files
- `simulation_log.txt`: General simulation activities and events
- `firewall_log.txt`: Blocked IP requests and security events
- `docs/html/`: Generated HTML documentation (after running doxygen)

## Usage Example
When you run the program, you'll be prompted for:
1. Number of servers (e.g., 3)
2. Number of simulation cycles (e.g., 10)
3. Initial queue size (e.g., 5, or -1 for default)

The simulation will then run and display real-time updates while logging all activities to files.
