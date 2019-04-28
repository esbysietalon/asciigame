#include "stdafx.h"
#include "overseer.h"
#include "rng.h"
#include <queue>


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

bool Overseer::isOccupied(int x, int y, int actor){
	int index = x + y * MAP_WIDTH;
	if(world->getAt(x, y) != terrain_t::EMPTY){
		return true;
	}
	for(int i = 0; i < actors->size(); i++){
		if (actor == i)
			continue;
		if (index == actors->at(i)->getIndex()) {
			return true;
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

void Overseer::parseInput(int i, std::string* str_tell) {
	switch (input) {
	case 'q':
	case 'Q':
		if (!isOccupied(actors->at(i)->getX() - 1, actors->at(i)->getY() - 1, i)) {
			actors->at(i)->setX(actors->at(i)->getX() - 1);
			actors->at(i)->setY(actors->at(i)->getY() - 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves northwest.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move northwest, but was blocked.\n";
		}

		break;
	case 'w':
	case 'W':
		if (!isOccupied(actors->at(i)->getX(), actors->at(i)->getY() - 1, i)) {
			actors->at(i)->setY(actors->at(i)->getY() - 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves north.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move north, but was blocked.\n";
		}
		break;
	case 'e':
	case 'E':
		if (!isOccupied(actors->at(i)->getX() + 1, actors->at(i)->getY() - 1, i)) {
			actors->at(i)->setX(actors->at(i)->getX() + 1);
			actors->at(i)->setY(actors->at(i)->getY() - 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves northeast.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move northeast, but was blocked.\n";
		}
		break;
	case 'a':
	case 'A':
		if (!isOccupied(actors->at(i)->getX() - 1, actors->at(i)->getY(), i)) {
			actors->at(i)->setX(actors->at(i)->getX() - 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves west.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move west, but was blocked.\n";
		}
		break;
	case 's':
	case 'S':
		if (!isOccupied(actors->at(i)->getX(), actors->at(i)->getY() + 1, i)) {
			actors->at(i)->setY(actors->at(i)->getY() + 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves south.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move south, but was blocked.\n";
		}
		break;
	case 'd':
	case 'D':
		if (!isOccupied(actors->at(i)->getX() + 1, actors->at(i)->getY(), i)) {
			actors->at(i)->setX(actors->at(i)->getX() + 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves east.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move east, but was blocked.\n";
		}
		break;
	case 'z':
	case 'Z':
		if (!isOccupied(actors->at(i)->getX() - 1, actors->at(i)->getY() + 1, i)) {
			actors->at(i)->setX(actors->at(i)->getX() - 1);
			actors->at(i)->setY(actors->at(i)->getY() + 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves southwest.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move southwest, but was blocked.\n";
		}
		break;
	case 'c':
	case 'C':
		if (!isOccupied(actors->at(i)->getX() + 1, actors->at(i)->getY() + 1, i)) {
			actors->at(i)->setX(actors->at(i)->getX() + 1);
			actors->at(i)->setY(actors->at(i)->getY() + 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves southeast.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move southeast, but was blocked.\n";
		}
		break;
	default:
		*str_tell = prefix_s + "Actor " + std::to_string(i) + " sits around and does nothing.\n";
	}
}

void Overseer::parseInput(int i, std::string* str_tell, char in) {
	switch (in) {
	case 'q':
	case 'Q':
		if (!isOccupied(actors->at(i)->getX() - 1, actors->at(i)->getY() - 1, i)) {
			actors->at(i)->setX(actors->at(i)->getX() - 1);
			actors->at(i)->setY(actors->at(i)->getY() - 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves northwest.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move northwest, but was blocked.\n";
		}

		break;
	case 'w':
	case 'W':
		if (!isOccupied(actors->at(i)->getX(), actors->at(i)->getY() - 1, i)) {
			actors->at(i)->setY(actors->at(i)->getY() - 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves north.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move north, but was blocked.\n";
		}
		break;
	case 'e':
	case 'E':
		if (!isOccupied(actors->at(i)->getX() + 1, actors->at(i)->getY() - 1, i)) {
			actors->at(i)->setX(actors->at(i)->getX() + 1);
			actors->at(i)->setY(actors->at(i)->getY() - 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves northeast.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move northeast, but was blocked.\n";
		}
		break;
	case 'a':
	case 'A':
		if (!isOccupied(actors->at(i)->getX() - 1, actors->at(i)->getY(), i)) {
			actors->at(i)->setX(actors->at(i)->getX() - 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves west.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move west, but was blocked.\n";
		}
		break;
	case 's':
	case 'S':
		if (!isOccupied(actors->at(i)->getX(), actors->at(i)->getY() + 1, i)) {
			actors->at(i)->setY(actors->at(i)->getY() + 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves south.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move south, but was blocked.\n";
		}
		break;
	case 'd':
	case 'D':
		if (!isOccupied(actors->at(i)->getX() + 1, actors->at(i)->getY(), i)) {
			actors->at(i)->setX(actors->at(i)->getX() + 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves east.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move east, but was blocked.\n";
		}
		break;
	case 'z':
	case 'Z':
		if (!isOccupied(actors->at(i)->getX() - 1, actors->at(i)->getY() + 1, i)) {
			actors->at(i)->setX(actors->at(i)->getX() - 1);
			actors->at(i)->setY(actors->at(i)->getY() + 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves southwest.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move southwest, but was blocked.\n";
		}
		break;
	case 'c':
	case 'C':
		if (!isOccupied(actors->at(i)->getX() + 1, actors->at(i)->getY() + 1, i)) {
			actors->at(i)->setX(actors->at(i)->getX() + 1);
			actors->at(i)->setY(actors->at(i)->getY() + 1);
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " moves southeast.\n";
		}
		else {
			*str_tell = prefix_s + "Actor " + std::to_string(i) + " attempted to move southeast, but was blocked.\n";
		}
		break;
	default:
		*str_tell = prefix_s + "Actor " + std::to_string(i) + " sits around and does nothing.\n";
	}
}

void add_to_path(char** path, int* len, int* size, char item) {
	std::cout << *len << " " << *size << std::endl;
	if (*len + 1 >= *size) {
		*size *= 2;
		*path = (char *) realloc(*path, (*size) * sizeof(char));
	}
	(*path)[*len] = item;
	(*len)++;
}
void Overseer::dijkstraesque(int sx, int sy, int ex, int ey) {
	path.clear();
	intpair_t currpoint(sx, sy);
	intpair_t endpoint(ex, ey);
	intpair_t* floodMap = new intpair_t[MAP_WIDTH*MAP_HEIGHT];
	bool* checkMap = new bool[MAP_WIDTH*MAP_HEIGHT];
	for (int i = 0; i < MAP_WIDTH*MAP_HEIGHT; i++) {
		floodMap[i] = intpair_t(-1, -1);
		checkMap[i] = false;
	}
	
	std::queue<intpair_t> points;
	std::queue<char> movechain;
	points.push(currpoint);
	while (points.size() > 0) {
		int x = points.front().x;
		int y = points.front().y;
		if (checkMap[x + y * MAP_WIDTH]) {
			points.pop();
			continue;
		}
		//printf("x: %d y: %d\n", x, y);
		std::string reportstr = "x: " + std::to_string(x) + " y: " + std::to_string(y) + '\n';
		report(reportstr.data(), reportstr.length());
		checkMap[x + y * MAP_WIDTH] = true;
		floodMap[x + y * MAP_WIDTH] = points.front();
		
		/*for (int yy = 0; yy < MAP_HEIGHT; yy++) {
			for (int xx = 0; xx < MAP_WIDTH; xx++) {
				std::cout << floodMap[xx + yy * MAP_WIDTH].move << ' ';
			}
			std::cout << std::endl;
		}*/
		if (x == ex && y == ey) {
			floodMap[sx + sy * MAP_WIDTH].move = points.front().origin;
			break;
		}
		points.pop();
		for (int ix = -1; ix <= 1; ix++) {
			for (int iy = -1; iy <= 1; iy++) {
				if (ix == 0 && iy == 0)
					continue;
				if ((x + ix) < 0 || (x + ix) >= MAP_WIDTH || (y + iy) < 0 || (y + iy) >= MAP_HEIGHT)
					continue;
				if (checkMap[(x + ix) + (y + iy)*MAP_WIDTH])
					continue;
				if (world->getAt(x + ix, y + iy) != terrain_t::EMPTY)
					continue;
				intpair_t thispoint(x + ix, y + iy);
				switch (ix) {
				case -1:
					switch (iy) {
					case -1:
						thispoint.move = 'q';
						if (x == sx && y == sy)
							thispoint.origin = 'q';
						else
							thispoint.origin = floodMap[x + y * MAP_WIDTH].origin;
						break;
					case 0:
						thispoint.move = 'a';
						if (x == sx && y == sy)
							thispoint.origin = 'a';
						else
							thispoint.origin = floodMap[x + y * MAP_WIDTH].origin;

						break;
					case 1:
						thispoint.move = 'z';
						if (x == sx && y == sy)
							thispoint.origin = 'z';
						else
							thispoint.origin = floodMap[x + y * MAP_WIDTH].origin;

						break;
					}
					break;
				case 0:
					switch (iy) {
					case -1:
						thispoint.move = 'w';
						if (x == sx && y == sy)
							thispoint.origin = 'w';
						else
							thispoint.origin = floodMap[x + y * MAP_WIDTH].origin;

						break;
					case 0:
						break;
					case 1:
						thispoint.move = 's';
						if (x == sx && y == sy)
							thispoint.origin = 's';
						else
							thispoint.origin = floodMap[x + y * MAP_WIDTH].origin;

						break;
					}
					break;
				case 1:
					switch (iy) {
					case -1:
						thispoint.move = 'e';
						if (x == sx && y == sy)
							thispoint.origin = 'e';
						else
							thispoint.origin = floodMap[x + y * MAP_WIDTH].origin;

						break;
					case 0:
						thispoint.move = 'd';
						if (x == sx && y == sy)
							thispoint.origin = 'd';
						else
							thispoint.origin = floodMap[x + y * MAP_WIDTH].origin;

						break;
					case 1:
						thispoint.move = 'c';
						if (x == sx && y == sy)
							thispoint.origin = 'c';
						else
							thispoint.origin = floodMap[x + y * MAP_WIDTH].origin;

						break;
					}
					break;
				}
				thispoint.parent = x + y * MAP_WIDTH;
				points.push(thispoint);
			}
		}
	}
	int cx = sx;
	int cy = sy;
	bool emergencybrake = false;
	while (cx != ex && cy != ey) {
		//std::cout << "cx: " << cx << " cy: " << cy << std::endl;
		path.push_back(floodMap[cx + cy * MAP_WIDTH].move);
		std::string str = floodMap[cx + cy * MAP_WIDTH].move+"";
		//report(str.data(), str.length());
		switch (floodMap[cx + cy * MAP_WIDTH].move) {
		case 'q':
			cx--;
			cy--;
			break;
		case 'w':
			cy--;
			break;
		case 'e':
			cx++;
			cy--;
			break;
		case 'a':
			cx--;
			break;
		case 's':
			cy++;
			break;
		case 'd':
			cx++;
			break;
		case 'z':
			cx--;
			cy++;
			break;
		case 'c':
			cx++;
			cy++;
			break;
		default:
			emergencybrake = true;
		}
		if (emergencybrake)
			break;
	}
}
void Overseer::think(int i, std::string* str_tell) {
	Actor* actor = actors->at(i);
	
	int ax = actor->getX();
	int ay = actor->getY();
	

	std::uniform_int_distribution<> randw(0, MAP_WIDTH);
	std::uniform_int_distribution<> randh(0, MAP_HEIGHT);
	std::uniform_int_distribution<> smallw(ax - 10, ax + 10);
	std::uniform_int_distribution<> smallh(ay - 10, ay + 10);
	
	if (actor->getAIState() == aistate_t::READY) {
		int ex = randw(rng), ey = randh(rng);
		while (isOccupied(ex, ey)) {
			ex = randw(rng);
			ey = randh(rng);
		}
		dijkstraesque(ax, ay, ex, ey);
		std::string str_report;
		for (int i = 0; i < path.size(); i++) {
			str_report += path.at(i);
		}
		str_report += '\n';
		//report(str_report.data(), str_report.length());
		actor->setAIState(aistate_t::SEARCHING);
	}
	switch(actor->getAIState){
	case aistate_t::READY:
		*str_tell = prefix_s + "Actor " + std::to_string(i) + " sits around and does nothing.\n";
		break;
	case aistate_t::SEARCHING:
		*str_tell = prefix_s + "Actor " + std::to_string(i) + " is searching.\n";
		break;
	}
	

}

void Overseer::update() {
	std::string str_report = "update\n";
	std::string str_tell = legend;
	report(str_report.data(), str_report.length());
	tell(str_tell.data(), str_tell.length());
	for (int i = 0; i < actors->size(); i++) {
		if (actors->at(i)->getAIState()) {
			think(i, &str_tell);
			if (path.size() > 0) {
				parseInput(i, &str_tell, path.front());
				path.erase(path.begin());
				
			}
			if (path.size() == 0) {
				actors->at(i)->setAIState(aistate_t::READY);
			}
		}	
		else {
			parseInput(i, &str_tell);
		}
		tell(str_tell.data(), str_tell.length());
		print();
	}
	//str_report = "actors size is " + std::to_string(actors->size()) + "\n";
	//report(str_report.data(), str_report.length());
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
	for (int i = 0; i < (MAP_WIDTH) * (MAP_HEIGHT); i++) {
		CHAR_INFO cinfo;
		cinfo.Char.AsciiChar = ' ';
		cinfo.Attributes = DEFAULTCOLOR;
		consoleBuffer[i] = cinfo;
	}
	
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
		//print();
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
