#include "debug.hpp"

D(
void slowPrint(int middle, int after, std::string txt) {
	for (int i = 0; i < txt.size() - 1; i++) {
		std::cout << txt[i];
		std::this_thread::sleep_for(std::chrono::milliseconds(middle));
	}
	std::cout << txt[txt.size() - 1];
	std::this_thread::sleep_for(std::chrono::milliseconds(after));
}
)

D(
void OK() {
	std::cout << "OK" << std::endl;
}
)