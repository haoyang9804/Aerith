#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <string>

struct Config {  
private:
  Config() = default;
public:
  std::string techName = "Arthur";
  /*
    Seems that clang::tooling::runToolOnCodeWithArgs cannot header files
    if we build llvm from source or install pre-compiled llvm by brew.
    So we need to configure it here.
  */ 
  std::string includePath = "/opt/homebrew/Cellar/llvm/16.0.6/lib/clang/16/include";
  std::string cppStandard = "-std=c++20";
  int conceptID = 0;
  Config(Config&&) noexcept = delete;
  Config(const Config&) = delete;

  Config& operator=(const Config&) = delete;
  Config& operator=(Config&&) noexcept = delete;

  static Config& getInstance() {
    static Config config = Config();
    return config;
  }  

};


#endif