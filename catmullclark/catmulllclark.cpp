//
//  catmulllclark.cpp
//  catmullclark
//
//  Created by 菊池祐作 on 2024/09/21.
//

#include "catmulllclark.hpp"

CatmullClark::CatmullClark(ControlMesh *cmesh)
{
    cV = &(cmesh->V);
    cF = &(cmesh->F);
    V = (*cV);
    valence.resize((*cV).size(), 0);
}

void CatmullClark::add_vertex(Eigen::Vector3d const& p, std::map<std::tuple<double, double, double>, int> &pmap, std::vector<int> &p_idx)
{
    auto pt = std::make_tuple(p(0), p(1), p(2));
    if (pmap.contains(pt))
    {
        p_idx.push_back(pmap.at(pt));
    }
    else
    {
        p_idx.push_back((int)pmap.size());
        pmap.emplace(pt, p_idx[p_idx.size()-1]);
        V.push_back(p);
    }
}

void CatmullClark::calc_opposite_edge()
{
    for (int i = 0; i < (*cF).size(); i++)
    {
        for (int j = 0; j < (*cF)[i].size(); j++)
        {
            Eigen::Vector3d p0 = (*cV)[(*cF)[i](j)];
            Eigen::Vector3d p1 = (*cV)[(*cF)[i]((j+1)%(*cF)[i].size())];

            auto edge = std::make_pair((*cF)[i](j), (*cF)[i]((j+1)%(*cF)[i].size()));
            auto f_v_idx = std::make_pair(i, j);

            emap.emplace(edge, f_v_idx);
        }
    }
}

void CatmullClark::calc_valence()
{
    for (int i = 0; i < (*cF).size(); i++)
    {
        for (int j = 0; j < (*cF)[i].size(); j++)
        {
            valence[(*cF)[i](j)]++;
        }
    }
}

void CatmullClark::subdivide()
{
    std::map<std::tuple<double, double, double>, int> pmap;
    std::vector<std::vector<Eigen::Vector3d>> edgeP((*cF).size());
    std::vector<Eigen::Vector3d> faceP((*cF).size());

    // v_weight.resize((*cV).size(), 0);

    calc_opposite_edge();
    calc_valence();

    int vnum = (int)V.size();

    // initialize edgeP and V
    for (int i = 0; i < edgeP.size(); i++)
    {
        edgeP[i].resize((*cF)[i].size(), Eigen::Vector3d::Zero());
    }
    for (int i = 0; i < (*cV).size(); i++)
    {
        double n = (double)valence[i];
        V[i] = (n - 2) / n * (*cV)[i];
        // v_weight[i] += (n - 2) / n;
    }

    // calc face_vertices
    for (int i = 0; i < (*cF).size(); i++)
    {
        Eigen::Vector3d centroid = Eigen::Vector3d::Zero();
        for (int j = 0; j < (*cF)[i].size(); j++)
        {
            centroid += (*cV)[(*cF)[i](j)];
        }
        centroid /= (*cF)[i].size();
        faceP[i] = centroid;
    }

    // cals edge_vertices and original_vertices
    for (int i = 0; i < (*cF).size(); i++)
    {
        for (int j = 0; j < (*cF)[i].size(); j++)
        {
            int idx = (*cF)[i](j);
            // edge
            auto edge_opp = std::make_pair((*cF)[i]((j+1)%(*cF)[i].size()), (*cF)[i](j));
            int f = emap.at(edge_opp).first;
            int v = emap.at(edge_opp).second;

            edgeP[i][j] += ( (*cV)[idx] + faceP[i] ) / 4;
            edgeP[f][v] += ( (*cV)[idx] + faceP[i] ) / 4;

            // original
            double n = (double)valence[idx];
            V[idx] += (*cV)[(*cF)[i]((j+1)%(*cF)[i].size())] / (n*n) + faceP[i] / (n*n);
            // v_weight[idx] += 2 / (n*n);
        }
    }

    // add data
    for (int i = 0; i < (*cF).size(); i++)
    {
        std::vector<int> p_idx; // 分割前の面に関する頂点の添え字を保存

        // add vertex
        for (int j = 0; j < (*cF)[i].size(); j++)
        {
            add_vertex(edgeP[i][j], pmap, p_idx);
        }
        add_vertex(faceP[i], pmap, p_idx);
        p_idx.resize(p_idx.size()-1);

        // add face
        for (int j = 0; j < (*cF)[i].size(); j++)
        {
            F.push_back(Eigen::Vector4i{(*cF)[i](j), vnum + p_idx[j], (int)V.size()-1, vnum + p_idx[(j+p_idx.size()-1)%p_idx.size()]});
        }
    }
}
