#include <cassert>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <string_view>
#include <iostream>
#include <chrono>
#include <thread>

namespace chrono = std::chrono;
namespace fs = std::filesystem;
using sys_time = chrono::time_point<chrono::system_clock>;

const int refresh_rate = 200;

void print_help(const std::string_view sv);
sys_time get_latest_timepoint_from_dir(
  const fs::path p, 
  const sys_time curr_time);

int main(int argc, char* argv[]) {
  std::string_view sv (argv[0]);
  if (argc == 1) {
    print_help(sv);
  }

  for (int i = 1; i < argc; ++i) {
    std::string_view sv_arg { argv[i] };
    if (sv_arg == "help") {
      print_help(sv);

      return 0;
    }
  }

  assert(argc == 3);

  fs::path working_dir { argv[1] };

  if (!fs::exists(working_dir)) {
    std::cerr << "Provided path " << working_dir << " does not exist!\n";
    return 1;
  }

  bool is_directory = fs::is_directory(working_dir);
  if (!is_directory && !fs::is_regular_file(working_dir)) {
    std::cerr << "Provided path " << working_dir << " is pointing to a unsupported target\n";
    return 1;
  }

  auto timestamp = chrono::system_clock::now();
  bool execute_cmd = false;
  while (true) {
    
    if (!is_directory) {
      auto ftime = chrono::file_clock::to_sys(fs::last_write_time(working_dir));
      if (timestamp < ftime) {
        execute_cmd = true;
      }
    } else {
        
    }

    if (execute_cmd) {
      system(argv[2]);
      execute_cmd = false;
    }

    std::this_thread::sleep_for(chrono::milliseconds(refresh_rate));  
  }

  return 0;
}

void print_help(const std::string_view program_name) {
  std::cout << "To execute " << program_name << " correctly you need to provide arguments. \nUsage: '" << program_name << " {directory} {command}'\n";
}

sys_time get_latest_timepoint_from_dir(
  const fs::path p, 
  const sys_time curr_time) {

  sys_time last { curr_time };
  
  for (auto entry : fs::directory_iterator{p}) {

    if (fs::is_regular_file(entry)) {
      auto file_time = chrono::file_clock::to_sys(fs::last_write_time(entry));
      if (file_time > last) {
        last = file_time;
      }
    }
    else if (fs::is_directory(entry)) {
      auto dir_time = get_latest_timepoint_from_dir(entry, curr_time);
      if (dir_time > last) {
        last = dir_time;
      }
    }
  }

  return last;
}
