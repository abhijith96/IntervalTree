//
// Created by Abhijith  K A on 27/01/24.
//

#include "AVLNode.h"

#include "TestClass.h"

template <typename U>

const U& AVLNode<U>::GetKey() const {
    return key_;
}

template <typename U>
void AVLNode<U>::SetSize(size_type size) {
    size_ = size;
}

template <typename U>
size_type AVLNode<U>::GetSize() const {
    return size_;
}

template <typename U>
AVLNode<U>* AVLNode<U>::GetLeft() const {
    return left_.get();
}

template <typename U>
AVLNode<U>* AVLNode<U>::GetRight() const {
    return right_.get();
}

template <typename U>
void AVLNode<U>::SetLeft(std::unique_ptr<AVLNode<U>> &&left) {
    left_ = std::move(left);
}

template <typename U>
void AVLNode<U>::SetRight(std::unique_ptr<AVLNode<U>> &&right) {
    right_ = std::move(right);
}

template <typename U>
size_type AVLNode<U>::GetDepth() const {
    return depth_;
}

template <typename U>
void AVLNode<U>::SetDepth(size_type depth) {
    depth_ = depth;
}

template <typename U>
bool AVLNode<U>::IsEmpty() const {
    return size_ == 0;
}

template <typename U>
std::unique_ptr<AVLNode<U>> AVLNode<U>::GetLeftValue() {
    return std::move(left_);
}

template <typename U>
std::unique_ptr<AVLNode<U>> AVLNode<U>::GetRightValue() {
    return std::move(right_);
}

template class AVLNode<int>;

template class AVLNode<TestClass>;



