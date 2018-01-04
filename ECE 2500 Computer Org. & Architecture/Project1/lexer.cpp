#include "lexer.h"

#include <assert.h>
#include <cctype>
#include <sstream>
#include<iostream>

#include "exceptions.h"
#include "util.h"

const char COMMENT_DELIM = '#';
const char LABEL_DELIM = ':';
const char ARG_DELIM = ',';
const std::string HEX_LEAD = "0x";

namespace lexer {
  // Token stuff.
  token::token(std::string val):
    type(token::String)
  {
    _value.string = std::move(val);
  }

  token::token(int val):
    type(token::Integer)
  { 
    _value.integer = val;
  }

  int token::integer() const {
    if (type != token::Integer) {
      throw illegal_request("Requesting int from non-int token");
    }
    return _value.integer;
  }

  const std::string& token::string() const {
    if (type != token::String) {
      throw illegal_request("Requesting string from non-string token");
    }
    return _value.string;
  }

  std::string& token::string() {
    if (type != token::String) {
      throw illegal_request("Requesting string from non-string token");
    }
    return _value.string;
  }

  std::string token::to_string() const {
    switch (type) {
      case token::Integer: 
        {
          std::stringstream stream;
          stream << _value.integer;
          return std::move(stream.str());
        }
      case token::String:
        {
          return _value.string;
        }
      // No default case -- we want warnings upon nonexhaustive coverage.
    }
    throw std::runtime_error("Incomplete switch coverage");
  }


  // Instruction methods!
  std::string instruction::to_string(bool lineNumber) const {
    std::string result;

    if (lineNumber) {
      std::stringstream stream;
      stream << "(" << line << ") ";
      result = std::move(stream.str());
    }

    for (const auto& l : labels) {
      result += l;
      result += ": ";
    }

    result += name + " ";

    bool put_arg = false;
    for (const auto& a : args) {
      if (put_arg) {
        result += ", ";
      } 
      put_arg = true;
      result += a.to_string();
    }

    return result;
  }

  // Lexer stuff.

  static bool is_delim(char ch) {
    return isspace(ch) || ch == COMMENT_DELIM || ch == ARG_DELIM || ch == LABEL_DELIM;
  }

  static bool is_valid_token(const std::string& s) {
    if (s.empty()) return true;

    auto parenNest = 0;
    for (const auto c : s) {
      switch(c) {
        // $ is allowed because register names 
        case '$': 
          break;
        // () are allowed because 0($s1)
        case '(':
          ++parenNest;
          break;
        case ')':
          if (--parenNest < 0) return false;
          break;
        default:
          if (!isalpha(c) && !isdigit(c)) return false;
          break;
      }
    }
    return parenNest == 0;
  }

  static bool is_valid_instruction(const std::string& s) {
    for (const auto c : s) {
      if (!isalpha(c) && !isdigit(c)) {
        return false;
      }
    }
    return true;
  }

  static bool is_valid_label(const std::string& s) {
    for (const auto c : s) {
      if (!isalpha(c) && !isdigit(c) && c != '.' && c != '_') {
        return false;
      }
    }
    return true;
  }

  static token parse_token(const std::string& str, std::string* remainder = nullptr) {
    const auto setRemainder = [&] (std::stringstream* stream) {
      if (remainder == nullptr) return;
      if (stream == nullptr || (unsigned int)stream->tellg() >= str.size()) {
        *remainder = std::string();
      } else {
        auto str = stream->str();
        auto start = (unsigned int)stream->tellg();
        *remainder = str.substr(start);
      }
    };

    if (str.find(HEX_LEAD) == 0) {
      std::stringstream stream(str.substr(HEX_LEAD.size()));
      int num = 0;

      if (stream >> std::hex >> num) {
        setRemainder(&stream);
        return token(num);
      }
    }

    std::stringstream intStream(std::move(str));
    int intNum = 0;
    if (intStream >> intNum) {
      setRemainder(&intStream);
      return token(intNum);
    }

    setRemainder(nullptr);
    return token(intStream.str());
  }

  static int find_next_tokenable_counting_newlines(const std::string& str, int current, int* start, int* end, char* delim) {
    // Don't want to have to deal with null start/end/delim
    int start_;
    int end_;
    char delim_;

    if (start == nullptr) start = &start_;
    if (end == nullptr) end = &end_;
    if (delim == nullptr) delim = &delim_;

    const int strEnd = (int)str.size();
    int nlines = 0;

    
    const auto at_end = [&] () {
      return current >= strEnd;
    };

    
    const auto skip_ws = [&] () {
      for (; !at_end() && isspace(str[current]); ++current) {
        if (str[current] == '\n') {
          ++nlines;
        }
      }
    };

    const auto skip_until_newline = [&] () {
      for (; !at_end() && str[current] != '\n'; ++current);
    };

    const auto skip_until_delim = [&] () {
      for (; !at_end() && !is_delim(str[current]); ++current) {
        if (str[current] == '\n') {
          ++nlines;
        }
      }
    };

    // If we start on a delimeter, we're meant to skip it. Unless it's 
    // a comment delim. Because those are handled later.
    if (!isspace(str[current]) && is_delim(str[current]) && str[current] != COMMENT_DELIM) {
      ++current;
    }

   skip_ws();
    while (!at_end() && str[current] == COMMENT_DELIM) {
      skip_until_newline();
      skip_ws();
    }

    if (at_end()) {
      *start = strEnd;
      *end = strEnd;
      *delim = '\0';
      return nlines;
    }

    *start = current;
    skip_until_delim();
    *end = current;
    *delim = at_end() ? '\0' : str[current];

   
    return nlines;
  }

  std::vector<std::string> sanitize_and_lines(const std::string& str) {
    std::vector<std::string> result;
    std::string line;
    bool skipToNewline = false;
    bool encounteredWs = false;

    // Delims we want no whitespace on either side of.
    const char noWsDelims[] = {'(', ')', LABEL_DELIM, ARG_DELIM};
    auto isNoWsDelim = [&noWsDelims] (char c) -> bool {
      for (auto d : noWsDelims) {
        if (c == d) return true;
      }
      return false;
    };

    for (auto c : str) {
      // String of (do ... continue;) because these have a very specific order'
      // they need to be evaluated in. Some are nops.  
      // over if/else statements where else has an empty body
      if (c == '\n') {
        skipToNewline = false;
        encounteredWs = false;
        result.emplace_back(std::move(line));
        continue;
      } 
      if (skipToNewline) {
        continue;
      } 
      if (c == COMMENT_DELIM) {
        skipToNewline = true;
        continue;
      }
      if (isspace(c)) {
        encounteredWs = true;
        continue;
      }
      if (encounteredWs) {
        encounteredWs = false;
        // We don't want ws at the beginning of the line, and we don't want 
        // ( %esp or , %esp or L1: L2: addi -- so don't allow spaces after that.
        if (!line.empty() && !isNoWsDelim(c) && !isNoWsDelim(*line.rbegin())) {
          line += ' ';
        }
      }
      line += c;
    }

    if (!line.empty()) {
      result.emplace_back(std::move(line));
    }
    return std::move(result);
  }

  std::vector<instruction> analyze(const std::string& str) {
    auto lines = sanitize_and_lines(str); 
    std::vector<std::string> labels;
    std::vector<instruction> result;
    int lineno = 0;
    int lastLabelLine = 0;

    for (auto& line : lines) {
      ++lineno; // We start at line 1.
      std::size_t at = 0;

      // Grab all labels. Line will be (Label:)*(instruction( (arg,)*(arg))?)?
      std::string current;
      for (; at < line.size(); ++at) {
        if (line[at] == ARG_DELIM) {
          throw_bad_asm(lineno, "Argument delim found before instruction");
        } else if (line[at] == LABEL_DELIM) {
          if (!is_valid_label(current)) {
            throw_bad_asm(lineno, "Invalid label");
          }
          labels.emplace_back(std::move(current));
          lastLabelLine = lineno;
        } else if (isspace(line[at])) {
          // space signifies end of instruction name
          break;
        } else {
          current += line[at];
        }
      }

      if (current.empty()) {
        assert(at >= line.size());
        continue;
      }

      if (!is_valid_instruction(current)) {
        throw_bad_asm(lineno, "Invalid instruction name");
      }

      // We can have a 0-args instruction.
      result.emplace_back(instruction(lineno));
      assert(!result.empty());
      auto& instruction = *result.rbegin();
      instruction.name = std::move(current);
      instruction.labels = std::move(labels);

      // If zero args...
      if (at >= line.size()) {
        continue;
      }

      const auto push_back_token = [&] (std::string token) {
        std::string rem;
        auto result = parse_token(token, &rem);
        instruction.args.emplace_back(result);
        if (!rem.empty()) {
          if (result.type != token::Integer) {
            throw_bad_asm(lineno, "Need a comma seperating args.");
          }
          // Whether or not there are parens and the token is valid will 
          // be checked in the to_bin phase.
          instruction.args.emplace_back(parse_token(rem));
        }
      };

      assert(isspace(line[at]));
      std::string arg;
      for (++at; at < line.size(); ++at) {
        if (isspace(line[at])) {
          throw_bad_asm(lineno, "Unexpected arguments found");
        }
        if (line[at] == ARG_DELIM) {
          if (arg.empty()) {
            throw_bad_asm(lineno, "Empty argument detected.");
          }
          if (!is_valid_token(arg)) {
            throw_bad_asm(lineno, "Invalid argument detected.");
          }
          // TODO:
          push_back_token(std::move(arg));
          arg.clear();
        } else {
          arg += line[at];
        }
      }
      if (!arg.empty()) {
        push_back_token(std::move(arg));
      }
    }

    if (!labels.empty()) {
      instruction inst(lastLabelLine);
      inst.labels = std::move(labels);
      result.push_back(std::move(inst));
    }

    return result;
  }

  bool operator==(const instruction& a, const instruction& b) {
    return a.line == b.line &&
      util::vec_equal(a.labels, b.labels) && 
      a.name == b.name && 
      util::vec_equal(a.args, b.args);
  }

  bool operator==(const token& a, const token& b) {
    if (a.type != b.type) {
      return false;
    }

    switch (a.type) {
      case token::Integer:
        return a.integer() == b.integer();
      case token::String:
        return a.string() == b.string();
    }

    assert(false);
	return false; // VC11 gives a warning if this isn't here. G++ 4.7 doesn't.
  }

}
