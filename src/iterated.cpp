#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <map>
#include <fstream>
#include <typeinfo>
#include <functional>
#include "header.hpp"
#include "main.cpp"


template <typename Val, typename It>
FilterIterator<Val, It>::FilterIterator(It beg_arg, It end_arg, bool (*predicate1)(Val)):predicate(predicate1), begin(beg_arg), end(end_arg), cur(beg_arg) {
	while (cur != end) {
		if ((*predicate)(*cur)) break;
		cur++;
	}
		
}

template <typename Val, typename It>
bool FilterIterator<Val, It>::operator==(const FilterIterator& other) {
		return (cur == other.cur);
}

template <typename Val, typename It>
bool FilterIterator<Val, It>::operator!=(const FilterIterator& other) {
		return (cur != other.cur);
}

template <typename Val, typename It>
FilterIterator<Val, It>& FilterIterator<Val, It>::operator++() {
	while (cur != end) {
		cur++;
		if ((*predicate)(*cur)) break;
	}
	return *this;
}

template <typename Val, typename It>
FilterIterator<Val, It>& FilterIterator<Val, It>::operator++(int) {
	while (cur != end) {
		cur++;
		if ((*predicate)(*cur)) break;
	}
	return *this;
}

template <typename Val, typename It>
Val FilterIterator<Val, It>::operator*() {
	return (*cur);
}


int getNumberByPos(int number, int lastNumber) {
	for (; lastNumber < number; lastNumber++) {
		if (number % lastNumber == 0) {
			return lastNumber;
		}
	}
}

FileIterator::FileIterator(std::string name1 = "default.txt", bool endFlag1 = false) { 
	name = name1; 
	myFile.open(name1, std::ios::in);
	if (myFile.fail()) throw std::runtime_error("File open error");
	(*this)++;
	endFlag = endFlag1;
}

FileIterator::FileIterator(const FileIterator& other) {
	position = other.position;
	name = other.name;
	lastElement = other.lastElement;
	myFile.open(name, std::ios::in);
	if (myFile.fail()) throw std::runtime_error("File open error (copy)");
	endFlag = other.endFlag;

}

bool FileIterator::operator!= (FileIterator const other) { return (endFlag != other.endFlag); }
bool FileIterator::operator== (FileIterator const other) { return (position == other.position); }
int FileIterator::operator*()  { return lastElement; }
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
	if (myFile.fail()) throw std::runtime_error("Seekg() error");
	if (myFile >> number) {
		position = myFile.tellg();
		lastElement = number;
	} else endFlag = true;
	if (myFile.eof()) return *this;
	if (myFile.fail()) throw std::runtime_error("File read error");
	return *this;

}

DivIterator::DivIterator(int num = 1, const int i = 1) { number = num; lastNumber = i;}
bool DivIterator::operator!= (DivIterator const other) const { return (lastNumber != other.lastNumber); } 
bool DivIterator::operator== (DivIterator const other) const { return (lastNumber == other.lastNumber); }
DivIterator& DivIterator::operator++() { 
	lastNumber = getNumberByPos(number, ++lastNumber);
	return *this; }
DivIterator& DivIterator::operator++(int) { 
	lastNumber = getNumberByPos(number, ++lastNumber);
	return *this; }
const int DivIterator::operator*() { 		
	return lastNumber;
}

template<class It, class It_const>
AllDividers<It, It_const>::AllDividers(int i) { number = i; }

template<class It, class It_const>
It AllDividers<It, It_const>::begin() { return DivIterator(number, 1); }

template<class It, class It_const>
It AllDividers<It, It_const>::end() { return DivIterator(number, number+1); }

template<class It, class It_const>
It_const AllDividers<It,It_const>::begin() const { return DivIterator(number, 1); }

template<class It, class It_const>
It_const AllDividers<It,It_const>::end() const { return DivIterator(number, number+1); }

template<class It, class It_const>
FilterIterator<int, It> AllDividers<It, It_const>::filter(bool (*predicate)(const int)) {
	return FilterIterator<int, It>(begin(), end(), predicate);
}

Record::Record(std::string a, std::string b) { add(a,b); }
void Record::add(std::string a, std::string b) { body.push_back(std::pair<std::string, std::string>(a, b)); }

std::ostream& operator<<(std::ostream& os, const Record& r) {
	for (std::pair<std::string, std::string> v: r.body) {
		os << v.first << ":" << v.second << std::endl;
		
	}
	return os;
}
std::vector<std::pair<std::string, std::string> >::iterator Record::begin() { return std::begin(body); }
std::vector<std::pair<std::string, std::string> >::iterator Record::end() { return std::end(body); }

std::vector<std::pair<std::string, std::string> >::const_iterator Record::begin() const { return std::cbegin(body); }
std::vector<std::pair<std::string, std::string> >::const_iterator Record::end() const { return std::cend(body); }

bool Record::operator==(const Record& other) { return (body == other.body); }


template<class It, class It_const>
It MimeAttachment<It, It_const>::begin() { return std::begin(records); } 

template<class It, class It_const>
It MimeAttachment<It, It_const>::end() { return std::end(records); }

template<class It, class It_const>
It_const MimeAttachment<It, It_const>::begin() const { return std::cbegin(records); }

template<class It, class It_const>
It_const MimeAttachment<It, It_const>::end() const { return std::cend(records); }

template<class It, class It_const>
void MimeAttachment<It, It_const>::push_back(Record v) { records.push_back(v); }

template<class It, class It_const>
FilterIterator<Record, It_const> MimeAttachment<It, It_const>::filter(bool (*predicate)(Record)) {
	return FilterIterator<Record, It_const>(begin(), end(), predicate);
}


template<class It, class It_const>
TextFile<It, It_const>::TextFile(std::string c) { name = c; }

template<class It, class It_const>	
It TextFile<It, It_const>::begin() { return FileIterator(name); }

template<class It, class It_const>
It TextFile<It, It_const>::end() {	return FileIterator(name, true); }

template<class It, class It_const>
It_const TextFile<It, It_const>::begin() const { return FileIterator(name); }
	
template<class It, class It_const>
It_const TextFile<It, It_const>::end() const { return FileIterator(name, true); }

template<class It, class It_const>
FilterIterator<int, It> TextFile<It, It_const>::filter(bool (*predicate)(const int)) { 
	return FilterIterator<int, It>(begin(), end(), predicate);
}

void printContainer(const Iterated<auto, auto, auto> &obj) {
	for(auto l: obj) {
		std::cout << l << std::endl;
	}
}