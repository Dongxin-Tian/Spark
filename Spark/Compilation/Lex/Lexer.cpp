#include "Lexer.hpp"

#include <cctype>

#include "LexException.hpp"

namespace Spark {

    /* ===== Tokenization ===== */

    std::unordered_set<std::string> Lexer::keywordSet = {
        "let", "var", "const",
        "if", "else",
        "for",
        "while", "do",
        "try", "catch", "finally",
        "function", "return",
        "class",
        "namespace",
        "private", "protected", "public"
    };

    std::unordered_set<std::string> Lexer::operatorSet = {
        "+", "+=", "++",
        "-", "-=", "--",
        "*", "*=",
        "/", "/=",
        "%", "%=",
        "=", "==", "=>",
        "!", "!=",
        "&", "&=", "&&",
        "|", "|=", "||",
        "^", "^=", "^^",
        "<", "<=", "<<", "<<=",
        ">", ">=", ">>", ">>=",
        ".",
        "?", "??", "\?\?=",
        ":", "::",
        ",",
        ";",
        "{",
        "}",
        "[",
        "]",
        "(",
        ")"
    };

    bool Lexer::isKeyword(const std::string& str) {
        return keywordSet.find(str) != keywordSet.end();
    }

    bool Lexer::isOperator(const std::string& str) {
        return operatorSet.find(str) != operatorSet.end();
    }

    bool Lexer::isOperator(char c) {
        return isOperator(std::string(1, c));
    }

    bool Lexer::isNumericalLiteral(const std::string& str) {
        std::istringstream iss(str);
        double num;
        iss >> num;
        return iss.eof() && !iss.fail();
    }

    bool Lexer::isIdentifier(const std::string& str) {
        // Empty string
        if (str.empty())
            return false;

        // First character has to be letters, '_' or '@'
        char firstChar = str[0];
        if (!std::isalpha(firstChar) && (firstChar != '_') && (firstChar != '@'))
            return false;

        // Rest characters have to be letters, digits or '_'
        for (size_t i = 1; i < str.length(); i++) {
            char c = str[i];
            if (!std::isalpha(c) && !std::isdigit(c) && (c != '_'))
                return false;
        }

        return true;
    }



    /* ===== Operations ===== */

    std::vector<Token> Lexer::lex(const std::string& str) {
        return Lexer(str).lex();
    }

    std::vector<Token> Lexer::lex() {
        std::vector<Token> tokens;

        std::string current;

        auto tokenizeCurrent = [&]() -> void {
            if (current.empty())
                return;

            if (Lexer::isKeyword(current))
                tokens.emplace_back(TokenType::Keyword, current);
            else if (Lexer::isOperator(current))
                tokens.emplace_back(TokenType::Operator, current);
            else if (Lexer::isNumericalLiteral(current))
                tokens.emplace_back(TokenType::NumericalLiteral, current);
            else if (Lexer::isIdentifier(current))
                tokens.emplace_back(TokenType::Identifier, current);
            else {
                std::stringstream ss;
                ss << "Invalid identifier: " << current;
                throw LexException(ss.str());
            }

            current.clear();
        };

        int c;
        while ((c = iss.get()) != EOF) {
            // Check for line commenting end
            if (state == State::LineComment) {
                // Check for LF
                if (c == '\n')
                    state = State::None;
                // Check for CR and CRLF
                else if (c == '\r') {
                    int next = iss.peek();
                    if (next == EOF)
                        break;
                    if (next == '\n')
                        iss.ignore(1);

                    state = State::None;
                }

                continue;
            }
            // Check for group commenting end
            else if (state == State::GroupComment) {
                if (c == '*') {
                    int next = iss.peek();
                    if (next == EOF)
                        break;

                    if (next == '/') {
                        state = State::None;
                        iss.ignore(1);
                    }
                }

                continue;
            }

            // Check for comment beginnings
            if (c == '/') {
                if (int next = iss.peek(); next != EOF) {
                    // Check for line comment beginning
                    if (next == '/') {
                        state = State::LineComment;
                        iss.ignore(1);
                    }
                    // Check for group comment beginning
                    else if (next == '*') {
                        state = State::GroupComment;
                        iss.ignore(1);
                    }

                    continue;
                }
            }

            // Check for string beginnings
            if (c == '"' || c == '\'') {
                // Set state and string character
                state = State::String;
                stringEndChar = static_cast<char>(c);
            } else if (c == 'r' || c == 'R') {
                if (int next = iss.peek(); next != EOF) {
                    if (next == '"' || next == '\'') {
                        // Set state and string character
                        state = State::RawString;
                        stringEndChar = static_cast<char>(next);

                        iss.ignore(1);
                    }
                }
            } else if (c == 'f' || c == 'F') {
                if (int next = iss.peek(); next != EOF) {
                    if (next == '"' || next == '\'') {
                        // Set state and string character
                        state = State::FormattedString;
                        stringEndChar = static_cast<char>(next);

                        iss.ignore(1);
                    }
                }
            }

            // Tokenize string
            if (state == State::String) {
                // TODO
                std::string str;
                int next;
                while ((next = iss.peek()) != EOF) {
                    // Move iss forward
                    iss.ignore(1);

                    // Tokenize string when reaches the end
                    if (next == stringEndChar) {
                        tokens.emplace_back(TokenType::StringLiteral, str);
                        state = State::None;
                        break;
                    }

                    // Append character
                    str += static_cast<char>(next);
                }

                continue;
            }
            // Tokenize raw string
            else if (state == State::RawString) {
                std::string str;
                int next;
                while ((next = iss.peek()) != EOF) {
                    // Move iss forward
                    iss.ignore(1);

                    // Tokenize string when reaches the end
                    if (next == stringEndChar) {
                        tokens.emplace_back(TokenType::StringLiteral, str);
                        state = State::None;
                        break;
                    }

                    // Append character
                    str += static_cast<char>(next);
                }

                continue;
            }
            // Tokenize formatted string
            else if (state == State::FormattedString) {
                // TODO
            }

            // Check for numerical literal and tokenize it
            if (std::isdigit(c) || (c == '+') || (c == '-') || (c == '.')) {
                bool hasDigit = std::isdigit(c);
                bool hasDot = (c == '.');

                // Save iss position
                std::streampos startPos = iss.tellg();

                // Build number
                std::string num = std::string(1, static_cast<char>(c));
                int next;
                while ((next = iss.peek()) != EOF) {
                    bool isDot = false;
                    if (next == '.') {
                        // Check for there's too many decimal points
                        if (hasDot)
                            throw LexException("Too many decimal points in a number");

                        isDot = true;
                        hasDot = true;
                    } else {
                        // Check if number ends
                        if (std::isspace(next) || isOperator(static_cast<char>(next)))
                            break;
                    }

                    // Check if it's a digit
                    bool isDigit = std::isdigit(next);
                    if (!isDot && !hasDigit)
                        hasDigit = isDigit;

                    // Check if it's an unexpected character
                    if (!isDot && !isDigit) {
                        std::stringstream ss;
                        ss << "Unexpected character '" << static_cast<char>(next) << "'" << " in a number";
                        throw LexException(ss.str());
                    }

                    // Append character to number string
                    num += static_cast<char>(next);
                    iss.ignore(1);
                }

                // Tokenize number if it's valid (have at least one digit)
                if (hasDigit) {
                    tokens.emplace_back(TokenType::NumericalLiteral, num);
                    continue;
                }
                // Otherwise resume iss position
                else
                    iss.seekg(startPos);
            }

            // Tokenize current if the current character is a white space character
            if (std::isspace(c)) {
                tokenizeCurrent();
                continue;
            }

            // Tokenize current if the current character is an operator
            if (Lexer::isOperator(static_cast<char>(c))) {
                // Tokenize current
                tokenizeCurrent();

                // Check if it's a sequence operator
                std::string op = std::string(1, static_cast<char>(c));
                int next;
                while ((next = iss.peek()) != EOF) {
                    if (Lexer::isOperator(op + static_cast<char>(next))) {
                        op += static_cast<char>(next);
                        iss.get();
                    }
                    else
                        break;
                }

                // Tokenize operator
                tokens.emplace_back(TokenType::Operator, op);

                continue;
            }

            // Append character to the current token
            current += static_cast<char>(c);
        }

        // Throw lex exception if state is invalid
        switch (state) {
            case State::GroupComment:
                throw LexException("Unterminated group comment");

            case State::String:
            case State::RawString:
            case State::FormattedString:
                throw LexException("Missing string terminating character");

            default:
                break;
        }

        // Tokenize current if it's not empty
        if (!current.empty())
            tokenizeCurrent();

        return tokens;
    }

} // Spark
