#if !defined LS_HELPERS_HPP
#define LS_HELPERS_HPP

namespace LocalStress {
  enum {
    ROOT_CALCULATOR = 0,
  };

  template <typename T>
  class LSHelpers final {
    static void accumulateRootCalculator(std::vector<std::unique_ptr<LSCalculator<T>>>& calculators) {
      const int num_calculators = calculators.size();
      for (int i = 0; i < num_calculators; i++) {
        if (i != ROOT_CALCULATOR) {
          accumulateResult(*calculators[ROOT_CALCULATOR], *calculators[i]);
        }
      }
    }

  public:
    static void showPressureTotalOMP(std::vector<std::unique_ptr<LSCalculator<T>>>& calculators) {
      Tensor<T> p_tot(0.0);
      for (const auto& calc : calculators) {
        p_tot += calc->pressure_tot();
      }
      std::cout << "pressure total = " << p_tot.trace() / 3.0 << std::endl;
    }

    static void saveLocalStressDistOMP(std::vector<std::unique_ptr<LSCalculator<T>>>& calculators) {
      accumulateRootCalculator(calculators);
      calculators[ROOT_CALCULATOR]->saveLocalStressDist();
      for (auto& calc : calculators) {
        calc->disableAutoSave();
      }
    }

    static void clearLSCalculatorsOMP(std::vector<std::unique_ptr<LSCalculator<T>>>& calculators) {
      for (auto& calc : calculators) {
        calc->clear();
      }
    }
  };
}
#endif
