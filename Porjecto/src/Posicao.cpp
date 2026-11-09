#include "Posicao.h"

#include <cmath>
using namespace std;
Posicao::Posicao(int _x, int _y)
{
    X = _x;
    Y = _y;
}

double Posicao::Distancia(const Posicao &outra) const
{
    double dx = X - outra.X;
    double dy = Y - outra.Y;
    return sqrt(dx * dx + dy * dy);
}
