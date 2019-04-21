#include "stdafx.h"
#include "overseer.h"
#include "rng.h"

bool actor_pos_sort(Actor* a, Actor* o) {
	return a->getIndex() < o->getIndex();
}
Overseer::Overseer()
{
	init();
	run();
	actors = new std::vector<Actor*>();
}
Overseer::~Overseer() {

}

void Overseer::init()
{
	initRNG();
	world = new Atlas();
	actors = new std::vector<Actor*>();
	world->create(MAP_WIDTH, MAP_HEIGHT);
	populate();
	//actors->front()->assume();
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
	if(actors->size() > 0)
		std::sort(actors->begin(), actors->end(), actor_pos_sort);
	
	int front = 0;
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			int index = x + y * MAP_WIDTH;
			if(front < actors->size() && index == actors->at(front)->getX() + actors->at(front)->getY() * MAP_WIDTH){
				std::cout << actors->at(front)->getLabel();
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
	char pause;
	while (running) {
		print();
		std::cout << "Enter 'q' to continue.." << std::endl;
		std::cin >> pause;
		running = false;
	}
}
