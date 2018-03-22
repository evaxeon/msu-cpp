#include "header.hpp"
//фильтр(константный), exception-safety, функция print без auto, дублирование кода(опционально)
int main() {
	std::cout << "              EMPTY CONTAINERS\n\n" << std::endl;
	std::cout << "    -Empty MIME\n" << std::endl;
	try {
		MimeAttachment<std::vector<Record>::iterator, std::vector<Record>::const_iterator> k;
		for(auto s: k) {
			std::cout << s << std::endl;
		}

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

	} catch (std::bad_alloc error) {
		std::cout << "Bad alloc: " << error.what() << std::endl;
	} 



	std::cout << "              FILLING CONTAINERS\n\n" << std::endl;
	std::cout << "    -AllDividers\n" << std::endl;
	const AllDividers<DivIterator, DivIterator> m(36);
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
