// Copyright 2020 Zakhar Ushakov <hvarzahar@gmail.com>

#include <hash_researcher.hpp>

#include <csignal>
#include <thread>
#include <fstream>

#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>

hash_researcher res("0000");

void create_json() {
  std::ofstream out;
  out.open("result.json");
  out << res.get_data();
}

void signal_handler(int) {
  std::cout << std::endl << "Received interrupt signal, aborting" << std::endl;
  create_json();
  std::cout << "All information saves in file ..." << std::endl;
  exit(0);
}

void init() {
  auto sinkFile = boost::log::add_file_log(
      boost::log::keywords::file_name = "research_%5N.log",
      boost::log::keywords::rotation_size = 5 * 1024 * 1024,
      boost::log::keywords::format = "[%TimeStamp%]: %Message%");
  sinkFile->set_filter(
      boost::log::trivial::severity >= boost::log::trivial::trace);
  auto sinkConsole = boost::log::add_console_log(
      std::cout,
      boost::log::keywords::format = "[%TimeStamp%]: %Message%");
  sinkConsole->set_filter(
      boost::log::trivial::severity >= boost::log::trivial::info);
  boost::log::add_common_attributes();
  // signal handling
  signal(SIGINT, signal_handler);
}


int main(int argc, char* argv[]) {
  long count_thread;

  init();

  if (argc <= 2) {
    count_thread = std::thread::hardware_concurrency();
  } else {
    count_thread = std::atol(argv[1]);
  }

  res.research(count_thread);

  return 0;
}
