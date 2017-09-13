#if !defined LS_HELPERS_HPP
#define LS_HELPERS_HPP

namespace LocalStress {
  template <typename T>
  void saveLocalStressDistOMP(std::vector<std::unique_ptr<LSCalculator<T>>>& calculators) {
    const int num_calculators = calculators.size();
    for (int i = 1; i < num_calculators; i++) {
      accumulateResult(calculators[0], calculators[i]);
    }
    calculators[0].saveLocalStressDist();
  }

  /*
    void saveLocalStressDistMPI() {}
   */
}
#endif
