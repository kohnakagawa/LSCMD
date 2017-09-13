#if !defined LS_CALCULATOR_IMPL_HPP
#define LS_CALCULATOR_IMPL_HPP

#include <fstream>
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
    bool is_saved_ = false;

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
      const auto num_of_cell = boundary_->number_of_cell();

      const auto sim_dim = conv2_lsb_first_if_needed(uint32_t(D));
      const auto box_low = boundary_->low();
      const auto box_len = boundary_->box_length();
      const auto mdim    = boundary_->mesh_dim();
      fout.write(reinterpret_cast<const char*>(&sim_dim),
                 sizeof(uint32_t));
      for (int32_t i = 0; i < D; i++) {
        const auto dat = conv2_lsb_first_if_needed(box_low[i]);
        fout.write(reinterpret_cast<const char*>(&dat), sizeof(T));
      }
      for (int32_t i = 0; i < D; i++) {
        const auto dat = conv2_lsb_first_if_needed(box_len[i]);
        fout.write(reinterpret_cast<const char*>(&dat), sizeof(T));
      }
      for (int32_t i = 0; i < D; i++) {
        const auto dat = conv2_lsb_first_if_needed(mdim[i]);
        fout.write(reinterpret_cast<const char*>(&dat), sizeof(T));
      }
      for (std::size_t i = 0; i < interaction_types_.size(); i++) {
        fout.write(interaction_types_[i].c_str(),
                   interaction_types_[i].length() * sizeof(char));
        fout.write("\0", sizeof(char));
        for (int32_t j = 0; j < num_of_cell; j++) {
          for (int axis = 0; axis < D*D; axis++) {
            const auto dat = conv2_lsb_first_if_needed(stress_dist_[i][j][axis]);
            fout.write(reinterpret_cast<const char*>(&dat), sizeof(T));
          }
        }
      }
    }

  public:
    LSCalculator(const Vec_t& box_low,
                 const Vec_t& box_high,
                 const BoundaryType btype,
                 const std::array<int32_t, D>& dim,
                 const std::vector<std::string>& itype) {
      boundary_ = make_unique<Boundary<T>>(btype, dim);
      boundary_->setBox(box_low, box_high);
      interaction_types_ = itype;
      stress_dist_.resize(interaction_types_.size());
      for (auto& sdist : stress_dist_) {
        sdist = make_unique<Tensor_t[]>(boundary_->number_of_cell());
      }
    }

    ~LSCalculator(void) {
      if (!is_saved_) { saveLocalStressDist(); }
    }

    void setSaveDir(const std::string dir_name) {
      save_dir_ = dir_name;
    }

    LSCalculator(const LSCalculator&) = delete;
    LSCalculator(LSCalculator&&) = delete;
    LSCalculator& operator = (const LSCalculator&) = delete;
    LSCalculator& operator = (LSCalculator&&) = delete;

    void calcLocalStressPot2(const Vec_t& r0,
                             const Vec_t& r1,
                             const Vec_t& F0,
                             const Vec_t& F1,
                             const int32_t type) {
      LOCAL_STRESS_UNUSED_VAR(F1);
      if (boundary_->isInBox(r0) &&
          boundary_->isInBox(r1)) {
        calcLocalStressPot2NoCheck(r0, r1, F0, F1, type);
      } else {
        LOCAL_STRESS_ERR("r0 and r1 should be in simulation box.");
      }
    }

    void calcLocalStressPot2NoCheck(const Vec_t& r0,
                                    const Vec_t& r1,
                                    const Vec_t& F0,
                                    const Vec_t& F1,
                                    const int32_t type) {
      LOCAL_STRESS_UNUSED_VAR(F1);
      auto dr01 = r0 - r1;
      boundary_->applyMinimumImage(dr01);
      spreadLocalStress(r1, dr01, F0, type);
    }

    void calcLocalStressPot3(const Vec_t& r0,
                             const Vec_t& r1,
                             const Vec_t& r2,
                             const Vec_t& F0,
                             const Vec_t& F1,
                             const Vec_t& F2,
                             const int32_t type) {
      if (boundary_->isInBox(r0) &&
          boundary_->isInBox(r1) &&
          boundary_->isInBox(r2)) {
        calcLocalStressPot3NoCheck(r0, r1, r2, F0, F1, F2, type);
      } else {
        LOCAL_STRESS_ERR("r0, r1, and r2 should be in simulation box.");
      }
    }

    void calcLocalStressPot3NoCheck(const Vec_t& r0,
                                    const Vec_t& r1,
                                    const Vec_t& r2,
                                    const Vec_t& F0,
                                    const Vec_t& F1,
                                    const Vec_t& F2,
                                    const int32_t type) {
      auto dr01 = r0 - r1; boundary_->applyMinimumImage(dr01);
      auto dr12 = r1 - r2; boundary_->applyMinimumImage(dr12);
      auto dr20 = r2 - r0; boundary_->applyMinimumImage(dr20);
      const auto dF = decomposeForce({F0, F1, F2}, {dr01, dr12, dr20});
      spreadLocalStress(r1, dr01, dF[0], type);
      spreadLocalStress(r2, dr12, dF[1], type);
      spreadLocalStress(r0, dr20, dF[2], type);
    }

    void calcLocalStressPot4(const Vec_t& r0,
                             const Vec_t& r1,
                             const Vec_t& r2,
                             const Vec_t& r3,
                             const Vec_t& F0,
                             const Vec_t& F1,
                             const Vec_t& F2,
                             const Vec_t& F3,
                             const int32_t type) {
      if (boundary_->isInBox(r0) &&
          boundary_->isInBox(r1) &&
          boundary_->isInBox(r2) &&
          boundary_->isInBox(r3)) {
        calcLocalStressPot4NoCheck(r0, r1, r2, r3, F0, F1, F2, F3, type);
      } else {
        LOCAL_STRESS_ERR("r0, r1, and r2 should be in simulation box.");
      }
    }

    void calcLocalStressPot4NoCheck(const Vec_t& r0,
                                    const Vec_t& r1,
                                    const Vec_t& r2,
                                    const Vec_t& r3,
                                    const Vec_t& F0,
                                    const Vec_t& F1,
                                    const Vec_t& F2,
                                    const Vec_t& F3,
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

    void calcLocalStressKin(const Vec_t& r,
                            const Vec_t& v,
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
      for (std::size_t i = 0; i < stress_dist_.size(); i++) {
        psum += pressure_tot(i);
      }
      return psum;
    }

    void saveLocalStressDist() {
      is_saved_ = true;
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
