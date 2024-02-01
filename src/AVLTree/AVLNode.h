//
// Created by Abhijith  K A on 27/01/24.
//

#ifndef AUGMENTEDAVLTREE_AVLNODE_H
#define AUGMENTEDAVLTREE_AVLNODE_H

#include <memory>
#include <optional>

using size_type = int;

template <typename T>
class AVLNode {
private:
    T key_;
    size_type size_;
    size_type depth_;
    std::unique_ptr<AVLNode> left_;
    std::unique_ptr<AVLNode> right_;
public:
    explicit AVLNode(): key_(), size_(0), depth_(0){}
    explicit AVLNode(T key): key_(key), size_(1), depth_(1), left_(std::make_unique<AVLNode<T>>()), right_(std::make_unique<AVLNode>()){
    }
    [[nodiscard]] const T& GetKey() const;

    void SetSize(size_type);

    [[nodiscard]] size_type GetSize() const;

    [[nodiscard]] AVLNode* GetLeft() const;



    [[nodiscard]] AVLNode* GetRight() const;

    std::unique_ptr<AVLNode<T>> GetLeftValue();

    std::unique_ptr<AVLNode<T>> GetRightValue();

    void SwapKeys(AVLNode& other){
        std::swap(key_, other.key_);
    }
    std::unique_ptr<AVLNode>& GetLeftRef(){
        return left_;
    }

    std::unique_ptr<AVLNode>& GetRightRef(){
        return right_;
    }

    [[nodiscard]] size_type GetDepth() const;

    void SetDepth(size_type depth);

    void SetLeft(std::unique_ptr<AVLNode<T>>&& left);

    void SetRight(std::unique_ptr<AVLNode<T>>&& right);

    [[nodiscard]]  bool IsEmpty() const;




};


#endif //AUGMENTEDAVLTREE_AVLNODE_H
