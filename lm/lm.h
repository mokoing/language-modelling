#ifndef lm_h
#define lm_h

#include <exception>
#include <list>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include "vocab.h"

class LM {
protected:
    Vocab *vocab;
    virtual std::list<size_t> WordsToIds(std::list<std::string> seq);
    virtual std::list<size_t> Trim(std::list<size_t>, int);
public:
    LM() : vocab(NULL) {}
    LM(int min_frequency) : vocab(new Vocab(min_frequency)) {}
    LM(Vocab *vocab) : vocab(vocab) {}
    virtual bool ContainsWord(std::string);
    virtual std::pair<int, int> ContextSize() = 0;
    virtual void Predict(std::list<std::string>, std::pair<std::string, double> &);
    virtual void PredictTopK(std::list<std::string>, std::list<std::pair<std::string, double>> &, int);
    virtual double Prob (std::list<std::string>) = 0;
    virtual void ProbAllFollowing (std::list<std::string>, std::list<std::pair<std::string, double>> &) = 0;
};

class PredictionCompare {
public:
    bool operator() (std::pair<std::string, double> const &a, std::pair<std::string, double> const &b) const {
        if (a.second == b.second) {
            return a.first > b.first;
        }
        return a.second > b.second;
    }
};

#endif // lm.h
