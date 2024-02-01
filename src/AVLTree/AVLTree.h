//
// Created by Abhijith  K A on 27/01/24.
//

#ifndef AUGMENTEDAVLTREE_AVLTREE_H
#define AUGMENTEDAVLTREE_AVLTREE_H

#include "AVLNode.h"
#include "Concepts.h"
#include <algorithm>

template <typename T, typename Compare = std::less<T>>

class AVLTree {
private:
    std::unique_ptr<AVLNode<T>> root_;

    std::pair<std::unique_ptr<AVLNode<T>>, bool> InsertHelper(std::unique_ptr<AVLNode<T>> node, const T& key);

    std::pair<std::unique_ptr<AVLNode<T>>, bool> DeleteHelper(std::unique_ptr<AVLNode<T>> node, T key);

    std::unique_ptr<AVLNode<T>> RightRotate(std::unique_ptr<AVLNode<T>>& grandParent, std::unique_ptr<AVLNode<T>>& parent);

    std::unique_ptr<AVLNode<T>> LeftRotate(std::unique_ptr<AVLNode<T>>& grandParent, std::unique_ptr<AVLNode<T>>& parent);

public:
    AVLTree():root_(std::make_unique<AVLNode<T>>()) {};

    size_type GetSize();

    bool Insert(T key);

    bool Delete (T key);

    [[nodiscard]] bool IsPresent(T key) const;

    [[nodiscard]]   bool IsEmpty() const;
    [[nodiscard]] size_type  GetSize() const;

    [[nodiscard]] bool IsValidAvlTree(int balanceFactor = 2) const;

    std::optional<T> FindKthElement(size_type k) const;
};


#endif //AUGMENTEDAVLTREE_AVLTREE_H
