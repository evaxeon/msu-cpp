#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <map>
#include <fstream>
#include <typeinfo>
#include <functional>
#include "header.hpp"
//#include "main.cpp"

FileIterator& FileIterator::operator++() {
	int number;
	myFile.seekg(position, myFile.beg); 
	if (myFile.fail()) throw std::runtime_error("Seekg() error");
	if (myFile >> number) {
		position = myFile.tellg();
		lastElement = number;
	} else endFlag = true;
	if (myFile.eof()) return *this;
	if (myFile.fail()) throw std::runtime_error("File read error");
	return *this;

}

FileIterator& FileIterator::operator++(int) {
	int number;
	myFile.seekg(position, myFile.beg);
	if (myFile.fail()) throw std::runtime_error("Seekg() error from int");
	if (myFile >> number) {
		position = myFile.tellg();
		lastElement = number;
	} else endFlag = true;
	if (myFile.eof()) return *this;
	if (myFile.fail()) throw std::runtime_error("File read error");
	return *this;

} 


int getNumberByPos(int number, int lastNumber) {
	for (; lastNumber < number; lastNumber++) {
		if (number % lastNumber == 0) {
			return lastNumber;
		}
	}
}


std::ostream& operator<<(std::ostream& os, const Record& r) {
	for (std::pair<std::string, std::string> v: r.body) {
		os << v.first << ":" << v.second << std::endl;
		
	}
	return os;
}


