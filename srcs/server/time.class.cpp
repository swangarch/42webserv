#include "../../includes/time.class.hpp"

std::string     time::getFormattedTime()
{
    std::time_t t = std::time(NULL);
    std::tm *gmt = std::gmtime(&t);

    char buffer[30];
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt);
    
    return (std::string(buffer));
}

std::string     time::getSimpleFormattedTime()
{
    std::time_t t = std::time(NULL);
    std::tm *local = std::localtime(&t);

    char buffer[30];
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", local);
    return (std::string(buffer));
}

long long       time::getTimeStamp()
{
    struct timeval nowtime;

    gettimeofday(&nowtime, NULL);

    long long     second = nowtime.tv_sec;
    long long     usecond = nowtime.tv_usec;

    return (second * 1000000 + usecond);
}