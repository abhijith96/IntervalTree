//
// Created by Abhijith  K A on 28/01/24.
//

#ifndef AUGMENTEDAVLTREE_INTERVALTREENODE_H
#define AUGMENTEDAVLTREE_INTERVALTREENODE_H

#include "Common.h"
#include <memory>

namespace DS::IntervalTree {
   template <typename KeyType>
    class IntervalTreeNode {
    private:
        KeyType low_;
        KeyType high_;
        KeyType max_high_in_subtree_;
        size_type size_;
        size_type depth_;
        std::unique_ptr<IntervalTreeNode> left_;
        std::unique_ptr<IntervalTreeNode> right_;

    public:
        explicit IntervalTreeNode():low_(), high_(), max_high_in_subtree_(),size_(0), depth_(0){}
        explicit IntervalTreeNode(const KeyType& low, const KeyType& high) : low_(low), high_(high), max_high_in_subtree_(high_),
        size_(1), depth_(1), left_(std::make_unique<IntervalTreeNode<KeyType>>()), right_(std::make_unique<IntervalTreeNode<KeyType>>()) {
            assert(high >= low);
        }
        [[nodiscard]]  bool IsEmpty() const;

        [[nodiscard]] size_type  GetSize() const;

        void SetSize(size_type size);

        [[nodiscard]] size_type  GetDepth() const;

        void SetDepth(size_type depth);

        const KeyType& GetLow() const;
        const KeyType& GetHigh() const;

        IntervalTreeNode* GetLeft();
        IntervalTreeNode* GetRight();

        void SwapKeys(IntervalTreeNode& other);

        std::unique_ptr<IntervalTreeNode>& GetLeftRef();

        std::unique_ptr<IntervalTreeNode>& GetRightRef();

        std::unique_ptr<IntervalTreeNode> GetLeftValue();

        std::unique_ptr<IntervalTreeNode> GetRightValue();

        void SetLeft(std::unique_ptr<IntervalTreeNode> && left);

        void SetRight(std::unique_ptr<IntervalTreeNode> && right);

        void SetMax(const KeyType& max);

        const KeyType& GetMax() const;


    };

   template <typename KeyType>
    bool IntervalTreeNode<KeyType>::IsEmpty() const {
        return size_ == 0;
    }

   template <typename KeyType>
    size_type IntervalTreeNode<KeyType>::GetSize() const {
        return size_;
    }

   template <typename KeyType>
    void IntervalTreeNode<KeyType>::SetSize(size_type size){
        size_ = size;
    }

   template <typename KeyType>
    size_type  IntervalTreeNode<KeyType>::GetDepth() const{
        return depth_;
    }

   template <typename KeyType>
    void  IntervalTreeNode<KeyType>::SetDepth(size_type depth){
        depth_ = depth;
    }

   template <typename KeyType>
    const KeyType&  IntervalTreeNode<KeyType>::GetLow() const{
        return low_;
    }
   template <typename KeyType>
    const KeyType&  IntervalTreeNode<KeyType>::GetHigh() const{
        return high_;
    }

   template <typename KeyType>
    IntervalTreeNode<KeyType>* IntervalTreeNode<KeyType>::GetLeft(){
        return left_.get();
    }
   template <typename KeyType>
    IntervalTreeNode<KeyType>* IntervalTreeNode<KeyType>::GetRight(){
        return right_.get();
    }

   template <typename KeyType>
    void IntervalTreeNode<KeyType>::SwapKeys(IntervalTreeNode<KeyType>& other){
        std::swap(low_, other.low_);
        std::swap(high_, other.high_);
    }


   template <typename KeyType>
    std::unique_ptr<IntervalTreeNode<KeyType>>&  IntervalTreeNode<KeyType>::GetLeftRef(){
        return left_;
    }

   template <typename KeyType>
    std::unique_ptr<IntervalTreeNode<KeyType>>&  IntervalTreeNode<KeyType>:: GetRightRef(){
        return right_;
    }

   template <typename KeyType>
    std::unique_ptr<IntervalTreeNode<KeyType>>  IntervalTreeNode<KeyType>:: GetLeftValue(){
        return std::move(left_);
    }

   template <typename KeyType>
    std::unique_ptr<IntervalTreeNode<KeyType>>  IntervalTreeNode<KeyType>:: GetRightValue(){
        return std::move(right_);
    }

   template <typename KeyType>
    void  IntervalTreeNode<KeyType>::SetLeft(std::unique_ptr<IntervalTreeNode> && left){
        left_ = std::move(left);
    }

   template <typename KeyType>
    void  IntervalTreeNode<KeyType>::SetRight(std::unique_ptr<IntervalTreeNode> && right){
        right_ = std::move(right);
    }

   template <typename KeyType>
    void  IntervalTreeNode<KeyType>::SetMax(const KeyType& max){
        max_high_in_subtree_ = max;
    }

   template <typename KeyType>
    const KeyType&  IntervalTreeNode<KeyType>::GetMax() const{
        return max_high_in_subtree_;
    }



}


#endif //AUGMENTEDAVLTREE_INTERVALTREENODE_H
