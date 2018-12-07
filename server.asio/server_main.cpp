// server.asio.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Server.h"

int main()
{
    _DEBUG_ << "test server\n";

    testServer();
    _DEBUG_ << "ends server\n";
    getchar();

    return 0;
}

