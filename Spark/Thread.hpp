#pragma once

#include <cstddef>
#include <stdexcept>

#include "Config.hpp"
#include "Exception.hpp"
#include "Opcode.hpp"
#include "Value.hpp"

namespace Spark {

class Thread {

    /* ===== Constructors ===== */

public:
    Thread(size_t stackCapacity, size_t maxStackCapacity) : _stackCapacity(stackCapacity), _maxStackCapacity(maxStackCapacity) {
        stackBuffer = reinterpret_cast<Value*>(new std::byte[stackCapacity]);
        stackPointer = basePointer = stackBuffer;
    }

    explicit Thread(size_t stackCapacity) : Thread(stackCapacity, stackCapacity) { }

    Thread() : Thread(DEFAULT_STACK_CAPACITY, DEFAULT_MAX_STACK_CAPACITY) { }



    /* ===== Destructor ===== */

public:
    ~Thread() {
        delete[] reinterpret_cast<std::byte*>(stackBuffer);
    }



    /* ===== Stack ===== */

private:
    Value* stackBuffer;

    size_t _stackCapacity;
    size_t _maxStackCapacity;

    size_t _stackLength;

public:
    [[nodiscard]]
    constexpr size_t stackCapacity() const {
        return _stackCapacity;
    }

    [[nodiscard]]
    constexpr size_t maxStackCapacity() const {
        return _maxStackCapacity;
    }

    [[nodiscard]]
    constexpr size_t stackLength() const {
        return _stackLength;
    }



    /* ===== Registers ===== */

private:
    Value* stackPointer;
    Value* basePointer;
    Opcode* programCounter = nullptr;



    /* ===== Operations ===== */

public:
    void push(const Value& value) {
        // Check for growing the stack and stack overflow
        size_t newStackLength = _stackLength + sizeof(Value);
        if (newStackLength > _stackCapacity) {
            // Need to grow the stack
        } else if (newStackLength > _maxStackCapacity) {
            // Stack overflow
            throw Exception("Stack overflow.", "StackOverflowException");
        }

        // Push the value
        *stackPointer = value;

        // Update the stack pointer
        stackPointer++;

        // Update the stack length
        _stackLength = newStackLength;
    }

    [[nodiscard]]
    Value top() const {
        if (_stackLength == 0)
            throw Exception("Stack underflow.", "StackUnderflowException");
        return *stackPointer;
    }

    void pop() {
        // TODO: Implement
        throw std::runtime_error("Not implemented.");
    }

    void execute() {
        // TODO: Implement
        throw std::runtime_error("Not implemented.");
    }

};

} // Spark
