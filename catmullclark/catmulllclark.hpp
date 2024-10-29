//
//  catmulllclark.hpp
//  catmullclark
//
//  Created by 菊池祐作 on 2024/09/21.
//

#ifndef catmulllclark_hpp
#define catmulllclark_hpp

#include <iostream>
#include <map>
#include <utility>
#include <tuple>
#include "controlmesh.hpp"

class CatmullClark
{
    std::vector<Eigen::Vector3d> const *cV;
    std::vector<Eigen::VectorXi> const *cF;

    std::vector<double> v_weight;
    std::vector<int> valence;
    std::map<std::pair<int, int>, std::pair<int, int>> emap;    // key : 点ijの組み合わせ  値 : 面の添え字と面に関する辺の添え字

public:
    std::vector<Eigen::Vector3d> V;
    std::vector<Eigen::Vector4i> F;

    CatmullClark(ControlMesh *cmesh);
    void add_vertex(Eigen::Vector3d const& p, std::map<std::tuple<double, double, double>, int> &pmap, std::vector<int> &p_idx);
    void calc_opposite_edge();
    void calc_valence();
    void subdivide();
};

#endif /* catmulllclark_hpp */
