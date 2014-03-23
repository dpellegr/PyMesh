#pragma once
#include <string>
#include <memory>

#include <Core/EigenTypedef.h>

class Material {
    public:
        typedef std::shared_ptr<Material> Ptr;
        static Ptr create(Float density, const MatrixF& material_tensor);
        static Ptr create_isotropic(size_t dim, Float desnity, Float young, Float poisson);
        static Ptr create_symmetric(Float density, const MatrixF& material_matrix);
        static Ptr create_orthotropic(Float density,
                const VectorF& young_modulus,
                const VectorF& poisson_ratio,
                const VectorF& shear_modulus);
        static Ptr create_periodic(Ptr mat1, Ptr mat2, VectorF axis,
                Float period, Float ratio, Float phase);

        virtual Float get_material_tensor(size_t i, size_t j, size_t k, size_t l, VectorF coord) const=0;
        virtual MatrixF strain_to_stress(const MatrixF& strain, VectorF coord) const=0;
        virtual Float get_density(VectorF coord) const=0;
        virtual Float get_density() const=0;
};
