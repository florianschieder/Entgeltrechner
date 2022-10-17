#ifndef CORE_ENTGELTRECHNER_APPLICATION_H
#define CORE_ENTGELTRECHNER_APPLICATION_H

#include "AbstractApplication.h"


class EntgeltrechnerApplication
    : public AbstractApplication
{
public:
    EntgeltrechnerApplication(const HINSTANCE& hInstance);

    int run();
};

#endif  // CORE_ENTGELTRECHNER_APPLICATION_H