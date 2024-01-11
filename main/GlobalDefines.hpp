#ifndef _GLOBAL_DEFINES_HPPP_
#define _GLOBAL_DEFINES_HPPP_

/*Global includes*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*Version*/
#define VERSION_APP "0.0.1"

/*Memory size*/
#define KB (1024UL)
#define MB (KB) * (KB)

/*Tcp Buffers*/
#define RX_BUFFER_SIZE 512
#define TCP_TASK_SIZE 4096

/*Namespaces*/
using namespace std;

#endif