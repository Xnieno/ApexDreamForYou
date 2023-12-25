#include <iostream>
#include <string>

class Debugger {
public:
	Debugger(bool enableDebug = true) : enableDebug(enableDebug) {}

	template <typename T>
	void log(const T& variable, const std::string& customMessage = "") {
		if (!enableDebug) return;

		
		if (!customMessage.empty()) {
			std::cout << "Message: " << customMessage << std::endl;
		}
		std::cout << "Variable Value: " << variable << std::endl;
	}

	void log(const std::string& customMessage) {
		if (!enableDebug) return;

		std::cout << "DEBUG LOG:" << std::endl;
		if (!customMessage.empty()) {
			std::cout << "Message: " << customMessage << std::endl;
		}
	}

private:
	bool enableDebug;
};





