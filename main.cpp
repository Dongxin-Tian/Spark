#include <iostream>
#include <string>

#include "Spark.hpp"
#include "Spark/GC/GCHandle.hpp"
#include "Spark/GC/GCNode.hpp"

using namespace Spark;

int main() {

    Value value = Value::make<Integer>(1);
    value = Pointer(&value);

    std::cout << "Type: " << value.type() << std::endl;
    std::cout << "Is Constant: " << value.isConstant() << std::endl;
    std::cout << "Value: " << value << std::endl;

    Integer i = Integer(12);
    Float f = Float(0.2) + i;
    Integer i2 = f;
    std::cout << i << std::endl;
    std::cout << f << std::endl;

    return 0;
}
