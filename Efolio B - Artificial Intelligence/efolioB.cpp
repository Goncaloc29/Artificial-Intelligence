#include <chrono>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

/* Estrutura de dados para representar uma cidade */
struct Cidade {
    vector<vector<int>> cidade;
    Cidade* pai;
    int custo;
    int heuristica;

    Cidade(Cidade* pai, vector<vector<int>> cidade, int heuristica, int custo) {
        this->cidade = cidade;
        this->pai = pai;
        this->custo = custo;
        this->heuristica = heuristica;
    }

    /* Função que retorna uma string com a representação da cidade */
    string toString() {
        string s = "";
        for (int m = 0; m < cidade.size() + 2; m++) {
            if (m != (cidade.size() / 2 + 1)) {
                s += " -1";
            } else {
                s += "  2";
            }
        }
        s += "\n";
        for (int m = 0; m < cidade.size(); m++) {
            if (m != (cidade.size() / 2)) {
                s += " -1";
            } else {
                s += "  3";
            }
            for (int n = 0; n < cidade[0].size(); n++) {
                if (cidade[m][n] != -1) {
                    s += " ";
                }
                s += " " + to_string(cidade[m][n]);
            }
            if (m != (cidade.size() / 2)) {
                s += " -1";
            } else {
                s += "  4";
            }
            s += "\n";
        }
        for (int m = 0; m < cidade.size() + 2; m++) {
            if (m != (cidade.size() / 2 + 1)) {
                s += " -1";
            } else {
                s += "  5";
            }
        }
        return s;
    }
};

/* Estrutura de dados para representar uma lista de cidades.
Para a procura em profundidade é necessário que a lista seja uma pilha
da qual se remove o último elemento inserido. */
struct Lista_DFS {
    vector<vector<int>> lista;

    void adicionar(vector<int> Cidade) {
        lista.push_back(Cidade);
    }

    vector<int> remover() {
        vector<int> last = lista[lista.size() - 1];
        lista.pop_back();
        return last;
    }

    bool vazia() {
        return lista.empty();
    }
};

/* Função que retorna as posições adjacentes a uma posição (x,y) na cidade */
vector<vector<int>> obterPosicoesVizinhos(vector<vector<int>> cidade, int posicao_X, int posicao_y, int tamanho) {
    vector<vector<int>> vizinhos;
    if (posicao_X > 0 && cidade[posicao_X - 1][posicao_y] == 0) {
        vizinhos.push_back({posicao_X - 1, posicao_y});
    }
    if (posicao_X < tamanho - 1 && cidade[posicao_X + 1][posicao_y] == 0) {
        vizinhos.push_back({posicao_X + 1, posicao_y});
    }
    if (posicao_y > 0 && cidade[posicao_X][posicao_y - 1] == 0) {
        vizinhos.push_back({posicao_X, posicao_y - 1});
    }
    if (posicao_y < tamanho - 1 && cidade[posicao_X][posicao_y + 1] == 0) {
        vizinhos.push_back({posicao_X, posicao_y + 1});
    }
    if (posicao_X > 0 && posicao_y > 0 && cidade[posicao_X - 1][posicao_y - 1] == 0) {
        vizinhos.push_back({posicao_X - 1, posicao_y - 1});
    }
    if (posicao_X > 0 && posicao_y < tamanho - 1 && cidade[posicao_X - 1][posicao_y + 1] == 0) {
        vizinhos.push_back({posicao_X - 1, posicao_y + 1});
    }
    if (posicao_X < tamanho - 1 && posicao_y > 0 && cidade[posicao_X + 1][posicao_y - 1] == 0) {
        vizinhos.push_back({posicao_X + 1, posicao_y - 1});
    }
    if (posicao_X < tamanho - 1 && posicao_y < tamanho - 1 && cidade[posicao_X + 1][posicao_y + 1] == 0) {
        vizinhos.push_back({posicao_X + 1, posicao_y + 1});
    }
    return vizinhos;
}

/* Função que verifica a que portas uma posicao (x,y) tem acesso */
bool verificarAcessoAPortas(int x, int y, int tamanho, vector<int>& portas) {
    if (x == 0 && y >= (tamanho / 2 - 1) && y <= (tamanho / 2 + 1)) {
        portas[0] = 1;
    } else if (x == tamanho - 1 && y >= (tamanho / 2 - 1) && y <= (tamanho / 2 + 1)) {
        portas[1] = 1;
    } else if (y == 0 && x >= (tamanho / 2 - 1) && x <= (tamanho / 2 + 1)) {
        portas[2] = 1;
    } else if (y == tamanho - 1 && x >= (tamanho / 2 - 1) && x <= (tamanho / 2 + 1)) {
        portas[3] = 1;
    }

    for (int i = 0; i < 4; i++) {
        if (portas[i] == 0) {
            return false;
        }
    }

    return true;
}

/* Função que retorna o número de zonas habitacionais sem caminho e o número de caminhos.
Realiza uma procura em profundidade em todas as zonas habitacionais da cidade e verifica
se a casa tem acesso a todas as portas e quantos caminhos para portas ha ao todo */
vector<int> numeroCasasSemCaminho(vector<vector<int>>& cidade) {
    int tamanho = cidade.size();
    int numero_casas = 0;
    int numero_casas_com_caminho = 0;
    int numero_caminhos = 0;
    int numero_portas = 4;

    for (int m = 0; m < tamanho; m++) {
        for (int n = 0; n < tamanho; n++) {
            if (cidade[m][n] == 1) {
                Lista_DFS q;
                vector<vector<int>> visitados(tamanho, vector<int>(tamanho, 0));
                numero_casas++;

                q.adicionar({m, n});
                visitados[m][n] = 1;
                vector<int> portas(numero_portas, 0);

                while (!q.vazia()) {
                    vector<int> v = q.remover();
                    int r = v[0];
                    int c = v[1];

                    if (verificarAcessoAPortas(r, c, tamanho, portas)) {
                        numero_casas_com_caminho++;
                        break;
                    }

                    vector<vector<int>> vizinhos = obterPosicoesVizinhos(cidade, r, c, tamanho);

                    for (vector<int>& vizinho : vizinhos) {
                        if (visitados[vizinho[0]][vizinho[1]] == 0) {
                            q.adicionar(vizinho);
                            visitados[vizinho[0]][vizinho[1]] = 1;
                        }
                    }
                }

                for (int m = 0; m < numero_portas; m++) {
                    if (portas[m]) {
                        numero_caminhos++;
                    }
                }
            }
        }
    }

    vector<int> v = {numero_casas - numero_casas_com_caminho, numero_caminhos / numero_portas};
    return v;
}

/* Função que calcula a heurística de uma cidade.
A heurística é calculada subtraindo o numero de caminhos por porta ao numero de zonas habitacioanis sem caminho.
Esta subtração diferencia entre zonas habitacioanais com o mesmo numero de zonas habitacionais sem caminho. */
int calcularHeuristica(vector<vector<int>> cidade) {
    vector<int> v = numeroCasasSemCaminho(cidade);
    return v[0] - v[1];
}

/* Função que verifica se a cidade é o objetivo.
O objetivo é quando não há mais zonas habitacionais sem caminho. */
bool verificarObjetivo(Cidade* Cidade) {
    return numeroCasasSemCaminho(Cidade->cidade)[0] == 0;
}

/* Estrutura de dados para representar uma lista de cidades.
Para procura A* é necessário que a lista seja uma fila de prioridade
da qual se remove o elemento com menor custo + heurística */
class Lista_A_Estrela {
   public:
    vector<Cidade*> lista;

    void adicionar(Cidade* Cidade) {
        lista.push_back(Cidade);
    }

    Cidade* remover() {
        int melhor_custo = lista[0]->custo + lista[0]->heuristica;
        int melhor_indice = 0;
        for (int m = 1; m < lista.size(); m++) {
            int custo = lista[m]->custo + lista[m]->heuristica;
            if (custo < melhor_custo) {
                melhor_custo = custo;
                melhor_indice = m;
            }
        }
        Cidade* melhor_cidade = lista[melhor_indice];
        lista.erase(lista.begin() + melhor_indice);
        return melhor_cidade;
    }

    bool vazia() {
        return lista.empty();
    }
};

/* Função que retorna os vizinhos de uma cidade.
Um vizinho é obtido trocando uma casa de lugar. */
vector<Cidade*> obterVizinhos(Cidade* cidade) {
    vector<Cidade*> vizinhos;
    for (int m = 0; m < cidade->cidade.size(); m++) {
        for (int n = 0; n < cidade->cidade[0].size(); n++) {
            if (cidade->cidade[m][n] == 1) {
                for (int k = 0; k < cidade->cidade.size(); k++) {
                    for (int l = 0; l < cidade->cidade[0].size(); l++) {
                        if (cidade->cidade[k][l] == 0) {
                            vector<vector<int>> nova_cidade = cidade->cidade;
                            nova_cidade[m][n] = 0;
                            nova_cidade[k][l] = 1;
                            Cidade* nova_cidade_obj = new Cidade(cidade, nova_cidade, calcularHeuristica(nova_cidade), cidade->custo + 1);
                            vizinhos.push_back(nova_cidade_obj);
                        }
                    }
                }
            }
        }
    }
    return vizinhos;
}

/* Função que verifica se uma cidade já foi visitada no algoritmo A* */
bool foiVisitado(Cidade* Cidade, vector<struct Cidade*> visitados) {
    for (int m = 0; m < visitados.size(); m++) {
        if (visitados[m]->cidade == Cidade->cidade) {
            return true;
        }
    }
    return false;
}

/* Função que implementa o algoritmo A*.
Retorna a cidade objetivo.
Caso não encontre a cidade objetivo, retorna a cidade com menor custo total. */
Cidade* A_Estrela(Cidade* cidade_inicial, int* gerados, int* avaliados, chrono::steady_clock::time_point temporizador_ini) {
    Lista_A_Estrela lista;
    vector<Cidade*> visitados;

    Cidade* cidade_melhor_custo = cidade_inicial;
    lista.adicionar(cidade_inicial);

    while (!lista.vazia()) {
        Cidade* cidade_atual = lista.remover();
        visitados.push_back(cidade_atual);

        (*avaliados)++;

        if (verificarObjetivo(cidade_atual)) {
            return cidade_atual;
        }

        vector<Cidade*> vizinhos = obterVizinhos(cidade_atual);
        for (Cidade* vizinho : vizinhos) {
            if (*gerados > 100000) {
                cidade_melhor_custo->custo += numeroCasasSemCaminho(cidade_melhor_custo->cidade)[0] * 100;
                return cidade_melhor_custo;
            }
            if (chrono::steady_clock::now() - temporizador_ini > chrono::seconds(60)) {
                cidade_melhor_custo->custo += numeroCasasSemCaminho(cidade_melhor_custo->cidade)[0] * 100;
                return cidade_melhor_custo;
            }
            if (!foiVisitado(vizinho, visitados)) {
                (*gerados)++;
                lista.adicionar(vizinho);
                if (vizinho->custo + vizinho->heuristica < cidade_melhor_custo->custo + cidade_melhor_custo->heuristica) {
                    cidade_melhor_custo = vizinho;
                }
            }
        }
    }

    cidade_melhor_custo->custo += numeroCasasSemCaminho(cidade_melhor_custo->cidade)[0] * 100;
    return cidade_melhor_custo;
}

/* Função que imprime o resultado da procura */
void imprimeResultado(Cidade* instancia, Cidade* cidade, long tempo, int gerados, int avaliados, int num_instancia) {
    cout << "Instancia " << num_instancia << ":" << endl;
    cout << instancia->toString() << endl;
    cout << "Resultado: " << endl;
    cout << cidade->toString() << endl;
    cout << "Custo: " << cidade->custo << endl;
    cout << "Tempo: " << tempo << endl;
    cout << "Gerados: " << gerados << endl;
    cout << "Avaliados: " << avaliados << endl;
    cout << endl;
}

int main() {
    vector<vector<vector<int>>> cidades = {
        // 5x5
        {
            {
                0,
                1,
                1,
                1,
                0,
            },
            {
                1,
                1,
                0,
                1,
                1,
            },
            {
                0,
                1,
                -1,
                -1,
                0,
            },
            {
                1,
                1,
                0,
                1,
                0,
            },
            {
                1,
                1,
                0,
                0,
                0,
            },
        },
        {
            {
                0,
                1,
                1,
                0,
                0,
            },
            {
                1,
                1,
                -1,
                1,
                1,
            },
            {
                0,
                -1,
                -1,
                -1,
                0,
            },
            {
                1,
                1,
                -1,
                1,
                0,
            },
            {
                0,
                1,
                0,
                1,
                1,
            },
        },
        {
            {
                0,
                1,
                1,
                0,
                0,
            },
            {
                1,
                -1,
                0,
                1,
                1,
            },
            {
                1,
                -1,
                -1,
                -1,
                0,
            },
            {
                0,
                1,
                1,
                -1,
                0,
            },
            {
                0,
                1,
                0,
                1,
                1,
            },
        },
        {
            {
                1,
                1,
                1,
                1,
                0,
            },
            {
                1,
                1,
                0,
                0,
                1,
            },
            {
                1,
                0,
                1,
                0,
                1,
            },
            {
                1,
                0,
                0,
                1,
                1,
            },
            {
                0,
                1,
                1,
                1,
                1,
            },
        },
        // 7x7
        {
            {
                1,
                1,
                1,
                1,
                0,
                1,
                1,
            },
            {
                0,
                1,
                0,
                0,
                0,
                0,
                0,
            },
            {
                1,
                1,
                0,
                1,
                0,
                -1,
                1,
            },
            {
                0,
                1,
                0,
                0,
                1,
                -1,
                0,
            },
            {
                1,
                -1,
                -1,
                -1,
                1,
                -1,
                1,
            },
            {
                1,
                0,
                0,
                0,
                1,
                0,
                1,
            },
            {
                0,
                0,
                1,
                0,
                0,
                0,
                0,
            },
        },
        {
            {
                0,
                1,
                0,
                0,
                0,
                1,
                0,
            },
            {
                0,
                1,
                0,
                1,
                1,
                1,
                0,
            },
            {
                1,
                0,
                -1,
                -1,
                -1,
                0,
                1,
            },
            {
                0,
                1,
                -1,
                -1,
                -1,
                1,
                0,
            },
            {
                1,
                1,
                -1,
                -1,
                -1,
                0,
                1,
            },
            {
                0,
                1,
                1,
                0,
                1,
                0,
                1,
            },
            {
                0,
                0,
                1,
                1,
                0,
                1,
                0,
            },
        },
        {
            {
                1,
                1,
                1,
                1,
                1,
                1,
                1,
            },
            {
                1,
                1,
                0,
                0,
                0,
                1,
                1,
            },
            {
                1,
                0,
                1,
                0,
                1,
                0,
                1,
            },
            {
                1,
                1,
                0,
                0,
                0,
                1,
                1,
            },
            {
                1,
                0,
                1,
                0,
                1,
                0,
                1,
            },
            {
                1,
                1,
                0,
                0,
                0,
                1,
                1,
            },
            {
                1,
                1,
                1,
                1,
                1,
                1,
                1,
            },
        },
        // 9x9
        {
            {
                0,
                1,
                1,
                1,
                0,
                -1,
                0,
                0,
                0,
            },
            {
                1,
                1,
                0,
                1,
                1,
                -1,
                1,
                1,
                1,
            },
            {
                0,
                1,
                -1,
                -1,
                0,
                0,
                0,
                0,
                0,
            },
            {
                1,
                1,
                0,
                -1,
                -1,
                -1,
                0,
                1,
                0,
            },
            {
                1,
                1,
                0,
                -1,
                0,
                -1,
                0,
                1,
                0,
            },
            {
                1,
                0,
                0,
                0,
                0,
                1,
                0,
                1,
                0,
            },
            {
                1,
                0,
                -1,
                -1,
                -1,
                1,
                0,
                1,
                0,
            },
            {
                0,
                1,
                0,
                1,
                0,
                1,
                0,
                0,
                0,
            },
            {
                0,
                0,
                0,
                1,
                0,
                0,
                0,
                0,
                0,
            },
        },
        {
            {
                0,
                1,
                1,
                1,
                0,
                0,
                0,
                0,
                -1,
            },
            {
                1,
                1,
                -1,
                1,
                1,
                1,
                1,
                1,
                -1,
            },
            {
                0,
                1,
                -1,
                1,
                0,
                1,
                -1,
                0,
                -1,
            },
            {
                -1,
                -1,
                -1,
                1,
                0,
                1,
                -1,
                1,
                0,
            },
            {
                1,
                1,
                0,
                1,
                0,
                1,
                -1,
                1,
                0,
            },
            {
                1,
                0,
                -1,
                -1,
                -1,
                -1,
                -1,
                1,
                0,
            },
            {
                1,
                0,
                1,
                0,
                0,
                0,
                -1,
                1,
                -1,
            },
            {
                0,
                1,
                0,
                1,
                0,
                1,
                -1,
                0,
                -1,
            },
            {
                0,
                0,
                0,
                1,
                0,
                0,
                0,
                0,
                -1,
            },
        },
        {
            {
                1,
                1,
                1,
                1,
                0,
                1,
                1,
                1,
                1,
            },
            {
                1,
                0,
                1,
                1,
                0,
                1,
                1,
                0,
                1,
            },
            {
                1,
                1,
                0,
                1,
                0,
                1,
                0,
                1,
                1,
            },
            {
                1,
                1,
                1,
                1,
                0,
                1,
                1,
                1,
                1,
            },
            {
                0,
                0,
                0,
                0,
                1,
                0,
                0,
                0,
                0,
            },
            {
                1,
                1,
                1,
                1,
                0,
                1,
                1,
                1,
                1,
            },
            {
                1,
                1,
                0,
                1,
                0,
                1,
                0,
                1,
                1,
            },
            {
                1,
                0,
                1,
                1,
                0,
                1,
                1,
                0,
                1,
            },
            {
                1,
                1,
                1,
                1,
                0,
                1,
                1,
                1,
                1,
            },
        },
    };

    for (int m = 0; m < cidades.size(); m++) {
        Cidade* instancia = new Cidade(nullptr, cidades[m], calcularHeuristica(cidades[m]), 0);
        int gerados = 0;
        int avaliados = 0;

        auto temporizador_ini = chrono::steady_clock::now();
        Cidade* resultado = A_Estrela(instancia, &gerados, &avaliados, temporizador_ini);
        auto temporizador_fim = chrono::steady_clock::now();

        chrono::duration<double> tempo = temporizador_fim - temporizador_ini;
        auto tempo_millis = chrono::duration_cast<std::chrono::milliseconds>(tempo).count();

        imprimeResultado(instancia, resultado, tempo_millis, gerados, avaliados, m + 1);
    }

    return 0;
}