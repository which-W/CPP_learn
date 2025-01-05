#pragma once
#include "Connection.h"
#include "boost/unordered_map.hpp"

class ConnectionMgr
{
public:
	static ConnectionMgr& GetInstance();
	void AddConnection(std::shared_ptr<Connection> conptr);
	void RmvConnection(std::string);
private:
	ConnectionMgr(const ConnectionMgr&) = delete;
	ConnectionMgr& operator = (const ConnectionMgr&) = delete;
	ConnectionMgr();
	boost::unordered_map<std::string, std::shared_ptr<Connection>> _map_cons;
};

