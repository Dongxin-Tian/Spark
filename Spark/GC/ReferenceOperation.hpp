#pragma once

#include "GCNode.hpp"
#include "GCOperation.hpp"

namespace Spark {

class ReferenceOperation : public GCOperation {

public:
    ReferenceOperation(GCNode* referencer, GCNode* referencee) : referencer(referencer), referencee(referencee) { }

private:
    GCNode* referencer;
    GCNode* referencee;

public:
    bool step() override {
        referencee->referenceCount++;
        referencer->neighbors().push_back(referencee);
        return true;
    }

};

} // Spark
