/*****
* Filename: ParseLine.hpp
* Author: Patrick Dodd
* Description: Header File for ParseLine.cpp
*****/
#define MAX 1000

// Struct definitions
struct RoomAction {
	char roomAction[MAX][MAX];
};

struct ObjectAction {
	char objectAction[MAX][MAX];
};

struct Rooms {
	char rooms[MAX][MAX];
};

struct Objects {
	char objects[MAX][MAX];
};

// Function Prototypes
int parseLine(char* token, RoomAction* r_action, ObjectAction* o_action, Rooms* rooms, Objects* objects);
void commandLoop(RoomAction* r_action, ObjectAction* o_action, Rooms* rooms, Objects* objects);
int actionType(char* token, RoomAction* r_action, ObjectAction* o_action);
int isRoom(char* token, Rooms* rooms, Objects* objects);
int isObject(char*token, Objects* objects, Rooms* rooms);
