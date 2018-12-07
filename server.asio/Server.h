#pragma once
#include "common.h"

using namespace boost;
namespace ASIO = boost::asio;

class Connection :public std::enable_shared_from_this<Connection>,
    boost::noncopyable
{
public:
    Connection(ASIO::io_service& service);

    void DoRead();
    void DoWrite(const char* data,UINT len);
    inline ASIO::ip::tcp::socket& Socket(){ return remoteSocket_;}
    inline void RegisterRecvCallback(RecvCallback cb) { recvCallback_ = cb; }
private:
    char data_[READ_BUFF_LEN];
    ASIO::ip::tcp::socket remoteSocket_;

    RecvCallback recvCallback_;
};

class Server:public boost::noncopyable
{
public:
    Server(UINT port);
    void Start();

    void DoAccept(Connection& connection);

    void DoBroadCast(const char* data, UINT len);
    
    inline void RegisterRecvCallback(RecvCallback cb) { recvCallback_ = cb; }

private:
    ASIO::io_service service_;
    ASIO::ip::tcp::endpoint endpoint_;
    ASIO::ip::tcp::acceptor acceptor_;
    
    RecvCallback recvCallback_;
    std::vector<SPTR<Connection>> connections_;
};

void testServer();

