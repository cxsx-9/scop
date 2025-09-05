#include "scop.hpp"

int checkFileAccessibility(const std::string &path) {
    std::ifstream file(path, std::ios::in);
    if(file.is_open()) {
        if (file.peek() == std::ifstream::traits_type::eof()) {
			std::cerr << "[Error] File is empty" << std::endl;
			file.close();
			return -1;
		}
		file.close();
    } else {
        std::cerr << "[Error] Cannot open file" << std::endl;
        file.close();
        return -1;
    }
    return 0;
}

void inputValidator(int ac, char **av){
    (void) av;
    if (ac < 2 || ac > 3) {
        std::cerr << "[Error] Usage : ./scop file.obj" << std::endl;
        exit(1);
    }
    std::string objPath = av[1];
    
    // Debug
    if (objPath == "mock") {
        std::cout << "[Debug]: using mock object" << std::endl;
        return;
    }

    if ((objPath.length() >= 4 && objPath.substr(objPath.length() - 4) != ".obj") || checkFileAccessibility(objPath) != 0) {
        std::cerr << "[Error] Invalid object file : " << objPath << std::endl;
        exit(1);
    }

    if (ac == 3) {
        std::string texPath = av[2];
        if ((texPath.length() >= 4 && (texPath.substr(texPath.length() - 4) != ".png" && texPath.substr(texPath.length() - 4) != ".jpg")) || checkFileAccessibility(texPath) != 0) {
            std::cerr << "[Error] Invalid texture file : " << texPath << std::endl;
            std::cerr << "[Error] Continue without texture" << std::endl;
            return;
        }
    }
}
