/*
* Open Source Software to apply Statistical Disclosure Control techniques
* 
* This program is free software; you can redistribute it and/or 
* modify it under the terms of the European Union Public Licence 
* (EUPL) version 1.1, as published by the European Commission.
* 
* You can find the text of the EUPL v1.1 on
* https://joinup.ec.europa.eu/software/page/eupl/licence-eupl
* 
* This software is distributed on an "AS IS" basis without 
* warranties or conditions of any kind, either express or implied.
*/

/* 
* Header file for shared library recordSwap.dll
* with source code recordSwap.cpp
* to perform Targeted Record Swapping
*/
#include <iostream>     
#include <algorithm>    // std::count
#include <vector>       // std::vector
#include <random>
#include <queue>
#include <array>
#include <map>
#include <unordered_set>
#include <unordered_map>

#ifndef __RECORDSWAP_H__
#define	__RECORDSWAP_H__

//class CrecordSwap
//{
//public:
/*
* Function to perform record swapping
*/
std::vector<std::vector<int> > recordSwap(std::vector< std::vector<int> > data, std::vector<int> similar,
                                          std::vector<int> hierarchy, std::vector<int> risk, int hid, int th, double swaprate, int seed);
//private:
/*
* Function to reorder data-set given one index vector 
*/
std::vector< std::vector<int> > orderData(std::vector< std::vector<int> > &data, int orderIndex);

/*
* Function to define levels 
*/
std::vector<int> setLevels(std::vector< std::vector<int> > data, std::vector<int> hierarchy, std::vector<int> risk, int hid, int th);

/*
* Function to set sampling probability 
* and reverse sampling probability (for donor sets)
*/
std::vector< std::vector<double> > setRisk(std::vector<std::vector<int> > data, std::vector<int> hierarchy, std::vector<int> risk, int hid);

/*
* test random sampling and seed
* used to check of state of RNG changes successfully with function calls
*/
std::vector<std::vector<int> > test_randSample(int B,std::vector<int> ID, int N, std::vector<double> prob,int seed);

/*
* Function to sample from std::vector<int> given a probability vector
*/
std::unordered_set<int> randSample(std::unordered_set<int> &ID, int N, std::vector<double> &prob, std::mt19937 &mersenne_engine,
                                   std::vector<int> &IDused, std::unordered_set<int> &mustSwap);

/*
* Testfunction for performance
*/
int test(std::vector< std::vector<int> > data, std::vector<int> loop_index);

std::vector<int> test_stuff(std::vector<int> vec1);
//};

#endif /* __RECORDSWAP_H__ */