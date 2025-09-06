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

int checkExtension(const std::string &filename, const std::string &extension) {
    if (filename.length() < extension.length() || filename.substr(filename.length() - extension.length()) != extension) {
        return -1;
    }
    std::cout << "[Info] File extension " << extension << " verified." << std::endl;
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
    if (checkExtension(objPath, ".obj") || checkFileAccessibility(objPath)) {
        std::cerr << "[Error] Invalid object file : " << objPath << std::endl;
        exit(1);
    }
}

int textureValidator(char *path){
    std::string texPath = path;
    if (((checkExtension(texPath, ".png")
        && checkExtension(texPath, ".jpg")
        && checkExtension(texPath, ".jpeg"))
        || checkFileAccessibility(texPath))) {
        std::cerr << "[Error] Invalid texture file : " << texPath << std::endl << "Continue without texture." << std::endl;
        return -1;
    }
    return 0;
}