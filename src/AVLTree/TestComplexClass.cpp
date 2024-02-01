//
// Created by Abhijith  K A on 28/01/24.
//


#include "AVLTree.h"

#include "TestClass.h"





int main(){

    AVLTree<TestClass,CompareTestClass> avlTree;
    assert(avlTree.IsEmpty());
    assert(avlTree.Insert(TestClass{1}));
    avlTree.Delete(TestClass{1});

}