#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;


struct Posto {
    int distancia;  
    int tempo;      
};


int menorTempoRecursivo(const vector<Posto>& postos, int postoAtual, int distanciaTotal) {
    
    if (postoAtual == postos.size() - 1) {
        return postos[postoAtual].tempo;
    }
    
    
    int tempoMinimo = INT_MAX;
    
   
    for (int i = postoAtual + 1; i < postos.size(); i++) {
        
        if (postos[i].distancia - postos[postoAtual].distancia <= 100) {
            int tempo = postos[postoAtual].tempo + menorTempoRecursivo(postos, i, distanciaTotal);
            
           
            tempoMinimo = min(tempoMinimo, tempo);
        }
       
        else {
            break;
        }
    }
    
    return tempoMinimo;
}

// Função para calcular o menor tempo de abastecimento
int menorTempoAbastecimento(const vector<Posto>& postos, int distanciaTotal) {
    // Adiciona a cidade de origem como um "posto" com tempo 0
    vector<Posto> todosPostos = {{0, 0}};
    
    // Adiciona todos os postos
    for (const auto& posto : postos) {
        todosPostos.push_back(posto);
    }
    
    // Adiciona a cidade de destino como um "posto" com tempo 0
    todosPostos.push_back({distanciaTotal, 0});
    
    // Verifica se é possível chegar ao destino
    bool possivel = false;
    for (int i = 0; i < todosPostos.size() - 1; i++) {
        if (todosPostos[i + 1].distancia - todosPostos[i].distancia <= 100) {
            possivel = true;
        } else {
            possivel = false;
            break;
        }
    }
    
    if (!possivel) {
        return -1;  
    }
    

    return menorTempoRecursivo(todosPostos, 0, distanciaTotal);
}

int menorTempoPD(const vector<Posto>& postos, int distanciaTotal) {
    // Adiciona a cidade de origem como um "posto" com tempo 0
    vector<Posto> todosPostos = {{0, 0}};
    
    // Adiciona todos os postos
    for (const auto& posto : postos) {
        todosPostos.push_back(posto);
    }
    
    
    todosPostos.push_back({distanciaTotal, 0});
    
    int n = todosPostos.size();
    
  
    for (int i = 0; i < n - 1; i++) {
        if (todosPostos[i + 1].distancia - todosPostos[i].distancia > 100) {
            return -1;  
        }
    }
    
    
    vector<int> dp(n, INT_MAX);
    
    // Caso base: tempo para chegar à cidade de origem é 0
    dp[0] = 0;
    
    // Preenche a tabela de baixo para cima
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            
            if (todosPostos[i].distancia - todosPostos[j].distancia <= 100) {
                // Atualiza o tempo mínimo para chegar ao posto i
                dp[i] = min(dp[i], dp[j] + todosPostos[j].tempo);
            }
        }
    }
    
    // O resultado é o tempo mínimo para chegar à cidade de destino
    return dp[n - 1];
}

// Função para testar os algoritmos
void testarAbastecimento(const vector<Posto>& postos, int distanciaTotal) {
    cout << "Testando para distância total " << distanciaTotal << " com postos: " << endl;
    for (const auto& posto : postos) {
        cout << "Distância: " << posto.distancia << ", Tempo: " << posto.tempo << endl;
    }
    
    // Teste do método recursivo
    int tempoMinRecursivo = menorTempoAbastecimento(postos, distanciaTotal);
    cout << "Método Recursivo: " << tempoMinRecursivo << " unidades de tempo" << endl;
    
    // Teste do método com programação dinâmica
    int tempoMinPD = menorTempoPD(postos, distanciaTotal);
    cout << "Método com Programação Dinâmica: " << tempoMinPD << " unidades de tempo" << endl;
    
    cout << "-----------------------------------" << endl;
}

int main() {
   
    vector<Posto> postos1 = {
        {50, 10},
        {80, 5},
        {140, 20},
        {180, 15}
    };
    testarAbastecimento(postos1, 200);
    
    // Teste 2
    vector<Posto> postos2 = {
        {30, 5},
        {60, 10},
        {100, 15},
        {150, 5},
        {180, 20}
    };
    testarAbastecimento(postos2, 200);
    
    // Teste 3
    vector<Posto> postos3 = {
        {40, 30},
        {70, 10},
        {110, 5},
        {170, 20}
    };
    testarAbastecimento(postos3, 200);
    
    return 0;
}
