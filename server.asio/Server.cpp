#include "stdafx.h"
#include "Server.h"

Connection::Connection(ASIO::io_service& service) :
    remoteSocket_(service)
{
    memset(data_, 0, READ_BUFF_LEN);
}

void Connection::DoRead()
{
    remoteSocket_.async_read_some(ASIO::buffer(data_, READ_BUFF_LEN),
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

void Connection::DoWrite(const char* data, UINT len)
{
    remoteSocket_.async_write_some(ASIO::buffer(data, len),
        [this](boost::system::error_code code, std::size_t len)
    {
        _DEBUG_ << "handle_write:  [len] " << len << "\n";
        DoRead();
    });
}

Server::Server(UINT port) :
    service_(),
    endpoint_(ASIO::ip::address_v4(), port),
    acceptor_(service_, endpoint_)
{

}

void Server::Start()
{
    auto newConn = std::make_shared<Connection>(service_);
    connections_.push_back(newConn);
    DoAccept(*newConn);

    service_.run();
}

void Server::DoAccept(Connection& connection)
{
    acceptor_.async_accept(connection.Socket(),
        [this,&connection](boost::system::error_code code)
    {
        _DEBUG_ << "handle accept connection\n";

        //handle accepted connection
        connection.Socket().set_option(
            ASIO::ip::tcp::socket::keep_alive(true));
        if (recvCallback_)
        {
            connection.RegisterRecvCallback(recvCallback_);
        }

        //wait next connection 
        auto newConn = std::make_shared<Connection>(service_);
        connections_.push_back(newConn);
        DoAccept(*newConn);
    });
}

void Server::DoBroadCast(const char* data, UINT len)
{
    for (auto& conn : connections_)
    {
        conn->DoWrite(data, len);
    }
}

void testServer()
{
    Server server(2001);
    std::thread t([&]()
    {
        server.Start();
    });

    using std::cin;
    std::string cmd;
    while (cin >> cmd)
    {
        _DEBUG_ << "cmd:  " << cmd << "\n";
        if (cmd == "q")
        {
            break;
        }

        server.DoBroadCast(cmd.c_str(), cmd.length() + 1);
    }

    t.join();
}
