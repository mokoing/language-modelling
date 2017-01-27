#ifndef rnn_h
#define rnn_h

#include <algorithm>
#include <assert.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <list>
#include <map>
#include <stdexcept>
#include <string>
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/Source/lm/lm.h"
#include "tensorflow/Source/lm/vocab.h"
#include "tensorflow/Source/lm/rnn/rnn.pb.h"


class RNN : public LM {
protected:
    tensorflow::Session *session;
    tensorflow::Status status;
    std::vector<tensorflow::Tensor> state;
    enum Type { VANILLA, GRU, LSTM };
    Type type;
    std::string input_tensor_name;
    std::string predictions_tensor_name;
    std::list<std::pair<std::string, std::string>> state_tensor_names;
    virtual void ResetState();
    virtual void RunInference(size_t, std::vector<tensorflow::Tensor> &, bool);
    virtual void RunInference(std::list<size_t>, std::vector<tensorflow::Tensor> &, bool);
public:
    RNN(std::string);
    virtual std::pair<int, int> ContextSize();
    virtual double Prob(std::list<std::string>);
    virtual double Prob(std::list<std::string>, bool);
    virtual void ProbAllFollowing (std::list<std::string>, std::list<std::pair<std::string, double>> &);
    virtual void ProbAllFollowing (std::list<std::string>, std::list<std::pair<std::string, double>> &, bool);
};

#endif // rnn.h