/** 
 * @file Request.cpp
 * @brief Request class implementation
 * 
 * Contains the implementation of the Request class methods for managing
 * network request data including IP addresses and processing time.
 */

#include "Request.h"

Request::Request(const std::string& ip_in, const std::string& ip_out, int process_time)
    : ip_in(ip_in), ip_out(ip_out), process_time(process_time) {}

Request::Request() 
    : ip_in(""), ip_out(""), process_time(0) {}

std::string Request::getin() const {
    return ip_in;
}

std::string Request::getout() const {
    return ip_out;
}

int Request::gettime() const{
    return process_time;
}

void Request::setin(const std::string& ip_in) {
    Request::ip_in = ip_in;
}

void Request::setout(const std::string& ip_out) {
    Request::ip_out = ip_out;
}

void Request::settime(int process_time) {
    Request::process_time = process_time;
}