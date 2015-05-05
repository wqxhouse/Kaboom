#include "../../stdafx.h"
#include "ConfigSettings.h"

#ifdef _WIN32 
// Replace STL fstream with OSG fstream 
#include <osgDB/fstream> 
#define ifstream osgDB::ifstream 
#define ofstream osgDB::ofstream 
#else 
#include <fstream> 
#define std::ifstream std::ifstream 
#define std::ofstream std::ofstream 
#endif

//---------------- Add settings/properties here ----------------//
// rendering engine specific vars
std::string ConfigSettings::str_screen_width = "ScreenWidth";
std::string ConfigSettings::str_screen_height = "ScreenHeight";
std::string ConfigSettings::str_full_screen = "FullScreen";
std::string ConfigSettings::str_renderBuffer_width = "RenderBufferWidth";
std::string ConfigSettings::str_renderBuffer_height = "RenderBufferHeight";
std::string ConfigSettings::str_mediaFilePath = "MediaPath";
std::string ConfigSettings::str_screenPosX = "ScreenPosX";
std::string ConfigSettings::str_screenPosY = "ScreenPosY";

std::string ConfigSettings::str_material_xml = "MaterialXML";
std::string ConfigSettings::str_world_xml = "WorldXML";
std::string ConfigSettings::str_export_xml = "ExportXML";
std::string ConfigSettings::str_typeid_xml = "TypeIdXML";

// server and client shared vars
std::string ConfigSettings::str_server_port = "ServerPort";
std::string ConfigSettings::str_server_address = "ServerAddress";

// server specific vars
std::string ConfigSettings::str_max_client = "MaxClient";
std::string ConfigSettings::str_channel = "Channel";
std::string ConfigSettings::str_incoming_bandwidth = "IncomingBandwidth";
std::string ConfigSettings::str_outgoing_bandwidth = "OutgoingBandwidth";


typedef std::pair <std::string, std::string> String_Pair;
ConfigSettings * ConfigSettings::config = new ConfigSettings();

ConfigSettings::ConfigSettings(std::string fname, std::string template_fname) {
	file_name = fname;
	template_file_name = template_fname;

	if (!loadSettingsFile()){ //loads settings
		copySettingsTemplate();
	}
	else{
		copyMissingSettings();
	}
}

//Reads and loads the settings
bool ConfigSettings::loadSettingsFile(){

	ifstream myfile;
	myfile.open(file_name.c_str());

	if (myfile.is_open()){
		std::string line;
		while (myfile.good()){
			getline(myfile, line);
			size_t pos = line.find_first_of(':');

			if (pos > 0 && pos < line.length()){
				std::string key, value;
				key = line.substr(0, pos);
				value = line.substr(pos + 2);

				settings.insert(String_Pair(key, value));
			}
		}
		settings_loaded = true;
		myfile.close();
		return true;
	}

	return false;
}

//clears data read and loads from the settings file
bool ConfigSettings::reloadSettingsFile(){
	settings.clear();
	return loadSettingsFile();
}

//set the default settings from the config file
//loads the config file while copying
void ConfigSettings::copySettingsTemplate() {
	ifstream template_file;
	ofstream myfile;

	template_file.open(template_file_name.c_str());
	myfile.open(file_name.c_str());

	if (template_file.is_open()){
		std::string line;
		while (template_file.good()){
			getline(template_file, line);

			size_t pos = line.find_first_of(':');
			if (pos > 0 && pos < line.length()){
				std::string key, value;
				key = line.substr(0, pos);
				value = line.substr(pos + 2);


				if (myfile.is_open()){
					myfile << line << std::endl;
				}

				settings.insert(String_Pair(key, value)); //loads the config file while copying
			}
		}
		settings_loaded = true;
		template_file.close();
		myfile.close();
	}
}

void ConfigSettings::copyMissingSettings() {

	ifstream template_file;
	std::fstream myfile;

	template_file.open(template_file_name.c_str());
	myfile.open(file_name.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);

	if (template_file.is_open()){
		std::string line;
		while (template_file.good()){
			getline(template_file, line);

			size_t pos = line.find_first_of(':');
			if (pos > 0 && pos < line.length()){
				std::string key, value;
				key = line.substr(0, pos);
				value = line.substr(pos + 2);

				stdext::hash_map <std::string, std::string>::iterator i = settings.find(key);

				if (i == settings.end()){
					if (myfile.is_open()){
						myfile << line << std::endl;
					}
					settings.insert(String_Pair(key, value));
				}
			}
		}
		template_file.close();
		myfile.close();
	}

}

void ConfigSettings::saveSettingsFile(){

	ofstream myfile;

	myfile.open(file_name.c_str());

	if (myfile.is_open()){
		std::string line;
		stdext::hash_map <std::string, std::string>::iterator endIter = settings.end();
		stdext::hash_map <std::string, std::string>::iterator iter;

		for (iter = settings.begin(); iter != endIter; iter++){
			myfile << iter->first << ": " << iter->second << std::endl;
		}

		myfile.close();
	}

}

bool ConfigSettings::getValue(std::string key, std::string & ret){
	stdext::hash_map <std::string, std::string>::iterator i = settings.find(key);

	if (i != settings.end()){
		ret = i->second;
		return true;
	}
	return false;
}

bool ConfigSettings::getValue(std::string key, bool & ret){
	stdext::hash_map <std::string, std::string>::iterator i = settings.find(key);

	if (i != settings.end()){
		if (i->second == "true"){
			ret = true;
		}
		else{
			ret = false;
		}
		return true;
	}
	return false;
}

bool ConfigSettings::getValue(std::string key, int & ret){
	stdext::hash_map <std::string, std::string>::iterator i = settings.find(key);

	if (i != settings.end()){
		ret = atoi(i->second.c_str()); //error results in 0
		return true;
	}
	return false;
}

bool ConfigSettings::getValue(std::string key, float & ret){
	stdext::hash_map <std::string, std::string>::iterator i = settings.find(key);

	if (i != settings.end()){
		ret = (float)atof(i->second.c_str()); //error results in 0
		return true;
	}
	return false;
}

bool ConfigSettings::getValue(std::string key, double & ret){
	stdext::hash_map <std::string, std::string>::iterator i = settings.find(key);

	if (i != settings.end()){
		ret = atof(i->second.c_str()); //error results in 0
		return true;
	}
	return false;
}

void ConfigSettings::updateValue(std::string key, std::string value){
	settings.insert(String_Pair(key, value));
}


void ConfigSettings::updateValue(std::string key, bool value){
	if (value){
		settings.insert(String_Pair(key, "true"));
	}
	else{
		settings.insert(String_Pair(key, "false"));
	}
}

void ConfigSettings::updateValue(std::string key, int value){
	std::stringstream s;
	s << value;

	settings.insert(String_Pair(key, s.str()));
}


void ConfigSettings::updateValue(std::string key, float value){
	std::stringstream s;
	s << value;

	settings.insert(String_Pair(key, s.str()));
}

void ConfigSettings::updateValue(std::string key, double value){
	std::stringstream s;
	s << value;

	settings.insert(String_Pair(key, s.str()));
}

//returns true if loaded the saved settings, otherwise false
bool ConfigSettings::checkIfLoaded() {
	return settings_loaded;
}