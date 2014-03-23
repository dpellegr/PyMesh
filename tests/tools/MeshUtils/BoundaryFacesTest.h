#pragma once

#include <algorithm>
#include <iostream>
#include <string>

#include <Core/Exception.h>
#include <Mesh.h>
#include <MeshFactory.h>
#include <Misc/Environment.h>

#include <MeshUtils/BoundaryFaces.h>

class BoundaryFacesTest : public ::testing::Test {
    protected:
        typedef Mesh::Ptr MeshPtr;
        virtual void SetUp() {
            std::string project_dir = Environment::get("PYMESH_PATH");
            m_data_dir = project_dir + "/tests/data/";
        }

        void load_mesh(const std::string& filename) {
            std::string mesh_file = m_data_dir + filename;
            m_mesh = MeshPtr(MeshFactory().load_file(mesh_file).create());
        }

    protected:
        MeshPtr m_mesh;
        std::string m_data_dir;
};

TEST_F(BoundaryFacesTest, CubeBoundary) {
    load_mesh("cube.msh");
    const size_t vertex_per_face = m_mesh->get_vertex_per_face();
    BoundaryFaces bd(*m_mesh.get());
    ASSERT_EQ(12, bd.get_num_boundaries());

    // Check each boudnary vertex lies on the boundary.
    // A vertex is on the boundary of cube if it has a coordinate
    // equals 1 or -1
    for (size_t i=0; i<12; i++) {
        VectorI face = bd.get_boundary(i);
        for (size_t j=0; j<vertex_per_face; j++) {
            VectorF vertex = m_mesh->get_vertex(face[j]);
            Float max_coordinate = *std::max_element(vertex.data(), vertex.data() + vertex.size());
            Float min_coordinate = *std::min_element(vertex.data(), vertex.data() + vertex.size());
            ASSERT_FLOAT_EQ(1.0, std::max(fabs(min_coordinate), fabs(max_coordinate)));
        }
    }
}

TEST_F(BoundaryFacesTest, BoundaryElementsContainsBoundary) {
    load_mesh("cube.msh");
    BoundaryFaces bd(*m_mesh.get());
    const size_t num_boundaries = bd.get_num_boundaries();
    for (size_t i=0; i<num_boundaries; i++) {
        VectorI face = bd.get_boundary(i);
        size_t voxel_idx = bd.get_boundary_element(i);
        VectorI voxel = m_mesh->get_voxel(voxel_idx);
        std::set<size_t> voxel_v_set(voxel.data(), voxel.data()+voxel.size());
        for (size_t j=0; j<face.size(); j++) {
            ASSERT_FALSE(voxel_v_set.find(face[j]) == voxel_v_set.end());
        }
    }
}

TEST_F(BoundaryFacesTest, BoundaryNodes) {
    load_mesh("cube.msh");
    BoundaryFaces bd(*m_mesh.get());
    VectorI bd_nodes = bd.get_boundary_nodes();
    const size_t num_bd_nodes = bd_nodes.size();

    ASSERT_EQ(8, num_bd_nodes);

    // Check that all nodes are boundary nodes.
    std::sort(bd_nodes.data(), bd_nodes.data() + num_bd_nodes);
    for (size_t i=0; i<bd_nodes.size(); i++) {
        ASSERT_EQ(i, bd_nodes[i]);
    }
}

TEST_F(BoundaryFacesTest, ZeroVoxel) {
    load_mesh("cube.obj");
    ASSERT_THROW(BoundaryFaces bd(*m_mesh.get()), RuntimeError);
}

