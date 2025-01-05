#include "ConnectionMgr.h"

ConnectionMgr& ConnectionMgr::GetInstance()
{
	static ConnectionMgr instance;
	return instance;
}

void ConnectionMgr::AddConnection(std::shared_ptr<Connection> conptr)
{
	_map_cons[conptr->GetID()] = conptr;

}

void ConnectionMgr::RmvConnection(std::string id)
{
	_map_cons.erase(id);
}

ConnectionMgr::ConnectionMgr()
{
}
