#include <offsets/offsets.json>
#include <offsets/buttons.json>
#include <offsets/client_dll.json>
#include <json/value.h>
#include <json/json.h>
#include <iostream>
#include <fstream>
void parseJson() {
	std::ifstream offsets_file("offsets/offsets.json", std::ifstream::binary);
	Json::Value values;
	offsets_file >> values;
	std::cout << values;
}