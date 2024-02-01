Introduction
This repository contains a C++ implementation of an Interval Tree data structure based on the description provided in the textbook "Introduction to Algorithms" by Cormen, Leiserson, Rivest, and Stein. The implementation utilizes unique pointers to ensure memory safety and prevent memory leaks.
An Interval has [Low, High] as key. Low and High can be any type, for custom types a Compare struct should be passed as template argument.
Two Intervals I1 and I2 are equal only if I1.Low == I2.Low and I1.High == I2.High.

Interval Tree Overview
An Interval Tree is a tree data structure used for efficient retrieval of all intervals that overlap with any given interval or point. It is particularly useful in scheduling, database systems, and other applications where interval queries are common.

Features
Efficient Interval Querying: Allows fast retrieval of intervals overlapping with a given interval or point.
Balanced Structure: The tree built over a balanced AVL Tree.
Memory Safety: Utilizes C++ unique pointers to prevent memory leaks and manage memory efficiently.
Modular Design: The implementation is designed to be modular, allowing for easy integration into other projects and applications.

Time Complexity for an Interval Tree with Size N

Insert O(log(N))
Delete O(log(N))
Find O(log(N))
FindAnOverlappingInterval O(log(N))
FindAllOverlappingIntervals Intervals O(k*log(N)) (k being the number of overlapping intervals)

Only insert and delete are modifying operations.
All other operations are thread safe.