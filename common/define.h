#pragma once
#include "include.h"

template<typename T>
using SPTR = std::shared_ptr<T>;


#ifndef UINT
using UINT = unsigned int;
#endif // !UINT


#ifndef _DEBUG_
#define _DEBUG_ std::cout
#endif // !_DEBUG_

#define READ_BUFF_LEN (1024)

using RecvCallback = std::function<void(const char*, UINT)>;