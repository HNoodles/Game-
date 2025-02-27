#pragma once
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <mutex>

#include "../EventMgmt/EventManager.h"
#include "../Times/LocalTime.h"
#include "../Objects/Character.h"

extern const char* const SELF_NAME;

using namespace std;
using namespace zmq;

class Client
{
private:
	context_t context;
	socket_t sender;
	socket_t subscriber;
	map<string, GameObject*>* objects;
	bool connected, replaying;
	double connectedTime;
	EventManager* manager;
	Replay* replay;

	string s_recv(socket_t& socket);

	void s_send(socket_t& socket, const string& string);

	void Split(const string& string, const std::string& separator, vector<std::string>& result);
public:
	Client(map<string, GameObject*>* objects, EventManager* manager, Replay* replay);

	void connect();

	void sendHandler();

	void subscribeHandler(GameTime* gameTime);

	void disconnect(bool forReplay = false);
};

