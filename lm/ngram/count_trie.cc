#include "count_trie.h"


void CountTrie::ProcessFile(std::string file_name, Vocab *vocab) {
    std::ifstream f (file_name);

    if (f.is_open()) {
        std::string line;
        int line_number = 0;
        size_t start_marker_index = vocab->Insert("<s>");

        while (std::getline(f, line)) {
            size_t pos = 0;
            std::string word;
            std::list<size_t> ngram_window;

            for (int i = 0; i < n - 1; i++) {
                ngram_window.push_back(start_marker_index);
            }

            while (!line.empty()) {
                pos = line.find(" ");
                if (pos == std::string::npos) {
                    pos = line.size();
                }
                word = line.substr(0, pos);
                size_t word_index = vocab->Insert(word);

                ngram_window.push_back(word_index);
                std::list<size_t> ngram;
                for (std::list<size_t>::reverse_iterator it = ngram_window.rbegin(); it != ngram_window.rend(); ++it) {
                    ngram.push_front(*it);
                    Insert(ngram);
                }

                if (ngram_window.size() >= n) {
                    ngram_window.pop_front();
                }

                line.erase(0, pos + 1);
            }

            line_number++;
            if (line_number % 10000 == 0) {
                std::cout << "Read " << line_number << " lines." << std::endl;
            }
        }
        std::list<size_t> seq;
        ComputeCountsAndSums(root, seq);
    }
}

int CountTrie::Count(std::list<size_t> seq) {
    CountTrie::Node *node = GetNode(seq, false);
    if (node) {
        return node->count;
    }
    return 0;
}

int CountTrie::CountFollowing(std::list<size_t> seq) {
    CountTrie::Node *node = GetNode(seq, false);
    if (node) {
        return node->count_following;
    }
    return 0;
}

int CountTrie::CountPreceding(std::list<size_t> seq) {
    CountTrie::Node *node = GetNode(seq, false);
    if (node) {
        return node->count_preceding;
    }
    return 0;
}

int CountTrie::CountPrecedingAndFollowing(std::list<size_t> seq) {
    CountTrie::Node *node = GetNode(seq, false);
    if (node) {
        return node->count_preceding_and_following;
    }
    return 0;
}

int CountTrie::SumFollowing(std::list<size_t> seq) {
    CountTrie::Node *node = GetNode(seq, false);
    if (node) {
        return node->sum_following;
    }
    return 0;
}

int CountTrie::VocabSize() {
    // Minus 1 because of the <s> marker.
    return root->count_following - 1;
}

CountTrie::Node *CountTrie::GetNode(std::list<size_t> seq, bool create_new) {
    CountTrie::Node *curr = root;

    for (std::list<size_t>::iterator it = seq.begin(); it != seq.end(); ++it) {
        if (curr->children.count(*it) < 1) {
            if (create_new) {
                curr->children.insert(std::make_pair(*it, new CountTrie::Node()));
            } else {
                return NULL;
            }
        }
        curr = (curr->children.find(*it))->second;
    }
    return curr;
}

void CountTrie::Insert(std::list<size_t> seq) {
    if (seq.size() <= n) {
        CountTrie::Node *curr = root;

        for (std::list<size_t>::iterator it = seq.begin(); it != seq.end(); ++it) {
            if (curr->children.empty() || curr->children.count(*it) < 1) {
                curr->children.insert(std::make_pair(*it, new CountTrie::Node()));
            }
            curr = (curr->children.find(*it))->second;
        }

        curr->count++;

        size_t predecessor = seq.front();
        seq.pop_front();
        if (seq.size() > 0) {
            CountTrie::Node *node = GetNode(seq, true);
            if (node->predecessors.count(predecessor) == 0) {
                node->predecessors.insert(predecessor);
                node->count_preceding++;
            }
        }
    }
}

void CountTrie::ComputeCountsAndSums(CountTrie::Node *node, std::list<size_t> seq) {
    int sum_following = 0;
    int count_following = 0;
    int count_preceding_and_following = 0;

    for (auto child = node->children.begin(); child != node->children.end(); ++child) {
        seq.push_back(child->first);
        ComputeCountsAndSums(child->second, seq);
        seq.pop_back();

        sum_following += child->second->count;
        count_following += 1;
        count_preceding_and_following += child->second->count_preceding;
    }

    node->sum_following = sum_following;
    node->count_following = count_following;
    node->count_preceding_and_following = count_preceding_and_following;
}

CountTrie::Node *CountTrie::GetRoot() {
    return root;
}
