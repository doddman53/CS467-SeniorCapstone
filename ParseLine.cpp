/*****
* Filename: ParseLine.cpp
* Author: Patrick Dodd
* Description: Implementation for ParseLine.hpp
*****/

#define _CRT_SECURE_NO_WARNINGS
#include "ParseLine.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <cstdlib>
#include <istream>
#include <sstream>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::istream;
using std::getline;
using std::malloc;

int main() {

	/* For testing -- not to be used in final version */
	char rAction[8][100] = {
		"move",
		"Move",
		"go",
		"Go",
		"walk",
		"Walk",
		"run",
		"Run"
	};

	/* For testing -- not to be used in final version */
	char oAction[8][100] = {
		"pick",
		"Pick",
		"grab",
		"Grab",
		"use",
		"Use",
		"look",
		"Look"
	};

	/* For testing -- not to be used in final version */
	char rms[3][100] = {
		"cave",
		"vault",
		"cellar"
	};

	/* For testing -- not to be used in final version */
	char objs[3][100] = {
		"knife",
		"shovel",
		"hammer"
	};

	/* Initialize all structs and fill their arrays */

	RoomAction* r_action = (RoomAction*)malloc(sizeof(RoomAction));
	for (int i = 0; i < 8; i++) {
		strcpy(r_action->roomAction[i], rAction[i]);
	}

	ObjectAction* o_action = (ObjectAction*)malloc(sizeof(ObjectAction));
	for (int i = 0; i < 8; i++) {
		strcpy(o_action->objectAction[i], oAction[i]);
	}

	Rooms* rooms = (Rooms*)malloc(sizeof(Rooms));
	for (int i = 0; i < 3; i++) {
		strcpy(rooms->rooms[i], rms[i]);
	}

	Objects* objects = (Objects*)malloc(sizeof(Objects));
	for (int i = 0; i < 3; i++) {
		strcpy(objects->objects[i], objs[i]);
	}


	// Start the game
	commandLoop(r_action, o_action, rooms, objects);
	return 0;
}

/*****
* Function: void commandLoop(RoomAction* r_action, ObjectAction* o_action, Rooms* rooms, Objects* objects)
* Parameters: RoomAction* r_action, ObjectAction* o_action, Rooms* rooms, Objects* objects
* Description: keeps the command prompt active until the game
  ends or the user quits.
*****/
void commandLoop(RoomAction* r_action, ObjectAction* o_action, Rooms* rooms, Objects* objects) {
	char line[100]; // Array to hold user's entered line
	int activeGame = 0; // controls the loop
	int action;
	bool is_room = false, is_object = false; // flags to control search

	while (activeGame != -1) {
		memset(line, '\0', sizeof(line)); // Clear the array
		cout << "Enter line: ";
		cin.getline(line, 100); // Get input
	
		// Split line into tokens
		char* token = strtok(line, " ");
		while (token != NULL) {
			activeGame = parseLine(token, r_action, o_action, rooms, objects);
			
			// If the user entered a room action verb, search for a room name
			if (activeGame == 1) {
				is_room = true;
				while (is_room && token != NULL) {
					action = isRoom(token, rooms, objects);
					if (action == 1)
						is_room = false;
					else {
						token = strtok(NULL, " "); // Find the next token
						if (token == NULL && action != -1)
							cout << "You can't move if you don't specify a room name...:P" << endl;
					}
				}
			}

			// If the user entered an object action verb, search for an object name
			if (activeGame == 2) {
				is_object = true;
				while (is_object && token != NULL) {
					action = isObject(token, objects, rooms);
					if (action == 2)
						is_object = false;
					else {
						token = strtok(NULL, " "); // Find the next token
						if (token == NULL && action != -1)
							cout << "You didn't select an object." << endl;
					}
				}
			}
			token = strtok(NULL, " "); // Find the next token
		}
	}
}

/*****
* Function: void parseLine(char* token, RoomAction* r_action, ObjectAction* o_action, Rooms* rooms, Objects* objects)
* Parameters: char* token, RoomAction* r_action, ObjectAction* o_action, Rooms* rooms, Objects* objects
* Description: Parses the user's command and determines the action
  the game takes via the game engine
*****/
int parseLine(char* token, RoomAction* r_action, ObjectAction* o_action, Rooms* rooms, Objects* objects) {

	int verb = 0;
	verb = actionType(token, r_action, o_action);
	if (verb == 1) return 1; // Check for room action
	if (verb == 2) return 2; // Check for object action

	else {
		// List keywords for user if they type the command 'help'
		if (strcmp(token, "help") == 0 || strcmp(token, "Help") == 0) {
			cout << "Insert help message here..." << endl;
			return 3;
		}

		// Save game
		if (strcmp(token, "savegame") == 0 || strcmp(token, "Savegame") == 0) {
			cout << "Savegame logic here..." << endl;
			return 4;
		}

		// Load game
		if (strcmp(token, "loadgame") == 0 || strcmp(token, "Loadgame") == 0) {
			cout << "Loadgame logic here..." << endl;
			return 5;
		}

		// Quit game
		if (strcmp(token, "quit") == 0 || strcmp(token, "Quit") == 0) {
			cout << "Quit logic here..." << endl;
			return -1;
		}
	}
	return 0;
}

/*****
* Function: int actionType(char* token, RoomAction* r_action, ObjectAction* o_action)
* Parameters: char* token, RoomAction* r_action, ObjectAction* o_action
* Description: takes a token, r_action, and o_action as parameters - compares the token
  to the contents of each action word array to determine what type of object to search for
  next in the user's input, if any at all.
*****/
int actionType(char* token, RoomAction* r_action, ObjectAction* o_action)
{
	// Check for room actions
	for (int i = 0; i < 8; i++) {
		if (strcmp(r_action->roomAction[i], token) == 0) {
			//cout << token << " is a room action verb." << endl;
			return 1;
		}
		if (strcmp(o_action->objectAction[i], token) == 0) {
			//cout << token << " is an object action verb." << endl;
			return 2;
		}
	}
	return 0;
}

/*****
* Function: int isRoom(char* token, Rooms* rooms, Objects* objects)
* Parameters: char* token, Rooms* rooms, Objects* objects
* Description: if actionType() determines the user entered a room
  action verb, this function searches for a room name. Compares token
  to the contents of the rooms and objects arrays - if token matches
  an item in rooms array, prints message stating the user has moved to
  that room and returns 1. If token matches with an item in objects array,
  user is given an error message and prompted to choose a room name and 
  returns -1.
*****/
int isRoom(char* token, Rooms* rooms, Objects* objects)
{
	for (int i = 0; i < 3; i++) {
		if (strcmp(rooms->rooms[i], token) == 0) {
			cout << "You have moved to the " << token << "." << endl;
			return 1;
		}
		if (strcmp(objects->objects[i], token) == 0) {
			cout << token << " is not a room name.\nPlease enter a room name." << endl;
			return -1;
		}
	}
	return 0;
}

/*****
* Function: int isObject(char* token, Objects* objects, Rooms* rooms)
* Parameters: char* token, Objects* objects, Rooms* rooms
* Description: if actionType() determines the user entered an object
action verb, this function searches for an object name. Compares token
to the contents of the rooms and objects arrays - if token matches
an item in objects array, prints message stating the user has found the
object and returns 1. If token matches with an item in rooms array,
user is given an error message and prompted to choose an object name and
returns -1.
*****/
int isObject(char* token, Objects* objects, Rooms* rooms)
{
	for (int i = 0; i < 3; i++) {
		if (strcmp(objects->objects[i], token) == 0) {
			cout << "You have found the " << token << "." << endl;
			return 2;
		}
		if (strcmp(rooms->rooms[i], token) == 0) {
			cout << token << " is not an object.\nPlease enter an object name." << endl;
			return -1;
		}
	}
	return 0;
}
