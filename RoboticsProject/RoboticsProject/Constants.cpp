/* Bydlocode starts. */

#include "tinyxml2.h"
#include <iostream>

#include "Constants.h"

using namespace std;
using namespace tinyxml2;

int Constants::IMG_WIDTH;
int Constants::IMG_HEIGHT;
int Constants::FOC_WIDTH;
int Constants::FOC_HEIGHT;
int Constants::THRESH_H_LOW;
int Constants::THRESH_H_HIGH;
int Constants::THRESH_S_LOW;
int Constants::THRESH_V_LOW;
int Constants::THRESH_V_HIGH;
int Constants::EROSION_SIZE;
int Constants::VEL_LINEAR;
int Constants::VEL_ANGULAR;
int Constants::VEL_SCALE;
int Constants::ADDR_LCD;
int Constants::ADDR_DISTS;
int Constants::ADDR_MOTION;

double Constants::AREA_RATIO;

Constants::Constants(char* filename) {
	loadConstants(filename);
}

int loadConstants(char* filename) {

	XMLDocument doc;
	XMLError e = doc.LoadFile(filename);

	if (e != XML_SUCCESS) {
		cerr << "File load error." << endl;
		return e;
	}
	XMLElement* root = doc.FirstChildElement("config");
	XMLElement* ints = root->FirstChildElement("int");

	Constants::IMG_WIDTH = ints->FirstChildElement("IMG_WIDTH")->IntAttribute("value");
	Constants::IMG_HEIGHT = ints->FirstChildElement("IMG_WIDTH")->IntAttribute("value");
	Constants::FOC_WIDTH = ints->FirstChildElement("FOC_WIDTH")->IntAttribute("value");
	Constants::FOC_HEIGHT = ints->FirstChildElement("FOC_HEIGHT")->IntAttribute("value");
	Constants::THRESH_H_LOW = ints->FirstChildElement("THRESH_H_LOW")->IntAttribute("value");
	Constants::THRESH_H_HIGH = ints->FirstChildElement("THRESH_H_HIGH")->IntAttribute("value");
	Constants::THRESH_S_LOW = ints->FirstChildElement("THRESH_S_LOW")->IntAttribute("value");
	Constants::THRESH_V_LOW = ints->FirstChildElement("THRESH_V_LOW")->IntAttribute("value");
	Constants::THRESH_V_HIGH = ints->FirstChildElement("THRESH_V_HIGH")->IntAttribute("value");
	Constants::EROSION_SIZE = ints->FirstChildElement("EROSION_SIZE")->IntAttribute("value");
	Constants::VEL_LINEAR = ints->FirstChildElement("VEL_LINEAR")->IntAttribute("value");
	Constants::VEL_ANGULAR = ints->FirstChildElement("VEL_ANGULAR")->IntAttribute("value");
	Constants::VEL_SCALE = ints->FirstChildElement("VEL_SCALE")->IntAttribute("value");
	// Attempt
	Constants::ADDR_LCD = ints->FirstChildElement("ADDR_LCD")->IntAttribute("value");
	Constants::ADDR_DISTS = ints->FirstChildElement("ADDR_DISTS")->IntAttribute("value");
	Constants::ADDR_MOTION = ints->FirstChildElement("ADDR_MOTION")->IntAttribute("value");

	XMLElement* doubles = root->FirstChildElement("float");

	Constants::AREA_RATIO = doc.FirstChildElement("config")->DoubleAttribute("value");

	cout << "Constants loaded" << endl;

	return 0;
}