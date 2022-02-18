#include "PlyLoader.h"

#include <sstream>
#include <fstream>
#include <iostream>


PlyLoader::PlyLoader(const std::string& path) {
    std::ifstream plyStream(path);
    std::string line;
    int verticesCnt = 0;

    plyStream >> line;
    if (line != std::string("ply")) {
        std::cerr << "Not a PLY format!" << std::endl;
        return;
    }

    // 参数信息
    while (getline(plyStream, line)) {

        if (line.find("element vertex ") == 0) {
            verticesCnt = std::stoi(line.substr(15));
            std::cout << verticesCnt << std::endl;
        }
        
        if (line.find("end_header") == 0) {
            break;
        }
    }

    Vec3 pos, normal, color;
    for (int i = 0; i < verticesCnt; ++i) {
        getline(plyStream, line);
        std::stringstream ss(line);
        ss >> pos.x >> pos.y >> pos.z >> normal.x >> normal.y >> normal.z >> color.x >> color.y >> color.z;
        vertices.push_back(pos);
        normals.push_back(normal);
        colors.push_back(color);
    }

}


//void PlyLoader::loadPlyFile(const std::string filePath, float* &positions, unsigned int* &indices, int &vertexFloatCount, int &indexIntCount, float scale,
//                            const float translateX, const float translateY, const float translateZ){
//    std::vector<float> positionsVector;
//    std::vector<float> indicesVector;
//    int vertexCount;
//    int triangleCount;
//    short state = 0;
//    unsigned int i=0;
//    
//    std::string s;
//    std::fstream fs(filePath);
//    while(std::getline(fs, s)){
//        if(state == 0){ //in header
//            if(s.find("element vertex ") == 0){
//                //parse vertexCount
//                vertexCount = std::stoi(s.substr(15));
//            }else if(s.find("element face ") == 0){
//                //parse triangleCount
//                triangleCount = std::stoi(s.substr(13));
//            }else if(s.find("end_header") == 0){
//                state = 1; //exit header
//                continue;
//            }
//        }else if(state == 1){ //not in header - loading vertices
//            if(i == vertexCount){
//                state = 2;
//                i = 0;
//                continue;
//            }
//            //load vertex coords
//            size_t idx = 0;
//            float x = std::stof(s, &idx);
//            s.erase(0, idx+1);
//            float y = std::stof(s, &idx);
//            s.erase(0, idx+1);
//            float z = std::stof(s, &idx);
//            positionsVector.push_back(x*scale + translateX);
//            positionsVector.push_back(z*scale + translateY);
//            positionsVector.push_back(y*scale + translateZ);
//            
//            i++;
//        }else if(state == 2){ //not in header - loading traingles
//            if(i == triangleCount){
//                break;
//            }
//            //load triangle point indexes
//            size_t idx = 0;
//            int num = std::stoi(s, &idx);
//            if(num != 3) std::cout << "WARN: num is " << num << " != 3!" << std::endl;
//            s.erase(0, idx+1);
//            int p1 = std::stoi(s, &idx);
//            s.erase(0, idx+1);
//            int p2 = std::stoi(s, &idx);
//            s.erase(0, idx+1);
//            int p3 = std::stoi(s, &idx);
//            indicesVector.push_back(p3);
//            indicesVector.push_back(p2);
//            indicesVector.push_back(p1);
//            
//            i++;
//        }
//        
//    }
//    fs.close();
//    positions = new float[positionsVector.size()];
//    for(i=0; i<positionsVector.size(); i++){
//        positions[i] = positionsVector[i];
//    }
//    vertexFloatCount = positionsVector.size();
//    indices = new unsigned int[indicesVector.size()];
//    for(i=0; i<indicesVector.size(); i++){
//        indices[i] = indicesVector[i];
//    }
//    indexIntCount = indicesVector.size();
//}
