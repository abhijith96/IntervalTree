//
// Created by Abhijith  K A on 28/01/24.
//

#ifndef AUGMENTEDAVLTREE_TESTCLASS_H
#define AUGMENTEDAVLTREE_TESTCLASS_H

struct TestClass{
    int data;
};

struct CompareTestClass{
    bool operator()(const TestClass& second, const TestClass& first) const{
        return second.data < first.data;
    }
};

#endif //AUGMENTEDAVLTREE_TESTCLASS_H
