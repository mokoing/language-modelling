#include "benchmark.h"
#include "tensorflow/core/platform/test.h"


class LanguageModelMock : public LM {
public:
    bool ContainsWord(std::string) {
        return true;
    }
    int ContextSize() {
        return 3;
    }
    void Predict(std::list<std::string>, std::pair<std::string, double> &) {}
    void PredictTopK(std::list<std::string>, std::list<std::pair<std::string, double>> &, int) {}
    double Prob (std::list<std::string>) {
        return 0.25;
    }
};

TEST(BenchmarkTest, Perplexity) {
    std::ofstream test_file;
    std::string test_file_name = "/tmp/benchmark_test_file";
    test_file.open (test_file_name);
    test_file << "the cat sat on the mat .\n";
    test_file << "the cat ate the mouse .\n";
    test_file << "the dog sat on the cat .\n";
    test_file.close();

    LanguageModelMock *lm = new LanguageModelMock();
    Benchmark *under_test = new Benchmark(lm);

    ASSERT_DOUBLE_EQ(under_test->Perplexity(test_file_name), 4.0);
}

TEST(BenchmarkTest, PerplexityExp) {
    std::ofstream test_file;
    std::string test_file_name = "/tmp/benchmark_test_file";
    test_file.open (test_file_name);
    test_file << "the cat sat on the mat .\n";
    test_file << "the cat ate the mouse .\n";
    test_file << "the dog sat on the cat .\n";
    test_file.close();

    LanguageModelMock *lm = new LanguageModelMock();
    Benchmark *under_test = new Benchmark(lm);

    ASSERT_DOUBLE_EQ(under_test->PerplexityExp(test_file_name), 4.0);
}