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
	FilterIterator(It beg_arg, It end_arg, bool (*predicate1)(Val));
	bool operator==(const FilterIterator& other);
	bool operator!=(const FilterIterator& other);
	FilterIterator& operator++();
	FilterIterator& operator++(int);
	Val operator*();
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

int getNumberByPos(int number, int lastNumber);
class FileIterator: public std::iterator<std::input_iterator_tag, int>
{
public:

	int position = 0;
	std::string name;
	std::ifstream myFile;
	int lastElement = 0;
	bool endFlag;
	FileIterator(std::string name1, bool endFlag1);
	FileIterator(const FileIterator& other);

	bool operator!= (FileIterator const other);
	bool operator== (FileIterator const other);
	FileIterator& operator++();
	FileIterator& operator++(int);
	int operator*();
};

class DivIterator: public std::iterator<std::input_iterator_tag, int>
{
public:
	int number;
	int lastNumber;
	DivIterator(int num, const int i);
	bool operator!= (DivIterator const other) const;// *this != other
	bool operator== (DivIterator const other) const;
	DivIterator& operator++();
	DivIterator& operator++(int);
	const int operator*();
};

template<class It, class It_const>
class AllDividers: public Iterated<It, It_const, int>
{
public:
	int number;
	typedef DivIterator iterator;

	AllDividers(int i);

	It begin();
	It end();
	It_const begin() const;
	It_const end() const;
	FilterIterator<int, It> filter(bool (*predicate)(const int));
};

class Record {
	public:
	std::vector<std::pair<std::string, std::string> > body;

	Record(std::string a, std::string b);
	void add(std::string a, std::string b);
	std::vector<std::pair<std::string, std::string> >::iterator begin();
	std::vector<std::pair<std::string, std::string> >::iterator end();

	std::vector<std::pair<std::string, std::string> >::const_iterator begin() const;
	std::vector<std::pair<std::string, std::string> >::const_iterator end() const;
	bool operator==(const Record& other);

};

std::ostream& operator<<(std::ostream& os, const Record& r);

template<class It, class It_const>
class MimeAttachment: public Iterated<It, It_const, Record>
{
public:
	std::vector<Record> records;
	It begin();
	It end();

	It_const begin() const;
	It_const end() const;

	void push_back(Record v);
	FilterIterator<Record, It_const> filter(bool (*predicate)(Record));
};

template<class It, class It_const>
class TextFile: public Iterated<It, It_const, int>
{
	std::string name;

public:
	typedef FileIterator iterator;
	TextFile(std::string c);
	It begin();

	It end();

	It_const begin() const;
	It_const end() const;
	FilterIterator<int, It> filter(bool (*predicate)(const int));
};

void printContainer(const Iterated<auto, auto, auto> &obj);