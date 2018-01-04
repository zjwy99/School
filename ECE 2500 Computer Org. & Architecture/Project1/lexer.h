#ifndef LEXER_H_
#define LEXER_H_

#include <string>
#include <vector>

namespace lexer {
  // Represents a token, which is a single parsed field [i.e. string, int, etc.].
  struct token {
    enum token_type {
      Integer,
      String
    };

    token_type type;

    explicit token(int val);
    explicit token(std::string val);

    int integer() const;

    const std::string& string() const;
    std::string& string();

    std::string to_string() const;

  private:
   struct {
      std::string string;
      int integer;
    } _value;
  };

  struct instruction {
    std::vector<std::string> labels;
    std::string name;
    std::vector<token> args;
    const int line;

    instruction(int line_):
      line(line_)
    {}

    instruction(const std::vector<std::string>& labels_,
                const std::string& name_,
                const std::vector<token>& args_,
                int line_):
      labels(labels_),
      name(name_),
      args(args_),
      line(line_)
    { }

    instruction (const instruction& i):
      labels(i.labels),
      name(i.name),
      args(i.args),
      line(i.line)
    { }

    instruction(instruction&& i):
      labels(std::move(i.labels)),
      name(std::move(i.name)),
      args(std::move(i.args)),
      line(i.line)
    { }

    std::string to_string(bool lineNumber = false) const;
  };

  bool operator==(const instruction& a, const instruction& b);
  inline bool operator!=(const instruction& a, const instruction b) {
    return !(a == b);
  }

  bool operator==(const token& a, const token& b);
  inline bool operator!=(const token& a, const token& b) {
    return !(a == b);
  }

  std::vector<instruction> analyze(const std::string& str);
}

#endif // LEXER_H_
