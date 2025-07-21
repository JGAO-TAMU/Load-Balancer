/**
 * @file Request.h
 * @brief Request class header file
 */
#ifndef REQUEST_H
#define REQUEST_H 

#include <string>

/**
 * @brief Represents a network request with source/destination IPs and processing time
 * 
 * The Request class encapsulates a network request that needs to be processed by a web server.
 * Each request contains source and destination IP addresses and requires a specific amount
 * of processing time to complete.
 */
class Request {
private:
    std::string ip_in;      ///< Source IP address of the request
    std::string ip_out;     ///< Destination IP address of the request
    int process_time;       ///< Number of time cycles required to process this request

public:
    /**
     * @brief Constructs a Request with specified parameters
     * @param ip_in Source IP address
     * @param ip_out Destination IP address  
     * @param process_time Number of time cycles needed to process the request
     */
    Request(const std::string& ip_in, const std::string& ip_out, int process_time);

    /**
     * @brief Default constructor that creates an empty request
     */
    Request();

    /**
     * @brief Gets the source IP address
     * @return Source IP address as a string
     */
    std::string getin() const;
    
    /**
     * @brief Gets the destination IP address
     * @return Destination IP address as a string
     */
    std::string getout() const;
    
    /**
     * @brief Gets the processing time required for this request
     * @return Number of time cycles needed to process the request
     */
    int gettime() const;

    /**
     * @brief Sets the source IP address
     * @param ip_in New source IP address
     */
    void setin(const std::string& ip_in);
    
    /**
     * @brief Sets the destination IP address
     * @param ip_out New destination IP address
     */
    void setout(const std::string& ip_out);
    
    /**
     * @brief Sets the processing time for this request
     * @param process_time New processing time in time cycles
     */
    void settime(int process_time);
};

#endif // REQUEST_H
