#if !defined LS_FACTORY_HPP
#define LS_FACTORY_HPP

namespace LocalStress {
  template <typename T, class Enable = void>
  class CalculatorFactory;

  template <typename T>
  class CalculatorFactory<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
  public:
    static std::unique_ptr<LSCalculator<T>> create(Vec<T>&& box_low,
                                                   Vec<T>&& box_high,
                                                   const BoundaryType btype,
                                                   std::array<int32_t, D>&& dim,
                                                   std::vector<std::string>&& itype) {
      return make_unique<LSCalculator<T>>(std::move(box_low), std::move(box_high), btype,
                                          std::move(dim), std::move(itype));
    }

    static std::vector<std::unique_ptr<LSCalculator<T>>> createOMP(const int num_threads,
                                                                   Vec<T>&& box_low,
                                                                   Vec<T>&& box_high,
                                                                   const BoundaryType btype,
                                                                   std::array<int32_t, D>&& dim,
                                                                   std::vector<std::string>&& itype) {
      std::vector<std::unique_ptr<LSCalculator<T>>> calculators(num_threads);
      for (int i = 0; i < num_threads; i++) {
        calculators[i] = make_unique<LSCalculator<T>>(std::move(box_low), std::move(box_high), btype,
                                                      std::move(dim), std::move(itype));
      }
      return calculators;
    }

    // TODO: support MPI version based on LAMMPS impl
    // static std::unique_ptr<LSCalculator<T>> createMPI(const int num_procs,
    //                                                             ) {
    // }
  };
}

#endif
