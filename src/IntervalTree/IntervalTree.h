//
// Created by Abhijith  K A on 28/01/24.
//

#ifndef AUGMENTEDAVLTREE_INTERVALTREE_H
#define AUGMENTEDAVLTREE_INTERVALTREE_H

#include "Common.h"
#include "IntervalTreeNode.h"
#include <optional>
#include <algorithm>
#include <stack>

namespace DS::IntervalTree {


    template <typename KeyType, typename Compare=std::less<KeyType>>
    class IntervalTree {
    private:

        //template <typename KeyType, typename Compare>

        struct CompareIntervals{
            bool operator()(const IntervalTreeNode<KeyType>& lhs, const IntervalTreeNode<KeyType>& rhs) const{
                Compare compare;
                if(lhs.IsEmpty()) return true;
                if(rhs.IsEmpty()) return false;
                if(compare(lhs.GetLow(), rhs.GetLow())){
                    return true;
                }
                if(compare(rhs.GetLow(), lhs.GetLow())){
                    return false;
                }
                if(compare(lhs.GetHigh(), rhs.GetHigh())){
                    return true;
                }
                return false;
            }
        };

    public:
        class Iterator;
        class ReverseIterator;

    private:
        std::unique_ptr<IntervalTreeNode<KeyType>> root_;
        
        std::pair<std::unique_ptr<IntervalTreeNode<KeyType>>, bool> InsertHelper(std::unique_ptr<IntervalTreeNode<KeyType>> node, const KeyType& low, const KeyType& high);

        std::pair<std::unique_ptr<IntervalTreeNode<KeyType>>, bool> DeleteHelper(std::unique_ptr< IntervalTreeNode<KeyType>> node, const KeyType& low, const KeyType& high);

        std::unique_ptr< IntervalTreeNode<KeyType>> RightRotate(std::unique_ptr< IntervalTreeNode<KeyType>>& grandParent, std::unique_ptr< IntervalTreeNode<KeyType>>& parent);

        std::unique_ptr< IntervalTreeNode<KeyType>> LeftRotate(std::unique_ptr< IntervalTreeNode<KeyType>>& grandParent, std::unique_ptr< IntervalTreeNode<KeyType>>& parent);

        const KeyType& GetMaximum(const IntervalTreeNode<KeyType>& currentNode, const IntervalTreeNode<KeyType>& leftNode,
                                  const IntervalTreeNode<KeyType>& rightNode);
    public:
        explicit IntervalTree() : root_(std::make_unique<IntervalTreeNode<KeyType>>()){}

        [[nodiscard]]   bool IsEmpty() const;
        [[nodiscard]] size_type  GetSize() const;

        bool Insert(KeyType low, KeyType high);

        bool Delete (KeyType low, KeyType high);

        Iterator cBegin(){
            return Iterator(root_.get());
        }
        Iterator cEnd(){
            return Iterator();
        }

        ReverseIterator rcBegin(){
            return ReverseIterator(root_.get());
        }
        ReverseIterator rcEnd(){
            return ReverseIterator();
        }
        [[nodiscard]] bool IsPresent(KeyType low, KeyType high) const;
        
        [[nodiscard]] bool IsValidAvlTree(int balanceFactor = 2) const;

        std::optional<KeyType> FindKthElement(size_type k) const;

        std::optional<std::pair<KeyType,KeyType>> FindAnOverlappingInterval(KeyType low, KeyType high);

        std::vector<std::pair<KeyType, KeyType>> FindAllOverlappingIntervals(KeyType low, KeyType high);

    };
    
    template <typename KeyType, typename Compare>
    size_type IntervalTree<KeyType,Compare>::GetSize() const {
        return root_ ? root_.GetSize() : 0;
    }

    template <typename KeyType, typename Compare>
    bool IntervalTree<KeyType,Compare>::IsEmpty() const {
       return !root_.get() || root_->IsEmpty();
   }

    template <typename KeyType, typename Compare>
    bool IntervalTree<KeyType,Compare>::Insert(KeyType low, KeyType high) {
        auto [newRoot, isNewInsert] = InsertHelper(std::move(root_), low, high);
        root_ = std::move(newRoot);
        return isNewInsert;
    }

    template <typename KeyType, typename Compare>
    bool IntervalTree<KeyType,Compare>::Delete(KeyType low, KeyType high) {
        auto [newRoot, isFound] = DeleteHelper(std::move(root_), low, high);
        root_ = std::move(newRoot);
        return isFound;
    }

    template <typename KeyType, typename Compare>
    bool IntervalTree<KeyType,Compare>::IsPresent(KeyType low, KeyType high) const {

        IntervalTreeNode searchNode {low, high};

         IntervalTreeNode<KeyType>* ptr = root_.get();

        CompareIntervals compareIntervals;

        while(ptr && !ptr->IsEmpty()){
            if(compareIntervals(searchNode, *ptr)){
                ptr = ptr->GetLeft();
            }
            else if(compareIntervals(*ptr, searchNode)){
                ptr = ptr->GetRight();
            }
            else{
                return !compareIntervals(searchNode, *ptr) && !compareIntervals(*ptr, searchNode);
            }
        }
        return false;

    }

    template <typename KeyType, typename Compare>
    std::pair<size_type, bool> IsValidAvlTreeHelper( IntervalTreeNode<KeyType>* currentNode, int balanceFactor){
        if(currentNode->IsEmpty()) return {0, true};
        size_type  leftDepth;
        bool isLeftSubTreeValid;
        std::tie(leftDepth, isLeftSubTreeValid)= IsValidAvlTreeHelper(currentNode->GetLeft(), balanceFactor);
        size_type  rightDepth;
        bool isRightSubTreeValid;
        std::tie(rightDepth, isRightSubTreeValid)= IsValidAvlTreeHelper(currentNode->GetRight(), balanceFactor);
        if(!(isLeftSubTreeValid && isRightSubTreeValid)) return {0, false};
        size_type heightDiff;
        if(rightDepth > leftDepth) heightDiff = rightDepth - leftDepth;
        else heightDiff = leftDepth - rightDepth;
        if(heightDiff >= balanceFactor){
            return {0, false};
        }
        return {1 + std::max(leftDepth, rightDepth), true};
    }

    template <typename KeyType, typename Compare>
    bool IntervalTree<KeyType,Compare>::IsValidAvlTree(int balanceFactor) const {
    return IsValidAvlTreeHelper<KeyType, Compare>(balanceFactor).second;
    }

    template <typename KeyType, typename Compare>
    std::optional<KeyType> IntervalTree<KeyType,Compare>::FindKthElement(size_type k) const {

        if(k >= GetSize()){
            return {};
        }
         IntervalTreeNode<KeyType>* currentNode = root_.get();
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
        return  currentNode->GetLow();

    }

    template <typename KeyType, typename Compare>
    std::pair<std::unique_ptr<IntervalTreeNode<KeyType>>, bool> IntervalTree<KeyType,Compare>::InsertHelper(std::unique_ptr<IntervalTreeNode<KeyType>> node, const KeyType& low, const KeyType& high){
        if(node->IsEmpty()){
            node = std::move(std::make_unique< IntervalTreeNode<KeyType>>(low, high));
            node->SetMax(high);
            return {std::move(node), true};
        }
        else{
            bool isNewInsert = true;
            bool isLeft = false;
            bool isRight = false;
            CompareIntervals compareIntervals;

            Compare compare;
            if(compareIntervals(IntervalTreeNode<KeyType>{low, high}, *node)){
                auto [leftNode, isNewInsertLeft] = InsertHelper(node->GetLeftValue(), low, high);
                node->SetLeft(std::move(leftNode));
                isNewInsert &= isNewInsertLeft;
                isLeft = true;
            }
            else if(compareIntervals(*node, IntervalTreeNode<KeyType>{low, high})){
                auto [rightNode, isNewInsertRight] = InsertHelper(node->GetRightValue(), low, high);
                node->SetRight(std::move(rightNode));
                isNewInsert &= isNewInsertRight;
                isRight = true;
            }
            else{
                isNewInsert = false;
            }

            node->SetDepth(1 + std::max(node->GetLeft()->GetDepth(), node->GetRight()->GetDepth()));
            node->SetSize(1 + node->GetLeft()->GetSize() + node->GetRight()->GetSize());
            node->SetMax(GetMaximum(*node, *node->GetLeft(), *node->GetRight()));
            size_type depth_diff = 0;
            if(node->GetLeft()->GetDepth() > node->GetRight()->GetDepth()) depth_diff = node->GetLeft()->GetDepth() -
                                                                                        node->GetRight()->GetDepth();
            if(node->GetRight()->GetDepth() > node->GetLeft()->GetDepth()) depth_diff = node->GetRight()->GetDepth() -node->GetLeft()->GetDepth();


            if(depth_diff > 1) {
                if (isLeft && !node->GetLeft()->IsEmpty() &&  !compareIntervals(*node->GetLeft(),IntervalTreeNode<KeyType>{low, high})) {
                    node = std::move(RightRotate(node, node->GetLeftRef()));
                } else if (isLeft) {
                    node->SetLeft(LeftRotate(node->GetLeftRef(), node->GetLeftRef()->GetRightRef()));
                    node = std::move(RightRotate(node, node->GetLeftRef()));
                } else if (isRight && !node->GetRight()->IsEmpty() &&
                           !compareIntervals(IntervalTreeNode<KeyType>{low, high}, *node->GetRight())) {
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

    template <typename KeyType, typename Compare>
    inline std::unique_ptr< IntervalTreeNode<KeyType>>& FindPredecessorForDelete(std::unique_ptr< IntervalTreeNode<KeyType>>& node) {
        assert(node->GetLeft() && !node->GetLeft()->IsEmpty());
        auto prevNode = node.get();
        auto currentNode = node->GetLeft();
        auto nextNode = currentNode->GetRight();
        while (!nextNode->IsEmpty()) {
            prevNode = currentNode;
            currentNode = nextNode;
            nextNode = nextNode->GetRight();
        }
        return prevNode->GetLeft() == currentNode ? prevNode->GetLeftRef() : prevNode->GetRightRef();
    }

    template <typename KeyType, typename Compare>
    std::pair<std::unique_ptr<IntervalTreeNode<KeyType>>, bool> IntervalTree<KeyType,Compare>::DeleteHelper(
            std::unique_ptr<IntervalTreeNode<KeyType>> node, const KeyType &low, const KeyType &high) {
        if(node->IsEmpty()){
            return {std::make_unique< IntervalTreeNode<KeyType>>(), false};
        }
        Compare compare;
        CompareIntervals compareIntervals;
        bool isLeft = false;
        bool isRight = false;
        bool isSecondLeft = false;
        bool isSecondRight = false;
        bool isFound = false;


        if(compareIntervals(IntervalTreeNode<KeyType>{low, high}, *node)){
            auto [leftNode, isFoundLeft] = DeleteHelper(std::move(node->GetLeftValue()), low, high);
            node->SetLeft(std::move(leftNode));
            isFound = isFoundLeft;
        }
        else if(compareIntervals(*node, IntervalTreeNode<KeyType>{low, high})){
            auto [rightNode, isFoundRight] = DeleteHelper(std::move(node->GetRightValue()), low, high);
            node->SetRight(std::move(rightNode));
            isFound = isFoundRight;
        }
            //keys are equal
        else {
            if (!node->GetLeft()->IsEmpty() && !node->GetRight()->IsEmpty()) {
                std::unique_ptr< IntervalTreeNode<KeyType>> &predecessor = FindPredecessorForDelete<KeyType,Compare>(node);
                node->SwapKeys(*predecessor);
                auto [leftNode, isFoundLeft] = DeleteHelper(std::move(node->GetLeftValue()), low, high);
                node->SetLeft(std::move(leftNode));
                isFound = isFoundLeft;
            } else {
                if (node->GetLeft()->IsEmpty() && node->GetRight()->IsEmpty()) {
                    node = std::move(std::make_unique< IntervalTreeNode<KeyType>>());
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
        node->SetMax(GetMaximum(*node, *node->GetLeft(), *node->GetRight()));

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
            else if(isLeft){
                node->SetLeft(LeftRotate(node->GetLeftRef(), node->GetLeftRef()->GetRightRef()));
                node = RightRotate(node, node->GetLeftRef());
            }

                //isRight && isSecondRight
            else if(isSecondRight){
                node = LeftRotate(node, node->GetRightRef());
            }

            //isRight && isSecondLeft
            else{
                node->SetRight(RightRotate(node->GetRightRef(), node->GetRightRef()->GetLeftRef()));
                node = LeftRotate(node, node->GetRightRef());
            }

        }
        return {std::move(node), isFound};
        
    }

    template <typename KeyType, typename Compare>
    std::unique_ptr< IntervalTreeNode<KeyType>> IntervalTree<KeyType,Compare>::RightRotate(std::unique_ptr< IntervalTreeNode<KeyType>>& grandParent, std::unique_ptr< IntervalTreeNode<KeyType>>& parent){
        grandParent->SetSize(grandParent->GetSize() - parent->GetSize() + parent->GetRight()->GetSize());
        parent->SetSize(parent->GetSize() + 1 + grandParent->GetRight()->GetSize());
        std::unique_ptr< IntervalTreeNode<KeyType>> parentRightCopy = std::move(parent->GetRightValue());
        std::unique_ptr< IntervalTreeNode<KeyType>> parentCopy = std::move(parent);
        parentCopy->SetRight(std::move(grandParent));
        parentCopy->GetRight()->SetLeft(std::move(parentRightCopy));
        parentCopy->GetRight()->SetDepth(1 + std::max(parentCopy->GetRight()->GetLeft()->GetDepth(),
                                                      parentCopy->GetRight()->GetRight()->GetDepth()));
        parentCopy->GetRight()->SetMax(GetMaximum(*parentCopy->GetRight(), *parentCopy->GetRight()->GetLeft(), *parentCopy->GetRight()->GetRight()));
        parentCopy->SetDepth(1 + std::max(parentCopy->GetLeft()->GetDepth(), parentCopy->GetRight()->GetDepth()));
        parentCopy->SetMax(GetMaximum(*parentCopy, *parentCopy->GetLeft(), *parentCopy->GetRight()));
        return parentCopy;
    }

    template <typename KeyType, typename Compare>
    std::unique_ptr< IntervalTreeNode<KeyType>> IntervalTree<KeyType,Compare>::LeftRotate(
            std::unique_ptr<IntervalTreeNode<KeyType>> &grandParent,
            std::unique_ptr<IntervalTreeNode<KeyType>> &parent) {

        grandParent->SetSize(grandParent->GetSize() - parent->GetSize() + parent->GetLeft()->GetSize());
        parent->SetSize(parent->GetSize() + 1 + grandParent->GetLeft()->GetSize());
        std::unique_ptr< IntervalTreeNode<KeyType>> parentLeftCopy = std::move(parent->GetLeftValue());
        std::unique_ptr< IntervalTreeNode<KeyType>> parentCopy = std::move(parent);
        parentCopy->SetLeft(std::move(grandParent));
        parentCopy->GetLeft()->SetRight(std::move(parentLeftCopy));
        
        parentCopy->GetLeft()->SetDepth(1 + std::max(parentCopy->GetLeft()->GetRight()->GetDepth(),
                                                     parentCopy->GetLeft()->GetLeft()->GetDepth()));
        parentCopy->GetLeft()->SetMax(GetMaximum(*parentCopy->GetLeft(), *parentCopy->GetLeft()->GetLeft(), *parentCopy->GetLeft()->GetRight()));
        
        parentCopy->SetDepth(1 + std::max(parentCopy->GetLeft()->GetDepth(), parentCopy->GetRight()->GetDepth()));
        parentCopy->SetMax(GetMaximum(*parentCopy, *parentCopy->GetLeft(), *parentCopy->GetRight()));
        return parentCopy;
        
    }

    template <typename KeyType, typename Compare>
    const KeyType& IntervalTree<KeyType,Compare>::GetMaximum(const IntervalTreeNode<KeyType>& currentNode, const IntervalTreeNode<KeyType>& leftNode,
                                                             const IntervalTreeNode<KeyType>& rightNode){
        Compare compare;
        if(currentNode.IsEmpty()){
            return currentNode.GetHigh();
        }
        if(leftNode.IsEmpty() && rightNode.IsEmpty()){
            return currentNode.GetHigh();
        }
        if(leftNode.IsEmpty() || compare(leftNode.GetHigh(), rightNode.GetHigh())){
            if(compare(currentNode.GetHigh(), rightNode.GetMax()))
                return rightNode.GetMax();
            return currentNode.GetHigh();
        }
        if(rightNode.IsEmpty() || compare(rightNode.GetHigh(), leftNode.GetHigh())){
            if(compare(currentNode.GetHigh(), leftNode.GetMax())){
                return leftNode.GetMax();
            }
            return currentNode.GetHigh();
        }
        return currentNode.GetHigh();
    }

    template <typename KeyType, typename Compare>
    std::optional<std::pair<KeyType,KeyType>>  IntervalTree<KeyType,Compare>::FindAnOverlappingInterval(KeyType low, KeyType high){



        IntervalTreeNode<KeyType>* currentNode = root_.get();

        if(currentNode->GetMax() < low) return {};

        Compare compare;

        while(!currentNode->IsEmpty() && (compare(high, currentNode->GetLow()) || compare(currentNode->GetHigh(), low))){
            if(!currentNode->GetLeft()->IsEmpty() &&  !compare(currentNode->GetLeft()->GetMax(), low)){
                currentNode = currentNode->GetLeft();
            }
            else{
                currentNode = currentNode->GetRight();
            }
        }

        return currentNode->IsEmpty() ? std::optional<std::pair<KeyType,KeyType>>{} : std::make_pair<>(currentNode->GetLow(), currentNode->GetHigh());
    }

    enum class currentSubTreeVisited{
            NOTHING,
            LEFT_SUBTREE,
            RIGHT_SUBTREE};

    template <typename KeyType, typename Compare>
    std::vector<std::pair<KeyType, KeyType>> IntervalTree<KeyType,Compare>::FindAllOverlappingIntervals(KeyType low, KeyType high){

        std::stack<std::tuple<IntervalTreeNode<KeyType>*,currentSubTreeVisited, bool>> stack;

        if(!root_->IsEmpty())
            stack.push({root_.get(), currentSubTreeVisited::NOTHING, false});

        std::vector<std::pair<KeyType, KeyType>> result;

        Compare compare;




        while(!stack.empty()) {
            IntervalTreeNode<KeyType> *stackTop;
            currentSubTreeVisited visitStatus;
            bool anyOverlapFoundInLeftSubTree;
            std::tie(stackTop, visitStatus, anyOverlapFoundInLeftSubTree) = stack.top();
            stack.pop();
            if (visitStatus == currentSubTreeVisited::NOTHING) {
                if (!(compare(high, stackTop->GetLow()) || compare(stackTop->GetHigh(),low))) {
                    result.emplace_back(stackTop->GetLow(), stackTop->GetHigh());
                    if(!stack.empty()) {
                        auto &parent = stack.top();
                        std::get<2>(parent) = true;
                    }
                }
                stack.push({stackTop, currentSubTreeVisited::LEFT_SUBTREE, anyOverlapFoundInLeftSubTree});
                if (!stackTop->GetLeft()->IsEmpty() && !compare(stackTop->GetLeft()->GetMax(), low)) {
                    stack.push({stackTop->GetLeft(), currentSubTreeVisited::NOTHING, false});
                }
            } else if (visitStatus == currentSubTreeVisited::LEFT_SUBTREE) {
                stack.push({stackTop, currentSubTreeVisited::RIGHT_SUBTREE, anyOverlapFoundInLeftSubTree});
                if(!stackTop->GetLeft()->IsEmpty() &&  !compare(stackTop->GetLeft()->GetMax(), low)) {
                    if (!stackTop->GetRight()->IsEmpty() && anyOverlapFoundInLeftSubTree &&
                            !compare(stackTop->GetRight()->GetMax(), low) &&  compare(stackTop->GetLow(), high)) {
                        stack.push({stackTop->GetRight(), currentSubTreeVisited::NOTHING, false});
                    }
                }
                else if(!stackTop->GetRight()->IsEmpty()){
                    stack.push({stackTop->GetRight(), currentSubTreeVisited::NOTHING, false});
                }
            } else {
                //pass
            }
        }

        return result;
    }

    template <typename KeyType, typename Compare>
    class IntervalTree<KeyType,Compare>::Iterator{
    private:
        std::stack<IntervalTreeNode<KeyType>*> stack_;
        bool PushAllLeftNodes(IntervalTreeNode<KeyType>* currentNode){
            bool isInserted = false;
            while(currentNode && !currentNode->IsEmpty()){
                stack_.push(currentNode);
                currentNode = currentNode->GetLeft();
                isInserted = true;
            }
            return isInserted;
        }

        void GoForward(){
            if(stack_.empty()) return;
            auto currentNode = stack_.top();
            stack_.pop();
            PushAllLeftNodes(currentNode->GetRight());
        }
    public:
        explicit Iterator(IntervalTreeNode<KeyType>* rootNode){
            PushAllLeftNodes(rootNode);
        }
        explicit Iterator() = default;


        [[nodiscard]] bool IsEnd() const{
            return stack_.empty();
        }

        Iterator & operator++(){
            if(!IsEnd()) {
                GoForward();
            }
            return *this;
        }


        bool operator==(const Iterator& other) const{
            return other.stack_ == stack_;
        }

        std::pair<const KeyType&,const KeyType&> operator*(){
            return {stack_.top()->GetLow(), stack_.top()->GetHigh()};
        }

    };

    template <typename KeyType, typename Compare>
    class IntervalTree<KeyType,Compare>::ReverseIterator{
    private:
        std::stack<IntervalTreeNode<KeyType>*> stack_;

        bool PushAllRightNodes(IntervalTreeNode<KeyType>* currentNode){
            bool isInserted = false;
            while(currentNode && !currentNode->IsEmpty()){
                stack_.push(currentNode);
                currentNode = currentNode->GetRight();
                isInserted = true;
            }
            return isInserted;
        }

        void GoBackward(){
            if(stack_.empty()) return;
            auto currentNode = stack_.top();
            stack_.pop();
            PushAllRightNodes(currentNode->GetLeft());
        }

    public:
        explicit ReverseIterator(IntervalTreeNode<KeyType>* rootNode){
            PushAllRightNodes(rootNode);
        }
        explicit ReverseIterator() = default;


        [[nodiscard]] bool IsEnd() const{
            return stack_.empty();
        }

        ReverseIterator & operator++(){
            if(!IsEnd()) {
               GoBackward();
            }
            return *this;
        }


        bool operator==(const ReverseIterator& other) const{
            return other.stack_ == stack_;
        }

        std::pair<const KeyType&,const KeyType&> operator*(){
            return {stack_.top()->GetLow(), stack_.top()->GetHigh()};
        }

    };

}





#endif //AUGMENTEDAVLTREE_INTERVALTREE_H
