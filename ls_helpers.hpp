#if !defined LS_HELPERS_HPP
#define LS_HELPERS_HPP

namespace LocalStress {
  template <typename T>
  void accumulateStressOMP(std::vector<std::unique_ptr<LSCalculator<T>>>& calculators) {
    const int num_calculators = calculators.size();
    for (int i = 1; i < num_calculators; i++) {
      accumulateResult(*calculators[0], *calculators[i]);
    }
  }

  template <typename T>
  void showPressureTotalOMP(std::vector<std::unique_ptr<LSCalculator<T>>>& calculators) {
    accumulateStressOMP(calculators);
    std::cout << "pressure total = " << calculators[0]->pressure_tot().trace() / 3.0 << std::endl;
  }

  template <typename T>
  void saveLocalStressDistOMP(std::vector<std::unique_ptr<LSCalculator<T>>>& calculators) {
    accumulateStressOMP(calculators);
    calculators[0]->saveLocalStressDist();
    for (auto& calc : calculators) {
      calc->disableAutoSave();
    }
  }

  template <typename T>
  void clearLSCalculatorsOMP(std::vector<std::unique_ptr<LSCalculator<T>>>& calculators) {
    for (auto& calc : calculators) {
      calc->clear();
    }
  }

  /*
    void saveLocalStressDistMPI() {}
   */
}
#endif
