#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <random>

// ============================================================
// Classe Matriz: encapsula os dados e operações sobre a matriz
// ============================================================
class Matriz {
private:
    int linhas;
    int colunas;
    std::vector<std::vector<double>> dados;

public:
    Matriz(int linhas, int colunas, double valorInicial = 0.0)
        : linhas(linhas), colunas(colunas),
          dados(linhas, std::vector<double>(colunas, valorInicial)) {}

    // Acesso simples como matriz[i][j]
    std::vector<double>& operator[](int i) {
        return dados[i];
    }

    const std::vector<double>& operator[](int i) const {
        return dados[i];
    }

    int getLinhas() const { return linhas; }
    int getColunas() const { return colunas; }

    // Preenche a matriz com valores aleatórios entre 0 e 1
    void preencherAleatorio(unsigned int semente = 42) {
        std::mt19937 gerador(semente);
        std::uniform_real_distribution<double> distribuicao(0.0, 1.0);

        for (int i = 0; i < linhas; i++)
            for (int j = 0; j < colunas; j++)
                dados[i][j] = distribuicao(gerador);
    }

    // Imprime um trecho da matriz (útil para verificação)
    void imprimirTrecho(int maxLinhas = 4, int maxColunas = 4) const {
        int linhasExibir  = std::min(linhas, maxLinhas);
        int colunasExibir = std::min(colunas, maxColunas);

        for (int i = 0; i < linhasExibir; i++) {
            for (int j = 0; j < colunasExibir; j++)
                std::cout << std::setw(8) << std::fixed << std::setprecision(4)
                          << dados[i][j] << " ";
            if (colunas > maxColunas) std::cout << "...";
            std::cout << "\n";
        }
        if (linhas > maxLinhas) std::cout << "...\n";
    }
};


// ============================================================
// Classe Cronometro: mede tempo de execução com facilidade
// ============================================================
class Cronometro {
private:
    std::chrono::high_resolution_clock::time_point inicio;

public:
    void iniciar() {
        inicio = std::chrono::high_resolution_clock::now();
    }

    double segundosDecorridos() const {
        auto agora = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> intervalo = agora - inicio;
        return intervalo.count();
    }
};


// ============================================================
// Função de multiplicação: C = A * B
// Cada elemento C[i][j] = soma(A[i][k] * B[k][j]) para todo k
// ============================================================
Matriz multiplicar(const Matriz& A, const Matriz& B) {
    if (A.getColunas() != B.getLinhas())
        throw std::invalid_argument(
            "Dimensões incompatíveis: colunas de A devem ser iguais às linhas de B.");

    int linhasC   = A.getLinhas();
    int colunasC  = B.getColunas();
    int dimensaoK = A.getColunas();

    Matriz C(linhasC, colunasC, 0.0);

    for (int i = 0; i < linhasC; i++)
        for (int k = 0; k < dimensaoK; k++)       // k antes de j melhora cache
            for (int j = 0; j < colunasC; j++)
                C[i][j] += A[i][k] * B[k][j];

    return C;
}


// ============================================================
// Programa principal
// ============================================================
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: ./matmul_seq <tamanho>\n";
        std::cerr << "Exemplo: ./matmul_seq 1024\n";
        return 1;
    }

    const int N = std::stoi(argv[1]);

    std::cout << "Multiplicação de matrizes sequencial\n";
    std::cout << "Dimensão: " << N << " x " << N << "\n\n";

    // Criação e preenchimento das matrizes A e B
    Matriz A(N, N);
    Matriz B(N, N);
    A.preencherAleatorio(42);
    B.preencherAleatorio(99);

    //std::cout << "Trecho de A:\n";
    //A.imprimirTrecho();
    //std::cout << "\nTrecho de B:\n";
    //B.imprimirTrecho();

    // Multiplicação com medição de tempo
    Cronometro cronometro;
    cronometro.iniciar();

    Matriz C = multiplicar(A, B);

    double tempoTotal = cronometro.segundosDecorridos();

    //std::cout << "\nTrecho do resultado C = A * B:\n";
    //C.imprimirTrecho();

    std::cout << "\n----------------------------------------\n";
    std::cout << "Tempo de execução: " << std::fixed << std::setprecision(4)
              << tempoTotal << " segundos\n";
    std::cout << "----------------------------------------\n";

    return 0;
}