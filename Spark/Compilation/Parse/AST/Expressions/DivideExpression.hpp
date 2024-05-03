#pragma once

#include "../../Types/Value.hpp"
#include "../EvaluateVisitor.hpp"
#include "BinaryExpression.hpp"

namespace Spark {

class DivideExpression : public BinaryExpression {

    /* ===== Constructor ===== */

public:
    DivideExpression(std::unique_ptr<ExpressionNode>&& rLeft, std::unique_ptr<ExpressionNode>&& rRight)
        : BinaryExpression(std::move(rLeft), std::move(rRight)) { }



    /* ===== Visitor ===== */

    friend EvaluateVisitor;

public:
    bool accept(const EvaluateVisitor& visitor, Value& out) override {
        return visitor.visit(*this, out);
    }

};

} // Spark