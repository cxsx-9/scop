#include "objectLoader.hpp"
#include "model.hpp"

std::vector<float> v = {
    // +Z (front)
    -0.5f,-0.5f, 0.5f,    0,0,1,        1,0,0,
     0.5f,-0.5f, 0.5f,    0,0,1,        1,0,0,
     0.5f, 0.5f, 0.5f,    0,0,1,        1,0,0,
    -0.5f, 0.5f, 0.5f,    0,0,1,        1,0,0,
    // -Z (back)
    -0.5f,-0.5f,-0.5f,    0,0,-1,       0,1,0,
     0.5f,-0.5f,-0.5f,    0,0,-1,       0,1,0,
     0.5f, 0.5f,-0.5f,    0,0,-1,       0,1,0,
    -0.5f, 0.5f,-0.5f,    0,0,-1,       0,1,0,
    // +X (right)
     0.5f,-0.5f, 0.5f,    1,0,0,        0,0,1,
     0.5f,-0.5f,-0.5f,    1,0,0,        0,0,1,
     0.5f, 0.5f,-0.5f,    1,0,0,        0,0,1,
     0.5f, 0.5f, 0.5f,    1,0,0,        0,0,1,
    // -X (left)
    -0.5f,-0.5f,-0.5f,   -1,0,0,        1,1,0,
    -0.5f,-0.5f, 0.5f,   -1,0,0,        1,1,0,
    -0.5f, 0.5f, 0.5f,   -1,0,0,        1,1,0,
    -0.5f, 0.5f,-0.5f,   -1,0,0,        1,1,0,
    // +Y (top)
    -0.5f, 0.5f, 0.5f,    0,1,0,        1,0,1,
     0.5f, 0.5f, 0.5f,    0,1,0,        1,0,1,
     0.5f, 0.5f,-0.5f,    0,1,0,        1,0,1,
    -0.5f, 0.5f,-0.5f,    0,1,0,        1,0,1,
    // -Y (bottom)
    -0.5f,-0.5f,-0.5f,    0,-1,0,       0,1,1,
     0.5f,-0.5f,-0.5f,    0,-1,0,       0,1,1,
     0.5f,-0.5f, 0.5f,    0,-1,0,       0,1,1,
    -0.5f,-0.5f, 0.5f,    0,-1,0,       0,1,1,
};

std::vector<unsigned int> idx = {
    0,1,2,      2,3,0,      // +Z
    4,5,6,      6,7,4,      // -Z
    8,9,10,     10,11,8,    // +X
    12,13,14,   14,15,12,   // -X
    16,17,18,   18,19,16,   // +Y
    20,21,22,   22,23,20    // -Y
};

void mockup(Model &model) {
    model.name = "Opengl";
    model.interleaved = v;
    model.indices = idx;
}

void objectLoader(const std::string &path, Model &model){
    
    std::ifstream file(path, std::ios::in);
    if(path.empty() || !file.is_open()) {
        std::cerr << "[Error] Cannot open object file from : '" << path << "'" << std::endl;
        mockup(model);
        return;
    }



    

}

