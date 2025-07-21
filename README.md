# Load Balancer Simulator

C++ simulation of a load balancer that distributes network requests across multiple web servers. The simulation includes dynamic server scaling, IP-based request blocking for security, and comprehensive logging of all activities.

 Classes
- **LoadBalancer**: Main class that manages the server pool and request distribution
- **WebServer**: Represents individual servers that process requests over time
- **Request**: Encapsulates network requests with source/destination IPs and pr

 Configuration Files
- `blocked_ips.txt`: Contains IP addresses to block (one per line)
- `Doxyfile`: Doxygen configuration for documentation generation

 Output Files
- `simulation_log.txt`: General simulation activities and events
- `firewall_log.txt`: Blocked IP requests and security events
- `docs/html/`: Generated HTML documentation (after running doxygen)

 Usage Example
When you run the program, you'll be prompted for:
1. Number of servers (e.g., 3)
2. Number of simulation cycles (e.g., 10)
3. Initial queue size (e.g., 5, or -1 for default)

The simulation will then run and display real-time updates while logging all activities to files.
