#include "absolute_discounting.h"

AbsoluteDiscounting::AbsoluteDiscounting(std::string file_name, int n, int min_frequency) : NGram(n, min_frequency) {
    ProcessFile(file_name);
}

std::pair<int, int> AbsoluteDiscounting::ContextSize() {
    return std::make_pair(1, n);
}

bool AbsoluteDiscounting::operator==(const NGram &to_compare) {
    if (const AbsoluteDiscounting *to_compare_ads = dynamic_cast<const AbsoluteDiscounting*>(&to_compare)) {
        return NGram::operator==(to_compare) && (discount == to_compare_ads->discount);
    }
    return false;
}

tensorflow::Source::lm::ngram::NGramProto *AbsoluteDiscounting::ToProto() {
    tensorflow::Source::lm::ngram::NGramProto *ngram_proto = NGram::ToProto();
    ngram_proto->set_smoothing(tensorflow::Source::lm::ngram::Smoothing::ABSOLUTE_DISCOUNTING);
    ngram_proto->add_discount(discount);
    return ngram_proto;
}

void AbsoluteDiscounting::ProcessCountTrie(CountTrie *count_trie) {
    std::vector<std::vector<int>> n_r (n + 1, std::vector<int>(3));
    count_trie->CountNGrams(&n_r);
    discount = n_r[n][1] / (double) (n_r[n][1] + (2 * n_r[n][2]));

    std::list<size_t> seq;
    PopulateProbTrie(count_trie, count_trie->GetRoot(), 0, seq);
}

void AbsoluteDiscounting::PopulateProbTrie(CountTrie *countTrie, CountTrie::Node *node, int depth, std::list<size_t> seq) {
    if (depth > 0) {

        // Calculate pseudo_prob.
        double pseudo_prob = 0;
        double pseudo_prob_numerator = 0;
        double pseudo_prob_denominator = 0;
        if (depth == 1) {
            pseudo_prob_numerator = countTrie->Count(seq);
            pseudo_prob_denominator = countTrie->SumFollowing(std::list<size_t>({}));
        } else if (depth <= n) {
            pseudo_prob_numerator = std::max(countTrie->Count(seq) - discount, 0.0);
            size_t last_word_index = seq.back();
            seq.pop_back();
            pseudo_prob_denominator = countTrie->SumFollowing(seq);
            seq.push_back(last_word_index);
        }
        if (pseudo_prob_denominator > 0) {
            pseudo_prob = pseudo_prob_numerator / pseudo_prob_denominator;
        }

        // Calculate backoff.
        double backoff = 1;
        double backoff_denominator = 0;
        if (depth <= n) {
            backoff_denominator = countTrie->SumFollowing(seq);
        }
        if (backoff_denominator > 0) {
            backoff = (discount * countTrie->CountFollowing(seq)) / backoff_denominator;
        }

        prob_trie->Insert(seq, pseudo_prob, backoff);
    }
    for (std::unordered_map<size_t, CountTrie::Node*>::iterator it = node->children.begin(); it != node->children.end(); ++it) {
        seq.push_back(it->first);
        PopulateProbTrie(countTrie, it->second, depth + 1, seq);
        seq.pop_back();
    }
}
