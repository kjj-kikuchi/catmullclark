//
//  controlmesh.hpp
//  catmullclark
//
//  Created by 菊池祐作 on 2024/09/20.
//

#ifndef controlmesh_hpp
#define controlmesh_hpp

#include <iostream>
#include <vector>
#include <Eigen/Core>

class ControlMesh
{
public:
    std::vector<Eigen::Vector3d> V;
    std::vector<Eigen::VectorXi> F;
    std::vector<Eigen::Vector3d> Vt;
    std::vector<Eigen::VectorXi> Ft;
};


#endif /* controlmesh_hpp */
