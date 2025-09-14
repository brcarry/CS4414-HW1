#include "knn.hpp"
#include <vector>
#include <chrono>
#include <algorithm>

// Definition of static member
Embedding_T Node::queryEmbedding;


float distance(const Embedding_T &a, const Embedding_T &b)
{
    return std::abs(a - b);
}


constexpr float getCoordinate(Embedding_T e, size_t axis)
{
    return e;  // scalar case
}

// Build a balanced KD‐tree by splitting on median at each level.
Node* buildKD(std::vector<std::pair<Embedding_T,int>>& items, int depth) {
    /*
    TODO: Implement this function to build a balanced KD-tree.
    You should recursively construct the tree and return the root node.
    For now, this is a stub that returns nullptr.
    */
    if (items.empty()) return nullptr;

    static bool sorted = false;
    if (!sorted) {
        std::sort(items.begin(), items.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });
        sorted = true;
    }
    size_t mid = items.size() / 2;
    Node* node = new Node(items[mid].first, items[mid].second);
    std::vector<std::pair<Embedding_T,int>> left(items.begin(), items.begin() + mid);
    std::vector<std::pair<Embedding_T,int>> right(items.begin() + mid + 1, items.end());
    node->left = buildKD(left, depth + 1);
    node->right = buildKD(right, depth + 1);
    return node;
}


void freeTree(Node *node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}


void knnSearch(Node *node,
               int depth,
               int K,
               MaxHeap &heap)
{
    /*
    TODO: Implement this function to perform k-nearest neighbors (k-NN) search on the KD-tree.
    You should recursively traverse the tree and maintain a max-heap of the K closest points found so far.
    For now, this is a stub that does nothing.
    */
   if (!node) return;
   float dist = distance(node->embedding, Node::queryEmbedding);
   heap.push({dist, node->idx});
   if (heap.size() > static_cast<size_t>(K)) heap.pop();
   knnSearch(node->left, depth + 1, K, heap);
   knnSearch(node->right, depth + 1, K, heap);
}