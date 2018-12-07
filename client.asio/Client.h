#pragma once
#include "common.h"

using namespace boost;
namespace ASIO = boost::asio;

class Client :public boost::noncopyable
{
public:
    Client(const std::string& ip, UINT port);

    void Start();

    void DoWrite(const char* data, UINT len);

    inline void RegisterRecvCallback(RecvCallback cb) { recvCallback_ = cb; }
private:

    void DoConnect();

    void DoRead();

private:

    char data_[READ_BUFF_LEN];
    ASIO::io_service service_;
    ASIO::ip::tcp::endpoint endpoint_;
    ASIO::ip::tcp::socket socket_;

    RecvCallback recvCallback_;
};

void threadClient(Client& client);

void testClient();
