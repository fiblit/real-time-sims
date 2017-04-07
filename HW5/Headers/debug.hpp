#ifndef DEBUG_H_GUARD
#define DEBUG_H_GUARD
#pragma once

#ifdef DEBUG
#define D(x) x

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
void OK();
void slowPrint(int middle, int after, std::string txt); // just for fun
#else
#define D(x)
#endif // DEBUG

#endif // DEBUG_H_GUARD
