#include <mpi.h>

#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

constexpr int TAG_CABECALHO = 1;
constexpr int TAG_BLOCO_A = 2;
constexpr int TAG_BLOCO_C = 3;

static void PreencherAleatorio(std::vector<double>& matriz, int n, unsigned int semente) {
    std::mt19937 gerador(semente);
    std::uniform_real_distribution<double> distribuicao(0.0, 1.0);

    for (int i = 0; i < n * n; ++i) {
        matriz[i] = distribuicao(gerador);
    }
}

static void MultiplicarBloco(const std::vector<double>& blocoA,
                             const std::vector<double>& B,
                             std::vector<double>& blocoC,
                             int linhas,
                             int n) {
    std::fill(blocoC.begin(), blocoC.end(), 0.0);

    for (int i = 0; i < linhas; ++i) {
        int baseA = i * n;
        int baseC = i * n;
        for (int k = 0; k < n; ++k) {
            double a = blocoA[baseA + k];
            int baseB = k * n;
            for (int j = 0; j < n; ++j) {
                blocoC[baseC + j] += a * B[baseB + j];
            }
        }
    }
}

static bool EhArvoreCompleta(int processos) {
    int valor = processos + 1;
    return processos > 0 && (valor & (valor - 1)) == 0;
}

static int Pai(int rank) {
    return (rank - 1) / 2;
}

static int FilhoEsquerdo(int rank) {
    return 2 * rank + 1;
}

static int FilhoDireito(int rank) {
    return 2 * rank + 2;
}

static bool TemFilhos(int rank, int totalProcessos) {
    return FilhoDireito(rank) < totalProcessos;
}

static double* Dados(std::vector<double>& v) {
    return v.empty() ? nullptr : v.data();
}

static double* DadosComDeslocamento(std::vector<double>& v, int deslocamento) {
    return v.empty() ? nullptr : v.data() + deslocamento;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank = 0;
    int totalProcessos = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcessos);

    int n = 0;

    if (rank == 0) {
        if (argc < 2) {
            std::cerr << "Uso: mpirun -np <1|3|7|15|31> ./mult_mpi <tamanho>\n";
            n = 0;
        } else {
            n = std::atoi(argv[1]);
        }

        if (!EhArvoreCompleta(totalProcessos)) {
            std::cerr << "Numero de processos invalido para arvore binaria completa.\n";
            std::cerr << "Use 1, 3, 7, 15, 31, 63, ... processos.\n";
            n = 0;
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (n <= 0) {
        MPI_Finalize();
        return 1;
    }

    std::vector<double> B(static_cast<size_t>(n) * n);
    std::vector<double> blocoA;
    std::vector<double> blocoC;

    int linhaInicial = 0;
    int linhas = 0;

    if (rank == 0) {
        linhas = n;
        blocoA.resize(static_cast<size_t>(n) * n);
        PreencherAleatorio(blocoA, n, 42);
        PreencherAleatorio(B, n, 99);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double inicio = MPI_Wtime();

    MPI_Bcast(B.data(), n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        int cabecalho[2];
        MPI_Recv(cabecalho, 2, MPI_INT, Pai(rank), TAG_CABECALHO,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        linhaInicial = cabecalho[0];
        linhas = cabecalho[1];

        blocoA.resize(static_cast<size_t>(linhas) * n);
        MPI_Recv(Dados(blocoA), linhas * n, MPI_DOUBLE, Pai(rank), TAG_BLOCO_A,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    blocoC.assign(static_cast<size_t>(linhas) * n, 0.0);

    if (TemFilhos(rank, totalProcessos)) {
        int filhoEsquerdo = FilhoEsquerdo(rank);
        int filhoDireito = FilhoDireito(rank);

        int linhasEsquerda = linhas / 2;
        int linhasDireita = linhas - linhasEsquerda;
        int deslocamentoDireita = linhasEsquerda * n;

        int cabecalhoEsquerdo[2] = {linhaInicial, linhasEsquerda};
        int cabecalhoDireito[2] = {linhaInicial + linhasEsquerda, linhasDireita};

        MPI_Send(cabecalhoEsquerdo, 2, MPI_INT, filhoEsquerdo, TAG_CABECALHO,
                 MPI_COMM_WORLD);
        MPI_Send(Dados(blocoA), linhasEsquerda * n, MPI_DOUBLE, filhoEsquerdo,
                 TAG_BLOCO_A, MPI_COMM_WORLD);

        MPI_Send(cabecalhoDireito, 2, MPI_INT, filhoDireito, TAG_CABECALHO,
                 MPI_COMM_WORLD);
        MPI_Send(DadosComDeslocamento(blocoA, deslocamentoDireita),
                 linhasDireita * n, MPI_DOUBLE, filhoDireito, TAG_BLOCO_A,
                 MPI_COMM_WORLD);

        MPI_Recv(Dados(blocoC), linhasEsquerda * n, MPI_DOUBLE, filhoEsquerdo,
                 TAG_BLOCO_C, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(DadosComDeslocamento(blocoC, deslocamentoDireita),
                 linhasDireita * n, MPI_DOUBLE, filhoDireito, TAG_BLOCO_C,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MultiplicarBloco(blocoA, B, blocoC, linhas, n);
    }

    if (rank != 0) {
        MPI_Send(Dados(blocoC), linhas * n, MPI_DOUBLE, Pai(rank), TAG_BLOCO_C,
                 MPI_COMM_WORLD);
    } else {
        double tempoTotal = MPI_Wtime() - inicio;

        std::cout << "Multiplicacao de matrizes com MPI (divisao e conquista)\n";
        std::cout << "Dimensao: " << n << " x " << n << "\n";
        std::cout << "Processos: " << totalProcessos << "\n\n";
        std::cout << "----------------------------------------\n";
        std::cout << "Tempo de execucao: " << std::fixed << std::setprecision(4)
                  << tempoTotal << " segundos\n";
        std::cout << "----------------------------------------\n";
    }

    MPI_Finalize();
    return 0;
}
