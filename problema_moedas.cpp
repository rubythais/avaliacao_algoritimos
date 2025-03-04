#include <iostream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <string>

using namespace std;

// (a) e (b) Solução recursiva
int contarFormasRecursivo(const vector<int>& moedas, int valor, int indice = 0) {
    // Caso base: se o valor é 0, encontramos uma forma válida
    if (valor == 0) {
        return 1;
    }
    
    // Caso base: se o valor é negativo ou não há mais moedas, não há forma válida
    if (valor < 0 || indice >= moedas.size()) {
        return 0;
    }
    
    // Recursão: incluir a moeda atual ou não incluir
    return contarFormasRecursivo(moedas, valor - moedas[indice], indice) + 
           contarFormasRecursivo(moedas, valor, indice + 1);
}

// Solução recursiva com memoization
int contarFormasMemoization(const vector<int>& moedas, int valor, int indice, unordered_map<string, int>& memo) {
    // Cria uma chave única para o estado atual
    string chave = to_string(valor) + "_" + to_string(indice);
    
    // Verifica se já calculamos este estado
    if (memo.find(chave) != memo.end()) {
        return memo[chave];
    }
    
    // Caso base: se o valor é 0, encontramos uma forma válida
    if (valor == 0) {
        return 1;
    }
    
    // Caso base: se o valor é negativo ou não há mais moedas, não há forma válida
    if (valor < 0 || indice >= moedas.size()) {
        return 0;
    }
    
    // Recursão: incluir a moeda atual ou não incluir
    int resultado = contarFormasMemoization(moedas, valor - moedas[indice], indice, memo) + 
                    contarFormasMemoization(moedas, valor, indice + 1, memo);
    
    // Armazena o resultado na tabela de memoization
    memo[chave] = resultado;
    return resultado;
}

int contarFormasComMemoization(const vector<int>& moedas, int valor) {
    unordered_map<string, int> memo;
    return contarFormasMemoization(moedas, valor, 0, memo);
}

// (c) e (d) Solução com programação dinâmica
int contarFormasPD(const vector<int>& moedas, int valor) {
    int n = moedas.size();
    
    // Tabela para armazenar resultados de subproblemas
    // dp[i][j] = número de formas de obter o valor j usando as primeiras i moedas
    vector<vector<int>> dp(n + 1, vector<int>(valor + 1, 0));
    
    // Caso base: há uma forma de obter o valor 0 (não usar nenhuma moeda)
    for (int i = 0; i <= n; i++) {
        dp[i][0] = 1;
    }
    
    // Preenche a tabela de baixo para cima
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= valor; j++) {
            // Não incluir a moeda atual
            dp[i][j] = dp[i - 1][j];
            
            // Incluir a moeda atual, se possível
            if (j >= moedas[i - 1]) {
                dp[i][j] += dp[i][j - moedas[i - 1]];
            }
        }
    }
    
    return dp[n][valor];
}

// Função para imprimir a tabela de programação dinâmica
void imprimirTabelaPD(const vector<int>& moedas, int valor) {
    int n = moedas.size();
    
    // Cria a tabela
    vector<vector<int>> dp(n + 1, vector<int>(valor + 1, 0));
    
    // Caso base: há uma forma de obter o valor 0 (não usar nenhuma moeda)
    for (int i = 0; i <= n; i++) {
        dp[i][0] = 1;
    }
    
    // Preenche a tabela
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= valor; j++) {
            dp[i][j] = dp[i - 1][j];
            if (j >= moedas[i - 1]) {
                dp[i][j] += dp[i][j - moedas[i - 1]];
            }
        }
    }
    
    // Imprime a tabela
    cout << "Tabela de Programação Dinâmica:" << endl;
    cout << "    ";
    for (int j = 0; j <= valor; j++) {
        cout << j << " ";
    }
    cout << endl;
    
    for (int i = 0; i <= n; i++) {
        cout << i << ": ";
        for (int j = 0; j <= valor; j++) {
            cout << dp[i][j] << " ";
        }
        cout << endl;
    }
}

// (e) Análise de desempenho
void analisarDesempenho(const vector<int>& moedas, int valor) {
    // Mede o tempo da recursão
    auto inicioRecursivo = chrono::high_resolution_clock::now();
    int resultadoRecursivo = contarFormasRecursivo(moedas, valor);
    auto fimRecursivo = chrono::high_resolution_clock::now();
    auto duracaoRecursivo = chrono::duration_cast<chrono::microseconds>(fimRecursivo - inicioRecursivo);
    
    // Mede o tempo da memoization
    auto inicioMemo = chrono::high_resolution_clock::now();
    int resultadoMemo = contarFormasComMemoization(moedas, valor);
    auto fimMemo = chrono::high_resolution_clock::now();
    auto duracaoMemo = chrono::duration_cast<chrono::microseconds>(fimMemo - inicioMemo);
    
    // Mede o tempo da programação dinâmica
    auto inicioPD = chrono::high_resolution_clock::now();
    int resultadoPD = contarFormasPD(moedas, valor);
    auto fimPD = chrono::high_resolution_clock::now();
    auto duracaoPD = chrono::duration_cast<chrono::microseconds>(fimPD - inicioPD);
    
    cout << "Análise de Desempenho:" << endl;
    cout << "Recursivo: " << duracaoRecursivo.count() << " microssegundos" << endl;
    cout << "Memoization: " << duracaoMemo.count() << " microssegundos" << endl;
    cout << "Programação Dinâmica: " << duracaoPD.count() << " microssegundos" << endl;
    cout << "Speedup (Recursivo vs. PD): " << (double)duracaoRecursivo.count() / duracaoPD.count() << "x" << endl;
    cout << "Speedup (Recursivo vs. Memoization): " << (double)duracaoRecursivo.count() / duracaoMemo.count() << "x" << endl;
}

// Função para testar os algoritmos
void testarMoedas(const vector<int>& moedas, int valor) {
    cout << "Testando para valor " << valor << " com moedas: ";
    for (int moeda : moedas) {
        cout << moeda << " ";
    }
    cout << endl;
    
    // Teste do método recursivo
    int formasRecursivo = contarFormasRecursivo(moedas, valor);
    cout << "Método Recursivo: " << formasRecursivo << " formas" << endl;
    
    // Teste do método com memoization
    int formasMemo = contarFormasComMemoization(moedas, valor);
    cout << "Método com Memoization: " << formasMemo << " formas" << endl;
    
    // Teste do método com programação dinâmica
    int formasPD = contarFormasPD(moedas, valor);
    cout << "Método com Programação Dinâmica: " << formasPD << " formas" << endl;
    
    // Imprime a tabela de programação dinâmica
    imprimirTabelaPD(moedas, valor);
    
    // Análise de desempenho
    analisarDesempenho(moedas, valor);
    
    cout << "-----------------------------------" << endl;
}

int main() {
    // Teste 1: exemplo do enunciado
    vector<int> moedas1 = {2, 3, 5};
    testarMoedas(moedas1, 9);
    
    // Teste 2
    vector<int> moedas2 = {1, 2, 5};
    testarMoedas(moedas2, 10);
    
    // Teste 3
    vector<int> moedas3 = {1, 3, 4, 5};
    testarMoedas(moedas3, 7);
    
    return 0;
}
