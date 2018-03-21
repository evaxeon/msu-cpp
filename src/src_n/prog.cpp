#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <map>
#include <fstream>
#include <typeinfo>
#include <functional>

template <typename Val, typename It>
class FilterIterator: public std::iterator<std::input_iterator_tag, Val> {
public:
	It begin;
	It end;
	It cur;
	bool (*predicate)(Val) = nullptr;
	FilterIterator(It beg_arg, It end_arg, bool (*predicate1)(Val)):predicate(predicate1), begin(beg_arg), end(end_arg), cur(beg_arg) {
		while (cur != end) {
			if ((*predicate)(*cur)) break;
			cur++;
		}
		
	};
	bool operator==(const FilterIterator& other) {
		return (cur == other.cur);
	}
	bool operator!=(const FilterIterator& other) {
		return (cur != other.cur);
	}
	FilterIterator& operator++() {
		while (cur != end) {
			cur++;
			if ((*predicate)(*cur)) break;
		}
		return *this;
	}
	FilterIterator& operator++(int) {
		while (cur != end) {
			cur++;
			if ((*predicate)(*cur)) break;
		}
		return *this;
	}
	Val operator*() {
		return (*cur);
	}


}; 

template<typename It, typename It_const, typename Val>
class Iterated {
public:
	virtual It begin() = 0;
	virtual It end() = 0;

	virtual It_const begin() const = 0;
	virtual It_const end() const = 0;

	virtual FilterIterator<Val, It_const> filter(bool (*predicate)(const Val)) = 0;


};



int getNumberByPos(int number, int lastNumber) {
	for (; lastNumber < number; lastNumber++) {
		if (number % lastNumber == 0) {
			return lastNumber;
		}
	}
}

class FileIterator: public std::iterator<std::input_iterator_tag, int>
{
public:

	int position = 0;
	std::string name;
	std::ifstream myFile;
	int lastElement = 0;
	bool endFlag;


	FileIterator(std::string name1 = "default.txt", bool endFlag1 = false) { 
		name = name1; 
		myFile.open(name1, std::ios::in);
		if (myFile.fail()) throw std::runtime_error("Construct file open error");
		(*this)++;
		endFlag = endFlag1;
	 }
	FileIterator(const FileIterator& other) {
		position = other.position;
		name = other.name;
		lastElement = other.lastElement;
		myFile.open(name, std::ios::in);
		if (myFile.fail()) throw std::runtime_error("Copy constructor file open error");
		endFlag = other.endFlag;

	}

	bool operator!= (FileIterator const other) { return (endFlag != other.endFlag); }
	bool operator== (FileIterator const other) { return (position == other.position); }
	FileIterator& operator++();
	FileIterator& operator++(int);
	int operator*()  { 
		return lastElement;
	}

};

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


class DivIterator: public std::iterator<std::input_iterator_tag, int>
{
public:
	int number;
	int lastNumber;
	DivIterator(int num = 1, const int i = 1) { number = num; lastNumber = i;}
	bool operator!= (DivIterator const other) const { return (lastNumber != other.lastNumber); } // *this != other
	bool operator== (DivIterator const other) const { return (lastNumber == other.lastNumber); }
	DivIterator& operator++() { 
		lastNumber = getNumberByPos(number, ++lastNumber);
		return *this; }
	DivIterator& operator++(int) { 
		lastNumber = getNumberByPos(number, ++lastNumber);
		return *this; }
	const int operator*() { 		
		return lastNumber;
	}
};


template<class It, class It_const>
class AllDividers: public Iterated<It, It_const, int>
{
public:
	int number;
	typedef DivIterator iterator;

	AllDividers(int i) { number = i; }

	It begin() { return iterator(number, 1); }
	It end() {
		return iterator(number, number+1);
	}

	It_const begin() const { return iterator(number, 1); }
	It_const end() const {
		return iterator(number, number+1);
	}
	FilterIterator<int, It> filter(bool (*predicate)(const int)) {
		return FilterIterator<int, It>(begin(), end(), predicate);
	}
};


class Record {
	public:
	std::vector<std::pair<std::string, std::string> > body;

	Record(std::string a, std::string b) {
		add(a,b);
	}
	void add(std::string a, std::string b) {
		body.push_back(std::pair<std::string, std::string>(a, b));
	}
	std::vector<std::pair<std::string, std::string> >::iterator begin() { return std::begin(body); }
	std::vector<std::pair<std::string, std::string> >::iterator end() { return std::end(body); }

	std::vector<std::pair<std::string, std::string> >::const_iterator begin() const { return std::cbegin(body); }
	std::vector<std::pair<std::string, std::string> >::const_iterator end() const { return std::cend(body); }
	bool operator==(const Record& other) {
		return (body == other.body);
	}


};

std::ostream& operator<<(std::ostream& os, const Record& r) {
	for (std::pair<std::string, std::string> v: r.body) {
		os << v.first << ":" << v.second << std::endl;
		
	}
	return os;
}





template<class It, class It_const>
class MimeAttachment: public Iterated<It, It_const, Record>
{
public:
	std::vector<Record> records;
	It begin() { return std::begin(records); } 
	It end() { return std::end(records); }

	It_const begin() const { return std::cbegin(records); }
	It_const end() const { return std::cend(records); }

	void push_back(Record v) { records.push_back(v); }
	FilterIterator<Record, It_const> filter(bool (*predicate)(Record)) {
		return FilterIterator<Record, It_const>(begin(), end(), predicate);
	}
};


template<class It, class It_const>
class TextFile: public Iterated<It, It_const, int>
{
	std::string name;

public:
	typedef FileIterator iterator;
	TextFile(std::string c) { name = c; }
	It begin() {
		return iterator(name);
	}

	It end() {
		return iterator(name, true);
	}

	It_const begin() const { return iterator(name); }
	It_const end() const { return iterator(name, true); }
	FilterIterator<int, It> filter(bool (*predicate)(const int)) {
		return FilterIterator<int, It>(begin(), end(), predicate);
	}
};

void printContainer(const Iterated<auto, auto, auto> &obj) {
	for(auto l: obj) {
		std::cout << l << std::endl;
	}
}
int main() {
	std::cout << "              EMPTY CONTAINERS\n\n" << std::endl;
	std::cout << "    -Empty MIME\n" << std::endl;
	MimeAttachment<std::vector<Record>::iterator, std::vector<Record>::const_iterator> k;
	for(auto s: k) {
		std::cout << s << std::endl;
	}

	std::cout << "              FILLING CONTAINERS\n\n" << std::endl;
	std::cout << "    -AllDividers\n" << std::endl;
	AllDividers<DivIterator, DivIterator> m(36);
	std::cout << " -PRINTING AllDividers from loop" << std::endl;
	for (auto c: m) {
		std::cout << "Divider of 36: " << c << std::endl;
	};

	std::cout << " -find function output: " << std::endl;
	auto p = std::find(m.begin(), m.end(), 18);
	if (p != m.end()) {
		std::cout << "Element found: " << *p <<  std::endl;
	} else {
		std::cout << "Element not found" << std::endl;
	}
	std::cout << " -FILTER function output" << std::endl;

	auto filtvar = (m.filter([] (int probe) -> bool {
		return (probe > 10);
	}));

	for(int temp1 = 0; temp1 < 3; temp1++) {
		std::cout << *(filtvar) << std::endl;
		filtvar++;
	}
	std::cout << " -printContainer function output" << std::endl;
	printContainer(m); 

	std::cout << "    -MIME Attachments\n" << std::endl;
	Record rec("Label1", "anotherstring");
	rec.add("Label2", "yetanotherstring");

	Record rec2("Name1", "stringthingie");
	rec2.add("Name2", "cutestring");

	k.push_back(rec);
	k.push_back(rec2);

	for(auto s: k) {
		std::cout << "MIME Element: " << std::endl;
		std::cout << s << std::endl;
	} 
	std::cout << " -find function output:" << std::endl;
	auto g = std::find(k.begin(), k.end(), rec2);
	if (g != k.end()) {
		std::cout << "Element found in MIME " << std::endl;
	} else {
		std::cout << "Element not found" << std::endl;
	}
	std::cout << " -printContainer function output" << std::endl;
	printContainer(k); 

	try {
		std::cout << "    -START OF WORK WITH FILE\n" << std::endl;
		TextFile<FileIterator, FileIterator> f("numbers.txt");
		for (auto wtf: f) {
			std::cout << "Number from file: " << wtf << std::endl;
		}

		std::cout << " -find function output: " << std::endl;
		auto t = std::find(f.begin(), f.end(), 555);
		if (t != f.end()) {
			std::cout << "Element found " << *t << std::endl;
		} else {
			std::cout << "Element not found" << std::endl;
		} 

		std::cout << " -FILTER function output: " << std::endl;
		auto filtvar1 = (f.filter([] (int probe) -> bool {
			return ((probe % 2) == 0);
		}));

		for (int temp = 0; temp < 5; temp++) {
			std::cout << *(filtvar1) << " " << std::endl;
			filtvar1++;
		}

		std::cout << " -printContainer function output: " << std::endl;
		printContainer(f);
		std::cout << "    -END OF WORK WITH FILE\n" << std::endl;
	} catch(std::runtime_error s) {
		std::cout << "Some error occured: " << s.what() <<  std::endl;
	}
}

