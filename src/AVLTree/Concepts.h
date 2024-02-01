//
// Created by Abhijith  K A on 28/01/24.
//

#ifndef AUGMENTEDAVLTREE_CONCEPTS_H
#define AUGMENTEDAVLTREE_CONCEPTS_H

#include <concepts>
#include <memory>

template<typename T>
concept AVLTreeKey = requires() {
    typename std::unique_ptr<T>;
    std::totally_ordered<T>;
};

#endif //AUGMENTEDAVLTREE_CONCEPTS_H
