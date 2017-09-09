#if !defined LS_CALCULATOR_IMPL_HPP
#define LS_CALCULATOR_IMPL_HPP

#include <fstream>

#include "filesystem/path.h"

namespace LocalStress {
  template <typename T, class Enable = void>
  class LSCalculator;

  template <typename T>
  class LSCalculator<T, typename std::enable_if<std::is_floating_point<T>::value>::type> final {
    typedef Vec<T> Vec_t;
    typedef Tensor<T> Tensor_t;

    std::vector<Tensor_t> stress_dist_;
    std::unique_ptr<Boundary<T>> boundary_;
    int32_t num_frames_ = 0;

    void normalizeStress() {
      const auto cell_volume = boundary_->cell_volume();
      const T factor = -1.0 / (num_frames_ * cell_volume);
      std::for_each(stress_dist_.begin(), stress_dist_.end(),
                    [factor](Tensor_t& val) { val *= factor; });
    }

    void spreadLocalStress(const Vec_t& r1,
                           const Vec_t& dr01,
                           const Vec_t& dF01) {
      const auto div_ratios = boundary_->getDividedLineRatio(r1, dr01);
      const auto d_virial = tensor_dot(dr01, dF01);
      for (auto it = div_ratios.cbegin(); it != div_ratios.cend(); ++it) {
        stress_dist_[it->first] += d_virial * it->second;
      }
    }

  public:
    LSCalculator(const Vec_t& box_low,
                 const Vec_t& box_high,
                 const BoundaryType type,
                 const std::array<int32_t, D>& dim) {
      boundary_ = make_unique<Boundary<T>>(type, dim);
      boundary_->setBox(box_low, box_high);
      stress_dist_.resize(boundary_->number_of_cell());
    }
    ~LSCalculator() {
      saveLocalStressDist();
    }

    LSCalculator(const LSCalculator&) = delete;
    LSCalculator(LSCalculator&&) = delete;
    LSCalculator& operator = (const LSCalculator&) = delete;
    LSCalculator& operator = (LSCalculator&&) = delete;

    // TODO: support optimized version of this function
    void calcLocalStressPot2(const Vec_t& r0,
                             const Vec_t& r1,
                             const Vec_t& F01) {
      if (boundary_->isInBox(r0) && boundary_->isInBox(r1)) {
        auto dr01 = r0 - r1;
        boundary_->applyMinimumImage(dr01);
        spreadLocalStress(r1, dr01, F01);
      } else {
        ERR("r0 and r1 should be placed in box.");
      }
    }

    // TODO: support optimized version of this function
    void calcLocalStressPot3(const Vec_t& r0,
                             const Vec_t& r1,
                             const Vec_t& r2,
                             const Vec_t& F0,
                             const Vec_t& F1,
                             const Vec_t& F2) {
      if (boundary_->isInBox(r0) && boundary_->isInBox(r1) && boundary_->isInBox(r2)) {
        auto dr01 = r0 - r1; boundary_->applyMinimumImage(dr01);
        auto dr12 = r1 - r2; boundary_->applyMinimumImage(dr12);
        auto dr20 = r2 - r0; boundary_->applyMinimumImage(dr20);
        const auto dF = decomposeForce({F0, F1, F2}, {dr01, dr12, dr20});
        spreadLocalStress(r1, dr01, dF[0]);
        spreadLocalStress(r2, dr12, dF[1]);
        spreadLocalStress(r0, dr20, dF[2]);
      } else {
        ERR("r0, r1, and r2 should be placed in box.");
      }
    }

    void calcLocalStressKin(const Vec_t& r,
                            const Vec_t& v,
                            const T mass) {
      if (boundary_->isInBox(r)) {
        stress_dist_[boundary_->getCellPositionHash(r)] += tensor_dot(v, v) * mass;
      } else {
        ERR("r should be placed in box.");
      }
    }

    void nextStep() { num_frames_++; }
    void clear() {
      num_frames_ = 0;
      std::for_each(stress_dist_.begin(), stress_dist_.end(),
                    [](Tensor_t& t) { t.clear(); });
    }

    const Tensor_t pressure_tot() const {
      return std::accumulate(stress_dist_.cbegin(),
                             stress_dist_.cend(),
                             Tensor_t(0.0)) / (boundary_->box_volume() * num_frames_);
    }

    // TODO: support big endian
    void saveLocalStressDist(const std::string dir_name) {
      normalizeStress();
      const std::string fname = (filesystem::path(dir_name) / filesystem::path("local_stress.bin")).filename();
      std::ofstream fout(fname, std::ios::binary);
      const auto& mdim = boundary_->mesh_dim();
      if (is_little_endian()) {
        fout.write(reinterpret_cast<const char*>(mdim.data()),
                   sizeof(remove_const_t<remove_reference_t<decltype(mdim)>>));
        fout.write(reinterpret_cast<const char*>(&stress_dist_[0].xx),
                   stress_dist_.size() * sizeof(Tensor_t));
      } else {
        ERR("big endian is not currently supported.");
      }
    }

    void saveLocalStressDist() {
      saveLocalStressDist("./");
    }
  };
}
#endif
