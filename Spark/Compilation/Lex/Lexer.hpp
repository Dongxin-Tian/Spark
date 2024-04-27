#pragma once

#include <sstream>
#include <vector>

#include "Token.hpp"

namespace Spark {

class Lexer {

    /* ===== Data ===== */

private:
    std::istringstream iss;

    enum class CommentState {
        None,
        LineCommenting,
        GroupCommenting
    };

    CommentState commentState = CommentState::None;



    /* ===== Constructor ===== */

public:
    explicit Lexer(const std::string& str) : iss(str) { }



    /* ===== Operations ===== */

public:
    [[nodiscard]]
    static std::vector<Token> lex(const std::string& str);

    [[nodiscard]]
    std::vector<Token> lex();

};

} // Spark
