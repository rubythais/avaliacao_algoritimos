#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <unordered_map>
#include <string>

using namespace std;

// (a) Solução usando método guloso
vector<int> trocoGuloso(const vector<int>& moedas, int valor) {
    vector<int> resultado(moedas.size(), 0);
    int valorRestante = valor;
    
    // Ordena as moedas em ordem decrescente
    vector<int> moedasOrdenadas = moedas;
    sort(moedasOrdenadas.rbegin(), moedasOrdenadas.rend());
    
    // Mapeia o índice das moedas ordenadas para o índice original
    vector<int> indiceOriginal(moedasOrdenadas.size());
    for (int i = 0; i < moedas.size(); i++) {
        for (int j = 0; j < moedasOrdenadas.size(); j++) {
            if (moedasOrdenadas[j] == moedas[i]) {
                indiceOriginal[j] = i;
                break;
            }
        }
    }
    
    // Aplica o algoritmo guloso
    for (int i = 0; i < moedasOrdenadas.size(); i++) {
        while (valorRestante >= moedasOrdenadas[i]) {
            resultado[indiceOriginal[i]]++;
            valorRestante -= moedasOrdenadas[i];
        }
    }
    
    return resultado;
}

// (c) Solução recursiva
int trocoRecursivo(const vector<int>& moedas, int valor) {
    // Caso base: se o valor é 0, não precisamos de moedas
    if (valor == 0) return 0;
    
    // Caso base: se o valor é negativo, não é possível formar o troco
    if (valor < 0) return INT_MAX;
    
    // Inicializa o resultado como infinito
    int minMoedas = INT_MAX;
    
    // Tenta cada moeda
    for (int moeda : moedas) {
        int subProblema = trocoRecursivo(moedas, valor - moeda);
        
        // Se o subproblema tem solução e é melhor que a atual
        if (subProblema != INT_MAX && subProblema + 1 < minMoedas) {
            minMoedas = subProblema + 1;
        }
    }
    
    return minMoedas;
}

// (e) Solução recursiva com memoization
int trocoMemoization(const vector<int>& moedas, int valor, unordered_map<int, int>& memo) {
    // Verifica se já calculamos este valor
    if (memo.find(valor) != memo.end()) {
        return memo[valor];
    }
    
    // Caso base: se o valor é 0, não precisamos de moedas
    if (valor == 0) return 0;
    
    // Caso base: se o valor é negativo, não é possível formar o troco
    if (valor < 0) return INT_MAX;
    
    // Inicializa o resultado como infinito
    int minMoedas = INT_MAX;
    
    // Tenta cada moeda
    for (int moeda : moedas) {
        int subProblema = trocoMemoization(moedas, valor - moeda, memo);
        
        // Se o subproblema tem solução e é melhor que a atual
        if (subProblema != INT_MAX && subProblema + 1 < minMoedas) {
            minMoedas = subProblema + 1;
        }
    }
    
    // Armazena o resultado na tabela de memoization
    memo[valor] = minMoedas;
    return minMoedas;
}

int trocoComMemoization(const vector<int>& moedas, int valor) {
    unordered_map<int, int> memo;
    return trocoMemoization(moedas, valor, memo);
}

// (d) Solução com programação dinâmica
int trocoPD(const vector<int>& moedas, int valor) {
    // Tabela para armazenar resultados de subproblemas
    vector<int> dp(valor + 1, INT_MAX);
    
    // Caso base: 0 moedas para formar o valor 0
    dp[0] = 0;
    
    // Preenche a tabela de baixo para cima
    for (int i = 1; i <= valor; i++) {
        // Tenta cada moeda
        for (int moeda : moedas) {
            if (i - moeda >= 0 && dp[i - moeda] != INT_MAX) {
                dp[i] = min(dp[i], dp[i - moeda] + 1);
            }
        }
    }
    
    return dp[valor] == INT_MAX ? -1 : dp[valor];
}

// Função para imprimir o resultado do troco
void imprimirResultado(const vector<int>& moedas, const vector<int>& resultado) {
    cout << "Solução: ";
    int total = 0;
    for (int i = 0; i < moedas.size(); i++) {
        if (resultado[i] > 0) {
            cout << resultado[i] << " moeda(s) de " << moedas[i] << ", ";
            total += resultado[i];
        }
    }
    cout << "Total: " << total << " moeda(s)" << endl;
}

// Função para testar os algoritmos
void testarTroco(const vector<int>& moedas, int valor) {
    cout << "Testando para valor " << valor << " com moedas: ";
    for (int moeda : moedas) {
        cout << moeda << " ";
    }
    cout << endl;
    
    // Teste do método guloso
    vector<int> resultadoGuloso = trocoGuloso(moedas, valor);
    cout << "Método Guloso: ";
    imprimirResultado(moedas, resultadoGuloso);
    
    // Teste do método recursivo
    int minMoedasRecursivo = trocoRecursivo(moedas, valor);
    cout << "Método Recursivo: " << minMoedasRecursivo << " moeda(s)" << endl;
    
    // Teste do método com memoization
    int minMoedasMemo = trocoComMemoization(moedas, valor);
    cout << "Método com Memoization: " << minMoedasMemo << " moeda(s)" << endl;
    
    // Teste do método com programação dinâmica
    int minMoedasPD = trocoPD(moedas, valor);
    cout << "Método com Programação Dinâmica: " << minMoedasPD << " moeda(s)" << endl;
    
    cout << "-----------------------------------" << endl;
}

int main() {
    // Teste 1: Sistema de moedas canônico (onde o método guloso funciona)
    vector<int> moedas1 = {1, 2, 5, 10, 20, 50, 100, 200, 500};
    testarTroco(moedas1, 7);
    
    // Teste 2: Sistema de moedas não canônico (onde o método guloso pode falhar)
    vector<int> moedas2 = {1, 3, 4};
    testarTroco(moedas2, 6);
    
    // Teste 3: Outro exemplo onde o método guloso falha
    vector<int> moedas3 = {1, 3, 5};
    testarTroco(moedas3, 13);
    
    return 0;
}
