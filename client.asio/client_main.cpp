// client.asio.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Client.h"


int main()
{
    _DEBUG_ << "test client\n";

    testClient();
    _DEBUG_ << "ends client\n";
    getchar();
    return 0;
}

