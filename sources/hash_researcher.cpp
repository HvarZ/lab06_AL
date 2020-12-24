//Copyright 2020 <hvarzahar@gmail.com>

#include <hash_researcher.hpp>

#include <algorithm>
#include <chrono>
#include <random>
#include <thread>
#include <vector>

#include <picosha2.h>

#include <boost/log/trivial.hpp>


struct Data {
  std::string data;
  std::string hash;
  size_t timestamp;

  Data(const std::string& _data,
       const std::string& _hash,
       const size_t _timestamp) noexcept
      : data(_data), hash(_hash), timestamp(_timestamp) {}
};

auto hash_researcher::get_data() const noexcept -> nlohmann::json {
  return data;
}

void to_json(nlohmann::json& j, const Data& _data) {
  j = nlohmann::json {{"timestamp", _data.timestamp},
            {"hash", _data.hash},
            {"data", _data.data}};
}


hash_researcher::hash_researcher(const std::string& _suffix) noexcept
    : data(nlohmann::json::array()), suffix(_suffix) {}

auto hash_researcher::timestamp() -> size_t {
  auto current_time = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::seconds>
        (current_time.time_since_epoch()).count();
}

auto random_string() -> std::string {
  std::mt19937 generator(std::random_device{}());
  return std::to_string(generator());
}

[[noreturn]] void hash_researcher::research_function() {
  for (;;) {
    auto rand_string = random_string();
    std::string hash_hex = picosha2::hash256_hex_string(rand_string);

    if (hash_hex.substr(hash_hex.size() - suffix.size()) == suffix) {
      BOOST_LOG_TRIVIAL(info) << "Thread id: "
                              << std::this_thread::get_id() << std::endl
                              << "Found suitable data '" << rand_string
                              << "' with hash '" << hash_hex << std::endl;
      std::lock_guard<std::mutex> lock(mutex_data);
      data.push_back(Data(hash_hex, rand_string, timestamp()));
    } else {
      BOOST_LOG_TRIVIAL(trace) << "Thread id: "
                               << std::this_thread::get_id() << std::endl
                               << "Unsuitable data '" << rand_string
                               << "' with hash '" << hash_hex << std::endl;
    }
  }
}

void hash_researcher::research(size_t count) {
  std::vector<std::thread> threads;
  threads.reserve(count);

  for (size_t i = 0; i < count; i++) {
    threads.emplace_back(&hash_researcher::research_function, this);
  }

  for (auto& thread : threads) {
    thread.join();
  }
}
