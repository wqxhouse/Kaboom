#ifndef _CONFIG_SETTINGS_H_
#define _CONFIG_SETTINGS_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <hash_map>
#include <iterator>

// Compatibility layer for VS2010 bug. The compiler tries to link the same 
// multiple symbols in for the OSG fstream classes which inherit from the STL 
// classes. The workaround is to use OSG fstream classes in all places rather 
// than STL. 


/*
* Note: Place Config.ini file in the same directory where
*       the .vcxproj file is located at.
*/

/*
//------------------ ConfigSettings (how-to-use example) ------------------//
#include "util/ConfigSettings.h"
#include <iostream>

int main() {

ConfigSettings* config = ConfigSettings::config;
int screen_width = 0;
config->getValue(ConfigSettings::str_screen_width, screen_width);

cout << "str_screen_width: " << screen_width << endl;
}
//------------------ ConfigSettings (end of code) ------------------//
*/

//---------------- Config file names ----------------//
const std::string str_settings_file = "Config.ini";						// Automatically generated
const std::string str_template_settings_file = "ConfigTemplate.ini";		// Modify settings in template file

class ConfigSettings {

public:
	//---------------- Add settings/properties here ----------------//
	// rendering engine vars
	static std::string str_screen_width;
	static std::string str_screen_height;
	static std::string str_full_screen;
	static std::string str_renderBuffer_width;
	static std::string str_renderBuffer_height;
	static std::string str_mediaFilePath;
	static std::string str_screenPosX;
	static std::string str_screenPosY;

	static std::string str_material_xml;
	static std::string str_world_xml;
	static std::string str_export_xml;

	// server and client shared vars
	static std::string str_server_address;
	static std::string str_server_port;
	
	// server specific vars
	static std::string str_max_client;
	static std::string str_channel;
	static std::string str_incoming_bandwidth;
	static std::string str_outgoing_bandwidth;

public:
	static ConfigSettings * config; // Use this static reference instead of your own call to the constructor

	bool checkIfLoaded();
	void saveSettingsFile();
	bool loadSettingsFile();
	bool reloadSettingsFile();

	void copySettingsTemplate();
	void copyMissingSettings();

	bool getValue(std::string key, bool & ret);
	bool getValue(std::string key, int & ret);
	bool getValue(std::string key, float & ret);
	bool getValue(std::string key, double & ret);
	bool getValue(std::string key, std::string & ret);

	void updateValue(std::string key, bool value);
	void updateValue(std::string key, int value);
	void updateValue(std::string key, float value);
	void updateValue(std::string key, double value);
	void updateValue(std::string key, std::string value);

private:
	//default parameters for a default constructor option
	ConfigSettings(std::string file_name = str_settings_file, std::string template_file_name = str_template_settings_file);


	stdext::hash_map<std::string, std::string> settings;
	bool settings_loaded;

	std::string file_name;
	std::string template_file_name;
};

#endif