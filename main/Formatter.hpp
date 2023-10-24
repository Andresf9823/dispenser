#ifndef _FORMATTER_HPP_
#define _FORMATTER_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

class Formatter
{
private:
    /* data */
public:
    Formatter(/* args */);
    void (*logString)(const char *TAG, const char *message);
    void (*logDword)(const char *TAG, uint32_t logNumber);
    void (*logFloat)(const char *TAG, double logFloating);
    ~Formatter();
};

#endif