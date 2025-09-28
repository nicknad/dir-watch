#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <string_view>
#include <iostream>
#include <chrono>
#include <thread>

const int refresh_rate = 200;

void print_help(const std::string_view sv);

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

  std::filesystem::path working_dir { argv[1] };

  if (!std::filesystem::exists(working_dir)) {
    std::cerr << "Provided path " << working_dir << " does not exist!\n";
    return 1;
  }
  
  auto timestamp = std::chrono::system_clock::now();
  while (true) {
    system(argv[2]);
    std::this_thread::sleep_for(std::chrono::milliseconds(refresh_rate));  
  }

  return 0;
}

void print_help(const std::string_view program_name) {
  std::cout << "To execute " << program_name << " correctly you need to provide arguments. \nUsage: '" << program_name << " {directory} {command}'\n";
}
