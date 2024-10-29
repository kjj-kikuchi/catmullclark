//
//  readmesh.hpp
//  catmullclark
//
//  Created by 菊池祐作 on 2024/09/20.
//

#ifndef readmesh_hpp
#define readmesh_hpp

#include <vector>
#include <string>
#include "mesh.hpp"
#include "controlmesh.hpp"
#include "catmulllclark.hpp"

namespace ReadMesh
{
    void read_obj(std::string const& filename, Mesh& mesh);
    void read_obj(std::string const& filename, ControlMesh& cmesh);
    void export_obj(std::string name, std::vector<Eigen::Vector3d> const& vert, std::vector<Eigen::VectorXi> const& face, std::vector<Eigen::Vector3d> const& tex_coord, std::vector<Eigen::VectorXi> const& tex_idx);
    void export_obj(std::string name, ControlMesh *cmesh);
    void export_obj(std::string name, std::vector<Eigen::Vector3d> const& vert, std::vector<Eigen::Vector4i> const& face);
//    void export_obj(std::string name, CatmullClark *quadmesh);

void open_file(std::string name);
}

#endif /* readmesh_hpp */
