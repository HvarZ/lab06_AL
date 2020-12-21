// Copyright 2020 Your Name <your_email>

#include <hash_researcher.hpp>
#include <iostream>
#include <thread>


int main() {
  std::cout << std::thread::hardware_concurrency() << std::endl;
}