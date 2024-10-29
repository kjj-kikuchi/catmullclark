//
//  readmesh.cpp
//  catmullclark
//
//  Created by 菊池祐作 on 2024/09/20.
//

#include <fstream>
#include "readmesh.hpp"

void ReadMesh::read_obj(std::string const& filename, Mesh& mesh)
{
    std::ifstream ifs(filename);
    if (ifs.fail())
    {
        std::cerr << "Failed to open file." << "\n";
        std::exit(1);
    }
    std::string line;
    while (std::getline(ifs, line)){
        if (line.empty() || line[0] == '#')
        {
            // Do nothing
        }
        else if (line[0] == 'v')
        {
            Eigen::Vector3d v;
            std::istringstream string_in{line.substr(1)};
            string_in >> v(0) >> v(1) >> v(2);
            mesh.V.push_back(v);
        }
        else if (line[0] == 'f')
        {
            Eigen::Vector3i f;
            std::istringstream string_in{line.substr(1)};
            string_in >> f(0) >> f(1) >> f(2);
            f -= Eigen::Vector3i{1, 1, 1};
            mesh.F.push_back(f);
        }
    }
}

void ReadMesh::read_obj(std::string const& filename, ControlMesh& cmesh)
{
    std::ifstream ifs(filename);
    if (ifs.fail())
    {
        std::cerr << "Failed to open file." << "\n";
        std::exit(1);
    }
    std::string line;
    while (std::getline(ifs, line))
    {
        std::istringstream string_in(line);
        std::string type;
        string_in >> type;

        //if (line.empty() || line[0] == '#')
        //{
        //    // Do nothing
        //}
        if (type == "v")
        {
            Eigen::Vector3d v;
            string_in >> v(0) >> v(1) >> v(2);
            cmesh.V.push_back(v);
        }
        else if (type == "vt")
        {
            Eigen::Vector3d vt;
            string_in >> vt(0) >> vt(1) >> vt(2);
            cmesh.Vt.push_back(vt);
        }
        else if (type == "f")
        {
            Eigen::VectorXi f;
            Eigen::VectorXi ft;
            Eigen::VectorXi fn;
            std::vector<Eigen::Vector3d> fs;
            std::string face_data;
            int idx = 0;
            while (string_in >> face_data)
            {
                std::replace(face_data.begin(), face_data.end(), '/', ' ');
                std::istringstream face_data_in(face_data);
                f.conservativeResize(idx + 1);
                ft.conservativeResize(idx + 1);
                fn.conservativeResize(idx + 1);
                face_data_in >> f(idx) >> ft(idx) >> fn(idx);
                idx++;
            }
            f -= Eigen::VectorXi::Ones(idx);
            ft -= Eigen::VectorXi::Ones(idx);
            fn -= Eigen::VectorXi::Ones(idx);
            cmesh.F.push_back(f);
            cmesh.Ft.push_back(ft);
        }
    }
}


void ReadMesh::export_obj(std::string name,
                          std::vector<Eigen::Vector3d> const& vert,
                          std::vector<Eigen::VectorXi> const& face,
                          std::vector<Eigen::Vector3d> const& tex_coord,
                          std::vector<Eigen::VectorXi> const& tex_idx)
{
    std::ofstream of;
    name.erase(name.length()-4);
    std::string filename = name + "_output.obj";
    of.open(filename, std::ios::out);
    for(auto& v : vert)
    {
        of << "v " << v(0) << " " << v(1) << " " << v(2) << std::endl;
    }
    for (auto& vt : tex_coord)
    {
        of << "vt " << vt(0) << " " << vt(1) << " " << vt(2) << std::endl;
    }
    for (int i = 0; i < face.size(); i++)
    {
        of << "f";
        int size = (int)face[i].size();
        for (int j = 0; j < size; j++)
        {
            of << " " << face[i](j)+1 << "/" << tex_idx[i](j)+1 << "/" << face[i](j)+1;
        }
        of << std::endl;
    }
    of.close();
}

void ReadMesh::export_obj(std::string name, ControlMesh *cmesh)
{
    ReadMesh::export_obj(name, cmesh->V, cmesh->F, cmesh->Vt, cmesh->Ft);
}

void ReadMesh::export_obj(std::string name, std::vector<Eigen::Vector3d> const& vert, std::vector<Eigen::Vector4i> const& face)
{
    std::ofstream of;
    name.erase(name.length()-4);
    std::string filename = name + "_quadrangulated.obj";
    of.open(filename, std::ios::out);
    for(auto& v : vert)
    {
        of << "v " << v(0) << " " << v(1) << " " << v(2) << std::endl;
    }
    for(auto& f : face)
    {
        of << "f " << f(0)+1 << " " << f(1)+1 << " " << f(2)+1 << " " << f(3)+1 << std::endl;
    }
    of.close();
}

//void ReadMesh::export_obj(std::string name, CatmullClark *quadmesh)
//{
//    ReadMesh::export_obj(name, quadmesh->V, quadmesh->F);
//}


void ReadMesh::open_file(std::string name)
{
    // original mesh
    std::string command = "open -a /Applications/MeshLab2023.12.app " + name;
//    system(command.c_str());
    
    // subdivided mesh
    name.erase(name.length()-4);
    command = "open -a /Applications/MeshLab2023.12.app " + name + "_quadrangulated.obj";
    system(command.c_str());
}
