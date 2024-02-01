//
// Created by Abhijith  K A on 28/01/24.
//

#include "IntervalTree.h"
#include <array>
#include <random>
#include <iostream>

int main(){
    using namespace DS::IntervalTree;

    IntervalTree<int> intervalTree;

    std::array<int, 8> elemsOneLow = {1,11,21,31, 81,71,61,51};
    std::array<int, 8> elemsOneHigh = {5,15,25,35,85,75,65,55};

    for(auto i = 0U; i < elemsOneLow.size(); ++i){
        assert(intervalTree.Insert(elemsOneLow[i], elemsOneHigh[i]));
    }

    for(auto i = 0U; i < elemsOneLow.size(); ++i){
        assert(intervalTree.IsPresent(elemsOneLow[i], elemsOneHigh[i]));
    }


    // enclosing overlaps
    for(auto i = 0U; i < elemsOneLow.size(); ++i){
        int low = elemsOneLow[i] + 1;
        int high = elemsOneHigh[i] - 1;
        auto res = intervalTree.FindAnOverlappingInterval(low, high);
        assert(res.has_value() && res->first == elemsOneLow[i] && res->second ==  elemsOneHigh[i]);
    }

    //no overlaps
    for(auto i = 0U; i < elemsOneLow.size(); ++i){
        int low = elemsOneLow[i] - 2;
        int high = elemsOneLow[i] - 1;
        auto res = intervalTree.FindAnOverlappingInterval(low, high);
        assert(!res.has_value());
    }

    // left overlaps
    for(auto i = 0U; i < elemsOneLow.size(); ++i){
        int low = elemsOneLow[i] - 2;
        int high = elemsOneHigh[i] - 1;
        auto res = intervalTree.FindAnOverlappingInterval(low, high);
        assert(res.has_value() && res->first == elemsOneLow[i] && res->second ==  elemsOneHigh[i]);
    }

    // right overlaps
    for(auto i = 0U; i < elemsOneLow.size(); ++i){
        int low = elemsOneHigh[i] - 2;
        int high = elemsOneHigh[i] + 2;
        auto res = intervalTree.FindAnOverlappingInterval(low, high);
        assert(res.has_value() && res->first == elemsOneLow[i] && res->second ==  elemsOneHigh[i]);
    }
    // full overlap

    auto res = intervalTree.FindAnOverlappingInterval(elemsOneLow.front() - 1, elemsOneHigh.back() + 1);
    assert(res.has_value());

    auto resultList = intervalTree.FindAllOverlappingIntervals(elemsOneLow.front() - 1, elemsOneHigh.back() + 1);
    assert(resultList.size() == 5);

    auto resultList2 = intervalTree.FindAllOverlappingIntervals(elemsOneLow.front() - 1, *std::max_element(elemsOneLow.begin(), elemsOneLow.end()) + 1);
    assert(resultList2.size() == elemsOneLow.size());

    auto resultList3 = intervalTree.FindAllOverlappingIntervals(70, 85);
    assert(resultList3.size() == 2);



    for(IntervalTree<int>::Iterator it = intervalTree.cBegin(); it != intervalTree.cEnd(); ++it){
        int low = (*it).first;
        int high = (*it).second;
        std::cout<<"low : "<<low<<" high : "<<high<<"\n";
    }



    std::random_device rd;
    std::mt19937 g(rd());

    std::array<int, 8> indices {0,1,2,3,4,5,6,7};
    std::shuffle(indices.begin(), indices.end(), rd);

    auto indexCopy = indices;

    auto elemsOneLowCopy = elemsOneLow;

    {
        int currentIndex = 0;
        while(currentIndex < indices.size()){
            while(currentIndex != indices[currentIndex]) {


                int TargetIndex = indices[indices[currentIndex]];
                int TargetElem = elemsOneLow[indices[currentIndex]];
                int TargetElem2 = elemsOneHigh[indices[currentIndex]];

                elemsOneLow[indices[currentIndex]] = elemsOneLow[currentIndex];
                elemsOneHigh[indices[currentIndex]] = elemsOneHigh[currentIndex];
                indices[indices[currentIndex]] = currentIndex;


                indices[currentIndex] = TargetIndex;
                elemsOneLow[currentIndex] = TargetElem;
                elemsOneHigh[currentIndex] = TargetElem2;
            }
            ++currentIndex;
        }
    }

    for(auto i = 0U; i < elemsOneLow.size(); ++i){
        assert(intervalTree.IsPresent(elemsOneLow[i], elemsOneHigh[i]));
        assert(intervalTree.Delete(elemsOneLow[i], elemsOneHigh[i]));
        assert(!intervalTree.IsPresent(elemsOneLow[i], elemsOneHigh[i]));
    }

    assert(intervalTree.IsEmpty());


    {

        IntervalTree<int> intervalTree2;

        // [10,20]

        intervalTree2.Insert(10,20);
        intervalTree2.Insert(10,15);
        intervalTree2.Insert(12,17);
        intervalTree2.Insert(7,15);
        intervalTree2.Insert(18,24);
        intervalTree2.Insert(1,4);
        intervalTree2.Insert(27,30);

        intervalTree2.Insert(21,25);

        intervalTree2.Insert(28,30);

        auto result = intervalTree2.FindAllOverlappingIntervals(10,20);
        int z = 0;


        for(IntervalTree<int>::Iterator it = intervalTree2.cBegin(); it != intervalTree2.cEnd(); ++it){
            int low = (*it).first;
            int high = (*it).second;
            std::cout<<"low : "<<low<<" high : "<<high<<"\n";
            std::cout<<"end\n";
        }

        for(IntervalTree<int>::ReverseIterator rt = intervalTree2.rcBegin(); rt != intervalTree2.rcEnd(); ++rt){
            int low = (*rt).first;
            int high = (*rt).second;
            std::cout<<"low : "<<low<<" high : "<<high<<"\n";
        }



    }


return 0;


};