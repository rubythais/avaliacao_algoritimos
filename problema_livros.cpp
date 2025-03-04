#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

// Estrutura para representar um livro
struct Livro {
    int preco;
    int paginas;
};

// (a) Solução com backtracking
int maxPaginasBacktracking(const vector<Livro>& livros, int orcamento, int indice = 0) {
    // Caso base: se não há mais livros ou o orçamento acabou
    if (indice >= livros.size() || orcamento <= 0) {
        return 0;
    }
    
    // Não incluir o livro atual
    int naoIncluir = maxPaginasBacktracking(livros, orcamento, indice + 1);
    
    // Incluir o livro atual, se possível
    int incluir = 0;
    if (livros[indice].preco <= orcamento) {
        incluir = livros[indice].paginas + maxPaginasBacktracking(livros, orcamento - livros[indice].preco, indice + 1);
    }
    
    // Retorna o máximo entre incluir e não incluir
    return max(incluir, naoIncluir);
}

// (b) e (c) Solução com programação dinâmica
int maxPaginasPD(const vector<Livro>& livros, int orcamento) {
    int n = livros.size();
    
    // Tabela para armazenar resultados de subproblemas
    // dp[i][j] = máximo de páginas que podemos obter com os primeiros i livros e orçamento j
    vector<vector<int>> dp(n + 1, vector<int>(orcamento + 1, 0));
    
    // Preenche a tabela de baixo para cima
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= orcamento; j++) {
            // Não incluir o livro atual
            dp[i][j] = dp[i - 1][j];
            
            // Incluir o livro atual, se possível
            if (j >= livros[i - 1].preco) {
                dp[i][j] = max(dp[i][j], dp[i - 1][j - livros[i - 1].preco] + livros[i - 1].paginas);
            }
        }
    }
    
    return dp[n][orcamento];
}

// Função para imprimir a tabela de programação dinâmica
void imprimirTabelaPD(const vector<Livro>& livros, int orcamento) {
    int n = livros.size();
    
    // Cria a tabela
    vector<vector<int>> dp(n + 1, vector<int>(orcamento + 1, 0));
    
    // Preenche a tabela
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= orcamento; j++) {
            dp[i][j] = dp[i - 1][j];
            if (j >= livros[i - 1].preco) {
                dp[i][j] = max(dp[i][j], dp[i - 1][j - livros[i - 1].preco] + livros[i - 1].paginas);
            }
        }
    }
    
    // Imprime a tabela
    cout << "Tabela de Programação Dinâmica:" << endl;
    cout << "    ";
    for (int j = 0; j <= orcamento; j++) {
        cout << j << " ";
    }
    cout << endl;
    
    for (int i = 0; i <= n; i++) {
        cout << i << ": ";
        for (int j = 0; j <= orcamento; j++) {
            cout << dp[i][j] << " ";
        }
        cout << endl;
    }
}

// (e) Análise de desempenho
void analisarDesempenho(const vector<Livro>& livros, int orcamento) {
    // Mede o tempo do backtracking
    auto inicioBacktracking = chrono::high_resolution_clock::now();
    int resultadoBacktracking = maxPaginasBacktracking(livros, orcamento);
    auto fimBacktracking = chrono::high_resolution_clock::now();
    auto duracaoBacktracking = chrono::duration_cast<chrono::microseconds>(fimBacktracking - inicioBacktracking);
    
    // Mede o tempo da programação dinâmica
    auto inicioPD = chrono::high_resolution_clock::now();
    int resultadoPD = maxPaginasPD(livros, orcamento);
    auto fimPD = chrono::high_resolution_clock::now();
    auto duracaoPD = chrono::duration_cast<chrono::microseconds>(fimPD - inicioPD);
    
    cout << "Análise de Desempenho:" << endl;
    cout << "Backtracking: " << duracaoBacktracking.count() << " microssegundos" << endl;
    cout << "Programação Dinâmica: " << duracaoPD.count() << " microssegundos" << endl;
    cout << "Speedup: " << (double)duracaoBacktracking.count() / duracaoPD.count() << "x" << endl;
}

// Função para testar os algoritmos
void testarLivros(const vector<Livro>& livros, int orcamento) {
    cout << "Testando para orçamento " << orcamento << " com livros: " << endl;
    for (int i = 0; i < livros.size(); i++) {
        cout << "Livro " << i + 1 << ": Preço = " << livros[i].preco << ", Páginas = " << livros[i].paginas << endl;
    }
    
    // Teste do método com backtracking
    int maxPaginasBack = maxPaginasBacktracking(livros, orcamento);
    cout << "Método com Backtracking: " << maxPaginasBack << " páginas" << endl;
    
    // Teste do método com programação dinâmica
    int maxPaginasDinamica = maxPaginasPD(livros, orcamento);
    cout << "Método com Programação Dinâmica: " << maxPaginasDinamica << " páginas" << endl;
    
    // Imprime a tabela de programação dinâmica
    imprimirTabelaPD(livros, orcamento);
    
    // Análise de desempenho
    analisarDesempenho(livros, orcamento);
    
    cout << "-----------------------------------" << endl;
}

int main() {
    // Teste 1
    vector<Livro> livros1 = {
        {10, 100},
        {20, 250},
        {30, 300},
        {40, 350},
        {50, 400}
    };
    testarLivros(livros1, 60);
    
    // Teste 2
    vector<Livro> livros2 = {
        {5, 50},
        {10, 80},
        {15, 120},
        {20, 150}
    };
    testarLivros(livros2, 30);
    
    // Teste 3
    vector<Livro> livros3 = {
        {8, 70},
        {12, 100},
        {25, 200},
        {30, 220},
        {40, 300}
    };
    testarLivros(livros3, 50);
    
    return 0;
}
