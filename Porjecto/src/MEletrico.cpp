#include "MEletrico.h"

MEletrico::MEletrico()
    : Motor()
{
}

MEletrico::~MEletrico()
{
}

double MEletrico::Aquecimento() const
{
    return 5;
}

Motor *MEletrico::Clone() const
{
    return new MEletrico(*this);
}
