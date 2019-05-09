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
	
	std::string seeder = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	std::cout << "Hello. Please enter desired seed:" << std::endl;
	std::string seed = "";
	std::cin >> seed;

	long seedVal = 0;
	
	for (int i = 0; i < seed.length(); i++) {
		int s = seeder.find(seed.at(i));
		if (s <= -1) {
			s = 0;
		}
		seedVal += s * (i + 1);
	}
	std::cout << "using seedVal " << seedVal << std::endl;
	if (seedVal <= 0) {
		initRNG();
	}
	else {
	
		initRNG(seedVal);
	}
	world = new Atlas();
	actors = new std::vector<Actor*>();
	world->create(MAP_WIDTH, MAP_HEIGHT);
	populate();
	if(actors->size() > 0)
		actors->front()->assume();
	for (int i = 0; i < actors->size(); i++) {
		std::vector<char> path;
		paths.push_back(path);
	}
}

bool Overseer::isOccupied(int x, int y, int actor){
	int index = x + y * MAP_WIDTH;
	if(world->getCostAt(x, y) == INFINITY){
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
			dijkstraesque(i, actors->at(i)->getGoalX(), actors->at(i)->getGoalY(), actors->at(i)->getX(), actors->at(i)->getY());
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
			dijkstraesque(i, actors->at(i)->getGoalX(), actors->at(i)->getGoalY(), actors->at(i)->getX(), actors->at(i)->getY());
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
			dijkstraesque(i, actors->at(i)->getGoalX(), actors->at(i)->getGoalY(), actors->at(i)->getX(), actors->at(i)->getY());
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
			dijkstraesque(i, actors->at(i)->getGoalX(), actors->at(i)->getGoalY(), actors->at(i)->getX(), actors->at(i)->getY());
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
			dijkstraesque(i, actors->at(i)->getGoalX(), actors->at(i)->getGoalY(), actors->at(i)->getX(), actors->at(i)->getY());
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
			dijkstraesque(i, actors->at(i)->getGoalX(), actors->at(i)->getGoalY(), actors->at(i)->getX(), actors->at(i)->getY());
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
			dijkstraesque(i, actors->at(i)->getGoalX(), actors->at(i)->getGoalY(), actors->at(i)->getX(), actors->at(i)->getY());
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
			dijkstraesque(i, actors->at(i)->getGoalX(), actors->at(i)->getGoalY(), actors->at(i)->getX(), actors->at(i)->getY());
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
bool Overseer::dijkstraesque(int index, int sx, int sy, int ex, int ey) {
	//std::cout << paths.size() << " out of " << (index) << std::endl;
	paths.at(index).clear();
	
	actors->at(index)->setGoal(sx, sy);

	intpair_t currpoint(sx, sy);
	intpair_t endpoint(ex, ey);
	
	intpair_t* floodMap = new intpair_t[MAP_WIDTH*MAP_HEIGHT];
	bool* checkMap = new bool[MAP_WIDTH*MAP_HEIGHT];
	for (int i = 0; i < MAP_WIDTH*MAP_HEIGHT; i++) {
		floodMap[i] = intpair_t(-1, -1);
		checkMap[i] = false;
	}
	
	//std::queue<intpair_t> points;

	auto cmp = [](intpair_t a, intpair_t b) { return a.cost > b.cost; };
	std::priority_queue < intpair_t, std::vector<intpair_t>, decltype(cmp)> points(cmp);
	
	bool reachable = false;

	std::queue<char> movechain;
	points.push(currpoint);
	while (points.size() > 0) {
		
		int x = points.top().x;
		int y = points.top().y;
		if (checkMap[x + y * MAP_WIDTH]) {
			//std::string reportstr = "\nUH OH\n";
			//report(reportstr.data(), reportstr.length());
			points.pop();
			continue;
		}
		//printf("x: %d y: %d\n", x, y);
		//std::string reportstr = "x: " + std::to_string(x) + " y: " + std::to_string(y) + '\n';
		//report(reportstr.data(), reportstr.length());
		checkMap[x + y * MAP_WIDTH] = true;
		floodMap[x + y * MAP_WIDTH] = points.top();
		/*
		for (int yy = 0; yy < MAP_HEIGHT; yy++) {
			for (int xx = 0; xx < MAP_WIDTH; xx++) {
				if (!checkMap[xx + yy * MAP_WIDTH]) {
					std::cout << " ";
				}
				else {
					std::cout << round(log(floodMap[xx + yy * MAP_WIDTH].cost));
				}
			}
			std::cout << std::endl;
		}
		for (int rr = 0; rr < MAP_WIDTH; rr++) {
			std::cout << '-';
		}
		*/
		if (x == ex && y == ey) {
			floodMap[sx + sy * MAP_WIDTH].move = points.top().origin;
			reachable = true;
			break;
		}
		points.pop();
		for (int ix = -1; ix <= 1; ix++) {
			for (int iy = -1; iy <= 1; iy++) {
				bool pass = false;
				if (ix == 0 && iy == 0) {
					pass = true;
				}
				if ((x + ix) < 0 || (x + ix) >= MAP_WIDTH || (y + iy) < 0 || (y + iy) >= MAP_HEIGHT) {
					pass = true;
				}
				if (checkMap[(x + ix) + (y + iy)*MAP_WIDTH]) {
					pass = true;
				}
				if (isOccupied(x+ix,y+iy,index))
					pass = true;
				if (!pass) {
					intpair_t thispoint(x + ix, y + iy);
					thispoint.px = x;
					thispoint.py = y;
					int mcost = 1;
					if (abs(ix) + abs(iy) > 1)
						mcost = 1.5;
					mcost *= world->getCostAt(x + ix, y + iy);
					int ecost = abs(ex - thispoint.x) + abs(ey - thispoint.y);
					thispoint.scost = floodMap[x + y * MAP_WIDTH].scost + mcost;
					thispoint.cost =  ecost + thispoint.scost;
					points.push(thispoint);
				}
			}
		}
	}
	std::string reportstr = "\nMADE IT HERE\n";
	report(reportstr.data(), reportstr.length());
	int cx = ex;
	int cy = ey;
	char move = ' ';
	bool emergencybrake = false;
	reportstr = "";
	while (reachable) {
		if (cx == sx && cy == sy) {
			break;
		}
		//std::cout << "(" << cx << ", " << cy << ") "; //<< sx << " " << sy << " " << ex << " " << ey << std::endl;
		//reportstr = "<" + std::to_string(cx) + "|" + std::to_string(cy) + ">" + reportstr;
		reportstr += "<" + std::to_string(cx) + "|" + std::to_string(cy) + ">";
		/*if (cx < 0 || cy < 0)
			break;*/
		int nx = floodMap[cx + cy * MAP_WIDTH].px;
		int ny = floodMap[cx + cy * MAP_WIDTH].py;
		/*if (nx < 0 || ny < 0)
			break;*/
		int i = nx - cx;
		int j = ny - cy;
		if (abs(i) + abs(j) > 2 || abs(i) - abs(j) > 1 || isOccupied(cx, cy, index)) {
			reportstr += "\n\n\nFUCKINGBREAK\n\n\n";
			//break;
		}
		switch (i) {
		case 0:
			switch (j) {
			case 0:
				move = ' ';
				break;
			case -1:
				move = 'w';
				break;
			case 1:
				move = 's';
				break;
			}
			break;
		case -1:
			switch (j) {
			case 0:
				move = 'a';
				break;
			case -1:
				move = 'q';
				break;
			case 1:
				move = 'z';
				break;
			}
			break;
		case 1:
			switch (j) {
			case 0:
				move = 'd';
				break;
			case -1:
				move = 'e';
				break;
			case 1:
				move = 'c';
				break;
			}
			break;
		}
		paths.at(index).push_back(move);
		
		cx = nx;
		cy = ny;
	}
	report(reportstr.data(), reportstr.length());
	reportstr = "\nACTOR STARTED AT <" + std::to_string(ex) + "|" + std::to_string(ey) + ">\n";
	report(reportstr.data(), reportstr.length());
	reportstr = "\nACTOR IS AT <" + std::to_string(actors->at(index)->getX()) + "|" + std::to_string(actors->at(index)->getY()) + ">\n";
	report(reportstr.data(), reportstr.length());
	reportstr = "\nACTOR GOAL AT <" + std::to_string(sx) + "|" + std::to_string(sy) + ">\n";
	report(reportstr.data(), reportstr.length());
	return reachable;
}
void Overseer::think(int i, std::string* str_tell) {
	Actor* actor = actors->at(i);

	int ax = actor->getX();
	int ay = actor->getY();


	std::uniform_int_distribution<> randw(0, MAP_WIDTH);
	std::uniform_int_distribution<> randh(0, MAP_HEIGHT);
	std::uniform_int_distribution<> smallw(ax - 10, ax + 10);
	std::uniform_int_distribution<> smallh(ay - 10, ay + 10);
	int ex = 0, ey = 0;
	if (actor->getAIState() == aistate_t::READY) {
		
		/*if (honeys->size() > 0) {
			std::shuffle(honeys->begin(), honeys->end(), rng);
			ex = honeys->front().x;
			ey = honeys->front().y;
		}
		else {*/	
		ex = randw(rng);
		ey = randh(rng);
		while (isOccupied(ex, ey)) {
			ex = randw(rng);
			ey = randh(rng);
		}
		//}
		if (dijkstraesque(i, ex, ey, ax, ay)) {
			/*std::string str_report;
			for (int ii = 0; ii < paths.at(i).size(); ii++) {
				str_report += paths.at(i).at(ii);
			}
			str_report += '\n';
			report(str_report.data(), str_report.length());*/
			actor->setAIState(aistate_t::SEARCHING);
		}
	}
	switch(actor->getAIState()){
	case aistate_t::READY:
		*str_tell = prefix_s + "Actor " + std::to_string(i) + " sits around and does nothing.\n";
		break;
	case aistate_t::SEARCHING:
		*str_tell = prefix_s + "Actor " + std::to_string(i) + " is searching.\n";
		break;
	}
	

}

void Overseer::update() {
	std::string str_report = "";
	std::string str_tell = legend;
	
	tell(str_tell.data(), str_tell.length());
	for (int i = 0; i < actors->size(); i++) {
		//if (world->getAt(actors->at(i)->getX(), actors->at(i)->getY()) != terrain_t::HONEY) {
		world->terraform(actors->at(i)->getX(), actors->at(i)->getY(), terrain_t::EMPTY);
		//}
		if (actors->at(i)->getAIState()) {
			think(i, &str_tell);
			if (paths.at(i).size() > 0) {
				parseInput(i, &str_tell, paths.at(i).front());
				paths.at(i).erase(paths.at(i).begin());
				
			}
			if (paths.at(i).size() == 0) {
				actors->at(i)->setAIState(aistate_t::READY);
			}
		}	
		else {
			parseInput(i, &str_tell);
		}
		str_report += "ACTOR " + std::to_string(i) + " is at <" + std::to_string(actors->at(i)->getX()) + "|" + std::to_string(actors->at(i)->getY()) + ">\n";
		report(str_report.data(), str_report.length());
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
