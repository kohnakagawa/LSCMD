#if !defined LS_CALCULATOR_IMPL_HPP
#define LS_CALCULATOR_IMPL_HPP

#include <string>

#include "filesystem/path.h"

namespace LocalStress {
  template <typename T, class Enable = void>
  class LSCalculator;

  template <typename T>
  class LSCalculator<T, typename std::enable_if<std::is_floating_point<T>::value>::type> final {
    typedef Vec<T> Vec_t;
    typedef Tensor<T> Tensor_t;

    std::vector<std::unique_ptr<Tensor_t[]>> stress_dist_;
    std::unique_ptr<Boundary<T>> boundary_;
    int32_t num_frames_ = 0;
    std::vector<std::string> interaction_types_;
    std::string save_dir_ = "./";
    bool auto_save_ = true;

    void normalizeStress() {
      const T factor = -1.0 / num_frames_;
      const auto num_of_cell = boundary_->number_of_cell();
      for (auto& sdist : stress_dist_) {
        std::for_each(sdist.get(), sdist.get() + num_of_cell,
                      [factor](Tensor_t& val) { val *= factor; });
      }
    }

    void spreadLocalStress(const Vec_t& r1,
                           const Vec_t& dr01,
                           const Vec_t& dF01,
                           const int32_t type) {
      const auto div_ratios = boundary_->getDividedLineRatio(r1, dr01);
      const auto d_virial   = tensor_dot(dr01, dF01);
      for (auto it = div_ratios.cbegin(); it != div_ratios.cend(); ++it) {
        stress_dist_[type][it->first] += d_virial * it->second;
      }
    }

    void writeStressDistAsBinary(std::ofstream& fout) const {
      write_as_lsbfirst(fout, uint32_t(D));

      const auto box_low = boundary_->low();
      for (int32_t i = 0; i < D; i++) {
        write_as_lsbfirst(fout, box_low[i]);
      }
      const auto box_len = boundary_->box_length();
      for (int32_t i = 0; i < D; i++) {
        write_as_lsbfirst(fout, box_len[i]);
      }
      const auto mdim    = boundary_->mesh_dim();
      for (int32_t i = 0; i < D; i++) {
        write_as_lsbfirst(fout, mdim[i]);
      }

      const auto num_of_cell = boundary_->number_of_cell();
      const auto num_itypes  = interaction_types_.size();
      write_as_lsbfirst(fout, uint32_t(num_itypes));
      for (std::size_t i = 0; i < num_itypes; i++) {
        const auto itype_name_len = uint32_t(interaction_types_[i].length());
        write_as_lsbfirst(fout, itype_name_len);
        write_as_lsbfirst(fout, interaction_types_[i]);
        for (int32_t j = 0; j < num_of_cell; j++) {
          for (int axis = 0; axis < D*D; axis++) {
            write_as_lsbfirst(fout, stress_dist_[i][j][axis]);
          }
        }
      }
    }

  public:
    LSCalculator(Vec_t&& box_low,
                 Vec_t&& box_high,
                 const BoundaryType btype,
                 std::array<int32_t, D>&& dim,
                 std::vector<std::string>&& itype) {
      boundary_ = make_unique<Boundary<T>>(btype, dim);
      boundary_->setBox(box_low, box_high);
      interaction_types_ = itype;
      stress_dist_.resize(interaction_types_.size());
      for (auto& sdist : stress_dist_) {
        sdist = make_unique<Tensor_t[]>(boundary_->number_of_cell());
      }
    }

    ~LSCalculator(void) {
      if (auto_save_) { saveLocalStressDist(); }
    }

    void setSaveDir(const std::string dir_name) {
      save_dir_ = dir_name;
    }

    void disableAutoSave(void) {
      auto_save_ = false;
    }

    LSCalculator(const LSCalculator&) = delete;
    LSCalculator(LSCalculator&&) = delete;
    LSCalculator& operator = (const LSCalculator&) = delete;
    LSCalculator& operator = (LSCalculator&&) = delete;

    void calcLocalStressPot2(Vec_t&& r0,
                             Vec_t&& r1,
                             Vec_t&& F0,
                             Vec_t&& F1,
                             const int32_t type) {
      LOCAL_STRESS_UNUSED_VAR(F1);
      if (boundary_->isInBox(r0) &&
          boundary_->isInBox(r1)) {
        calcLocalStressPot2NoCheck(std::move(r0), std::move(r1),
                                   std::move(F0), std::move(F1),
                                   type);
      } else {
        LOCAL_STRESS_ERR("r0 and r1 should be in simulation box.");
      }
    }

    void calcLocalStressPot2NoCheck(Vec_t&& r0, Vec_t&& r1,
                                    Vec_t&& F0, Vec_t&& F1,
                                    const int32_t type) {
      LOCAL_STRESS_UNUSED_VAR(F1);
      auto dr01 = r0 - r1;
      boundary_->applyMinimumImage(dr01);
      spreadLocalStress(r1, dr01, F0, type);
    }

    void calcLocalStressPot3(Vec_t&& r0, Vec_t&& r1, Vec_t&& r2,
                             Vec_t&& F0, Vec_t&& F1, Vec_t&& F2,
                             const int32_t type) {
      if (boundary_->isInBox(r0) &&
          boundary_->isInBox(r1) &&
          boundary_->isInBox(r2)) {
        calcLocalStressPot3NoCheck(std::move(r0), std::move(r1), std::move(r2),
                                   std::move(F0), std::move(F1), std::move(F2),
                                   type);
      } else {
        LOCAL_STRESS_ERR("r0, r1, and r2 should be in simulation box.");
      }
    }

    void calcLocalStressPot3NoCheck(Vec_t&& r0, Vec_t&& r1, Vec_t&& r2,
                                    Vec_t&& F0, Vec_t&& F1, Vec_t&& F2,
                                    const int32_t type) {
      auto dr01 = r0 - r1; boundary_->applyMinimumImage(dr01);
      auto dr12 = r1 - r2; boundary_->applyMinimumImage(dr12);
      auto dr20 = r2 - r0; boundary_->applyMinimumImage(dr20);
      const auto dF = decomposeForce({F0, F1, F2}, {dr01, dr12, dr20});
      spreadLocalStress(r1, dr01, dF[0], type);
      spreadLocalStress(r2, dr12, dF[1], type);
      spreadLocalStress(r0, dr20, dF[2], type);
    }

    void calcLocalStressPot4(Vec_t&& r0, Vec_t&& r1, Vec_t&& r2, Vec_t&& r3,
                             Vec_t&& F0, Vec_t&& F1, Vec_t&& F2, Vec_t&& F3,
                             const int32_t type) {
      if (boundary_->isInBox(r0) &&
          boundary_->isInBox(r1) &&
          boundary_->isInBox(r2) &&
          boundary_->isInBox(r3)) {
        calcLocalStressPot4NoCheck(std::move(r0), std::move(r1), std::move(r2), std::move(r3),
                                   std::move(F0), std::move(F1), std::move(F2), std::move(F3),
                                   type);
      } else {
        LOCAL_STRESS_ERR("r0, r1, and r2 should be in simulation box.");
      }
    }

    void calcLocalStressPot4NoCheck(Vec_t&& r0, Vec_t&& r1, Vec_t&& r2, Vec_t&& r3,
                                    Vec_t&& F0, Vec_t&& F1, Vec_t&& F2, Vec_t&& F3,
                                    const int32_t type) {
      auto dr01 = r0 - r1; boundary_->applyMinimumImage(dr01);
      auto dr02 = r0 - r2; boundary_->applyMinimumImage(dr02);
      auto dr03 = r0 - r3; boundary_->applyMinimumImage(dr03);
      auto dr12 = r1 - r2; boundary_->applyMinimumImage(dr12);
      auto dr13 = r1 - r3; boundary_->applyMinimumImage(dr13);
      auto dr23 = r2 - r3; boundary_->applyMinimumImage(dr23);
      const auto dF = decomposeForce({F0, F1, F2, F3}, {dr01, dr02, dr03, dr12, dr13, dr23});
      spreadLocalStress(r1, dr01, dF[0], type);
      spreadLocalStress(r2, dr02, dF[1], type);
      spreadLocalStress(r3, dr03, dF[2], type);
      spreadLocalStress(r2, dr12, dF[3], type);
      spreadLocalStress(r3, dr13, dF[4], type);
      spreadLocalStress(r3, dr23, dF[5], type);
    }

    void calcLocalStressKin(Vec_t&& r,
                            Vec_t&& v,
                            const T mass,
                            const int32_t type) {
      if (boundary_->isInBox(r)) {
        stress_dist_[type][boundary_->getCellPositionHash(r)] += tensor_dot(v, v) * mass;
      } else {
        LOCAL_STRESS_ERR("r should be in simulation box.");
      }
    }

    void nextStep(void) { num_frames_++; }
    void clear(void) {
      num_frames_ = 0;
      const auto num_of_cell = boundary_->number_of_cell();
      for (auto& sdist : stress_dist_) {
        std::for_each(sdist.get(), sdist.get() + num_of_cell,
                      [](Tensor_t& val) { val.clear(); });
      }
    }

    const Tensor_t pressure_tot(const int i) const {
      const auto num_of_cell = boundary_->number_of_cell();
      return std::accumulate(stress_dist_[i].get(),
                             stress_dist_[i].get() + num_of_cell,
                             Tensor_t(0.0)) / (boundary_->box_volume() * num_frames_);
    }

    const Tensor_t pressure_tot(void) const {
      Tensor_t psum(0.0);
      const auto num_itypes = interaction_types_.size();
      for (std::size_t i = 0; i < num_itypes; i++) {
        psum += pressure_tot(i);
      }
      return psum;
    }

    void saveLocalStressDist(void) {
      using filesystem::path;
      normalizeStress();
      const std::string fname = (path(save_dir_) / path("local_stress.bin")).filename();
      std::ofstream fout(fname, std::ios::binary);
      writeStressDistAsBinary(fout);
    }

    friend void accumulateResult(LSCalculator& lsc0,
                                 const LSCalculator& lsc1) {
      const int num_itypes = lsc0.interaction_types_.size();
      const int num_of_cell = lsc0.boundary_->number_of_cell();
      for (int type = 0; type < num_itypes; type++) {
        for (int at = 0; at < num_of_cell; at++) {
          lsc0.stress_dist_[type][at] += lsc1.stress_dist_[type][at];
        }
      }
    }
  };
}
#endif
