#if !defined BOUNDARY_HPP
#define BOUNDARY_HPP

#include <cstdint>
#include <array>
#include <vector>
#include <utility>
#include <algorithm>

// TODO: add functions for checking particles in box

namespace LocalStress {
  enum class BoundaryType : int32_t {
    PERIODIC_X = 0,
    PERIODIC_Y,
    PERIODIC_Z,
    PERIODIC_XY,
    PERIODIC_YZ,
    PERIODIC_ZX,
    PERIODIC_XYZ,
    FIXED,
  };

  template <typename T, class Enable = void>
  class Boundary;

  template <typename T>
  class Boundary<T, typename std::enable_if<std::is_floating_point<T>::value>::type> final {
    typedef Vec<T> Vec_t;
    typedef std::pair<std::int32_t, T> lineratio_t;

    BoundaryType btype_;
    std::array<bool, D> is_periodic_axis_;
    Vec_t low_, high_, box_length_, box_hlength_;
    T box_volume_;
    Vec_t mesh_length_, imesh_length_;
    std::array<int32_t, D> mesh_dim_;
    int32_t number_of_cell_ = -1;

    void calcMeshLengthFromMeshDim(void) {
      for (int32_t i = 0; i < D; i++) {
        mesh_length_[i]  = box_length_[i] / T(mesh_dim_[i]);
        imesh_length_[i] = 1.0 / mesh_length_[i];
      }
    }

    void calcNumberOfCell(void) {
      number_of_cell_ = std::accumulate(mesh_dim_.cbegin(), mesh_dim_.cend(), 1,
                                        [](const int32_t acc, const int32_t rhs) {
                                          return acc * rhs;
                                        });
    }

    void calcBoxLength(void) {
      box_length_  = high_ - low_;
      box_hlength_ = box_length_ * 0.5;
      box_volume_ = 1.0;
      for (int32_t i = 0; i < D; i++) box_volume_ *= box_length_[i];
    }

    void setPeriodicAxis(void) {
      is_periodic_axis_.fill(false);
      switch (btype_) {
      case BoundaryType::PERIODIC_X:
        is_periodic_axis_[X] = true;
        break;
      case BoundaryType::PERIODIC_Y:
        is_periodic_axis_[Y] = true;
        break;
      case BoundaryType::PERIODIC_Z:
        if (D != 3) { ERR("This periodic boundary type is only valid for 3D simulation."); }
        is_periodic_axis_[Z] = true;
        break;
      case BoundaryType::PERIODIC_XY:
        is_periodic_axis_[X] = true;
        is_periodic_axis_[Y] = true;
        break;
      case BoundaryType::PERIODIC_YZ:
        if (D != 3) { ERR("This periodic boundary type is only valid for 3D simulation."); }
        is_periodic_axis_[Y] = true;
        is_periodic_axis_[Z] = true;
        break;
      case BoundaryType::PERIODIC_ZX:
        if (D != 3) { ERR("This periodic boundary type is only valid for 3D simulation."); }
        is_periodic_axis_[Z] = true;
        is_periodic_axis_[X] = true;
        break;
      case BoundaryType::PERIODIC_XYZ:
        if (D != 3) { ERR("This periodic boundary type is only valid for 3D simulation."); }
        is_periodic_axis_[X] = true;
        is_periodic_axis_[Y] = true;
        is_periodic_axis_[Z] = true;
        break;
      case BoundaryType::FIXED:
        break;
      default:
        ERR("Unknown boundary type.");
        break;
      }
    }

    void adjustBoundary(std::array<int32_t, D>& idx) const {
      for (int32_t i = 0; i < D; i++) {
        if (is_periodic_axis_[i]) {
          if (idx[i] >= mesh_dim_[i]) idx[i] -= mesh_dim_[i];
          if (idx[i] <  0) idx[i] += mesh_dim_[i];
        }
      }
    }

    int32_t getCellPositionHash(std::array<int32_t, D>& idx) const {
      adjustBoundary(idx);
      int32_t hash = idx[D - 1];
      for (int32_t i = D - 2; i >= 0; i--) {
        hash = hash * mesh_dim_[i] + idx[i];
      }
      return hash;
    }

    std::array<int32_t, D> getCellPosition(const Vec_t& pos) const {
      std::array<int32_t, D> idx;
      for (int32_t i = 0; i < D; i++) {
        idx[i] = int32_t(std::floor((pos[i] - low_[i]) * imesh_length_[i]));
      }
      return idx;
    }

    void calcDividedLineRatioOneAxis(std::vector<T>& ratios,
                                     const T dx,
                                     const T x_org,
                                     const int32_t axis,
                                     const int32_t cell_org,
                                     const int32_t cell_dif) const {
      if (cell_dif > 0) {
        for (int32_t i = 0; i < cell_dif; i++) {
          const auto wall_pos = (cell_org + 1 + i) * mesh_length_[axis] + low_[axis];
          const auto ratio = (wall_pos - x_org) / dx;
          ratios.push_back(ratio);
        }
      } else if (cell_dif < 0) {
        for (int32_t i = 0; i < (-cell_dif); i++) {
          const auto wall_pos = (cell_org - i) * mesh_length_[axis] + low_[axis];
          const auto ratio = (wall_pos - x_org) / dx;
          ratios.push_back(ratio);
        }
      }
    }

  public:
    Boundary(BoundaryType type,
             const std::array<int32_t, D> mdim) : btype_(type), mesh_dim_(mdim) {
      setPeriodicAxis();
      calcNumberOfCell();
    }

    void setBox(const Vec_t& v0, const Vec_t& v1) {
      for (int32_t i = 0; i < D; i++) assert(v0[i] < v1[i]);
      low_  = v0;
      high_ = v1;
      calcBoxLength();
      calcMeshLengthFromMeshDim();
    }

    const Vec_t& low(void) const { return low_; }
    const Vec_t& high(void) const { return high_; }
    const Vec_t& mesh_length(void) const { return mesh_length_; }
    T box_volume(void) const { return box_volume_; }

    const Vec_t& box_length(void) const { return box_length_; }
    const std::array<int32_t, D>& mesh_dim(void) const { return mesh_dim_; }
    int32_t number_of_cell(void) const { return number_of_cell_; }

    void applyMinimumImage(Vec_t& dr01) const {
      for (int32_t i = 0; i < D; i++) {
        if (is_periodic_axis_[i]) {
          if (dr01[i] < -box_hlength_[i]) dr01[i] += box_length_[i];
          if (dr01[i] >  box_hlength_[i]) dr01[i] -= box_length_[i];
        }
      }
    }

    void adjustBoundary(Vec_t& pos) const {
      for (int32_t i = 0; i < D; i++) {
        if (is_periodic_axis_[i]) {
          if (pos[i] < low_[i] ) pos[i] += box_length_[i];
          if (pos[i] > high_[i]) pos[i] -= box_length_[i];
        }
      }
    }

    bool isInBox(const Vec_t& pos) const {
      bool in_range = true;
      for (int32_t i = 0; i < D; i++) {
        in_range &= (pos[i] >= low_[i]) && (pos[i] < high_[i]);
      }
      return in_range;
    }

    int32_t getCellPositionHash(const Vec_t& pos) const {
      auto idx = getCellPosition(pos);
      return getCellPositionHash(idx);
    }

    const std::vector<lineratio_t> getDividedLineRatio(const Vec_t& r1,
                                                       const Vec_t& dr01) const {
      const auto cell_pos0 = getCellPosition(r1 + dr01);
      const auto cell_pos1 = getCellPosition(r1);

      std::vector<T> ratios;
      ratios.push_back(0.0);
      for (int32_t axis = 0; axis < D; axis++) {
        calcDividedLineRatioOneAxis(ratios, dr01[axis], r1[axis], axis,
                                    cell_pos1[axis], cell_pos0[axis] - cell_pos1[axis]);
      }
      ratios.push_back(1.0);
      std::sort(ratios.begin(), ratios.end());

      const int32_t num_divided_line = ratios.size() - 1;
      std::vector<lineratio_t> div_ratios(num_divided_line);
      for (int32_t i = 0; i < num_divided_line; i++) {
        const auto base_pos  = r1 + dr01 * (ratios[i + 1] + ratios[i]) * 0.5;
        div_ratios[i].first  = getCellPositionHash(base_pos);
        div_ratios[i].second = ratios[i + 1] - ratios[i];
      }
      return div_ratios;
    }
  };
}
#endif
