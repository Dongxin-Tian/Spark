#include <iostream>

#include "Spark/GC/GC.hpp"
#include "Spark/GC/GCPtr.hpp"
#include "Spark/Bool8.hpp"
#include "Spark/Float64.hpp"
#include "Spark/Int64.hpp"
#include "Spark/Type.hpp"
#include "Spark/Value.hpp"

using namespace Spark;

int main() {

//    GC gc;
//
//    GCPtr<Int64> p = gc.make<Int64>(3);
//
//    std::cout << "Value: " << *p << std::endl;
//    std::cout << "Ref count: " << p.referenceCount() << std::endl;

    Value v = Value::makeInteger(1);
    v = 2;
    std::cout << v << std::endl;

    return 0;
}
