//
//  main.cpp
//  catmullclark
//
//  Created by 菊池祐作 on 2024/09/20.
//

#include <iostream>
#include <vector>
#include <numbers>
#include <fstream>
#include "mesh.hpp"
#include "controlmesh.hpp"
#include "catmulllclark.hpp"
#include "readmesh.hpp"

int main(int argc, const char * argv[])
{
    auto start = std::chrono::system_clock::now();
    
    // メッシュファイル読み込み
    std::string filename;
    if (argc != 2){
        std::cout << "command line error\n";
        std::exit(1);
    }
    filename = std::string(argv[1]);

    ControlMesh cmesh;
    ReadMesh::read_obj(filename, cmesh);


    CatmullClark quadmesh(&cmesh);
    quadmesh.subdivide();

    ReadMesh::export_obj(filename, quadmesh.V, quadmesh.F);
    ReadMesh::open_file(filename);

    auto end = std::chrono::system_clock::now();
    using namespace std::chrono_literals;
    std::cerr << "実行時間 : " << (end - start) / 1.0s << " 秒\n";

    return 0;
}
