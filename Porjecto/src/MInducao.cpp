#include "MInducao.h"

MInducao::MInducao()
    : Motor()
{
}

MInducao::~MInducao()
{
}

double MInducao::Aquecimento() const
{
    return 3;
}

Motor *MInducao::Clone() const
{
    return new MInducao(*this);
}
