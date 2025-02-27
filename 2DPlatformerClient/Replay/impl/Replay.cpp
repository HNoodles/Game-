#include "../../EventMgmt/EventManager.h"
#include "../../Networking/Client.h"

Replay::Replay(Timeline* gameTime, EventManager* manager, Client* client) :
	gameTime(gameTime), replayTime(1), startTime(0), 
	isRecording(false), isPlaying(false), manager(manager), client(client)
{
}

Replay::~Replay()
{
	while (!records.empty())
	{
		delete records.top();
		records.pop();
	}
}

void Replay::startRecording()
{
	replayTime = GameTime(1);
	startTime = gameTime->getTime();
	replayTime.setPaused(true);
	isRecording = true;// manager will begin to record EObjMovement events now
}

void Replay::endRecording()
{
	records.push(new EEndPlaying(gameTime->getTime(), this));
	isRecording = false;
}

void Replay::startPlaying()
{
	isPlaying = true;

	manager->setTimeline(&replayTime);
	client->disconnect(true);// this will also clear events in manager queues and GVTs
	manager->addQueue("R", &records);

	replayTime.setPaused(false);
}

void Replay::endPlaying()
{
	manager->setTimeline(gameTime);
	manager->setReplay(false); // this will remove queue for replay
	manager->removeQueue("R");
	client->connect();// reconnect to server

	// clear recorded events for this recording
	while (!records.empty())
	{
		delete records.top();
		records.pop();
	}
	isPlaying = false;
}

void Replay::resetPlaySpeed(double speed)
{
	double newStepSize = 1 / speed;
	
	lock_guard<mutex> guard(*manager->getMtxQueue());

	replayTime.resetStepSize(newStepSize);
}
