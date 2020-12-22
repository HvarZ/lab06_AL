// Copyright 2020 Zakhar Ushakov <hvarzahar@gmail.com>

#include <hash_researcher.hpp>

#include <thread>
#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>


void signal_handler(int) {
  std::cout << "Received interrupt signal, aborting" << std::endl;
  exit(0);
}

void init() {
  auto sink_file = boost::log::add_file_log(
      boost::log::keywords::file_name = "research_%5N.log",
      boost::log::keywords::rotation_size = 5 * 1024 * 1024,
      boost::log::keywords::format = "[%TimeStamp%]: %Message%");
  sink_file->set_filter(
      boost::log::trivial::severity >= boost::log::trivial::trace);
  auto sink_console = boost::log::add_console_log(
      std::cout,
      boost::log::keywords::format = "[%TimeStamp%]: %Message%");

  sink_console->set_filter(
      boost::log::trivial::severity >= boost::log::trivial::info);

  boost::log::add_common_attributes();

  signal(SIGINT, signal_handler);
}



int main(int argc, char* argv[]) {
  size_t count_thread;

  if (argc <= 2) {
    count_thread = std::thread::hardware_concurrency();
  } else {
    count_thread = std::atoi(argv[1]);
  }

  hash_researcher res("0000");
  res.research(count_thread);

  return 0;
}
