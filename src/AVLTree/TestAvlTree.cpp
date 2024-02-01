#include <iostream>
#include "AVLTree.h"
#include <array>
#include <random>
#include <mutex>
#include <deque>
int main() {

    AVLTree<int> avlTree;

    std::array<int, 8> elements = {1,2,3,4,8,7,6,5};

    for(int i : elements) {
        avlTree.Insert(i);
        assert(avlTree.IsValidAvlTree(2));
    }

    for(int i : elements){
        assert(avlTree.IsPresent(i));
    }



    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(elements.begin(), elements.end(), g);

    for(int i : elements){
        assert(avlTree.IsPresent(i));
        assert(avlTree.Delete(i));
        assert(!avlTree.IsPresent(i));
        assert(avlTree.IsValidAvlTree(2));
    }



    AVLTree<int> avlTree2;

    std::array<int, 8> elements2 = {8,7,6,5,1,2,3,4};



    for(int i : elements2) {
        avlTree2.Insert(i);
        assert(avlTree2.IsValidAvlTree(2));
    }

    for(int i : elements2){
        assert(avlTree2.IsPresent(i));
    }





   std::shuffle(elements2.begin(), elements2.end(), g);

    auto iter = std::find(elements2.begin(), elements2.end(), 7);
    *iter = *elements2.begin();
    *elements2.begin() = 7;

    int index = 0;
    for(auto i : elements2){
        assert(avlTree2.FindKthElement(i - 1).has_value() && *avlTree2.FindKthElement(i - 1) == i);
        ++index;
    }


    for(int i : elements2){
        assert(avlTree2.IsPresent(i));
        assert(avlTree2.Delete(i));
        assert(!avlTree2.IsPresent(i));
        assert(avlTree2.IsValidAvlTree(2));
    }



    return 0;
}
