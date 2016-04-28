#include <fstream>
#include <iostream>

#include "utilities/utilities.h"

std::string LoadFromFile(const std::string fileName){

	std::ifstream file;
	file.open((fileName).c_str());

	std::string line;
	std::string fileContent;

	if (file.is_open()){
		while (file.good()){
			getline(file, line);
			fileContent.append(line + '\n');
		}
	}
	file.close();
	return fileContent;
};

int UtilInt() {
	return 5;
}

void LogError(std::string prefix, std::string message){
	std::cout << prefix << ": " << message << std::endl;
}