#ifndef absolute_discounting_h
#define absolute_discounting_h

#include "tensorflow/Source/lm/ngram/ngram.h"
#include <algorithm>

class AbsoluteDiscounting : public NGram {
protected:
    double discount;
    void PopulateProbTrie(CountTrie *, CountTrie::Node *, int, std::list<size_t>);
public:
    AbsoluteDiscounting(std::string file_name, int n, double discount, int min_frequency);
    AbsoluteDiscounting(int n, double discount, ProbTrie *prob_trie, Vocab *vocab) : NGram(n, prob_trie, vocab), discount(discount) {}
    virtual std::pair<int, int> ContextSize();
    virtual bool operator==(const NGram &);
    tensorflow::Source::lm::ngram::NGramProto *ToProto();
};

#endif // absolute_discounting.h
