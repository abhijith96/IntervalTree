//
// Created by Abhijith  K A on 27/01/24.
//

#include "AVLTree.h"
#include <algorithm>
#include "TestClass.h"

template <typename T, typename Compare>
size_type AVLTree<T, Compare>::GetSize() {
    return root_.get() ? root_->GetSize() : 0;
}

template <typename T, typename Compare>
bool AVLTree<T, Compare>::Insert(T key) {
 auto [newRoot, isNewInsert] = InsertHelper(std::move(root_), key);
 root_ = std::move(newRoot);
 return isNewInsert;
}

template <typename T, typename Compare>
bool AVLTree<T, Compare>::Delete(T key) {
    auto [newRoot, isFound] = DeleteHelper(std::move(root_), key);
    root_ = std::move(newRoot);
    return isFound;
}

template <typename T, typename Compare>
bool AVLTree<T, Compare>::IsPresent(T key) const{
    AVLNode<T>* ptr = root_.get();
    Compare compare;
    while(ptr && !ptr->IsEmpty()){
        if(compare(key, ptr->GetKey())){
            ptr = ptr->GetLeft();
        }
        else if(compare(ptr->GetKey(), key)){
            ptr = ptr->GetRight();
        }
        else{
            return true;
        }
    }
    return false;
}

template <typename T, typename Compare>
std::pair<std::unique_ptr<AVLNode<T>>, bool>  AVLTree<T, Compare>::InsertHelper(std::unique_ptr<AVLNode<T>> node, const T& key) {
    if(node->IsEmpty()){
        node = std::move(std::make_unique<AVLNode<T>>(key));
        return {std::move(node), true};
    }
    else{
        bool isNewInsert = true;
        bool isLeft = false;
        bool isRight = false;
        Compare compare;
        if(compare(key, node->GetKey())){
            auto [leftNode, isNewInsertLeft] = InsertHelper(node->GetLeftValue(), key);
          node->SetLeft(std::move(leftNode));
          isNewInsert &= isNewInsertLeft;
          isLeft = true;
        }
        else if(compare(node->GetKey(), key)){
            auto [rightNode, isNewInsertRight] = InsertHelper(node->GetRightValue(), key);
            node->SetRight(std::move(rightNode));
            isNewInsert &= isNewInsertRight;
            isRight = true;
        }
        else{
            isNewInsert = false;
        }

        node->SetDepth(1 + std::max(node->GetLeft()->GetDepth(), node->GetRight()->GetDepth()));
        node->SetSize(1 + node->GetLeft()->GetSize() + node->GetRight()->GetSize());
        size_type depth_diff = 0;
        if(node->GetLeft()->GetDepth() > node->GetRight()->GetDepth()) depth_diff = node->GetLeft()->GetDepth() -
                                                                                    node->GetRight()->GetDepth();
        if(node->GetRight()->GetDepth() > node->GetLeft()->GetDepth()) depth_diff = node->GetRight()->GetDepth() -node->GetLeft()->GetDepth();

        if(depth_diff > 1) {
            if (isLeft && !node->GetLeft()->IsEmpty() &&  !compare(node->GetLeft()->GetKey(), key)) {
                node = std::move(RightRotate(node, node->GetLeftRef()));
            } else if (isLeft) {
                node->SetLeft(LeftRotate(node->GetLeftRef(), node->GetLeftRef()->GetRightRef()));
               node = std::move(RightRotate(node, node->GetLeftRef()));
            } else if (isRight && !node->GetRight()->IsEmpty() &&
                       !compare(key, node->GetRight()->GetKey())) {
                node = std::move(LeftRotate(node, node->GetRightRef()));
            } else if (isRight) {
                node->SetRight(RightRotate(node->GetRightRef(), node->GetRightRef()->GetLeftRef()));
                node = std::move(LeftRotate(node, node->GetRightRef()));
            } else {
                assert(false);
            }
        }
        return {std::move(node), isNewInsert};
    }
}

template <typename T>
std::pair<size_type, bool> IsValidAvlTreeHelper(AVLNode<T>* currentNode, int balanceFactor){
    if(currentNode->IsEmpty()) return {0, true};
    size_type  leftDepth;
    bool isLeftSubTreeValid;
    std::tie(leftDepth, isLeftSubTreeValid)= IsValidAvlTreeHelper(currentNode->GetLeft(), balanceFactor);
    size_type  rightDepth;
    bool isRightSubTreeValid;
    std::tie(rightDepth, isRightSubTreeValid)= IsValidAvlTreeHelper(currentNode->GetRight(), balanceFactor);
    if(!(isLeftSubTreeValid && isRightSubTreeValid)) return {0, false};
    size_type heightDiff = 0;
    if(rightDepth > leftDepth) heightDiff = rightDepth - leftDepth;
    else heightDiff = leftDepth - rightDepth;
    if(heightDiff >= balanceFactor){
        return {0, false};
    }
    return {1 + std::max(leftDepth, rightDepth), true};
}

template <typename T, typename Compare>
std::unique_ptr<AVLNode<T>> AVLTree<T, Compare>::LeftRotate(std::unique_ptr<AVLNode<T>>& grandParent, std::unique_ptr<AVLNode<T>>& parent){
    grandParent->SetSize(grandParent->GetSize() - parent->GetSize() + parent->GetLeft()->GetSize());
    parent->SetSize(parent->GetSize() + 1 + grandParent->GetLeft()->GetSize());
    std::unique_ptr<AVLNode<T>> parentLeftCopy = std::move(parent->GetLeftValue());
    std::unique_ptr<AVLNode<T>> parentCopy = std::move(parent);
    parentCopy->SetLeft(std::move(grandParent));
    parentCopy->GetLeft()->SetRight(std::move(parentLeftCopy));
    parentCopy->GetLeft()->SetDepth(1 + std::max(parentCopy->GetLeft()->GetRight()->GetDepth(),
                                                 parentCopy->GetLeft()->GetLeft()->GetDepth()));
    parentCopy->SetDepth(1 + std::max(parentCopy->GetLeft()->GetDepth(), parentCopy->GetRight()->GetDepth()));
    return parentCopy;
}

template <typename T, typename Compare>
std::unique_ptr<AVLNode<T>> AVLTree<T, Compare>::RightRotate(std::unique_ptr<AVLNode<T>>& grandParent, std::unique_ptr<AVLNode<T>>& parent){
    grandParent->SetSize(grandParent->GetSize() - parent->GetSize() + parent->GetRight()->GetSize());
    parent->SetSize(parent->GetSize() + 1 + grandParent->GetRight()->GetSize());
    std::unique_ptr<AVLNode<T>> parentRightCopy = std::move(parent->GetRightValue());
    std::unique_ptr<AVLNode<T>> parentCopy = std::move(parent);
    parentCopy->SetRight(std::move(grandParent));
    parentCopy->GetRight()->SetLeft(std::move(parentRightCopy));
    parentCopy->GetRight()->SetDepth(1 + std::max(parentCopy->GetRight()->GetLeft()->GetDepth(),
                                                  parentCopy->GetRight()->GetRight()->GetDepth()));
    parentCopy->SetDepth(1 + std::max(parentCopy->GetLeft()->GetDepth(), parentCopy->GetRight()->GetDepth()));
    return parentCopy;
}

template <typename T, typename Compare>
bool AVLTree<T, Compare>::IsValidAvlTree(int balanceFactor) const{
    return IsValidAvlTreeHelper(root_.get(), balanceFactor).second;
}

template <typename T>
std::unique_ptr<AVLNode<T>>& FindPredecessorForDelete(std::unique_ptr<AVLNode<T>>& node){
    assert(node->GetLeft() && !node->GetLeft()->IsEmpty());
    auto prevNode = node.get();
    auto currentNode = node->GetLeft();
    auto nextNode = currentNode->GetRight();
    while(!nextNode->IsEmpty()){
        prevNode = currentNode;
        currentNode = nextNode;
        nextNode = nextNode->GetRight();
    }
    return prevNode->GetLeft() == currentNode ? prevNode->GetLeftRef() : prevNode->GetRightRef();
}

template <typename T, typename Compare>
std::pair<std::unique_ptr<AVLNode<T>>, bool> AVLTree<T, Compare>::DeleteHelper(std::unique_ptr<AVLNode<T>> node, T key) {
    if(node->IsEmpty()){
        return {std::make_unique<AVLNode<T>>(), false};
    }
    Compare compare;
    bool isLeft = false;
    bool isRight = false;
    bool isSecondLeft = false;
    bool isSecondRight = false;
    bool isFound = false;

    if(compare(key, node->GetKey())){
       auto [leftNode, isFoundLeft] = DeleteHelper(std::move(node->GetLeftValue()), key);
       node->SetLeft(std::move(leftNode));
       isFound = isFoundLeft;
    }
    else if(compare(node->GetKey(), key)){
        auto [rightNode, isFoundRight] = DeleteHelper(std::move(node->GetRightValue()), key);
        node->SetRight(std::move(rightNode));
        isFound = isFoundRight;
    }
    //keys are equal
    else {
        if (!node->GetLeft()->IsEmpty() && !node->GetRight()->IsEmpty()) {
            std::unique_ptr<AVLNode<T>> &predecessor = FindPredecessorForDelete<T>(node);
            node->SwapKeys(*predecessor);
            auto [leftNode, isFoundLeft] = DeleteHelper(std::move(node->GetLeftValue()), key);
            node->SetLeft(std::move(leftNode));
            isFound = isFoundLeft;
        } else {
            if (node->GetLeft()->IsEmpty() && node->GetRight()->IsEmpty()) {
                node = std::move(std::make_unique<AVLNode<T>>());
            } else if (node->GetLeft()->IsEmpty()) {
                node = std::move(node->GetRightValue());
            } else if (node->GetRight()->IsEmpty()) {
                node = std::move(node->GetLeftValue());
            }

            if (!node->IsEmpty()) {
                node->SetSize(1 + node->GetLeft()->GetSize() + node->GetRight()->GetSize());
                node->SetDepth(1 + std::max(node->GetLeft()->GetDepth(), node->GetRight()->GetDepth()));
            }
            return {std::move(node), true};
        }
    }

    node->SetSize(1 + node->GetLeft()->GetSize() + node->GetRight()->GetSize());
    node->SetDepth(1 + std::max(node->GetLeft()->GetDepth(), node->GetRight()->GetDepth()));

    size_type depth_diff = 0;
    if(node->GetLeft()->GetDepth() > node->GetRight()->GetDepth()){
        isLeft = true;
        depth_diff = node->GetLeft()->GetDepth() - node->GetRight()->GetDepth();
        if(!node->GetLeft()->IsEmpty() && node->GetLeft()->GetLeft()->GetDepth() >= node->GetLeft()->GetRight()->GetDepth()){
            isSecondLeft = true;
        }
        else{
            isSecondRight = true;
        }
    }
    else if(node->GetRight()->GetDepth() > node->GetLeft()->GetDepth()){
        depth_diff = node->GetRight()->GetDepth() - node->GetLeft()->GetDepth();
        isRight = true;
        if(!node->GetRight()->IsEmpty() && node->GetRight()->GetRight()->GetDepth() >= node->GetRight()->GetLeft()->GetDepth()){
            isSecondRight = true;
        }
        else{
            isSecondLeft = true;
        }
    }
    if(depth_diff >= 2){
        if(isLeft && isSecondLeft){
            node = RightRotate(node, node->GetLeftRef());
        }
        else if(isLeft && isSecondRight){
            node->SetLeft(LeftRotate(node->GetLeftRef(), node->GetLeftRef()->GetRightRef()));
            node = RightRotate(node, node->GetLeftRef());
        }
        else if(isRight && isSecondRight){
            node = LeftRotate(node, node->GetRightRef());
        }
        else if(isRight && isSecondLeft){
            node->SetRight(RightRotate(node->GetRightRef(), node->GetRightRef()->GetLeftRef()));
            node = LeftRotate(node, node->GetRightRef());
        }
        else{
            assert(false);
        }
    }
    return {std::move(node), isFound};
}

template <typename T, typename Compare>
 bool AVLTree<T, Compare>::IsEmpty() const{
    return !root_.get() || root_->IsEmpty();
}

template <typename T, typename Compare>
size_type AVLTree<T, Compare>::GetSize() const {
    return root_->GetSize();
}

template <typename T, typename Compare>
std::optional<T> AVLTree<T, Compare>::FindKthElement(size_type k) const {
    if(k >= GetSize()){
        return {};
    }
    AVLNode<T>* currentNode = root_.get();
    size_type currentIndex = currentNode->GetLeft()->GetSize() + 1 -1;
    while(currentIndex != k){
        if(currentIndex < k){
            k -= currentNode->GetLeft()->GetSize() + 1;
            currentNode = currentNode->GetRight();
        }
        else{
            currentNode = currentNode->GetLeft();
        }
        currentIndex = currentNode->GetLeft()->GetSize() + 1 - 1;
    }
    return  currentNode->GetKey();
}

template class AVLTree<int>;
template class AVLTree<TestClass, CompareTestClass>;