// Copyright 2020 Zakhar Ushakov <hvarzahar@gmail.com>

#ifndef INCLUDE_HASH_RESEARCHER_HPP_
#define INCLUDE_HASH_RESEARCHER_HPP_

#include <mutex>
#include <string>
#include <iostream>

#include <nlohmann/json.hpp>

class hash_researcher final {
 private:
  nlohmann::json data;
  std::mutex mutex_data;
  const std::string suffix;

 public:
  explicit hash_researcher(const std::string& _suffix) noexcept;
  void research(size_t count);

 private:
  [[noreturn]] void research_function();
  static auto timestamp() -> size_t;
};


#endif // INCLUDE_HASH_RESEARCHER_HPP_
