#include "scop.hpp"

void inputValidator(int ac, char **av){
    (void) av;
    if (ac != 2) {
        std::cerr << "Usage : ./scop file.obj" << std::endl;
        exit(1);
    }
    
    std::string path = av[1];
    
    // Debug
    if (path == "mock") {
        std::cout << "[Debug]: using mock object" << std::endl;
        return;
    }
    if (path.length() >= 4 && path.substr(path.length() - 4) != ".obj") {
        std::cerr << "Invalid object file" << std::endl;
        exit(1);
    }
    std::ifstream file(path, std::ios::in);
    if(file.is_open()) {
        if (file.peek() == std::ifstream::traits_type::eof()) {
			std::cerr << "The object file is empty" << std::endl;
			file.close();
			exit(1);
		}
		file.close();
    } else {
        std::cerr << "Cannot open object file" << std::endl;
        file.close();
        exit(1);
    }
}