#ifndef BPLUS_TREE_HPP
#define BPLUS_TREE_HPP

#include <iostream>
#include <queue>

// Generic B+ Tree implementation in C++
// Template parameters:
//   K: key type (must support <, ==)
//   V: value type
//   M: order of the tree (max children per node)

template<typename K, typename V, int M = 64>
class BPlusTree {
private:
    static const int ORDER = M;
    struct Node {
        bool leaf;
        int num_keys;
        K keys[ORDER - 1];
        Node* parent;
        void* pointers[ORDER];  // if leaf: V* values; else: Node* children
        Node* next;             // sibling pointer for leaf nodes

        Node(bool leaf = false)
            : leaf(leaf), num_keys(0), parent(nullptr), next(nullptr) {
            for (int i = 0; i < ORDER; ++i) pointers[i] = nullptr;
        }
    };

    Node* root;

    // Find leaf node where key should reside
    Node* find_leaf(const K& key) const {
        if (!root) return nullptr;
        Node* c = root;
        while (!c->leaf) {
            int i = 0;
            while (i < c->num_keys && key >= c->keys[i]) ++i;
            c = static_cast<Node*>(c->pointers[i]);
        }
        return c;
    }

    // Search for key in a leaf node
    V* search_in_leaf(Node* leaf, const K& key) const {
        if (!leaf) return nullptr;
        for (int i = 0; i < leaf->num_keys; ++i) {
            if (leaf->keys[i] == key)
                return reinterpret_cast<V*>(leaf->pointers[i]);
        }
        return nullptr;
    }

    // Insert key/value into a leaf node (no overflow handling)
    void insert_in_leaf(Node* leaf, const K& key, const V& value) {
        int i = 0;
        while (i < leaf->num_keys && leaf->keys[i] < key) ++i;
        for (int j = leaf->num_keys; j > i; --j) {
            leaf->keys[j] = leaf->keys[j - 1];
            leaf->pointers[j] = leaf->pointers[j - 1];
        }
        leaf->keys[i] = key;
        leaf->pointers[i] = new V(value);
        ++leaf->num_keys;
    }

    // Split a leaf node when overflow occurs
    void split_leaf(Node* leaf) {
        int split = (ORDER - 1) / 2;
        Node* new_leaf = new Node(true);
        new_leaf->parent = leaf->parent;
        new_leaf->next = leaf->next;
        leaf->next = new_leaf;

        // Move half of the keys/values to new leaf
        for (int i = split, j = 0; i < leaf->num_keys; ++i, ++j) {
            new_leaf->keys[j] = leaf->keys[i];
            new_leaf->pointers[j] = leaf->pointers[i];
            ++new_leaf->num_keys;
        }
        leaf->num_keys = split;

        // Promote split key to parent
        insert_in_parent(leaf, new_leaf->keys[0], new_leaf);
    }

    // Insert key and child into parent after split
    void insert_in_parent(Node* left, const K& key, Node* right) {
        if (!left->parent) {
            Node* new_root = new Node(false);
            new_root->keys[0] = key;
            new_root->pointers[0] = left;
            new_root->pointers[1] = right;
            new_root->num_keys = 1;
            left->parent = right->parent = new_root;
            root = new_root;
            return;
        }
        Node* parent = left->parent;
        int idx = 0;
        while (idx < parent->num_keys && parent->pointers[idx] != left) ++idx;
        for (int i = parent->num_keys; i > idx; --i) {
            parent->keys[i] = parent->keys[i - 1];
            parent->pointers[i + 1] = parent->pointers[i];
        }
        parent->keys[idx] = key;
        parent->pointers[idx + 1] = right;
        right->parent = parent;
        ++parent->num_keys;

        if (parent->num_keys == ORDER - 1)
            split_internal(parent);
    }

    // Split an internal node when overflow occurs
    void split_internal(Node* node) {
        int split = (ORDER - 1) / 2;
        Node* new_node = new Node(false);
        new_node->parent = node->parent;

        // Move keys and children to new internal
        for (int i = split + 1, j = 0; i < node->num_keys; ++i, ++j) {
            new_node->keys[j] = node->keys[i];
            new_node->pointers[j] = node->pointers[i];
            if (new_node->pointers[j])
                static_cast<Node*>(new_node->pointers[j])->parent = new_node;
            ++new_node->num_keys;
        }
        new_node->pointers[new_node->num_keys] = node->pointers[node->num_keys];
        if (new_node->pointers[new_node->num_keys])
            static_cast<Node*>(new_node->pointers[new_node->num_keys])->parent = new_node;

        K up_key = node->keys[split];
        node->num_keys = split;
        insert_in_parent(node, up_key, new_node);
    }

    // Remove a key and pointer/value from a node and handle underflow
    void remove_entry(Node* node, const K& key, void* ptr) {
        // Remove key
        int i = 0;
        while (i < node->num_keys && node->keys[i] != key) ++i;
        if (i < node->num_keys) {
            for (int j = i; j < node->num_keys - 1; ++j)
                node->keys[j] = node->keys[j + 1];
        }
        // Remove pointer/value
        int total = node->leaf ? node->num_keys : node->num_keys + 1;
        int idx = 0;
        while (idx < total && node->pointers[idx] != ptr) ++idx;
        for (int j = idx; j < total - 1; ++j)
            node->pointers[j] = node->pointers[j + 1];
        --node->num_keys;

        if (node == root) {
            adjust_root();
            return;
        }

        int min_keys = node->leaf ? (ORDER - 1) / 2 : (ORDER - 1) / 2;
        if (node->num_keys < min_keys)
            coalesce_or_redistribute(node);
    }

    // Adjust root after deletion
    void adjust_root() {
        if (!root) return;
        if (!root->leaf && root->num_keys == 0) {
            Node* new_root = static_cast<Node*>(root->pointers[0]);
            new_root->parent = nullptr;
            delete root;
            root = new_root;
        } else if (root->leaf && root->num_keys == 0) {
            delete root;
            root = nullptr;
        }
    }

    // Handle underflow via redistribution or merge
    void coalesce_or_redistribute(Node* node) {
        Node* parent = node->parent;
        int idx = 0;
        while (idx <= parent->num_keys && parent->pointers[idx] != node) ++idx;
        Node* left_sib = (idx > 0) ? static_cast<Node*>(parent->pointers[idx - 1]) : nullptr;
        Node* right_sib = (idx < parent->num_keys) ? static_cast<Node*>(parent->pointers[idx + 1]) : nullptr;
        int min_keys = node->leaf ? (ORDER - 1) / 2 : (ORDER - 1) / 2;

        if (left_sib && left_sib->num_keys > min_keys)
            redistribute_nodes(left_sib, node, idx - 1);
        else if (right_sib && right_sib->num_keys > min_keys)
            redistribute_nodes(node, right_sib, idx);
        else if (left_sib)
            coalesce_nodes(left_sib, node, idx - 1);
        else
            coalesce_nodes(node, right_sib, idx);
    }

    // Redistribute entries between siblings
    void redistribute_nodes(Node* left, Node* right, int k_idx) {
        Node* parent = left->parent;
        if (left->leaf) {
            for (int i = right->num_keys; i > 0; --i) {
                right->keys[i] = right->keys[i - 1];
                right->pointers[i] = right->pointers[i - 1];
            }
            right->keys[0] = left->keys[left->num_keys - 1];
            right->pointers[0] = left->pointers[left->num_keys - 1];
            --left->num_keys; ++right->num_keys;
            parent->keys[k_idx] = right->keys[0];
        } else {
            for (int i = right->num_keys; i > 0; --i)
                right->keys[i] = right->keys[i - 1];
            for (int i = right->num_keys + 1; i > 0; --i)
                right->pointers[i] = right->pointers[i - 1];
            right->pointers[0] = left->pointers[left->num_keys];
            static_cast<Node*>(right->pointers[0])->parent = right;
            right->keys[0] = parent->keys[k_idx];
            parent->keys[k_idx] = left->keys[left->num_keys - 1];
            --left->num_keys; ++right->num_keys;
        }
    }

    // Merge two nodes and delete the separator key
    void coalesce_nodes(Node* left, Node* right, int k_idx) {
        Node* parent = left->parent;
        if (left->leaf) {
            for (int i = 0; i < right->num_keys; ++i) {
                left->keys[left->num_keys] = right->keys[i];
                left->pointers[left->num_keys] = right->pointers[i];
                ++left->num_keys;
            }
            left->next = right->next;
        } else {
            left->keys[left->num_keys++] = parent->keys[k_idx];
            for (int i = 0; i < right->num_keys; ++i) {
                left->keys[left->num_keys] = right->keys[i];
                left->pointers[left->num_keys] = right->pointers[i];
                static_cast<Node*>(left->pointers[left->num_keys])->parent = left;
                ++left->num_keys;
            }
            left->pointers[left->num_keys] = right->pointers[right->num_keys];
            static_cast<Node*>(left->pointers[left->num_keys])->parent = left;
        }
        remove_entry(parent, parent->keys[k_idx], right);
        delete right;
    }

public:
    BPlusTree() : root(nullptr) {}
    ~BPlusTree() {
        if (!root) return;
        std::queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node* n = q.front(); q.pop();
            if (!n->leaf) {
                for (int i = 0; i <= n->num_keys; ++i)
                    q.push(static_cast<Node*>(n->pointers[i]));
            } else {
                for (int i = 0; i < n->num_keys; ++i)
                    delete reinterpret_cast<V*>(n->pointers[i]);
            }
            delete n;
        }
    }

    // Public search: returns pointer to value or nullptr
    V* search(const K& key) const {
        Node* leaf = find_leaf(key);
        return search_in_leaf(leaf, key);
    }

    // Public insert
    void insert(const K& key, const V& value) {
        if (!root) {
            root = new Node(true);
            insert_in_leaf(root, key, value);
            return;
        }
        Node* leaf = find_leaf(key);
        V* existing = search_in_leaf(leaf, key);
        if (existing) { *existing = value; return; }
        if (leaf->num_keys < ORDER - 1) {
            insert_in_leaf(leaf, key, value);
        } else {
            insert_in_leaf(leaf, key, value);
            split_leaf(leaf);
        }
    }

    // Public remove
    void remove(const K& key) {
        Node* leaf = find_leaf(key);
        if (!leaf) return;
        V* val_ptr = search_in_leaf(leaf, key);
        if (!val_ptr) return;
        remove_entry(leaf, key, val_ptr);
    }

    // Display tree level by level
    void display() const {
        if (!root) { std::cout << "<empty>\n"; return; }
        std::queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; ++i) {
                Node* n = q.front(); q.pop();
                std::cout << "[";
                for (int j = 0; j < n->num_keys; ++j)
                    std::cout << n->keys[j] << (j < n->num_keys - 1 ? "," : "");
                std::cout << "] ";
                if (!n->leaf) {
                    for (int j = 0; j <= n->num_keys; ++j)
                        q.push(static_cast<Node*>(n->pointers[j]));
                }
            }
            std::cout << "\n";
        }
    }
};

#endif // BPLUS_TREE_HPP
