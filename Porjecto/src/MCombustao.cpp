#include "MCombustao.h"

MCombustao::MCombustao()
    : Motor()
{
}

MCombustao::~MCombustao()
{
}

double MCombustao::Aquecimento() const
{
    return 8;
}

Motor *MCombustao::Clone() const
{
    return new MCombustao(*this);
}
