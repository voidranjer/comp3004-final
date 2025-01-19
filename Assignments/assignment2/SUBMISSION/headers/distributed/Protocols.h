#ifndef PROTOCOLS_H
#define PROTOCOLS_H

class WithSafetyProtocols {
   public:
    virtual void obstacleWarning() = 0;
    virtual void fireWarning() = 0;
    virtual void overloadWarning() = 0;
    virtual void evacInstructions() = 0;
};

#endif