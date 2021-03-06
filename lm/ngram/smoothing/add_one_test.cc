#include "add_one.h"
#include "tensorflow/core/platform/test.h"
#include <fstream>
#include <google/protobuf/util/message_differencer.h>


class AddOneTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        std::ofstream test_file;
        std::string test_file_name = "/tmp/add_one_test_file";
        test_file.open (test_file_name, std::ofstream::out | std::ofstream::trunc);
        test_file << "the cat sat on the mat .\n";
        test_file << "the cat ate the mouse .\n";
        test_file << "the dog sat on the cat .\n";
        test_file.close();

        under_test = new AddOne(test_file_name, 3, 1);
    }
    AddOne *under_test;
};

void SetChildProperties(tensorflow::Source::lm::ngram::Node::Child *child, int id, tensorflow::Source::lm::ngram::Node *node) {
    child->set_id(id);
    child->set_allocated_node(node);
}

tensorflow::Source::lm::ngram::Node *BuildNode(double pseudo_prob, double backoff) {
    tensorflow::Source::lm::ngram::Node *node = new tensorflow::Source::lm::ngram::Node();
    node->set_pseudo_prob(pseudo_prob);
    node->set_backoff(backoff);
    return node;
}

TEST_F(AddOneTest, Prob) {
    ASSERT_DOUBLE_EQ(under_test->Prob(std::list<std::string>({"<s>", "the", "cat"})), 3/13.0);
    ASSERT_DOUBLE_EQ(under_test->Prob(std::list<std::string>({"the", "cat", "sat"})), 2/13.0);
    ASSERT_DOUBLE_EQ(under_test->Prob(std::list<std::string>({"on", "the", "mat"})), 1/6.0);
    ASSERT_DOUBLE_EQ(under_test->Prob(std::list<std::string>({"the", "mouse", "sat"})), 1/11.0);
    ASSERT_DOUBLE_EQ(under_test->Prob(std::list<std::string>({"the", "mouse", "."})), 2/11.0);
    ASSERT_DOUBLE_EQ(under_test->Prob(std::list<std::string>({"blah", "blah", "blah"})), 0.1);
}

TEST(AddOneTestToProto, ToProto) {
    std::ofstream test_file;
    std::string test_file_name = "/tmp/add_one_test_file";
    test_file.open (test_file_name, std::ofstream::out | std::ofstream::trunc);
    test_file << "the the cat\n";
    test_file.close();

    AddOne *under_test = new AddOne(test_file_name, 2, 1);

    tensorflow::Source::lm::ngram::NGramProto *expected_ngram_proto = new tensorflow::Source::lm::ngram::NGramProto();
    tensorflow::Source::lm::ngram::NGramProto *actual_ngram_proto = under_test->ToProto();

    /* The expected proto structure:

    a (0, 0)
    --<s>-- b (0, 1)
            --the-- e (1, 0)
    --the-- c (0, 2)
            --the-- f (1, 0)
            --cat-- g (1, 0)
    --cat-- d (0, 0)
    */

    expected_ngram_proto->set_n(2);
    expected_ngram_proto->set_smoothing(tensorflow::Source::lm::ngram::Smoothing::ADD_ONE);

    tensorflow::Source::lm::ngram::ProbTrieProto *prob_trie_proto = new tensorflow::Source::lm::ngram::ProbTrieProto();

    tensorflow::Source::lm::ngram::Node *node_e = ::BuildNode(1, 0);
    tensorflow::Source::lm::ngram::Node *node_f = ::BuildNode(1, 0);
    tensorflow::Source::lm::ngram::Node *node_g = ::BuildNode(1, 0);

    tensorflow::Source::lm::ngram::Node *node_b = ::BuildNode(0, 1);
    SetChildProperties(node_b->add_child(), 2, node_e);

    tensorflow::Source::lm::ngram::Node *node_c = ::BuildNode(0, 2);
    SetChildProperties(node_c->add_child(), 2, node_f);
    SetChildProperties(node_c->add_child(), 3, node_g);

    tensorflow::Source::lm::ngram::Node *node_d = ::BuildNode(0, 0);

    tensorflow::Source::lm::ngram::Node *node_a = ::BuildNode(0, 0);
    SetChildProperties(node_a->add_child(), 1, node_b);
    SetChildProperties(node_a->add_child(), 2, node_c);
    SetChildProperties(node_a->add_child(), 3, node_d);

    prob_trie_proto->set_allocated_root(node_a);
    expected_ngram_proto->set_allocated_prob_trie(prob_trie_proto);

    ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(*actual_ngram_proto, *expected_ngram_proto));
}
