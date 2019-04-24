#include "stdafx.h"
#include "overseer.h"
#include "rng.h"



bool actor_pos_sort(Actor* a, Actor* o) {
	return a->getIndex() < o->getIndex();
}
Overseer::Overseer(HANDLE pipe, HANDLE pipe_s)
{
	if (pipe != INVALID_HANDLE_VALUE) {
		openPipe = true;
		usedPipe = pipe;
	}
	if (pipe_s != INVALID_HANDLE_VALUE) {
		openPipe_s = true;
		usedPipe_s = pipe_s;
	}
	//std::cout << openPipe_s << " " << openPipe << std::endl;
	init();
	run();
	actors = new std::vector<Actor*>();
	
}
Overseer::~Overseer() {

}

void Overseer::init()
{

	/*
	
	/* Window size coordinates, be sure to start index at zero! */
	windowSize = { 0, 0, MAP_WIDTH - 1, MAP_HEIGHT - 1 };

	/* A COORD struct for specificying the console's screen buffer dimensions */ 
	bufferSize = { MAP_WIDTH, MAP_HEIGHT };

	/* Setting up different variables for passing to WriteConsoleOutput */
	characterBufferSize = { MAP_WIDTH, MAP_HEIGHT };
	characterPosition = { 0, 0 };
	consoleWriteArea = { 0, 0, MAP_WIDTH - 1, MAP_HEIGHT -1  };

	/* A CHAR_INFO structure containing data about a single character */
	//CHAR_INFO consoleBuffer[WINDOW_WIDTH * WINDOW_HEIGHT];

	/* initialize handles */
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	rHnd = GetStdHandle(STD_INPUT_HANDLE);

	/* Set the console's title 
	/* Set the window size 
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);*/

	/* Set the screen's buffer size */
	SetConsoleScreenBufferSize(wHnd, bufferSize);

	consoleBuffer = new CHAR_INFO[(MAP_WIDTH) * (MAP_HEIGHT)];
	


	initRNG();
	world = new Atlas();
	actors = new std::vector<Actor*>();
	world->create(MAP_WIDTH, MAP_HEIGHT);
	populate();
	if(actors->size() > 0)
		actors->front()->assume();
}

bool Overseer::isOccupied(int x, int y){
	if (actors->size() > 0)
		std::sort(actors->begin(), actors->end(), actor_pos_sort);
	int index = x + y * MAP_WIDTH;
	if(world->getAt(x, y) != terrain_t::EMPTY){
		return true;
	}
	for(int i = 0; i < actors->size(); i++){
		if (index == actors->at(i)->getIndex()) {
			return true;
		}
		if(index < actors->at(i)->getIndex()){
			return false;
		}
	}
	return false;
}

double Overseer::random()
{
	return distr(rng);
}

void Overseer::report(char* message, int msglen) {
	if (openPipe) {
		if (usedPipe != INVALID_HANDLE_VALUE)
		{
			WriteFile(usedPipe,
				message,
				msglen,   // = length of string + terminating '\0' !!!
				&dwWritten,
				NULL);
		}
	}
}
void Overseer::report(const char* message, int msglen) {
	if (openPipe) {
		if (usedPipe != INVALID_HANDLE_VALUE)
		{
			WriteFile(usedPipe,
				message,
				msglen,   // = length of string + terminating '\0' !!!
				&dwWritten,
				NULL);
		}
	}
}
void Overseer::tell(char* message, int msglen) {
	if (openPipe_s) {
		if (usedPipe_s != INVALID_HANDLE_VALUE)
		{
			WriteFile(usedPipe_s,
				message,
				msglen,   // = length of string + terminating '\0' !!!
				&dwWritten_s,
				NULL);
		}
	}
}
void Overseer::tell(const char* message, int msglen) {
	if (openPipe_s) {
		if (usedPipe_s != INVALID_HANDLE_VALUE)
		{
			WriteFile(usedPipe_s,
				message,
				msglen,   // = length of string + terminating '\0' !!!
				&dwWritten_s,
				NULL);
		}
	}
}

void Overseer::update() {
	std::string str_report = "update\n";
	std::string str_tell = legend;
	report(str_report.data(), str_report.length());
	tell(str_tell.data(), str_tell.length());
	for (int i = 0; i < actors->size(); i++) {
		if (actors->at(i)->getAIState()) {
			str_tell = prefix_s + "Actor " + std::to_string(i) + " sits around and does nothing.\n";
			
		}
		else {
			switch (input) {
			case 'q':
			case 'Q':
				if (world->getAt(actors->at(i)->getX() - 1, actors->at(i)->getY() - 1) == terrain_t::EMPTY) {
					actors->at(i)->setX(actors->at(i)->getX() - 1);
					actors->at(i)->setY(actors->at(i)->getY() - 1);
					str_tell = prefix_s + "Actor " + std::to_string(i) + " moves northwest.\n";
				}
				else {
					str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move northwest, but was blocked.\n";
				}
				
				break;
			case 'w':
			case 'W':
				if (world->getAt(actors->at(i)->getX(), actors->at(i)->getY() - 1) == terrain_t::EMPTY) {
					actors->at(i)->setY(actors->at(i)->getY() - 1);
					str_tell = prefix_s + "Actor " + std::to_string(i) + " moves north.\n";
				}
				else {
					str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move north, but was blocked.\n";
				}
				break;
			case 'e':
			case 'E':
				if (world->getAt(actors->at(i)->getX() + 1, actors->at(i)->getY() - 1) == terrain_t::EMPTY) {
					actors->at(i)->setX(actors->at(i)->getX() + 1);
					actors->at(i)->setY(actors->at(i)->getY() - 1);
					str_tell = prefix_s + "Actor " + std::to_string(i) + " moves northeast.\n";
				}
				else {
					str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move northeast, but was blocked.\n";
				}
				break;
			case 'a':
			case 'A':
				if (world->getAt(actors->at(i)->getX() - 1, actors->at(i)->getY()) == terrain_t::EMPTY) {
					actors->at(i)->setX(actors->at(i)->getX() - 1);
					str_tell = prefix_s + "Actor " + std::to_string(i) + " moves west.\n";
				}
				else {
					str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move west, but was blocked.\n";
				}
				break;
			case 's':
			case 'S':
				if (world->getAt(actors->at(i)->getX(), actors->at(i)->getY() + 1) == terrain_t::EMPTY) {
					actors->at(i)->setY(actors->at(i)->getY() + 1);
					str_tell = prefix_s + "Actor " + std::to_string(i) + " moves south.\n";
				}
				else {
					str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move south, but was blocked.\n";
				}
				break;
			case 'd':
			case 'D':
				if (world->getAt(actors->at(i)->getX() + 1, actors->at(i)->getY()) == terrain_t::EMPTY) {
					actors->at(i)->setX(actors->at(i)->getX() + 1);
					str_tell = prefix_s + "Actor " + std::to_string(i) + " moves east.\n";
				}
				else {
					str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move east, but was blocked.\n";
				}
				break;
			case 'z':
			case 'Z':
				if (world->getAt(actors->at(i)->getX() - 1, actors->at(i)->getY() + 1) == terrain_t::EMPTY) {
					actors->at(i)->setX(actors->at(i)->getX() - 1);
					actors->at(i)->setY(actors->at(i)->getY() + 1);
					str_tell = prefix_s + "Actor " + std::to_string(i) + " moves southwest.\n";
				}
				else {
					str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move southwest, but was blocked.\n";
				}
				break;
			case 'c':
			case 'C':
				if (world->getAt(actors->at(i)->getX() + 1, actors->at(i)->getY() + 1) == terrain_t::EMPTY) {
					actors->at(i)->setX(actors->at(i)->getX() + 1);
					actors->at(i)->setY(actors->at(i)->getY() + 1);
					str_tell = prefix_s + "Actor " + std::to_string(i) + " moves southeast.\n";
				}
				else {
					str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move southeast, but was blocked.\n";
				}
				break;
			default:
				str_tell = prefix_s + "Actor " + std::to_string(i) + " sits around and does nothing.\n";
			}
		}
		tell(str_tell.data(), str_tell.length());
	}
	str_report = "actors size is " + std::to_string(actors->size()) + "\n";
	report(str_report.data(), str_report.length());
}

void Overseer::populate(){
	std::uniform_int_distribution<> randw(0, MAP_WIDTH);
	std::uniform_int_distribution<> randh(0, MAP_HEIGHT);
	for(int i = 0; i < ACTOR_NUM; i++){
		int x = randw(rng);
		int y = randh(rng);
		
		while(isOccupied(x, y)){
			x = randw(rng);
			y = randh(rng);
		}
		enterActor(new Actor(x, y));

	}
}


/*void Overseer::print() {
	system("CLS");
	if(actors->size() > 0)
		std::sort(actors->begin(), actors->end(), actor_pos_sort);
	
	int front = 0;
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			int index = x + y * MAP_WIDTH;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULTCOLOR);
			if(front < actors->size() && index == actors->at(front)->getX() + actors->at(front)->getY() * MAP_WIDTH){
				if (actors->at(front)->getAIState()) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), AICOLOR);
				}
				else {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PLAYERCOLOR);
				}
				std::cout << actors->at(front)->getLabel();
				
				front++;
			}else{
				std::cout << world->getAt(x, y);
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}*/

DWORD Overseer::getInput(INPUT_RECORD **eventBuffer)
{
	/* Variable for holding the number of current events, and a point to it */
	DWORD numEvents = 0;


	/* Variable for holding how many events were read */
	DWORD numEventsRead = 0;


	/* Put the number of console input events into numEvents */
	GetNumberOfConsoleInputEvents(rHnd, &numEvents);


	if (numEvents) /* if there's an event */
	{
		/* Allocate the correct amount of memory to store the events */
		*eventBuffer = (INPUT_RECORD*)malloc(sizeof(INPUT_RECORD) * numEvents);

		/* Place the stored events into the eventBuffer pointer */
		ReadConsoleInput(rHnd, *eventBuffer, numEvents, &numEventsRead);
	}


	/* Return the amount of events successfully read */
	return numEventsRead;
}

void Overseer::print() {
	//system("CLS");
	/* Write our character buffer (a single character currently) to the console buffer */
	for (int i = 0; i < (MAP_WIDTH) * (MAP_HEIGHT); i++) {
		CHAR_INFO cinfo;
		cinfo.Char.AsciiChar = ' ';
		cinfo.Attributes = DEFAULTCOLOR;
		consoleBuffer[i] = cinfo;
	}
	/*for (int i = 0; i < strlen(legend); i++) {
		CHAR_INFO cinfo;
		cinfo.Char.AsciiChar = legend[i];
		cinfo.Attributes = DEFAULTCOLOR;
		consoleBuffer[i] = cinfo;
	}*/
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			CHAR_INFO cinfo;
			cinfo.Char.AsciiChar = world->getAt(x, y);//(unsigned char)219;
			cinfo.Attributes = DEFAULTCOLOR;//rand() % 256;
			consoleBuffer[x + (MAP_WIDTH) * y] = cinfo;
		}
	}
	for (int i = 0; i < actors->size(); i++) {
		CHAR_INFO cinfo;
		cinfo.Char.AsciiChar = actors->at(i)->getLabel();
		cinfo.Attributes = AICOLOR;
		if (!actors->at(i)->getAIState()) {
			cinfo.Attributes = PLAYERCOLOR;
		}
		if(actors->at(i)->getX() >= 0 && actors->at(i)->getX() < MAP_WIDTH && actors->at(i)->getY() >= 0 && actors->at(i)->getY() < MAP_HEIGHT)
			consoleBuffer[actors->at(i)->getX() + actors->at(i)->getY() * (MAP_WIDTH)] = cinfo;
	}
	WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
}
void Overseer::enterActor(Actor* actor)
{
	actors->push_back(actor);
}
void Overseer::run() {
	//std::string unparsed;
	while (running) {
		update();
		print();
		//std::cin >> unparsed;
		while (running) {
			input = -1;
			DWORD eventsRead = getInput(&eventBuffer);
			if (eventsRead) {
				switch (eventBuffer[0].EventType) {
				case KEY_EVENT:
					input = eventBuffer[0].Event.KeyEvent.uChar.AsciiChar;
					break;
				}
			}
			if (input != -1) {
				break;
			}
		}
		//input = unparsed[0];
		
		switch (input) {
		case 'x':
			running = false;
			break;
		}
	}
}
