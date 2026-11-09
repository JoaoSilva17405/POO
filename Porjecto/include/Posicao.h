#ifndef POSICAO_H
#define POSICAO_H

/**
 * @brief Posição (X, Y) de um dispositivo na fábrica.
 *
 * É um valor: guarda-se por cópia dentro de cada dispositivo, sem memória dinâmica.
 */
class Posicao
{
    /** @brief Coordenada X. */
    int X;
    /** @brief Coordenada Y. */
    int Y;

public:
    /**
     * @brief Cria uma posição.
     * @param _x Coordenada X (por omissão 0).
     * @param _y Coordenada Y (por omissão 0).
     */
    Posicao(int _x = 0, int _y = 0);

    /** @brief Coordenada X (usada pela Fábrica e pelas saídas). */
    int GetX() const { return X; }

    /** @brief Coordenada Y (usada pela Fábrica e pelas saídas). */
    int GetY() const { return Y; }

    /**
     * @brief Distância euclidiana até outra posição.
     * @param outra A outra posição.
     */
    double Distancia(const Posicao &outra) const;
};

#endif // POSICAO_H
