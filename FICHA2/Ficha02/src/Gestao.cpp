#include "Gestao.h"
#include "Ordenacao.h"
#include <fstream>
#include <sstream>

// Retira espacos (e o \r de ficheiros Windows) do inicio e do fim.
// Necessario porque o ficheiro tem linhas como "Viseu; Carlos Lopes Andrade; 30"
static string Trim(const string &s)
{
    const string espacos = " \t\r\n";
    size_t ini = s.find_first_not_of(espacos);
    if (ini == string::npos)
        return "";
    size_t fim = s.find_last_not_of(espacos);
    return s.substr(ini, fim - ini + 1);
}

Gestao::Gestao()
{
    //ctor
}

Gestao::~Gestao()
{
    //quando criamos com new, mas temos um ponteiro dentro da estrutura, ao fazer o delete, o conteudo do ponteiro nao e apagado, apenas o ponteiro para o conteudo,
    //para tal temos de percorrer a lista e apagar o conteudo.

    cout << "passei em <" << __FUNCTION__ << ">" << endl;
    map<string, list<Pessoa *>>::iterator it = DADOS.begin();
    while (it != DADOS.end())
    {
        list<Pessoa *> &LP = it->second;
        for (list<Pessoa *>::iterator i_l = LP.begin(); i_l != LP.end(); i_l++)
        {
            delete (*i_l); // apaga o conteudo do ponteiro
        }
        LP.clear(); // a lista fica so com ponteiros invalidos, limpamos
        it++; // iterador do while para percorrer o map
    }
    DADOS.clear();
}

// 3a - cria a pessoa e reutiliza o AddPessoa(Pessoa*)
bool Gestao::AddPessoa(string _nome, string _cidade, int _idade)
{
    Pessoa *P = new Pessoa(_nome, _cidade, _idade);
    if (!AddPessoa(P))
    {
        delete P; // nao ficou guardada, senao era memoria perdida
        return false;
    }
    return true;
}

// 3b - a partir daqui o Gestao fica responsavel por fazer delete de P
bool Gestao::AddPessoa(Pessoa *P)
{
    if (P == nullptr)
        return false;
    // nomes repetidos nao: o Remover(nome) e o PesquisarPessoa assumem nome unico
    if (PesquisarPessoa(P->GetNome()) != nullptr)
        return false;

    // o operator[] do map cria a lista vazia se a cidade ainda nao existir
    DADOS[P->GetCidade()].push_back(P);
    return true;
}

// 3c - formato: Cidade;Nome da Pessoa;Idade   (idade em falta -> 18)
bool Gestao::LerFicheiro(string nome_ficheiro)
{
    ifstream F(nome_ficheiro);
    if (!F.is_open())
    {
        cout << "Erro ao abrir o ficheiro <" << nome_ficheiro << ">" << endl;
        return false;
    }

    string linha;
    while (getline(F, linha))
    {
        if (Trim(linha).empty())
            continue;

        stringstream ss(linha);
        string cidade, nome, s_idade;
        getline(ss, cidade, ';');
        getline(ss, nome, ';');
        getline(ss, s_idade);   // o resto da linha (pode estar vazio)

        cidade = Trim(cidade);
        nome = Trim(nome);
        s_idade = Trim(s_idade);
        if (cidade.empty() || nome.empty())
            continue; // linha mal formada

        int idade = 18;
        if (!s_idade.empty())
        {
            try { idade = stoi(s_idade); }
            catch (...) { idade = 18; } // idade invalida -> 18
        }
        AddPessoa(nome, cidade, idade);
    }
    F.close();
    return true;
}

// 3d
void Gestao::Mostrar()
{
    if (DADOS.empty())
    {
        cout << "(sem dados)" << endl;
        return;
    }
    map<string, list<Pessoa *>>::iterator i_m;
    for (i_m = DADOS.begin(); i_m != DADOS.end(); i_m++)
    {
        cout << i_m->first << " [" << i_m->second.size() << " pessoa(s)]" << endl;
        for (list<Pessoa *>::iterator i_l = i_m->second.begin(); i_l != i_m->second.end(); i_l++)
            (*i_l)->Show();
    }
}

// 3e - retira da estrutura E liberta a memoria (P deixa de ser valido!)
bool Gestao::Remover(Pessoa *P)
{
    if (P == nullptr)
        return false;

    map<string, list<Pessoa *>>::iterator i_m = DADOS.find(P->GetCidade());
    if (i_m == DADOS.end())
        return false;

    list<Pessoa *> &lp = i_m->second;
    for (list<Pessoa *>::iterator i_l = lp.begin(); i_l != lp.end(); i_l++)
    {
        if (*i_l == P)
        {
            lp.erase(i_l);      // tira o ponteiro da lista
            delete P;           // apaga a pessoa
            if (lp.empty())     // cidade sem ninguem sai do map
                DADOS.erase(i_m);
            return true;
        }
    }
    return false;
}

// 3f
bool Gestao::Remover(string _nome)
{
    return Remover(PesquisarPessoa(_nome));
}

// 3g - mesmo formato do LerFicheiro, para se poder voltar a ler
bool Gestao::GravarFicheiro(string ficheiro)
{
    ofstream F(ficheiro);
    if (!F.is_open())
        return false;

    map<string, list<Pessoa *>>::iterator i_m;
    for (i_m = DADOS.begin(); i_m != DADOS.end(); i_m++)
        for (list<Pessoa *>::iterator i_l = i_m->second.begin(); i_l != i_m->second.end(); i_l++)
            (*i_l)->Gravar(F);

    F.close();
    return true;
}

// 3h
Pessoa *Gestao::PesquisarPessoa(string _nome)
{
    // i_m iterador do map
    //map divide-se entre nome e lista, lista contem apontadores para o elemento em i_m->second,
    //i_m->first sera o nome da cidade.
    //queremos devolver o ponteiro para uma pessoa
    //i_l iterador da lista

    map<string, list<Pessoa *>>::iterator i_m;
    for (i_m = DADOS.begin(); i_m != DADOS.end(); i_m++)
    {
        list<Pessoa *> &lp = i_m->second; // referencia: evita copiar a lista
        for (list<Pessoa *>::iterator i_l = lp.begin(); i_l != lp.end(); i_l++)
        {
            if ((*i_l)->GetNome() == _nome)
                return *i_l;
        }
    }
    return nullptr;
}

// 3i
string Gestao::GetCidadeMaisHabitantes()
{
    map<string, list<Pessoa *>>::iterator i_m;
    string CidadeMais = "";
    size_t MAX = 0;
    for (i_m = DADOS.begin(); i_m != DADOS.end(); i_m++)
    {
        if (i_m->second.size() > MAX)
        {
            MAX = i_m->second.size();
            CidadeMais = i_m->first;
        }
    }
    return CidadeMais;
}

// 3j - a Ordenacao trabalha com vetores (V[i]) e a list nao tem acesso por
// indice, por isso para cada cidade: list -> array, ordena, array -> list.
// So se copiam ponteiros, as Pessoas nao sao duplicadas.
void Gestao::OrdenarPorIdade()
{
    map<string, list<Pessoa *>>::iterator i_m;
    for (i_m = DADOS.begin(); i_m != DADOS.end(); i_m++)
    {
        list<Pessoa *> &lp = i_m->second;
        int N = lp.size();
        Pessoa **V = new Pessoa *[N];   // array de ponteiros

        int k = 0;
        for (list<Pessoa *>::iterator i_l = lp.begin(); i_l != lp.end(); i_l++)
            V[k++] = *i_l;

        Ordenacao::InsertionSort(V, N);

        lp.clear();                     // so tira os ponteiros, nao apaga Pessoas
        for (k = 0; k < N; k++)
            lp.push_back(V[k]);

        delete[] V;                     // apaga o array, nao as Pessoas
    }
}
