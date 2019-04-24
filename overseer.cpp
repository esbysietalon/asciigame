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
	
	/* Window size coordinates, be sure to start index at zero! 
	SMALL_RECT windowSize = { 0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1 };

	/* A COORD struct for specificying the console's screen buffer dimensions 
	COORD bufferSize = { WINDOW_WIDTH, WINDOW_HEIGHT };

	/* Setting up different variables for passing to WriteConsoleOutput 
	COORD characterBufferSize = { WINDOW_WIDTH, WINDOW_HEIGHT };
	COORD characterPosition = { 0, 0 };
	SMALL_RECT consoleWriteArea = { 0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1 };

	/* A CHAR_INFO structure containing data about a single character 
	//CHAR_INFO consoleBuffer[WINDOW_WIDTH * WINDOW_HEIGHT];

	/* initialize handles 
	HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE rHnd = GetStdHandle(STD_INPUT_HANDLE);

	/* Set the console's title 
	/* Set the window size 
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

	/* Set the screen's buffer size 
	SetConsoleScreenBufferSize(wHnd, bufferSize);

	/*for (y = 0; y < WINDOW_HEIGHT; ++y)
	{
	for (x = 0; x < WINDOW_WIDTH; ++x)
	{
	consoleBuffer[x + WINDOW_WIDTH * y].Char.AsciiChar = (unsigned char)219;
	consoleBuffer[x + WINDOW_WIDTH * y].Attributes = rand() % 256;
	}
	}*/

	/* Write our character buffer (a single character currently) to the console buffer */
	//WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
	
	


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
	std::string str_tell;
	report(str_report.data(), str_report.length());
	
	for (int i = 0; i < actors->size(); i++) {
		if (actors->at(i)->getAIState()) {
			str_tell = prefix_s + "Actor " + std::to_string(i) + " sits around and does nothing.\n";
			
		}
		else {
			switch (input) {
			case 'q':
			case 'Q':
				actors->at(i)->setX(actors->at(i)->getX() - 1);
				actors->at(i)->setY(actors->at(i)->getY() - 1);
				str_tell = prefix_s + "Actor " + std::to_string(i) + " moves northwest.\n";
				break;
			case 'w':
			case 'W':
				actors->at(i)->setY(actors->at(i)->getY() - 1);
				str_tell = prefix_s + "Actor " + std::to_string(i) + " moves north.\n";
				break;
			case 'e':
			case 'E':
				actors->at(i)->setX(actors->at(i)->getX() + 1);
				actors->at(i)->setY(actors->at(i)->getY() - 1);
				str_tell = prefix_s + "Actor " + std::to_string(i) + " moves northeast.\n";
				break;
			case 'a':
			case 'A':
				actors->at(i)->setX(actors->at(i)->getX() - 1);
				str_tell = prefix_s + "Actor " + std::to_string(i) + " moves west.\n";
				break;
			case 's':
			case 'S':
				actors->at(i)->setY(actors->at(i)->getY() + 1);
				str_tell = prefix_s + "Actor " + std::to_string(i) + " moves south.\n";
				break;
			case 'd':
			case 'D':
				actors->at(i)->setX(actors->at(i)->getX() + 1);
				str_tell = prefix_s + "Actor " + std::to_string(i) + " moves east.\n";
				break;
			case 'z':
			case 'Z':
				actors->at(i)->setX(actors->at(i)->getX() - 1);
				actors->at(i)->setY(actors->at(i)->getY() + 1);
				str_tell = prefix_s + "Actor " + std::to_string(i) + " moves southwest.\n";
				break;
			case 'c':
			case 'C':
				actors->at(i)->setX(actors->at(i)->getX() + 1);
				actors->at(i)->setY(actors->at(i)->getY() + 1);
				str_tell = prefix_s + "Actor " + std::to_string(i) + " moves southeast.\n";
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

void Overseer::printLegend()
{
	printf("Press 'x' to exit. Press 'wasdqezc' to navigate.\n");
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


void Overseer::print() {
	system("CLS");
	if(actors->size() > 0)
		std::sort(actors->begin(), actors->end(), actor_pos_sort);
	
	int front = 0;
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			int index = x + y * MAP_WIDTH;
			if(front < actors->size() && index == actors->at(front)->getX() + actors->at(front)->getY() * MAP_WIDTH){
				if (actors->at(front)->getAIState()) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), AICOLOR);
				}
				else {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PLAYERCOLOR);
				}
				std::cout << actors->at(front)->getLabel();
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULTCOLOR);
				front++;
			}else{
				std::cout << world->getAt(x, y);
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void Overseer::enterActor(Actor* actor)
{
	actors->push_back(actor);
}
void Overseer::run() {
	std::string unparsed;
	while (running) {
		update();
		print();
		printLegend();
		std::cin >> unparsed;
		input = unparsed[0];
		
		switch (input) {
		case 'x':
			running = false;
			break;
		}
	}
}
