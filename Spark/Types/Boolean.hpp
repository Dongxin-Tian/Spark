#pragma once

#include <cstdint>

namespace Spark {

struct Boolean {

    /* ===== Data ===== */

private:
    uint8_t value;



    /* ===== Constructors ===== */

public:
    constexpr Boolean() : value(0) { }
    constexpr Boolean(const bool value) : value(value ? 1 : 0) { }
    explicit constexpr Boolean(const uint8_t value) : value(value) { }



    /* ===== Operators ===== */

public:
    constexpr Boolean& operator=(const bool b) {
        value = b ? 1 : 0;
        return *this;
    }

    constexpr Boolean operator !() const { return !value; }



    /* ===== Conversion Operators ===== */

public:
    constexpr operator bool() const { return value != 0; }

};

}
