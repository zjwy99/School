#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <stdexcept>
#include <sstream>
#include <string>

struct illegal_request: public std::runtime_error {
  illegal_request(const std::string& msg):
    std::runtime_error(msg)
  { }
};

struct bad_asm: public std::runtime_error {
  const int line;

  bad_asm(int l, const std::string& msg):
    std::runtime_error(msg),
    line(l)
  { }
};

struct bad_label: public std::runtime_error {
  const int line;

  bad_label(int l, const std::string& msg):
    std::runtime_error(msg),
    line(l)
  { }
};

// Helper because we need to be able to complain from multiple parts of the code.
inline void throw_bad_label(int line, const std::string& msg = "") {
  throw bad_label(line, msg);
}

// Helper because we need to be able to complain from multiple parts of the code.
inline void throw_bad_asm(int line, const std::string& msg = "") {
  throw bad_asm(line, msg);
}

#endif // EXCEPTIONS_H_
