#include "stdafx.h"
#include "Client.h"

Client::Client(const std::string& ip, UINT port) :
    service_(),
    endpoint_(ASIO::ip::address_v4::from_string(ip), port),
    socket_(service_, endpoint_)
{

}

void Client::Start()
{
    DoConnect();
    service_.run();
}

void Client::DoWrite(const char* data, UINT len)
{
    socket_.async_write_some(ASIO::buffer(data, len),
        [this](boost::system::error_code code, std::size_t len)
    {
        DoRead();
    });
}

void Client::DoConnect()
{
    socket_.async_connect(endpoint_,
        [this](boost::system::error_code code)
    {
        _DEBUG_ << "handle_connect\n";
        DoRead();
    });
}

void Client::DoRead()
{
    socket_.async_read_some(ASIO::buffer(data_, READ_BUFF_LEN),
        [this](boost::system::error_code code, std::size_t len)
    {
        _DEBUG_ << "handle_read:  [len] " << len << "\n";
        if (recvCallback_)
        {
            recvCallback_(data_, len);
        }
        DoRead();
    });
}

void threadClient(Client& client)
{
    client.Start();
}

void testClient()
{
    Client client("127.0.0.1", 2001);

    client.RegisterRecvCallback([](const char* data, UINT len)
    {
        _DEBUG_ << "client on recv msg:   " << data << "len: " << len << "\n";

    });

    std::thread t(std::bind(threadClient, std::ref(client)));

    using std::cin;
    std::string cmd;
    while (cin >> cmd)
    {
        _DEBUG_ << "cmd:  " << cmd << "\n";
        if (cmd == "q")
        {
            break;
        }

        client.DoWrite(cmd.c_str(), cmd.length() + 1);
    }

    t.join();
}
