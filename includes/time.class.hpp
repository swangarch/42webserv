#ifndef TIME_CLASS_HPP
#define TIME_CLASS_HPP

#include "standardLib.hpp"

class time
{
    private:
        time();
        ~time();

    public:
        static std::string     getFormattedTime();
        static std::string     getSimpleFormattedTime();
        static long long       getTimeStamp();
};

#endif