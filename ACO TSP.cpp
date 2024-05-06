#include <bits/stdc++.h>
#include <chrono>
#define int long long

using namespace std;

int qtdCidades;

int Roleta(vector<int>& escolhas, vector<vector<double>> &probabilidades, int atual, double total, int tamanho) {
    
    double roleta = ((double)rand()/RAND_MAX);
    double escolha = 0.0;
    
    double x = (1.0-total)/double(tamanho);
    
    for (int i = 0; i < escolhas.size(); i++) {
        //cout<<roleta<<" teste "<<probabilidades[atual][escolhas[i]]<<" + "<<x <<"  =  "<<probabilidades[atual][escolhas[i]]+x<<endl;
        escolha += (probabilidades[atual][escolhas[i]])+x;
        if (roleta <= escolha){
            return escolhas[i]; 
        }
    }
}

double CalculoProbabilidade(int atual, int vizinho, vector<vector<pair<int,double>>> &grafo, vector<vector<double>> &Feromonios, double alfa, double beta){
    double denominador = 0.0;
    for(int i = 0; i < qtdCidades/4; i++) {
        if(atual!=i){
            double Txy = Feromonios[atual][i];
            double Nxy = 0.75/grafo[atual][i].second;
            denominador += pow(Txy, alfa) * pow(Nxy, beta);
        }
    }
    
    if (denominador == 0) return 0; 

    double TxyK = Feromonios[atual][vizinho];
    double NxyK = 2.0/grafo[atual][vizinho].second;
    double numerador = pow(TxyK, alfa) * pow(NxyK, beta);
    return numerador/denominador;
}

double distanciaEntrePontos(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}


void DefinicaoDeParametros(){
  //Definição dos parâmetros iniciais.

    // - qtdFormigas representa a quantidade de formigas fazendo rotas.
    // - iteracoes é a quantidade de iteracoes de cada formiga
    // - alfa controla a influencia do feromonio na prob. de transicao
    // - beta controla a influencia da atratividade  na prob. de transicao
    // - evaporacao representa a taxa de evaporação do feromonio

    int qtdFormigas = 5, qtdIteracoes = 50;
    double alfa = 1.0, beta = 2.0, p = 0.01, Q = 20, fator_ampliacao = 10;
    puts("\nDigite a quantidade de cidades existentes:\n");


    cin >> qtdCidades;
    vector<vector<pair<int, double>>> Grafo(qtdCidades+1, vector<pair<int, double>> (qtdCidades+1, {0, 0})); //Grafo das cidades
    vector<pair<int, pair<int, int>>> GrafoTransitorio(qtdCidades+1); //Grafo onde serão lidas as entradas
    


     for(int i = 0; i < qtdCidades; i++){ 
            int vertice, x, y;
            cin >> vertice >> x >> y;
            vertice--;
            GrafoTransitorio[i] = {vertice, {x, y}};
    }
    
     //Criando o Grafo
      for(int i = 0; i < qtdCidades; i++)
      {
            //v1 e v2 armazenam os vertices que serão interligados 
          
            int v1 = GrafoTransitorio[i].first;
            //cout<<v1<<endl;
            //coordenadas do v1 
            int x1 = GrafoTransitorio[i].second.first;
            int y1 = GrafoTransitorio[i].second.second;
            
        for(int j = i+1; j < qtdCidades ; j++)
        {
            int v2 = GrafoTransitorio[j].first;
            //cout<<v2<<" ";
        
            //coordenadas do v2
            int x2 = GrafoTransitorio[j].second.first;
            int y2 = GrafoTransitorio[j].second.second;
            
            double distancia = distanciaEntrePontos(x1, y1, x2, y2);
            
            Grafo[v1][v2] = make_pair(v2, distancia);
            Grafo[v2][v1] = make_pair(v1, distancia);
        }
        //cout<<endl;
    }
   
 
    cout<<fixed<<setprecision(4);

    vector<vector<double>> Feromonios(qtdCidades, vector<double> (qtdCidades)); //Matriz dos feromonios
    vector<vector<double>> Probabilidades(qtdCidades, vector<double> (qtdCidades)); //Matriz de probabilidades
    

    for(int i = 0; i < qtdCidades; i++){
        for(int j = 0; j < qtdCidades; j++){
            Feromonios[i][j]= 0.1;
            Probabilidades[i][j] = -1;
            
        }
    }
    //Preenchimento da matriz de probabilidades.
    for(int i = 0; i < qtdCidades; i++){
		for(int j=0;j<qtdCidades;j++){
		    if(i!=j){
			    Probabilidades[i][j] = CalculoProbabilidade(i, j, Grafo, Feromonios, alfa, beta);
		    }
		}
	}
	
    pair< vector <pair<int ,int>>, double> MelhorCaminho (vector<pair<int, int>> (1), INT_MAX);
    pair< vector <pair<int ,int>>, double> PiorCaminho (vector<pair<int, int>> (1), INT_MAX);

    double aux = INT_MAX;
    double aux2 = INT_MIN;

    //Para cada iteração:
    for(int i = 0; i < qtdIteracoes; i++){
        
        //Para cada formiga:
        vector<pair<vector<pair<int ,int> >, double>> BoxCaminhos;
        for(int j = 0; j < qtdFormigas; j++){
            //Colocar a formiga em um nó inicial.
            int Atual = 0; //MUDAR PARA ALEATORIO
            
            vector<pair<int, int>> Caminho; //Caminho percorrido pela formiga j.
            Caminho.push_back({Atual, Atual});
            bool visCidade[qtdCidades+1] = {0};
            visCidade[Atual] = 1;
            
            double total_caminhado = 0;
            //Visitar cada cidade
            while(Caminho.size() < qtdCidades){ //sair da cidade 0, passar por todas as cidades 1 vez e retornar à cidade 0.
                int x = 0;
                double total = 0, totalprobs = 0;
                vector<int> Escolhas; 
                
                for(int i = 0; i < Grafo[Atual].size(); i++){

                    if(Atual != i && !visCidade[Grafo[Atual][i].first] && Grafo[Atual][i].second != 0){
                        Escolhas.push_back(Grafo[Atual][i].first);
                        totalprobs += Probabilidades[Atual][Grafo[Atual][i].first];

                    }
            
                }
                //Selecionar o próximo nó.
                int tamanho = Escolhas.size();
                int at = Roleta(Escolhas, Probabilidades, Atual, totalprobs, tamanho);

                Caminho.push_back({Atual, at});

                total_caminhado += Grafo[Atual][at].second;
                Atual = at;
                visCidade[Atual] = 1;

            }

            //Iterar sobre todos os vizinhos do ultimo caminho para checar se ele se encontra com 0
            for(int k = 0; k<Grafo[Atual].size(); k++){
                if(Grafo[Atual][k].first == 0 && Grafo[Atual][k].second != 0){
                    Caminho.push_back({Atual, Grafo[Atual][k].first});
                    total_caminhado+= Grafo[Atual][k].second;
                    break;
                }
            }
            
               pair<vector<pair<int ,int>>, double> elemento = make_pair(Caminho, total_caminhado);
               BoxCaminhos.push_back(elemento);
            
        }
    


       

         //Pegar Melhor e Pior caminhos
         //Iterar sobre cada caminho construido
         for(int w=0; w<BoxCaminhos.size(); w++){
             
            //Verificar se o caminho W é válido
            if(BoxCaminhos[w].first.size() == qtdCidades+1)
            {
                //Interar sobre o caminho W
                for(int k=0;k<BoxCaminhos[w].first.size();k++)
                {
                     int x = BoxCaminhos[w].first[k].first;
                     int y = BoxCaminhos[w].first[k].second;
                     int dist = BoxCaminhos[w].second;
                     
                     
                     if(dist < aux) MelhorCaminho = BoxCaminhos[w], aux = dist;
                     if(dist > aux2) PiorCaminho = BoxCaminhos[w], aux2 = dist;
                    
                }
            }
         }
         
         
          /// Evaporação Geral
        for(int w = 0; w < qtdCidades; w++){
            for(int k = 0; k < qtdCidades; k++) {
                    Feromonios[w][k] *= (1 - p); // Evaporação padrão para os demais caminhos
            }
        }
        
        
         //Evaporar o Pior Caminho
        for(int w = 0; w < PiorCaminho.first.size(); w++)
        {
            int x = PiorCaminho.first[w].first;
            int y = PiorCaminho.first[w].second;
            
            Feromonios[x][y] *= (1 - p * fator_ampliacao);
        }

         
         
            //Atualizar Feromonios
        
          //Iterar sobre cada caminho construido
         for(int w=0; w<BoxCaminhos.size(); w++){
             
            //Verificar se o caminho W é válido
            if(BoxCaminhos[w].first.size() == qtdCidades+1)
            {
                //Interar sobre o caminho W
                for(int k=0;k<BoxCaminhos[w].first.size();k++)
                {
                     int x = BoxCaminhos[w].first[k].first;
                     int y = BoxCaminhos[w].first[k].second;
                     int dist = BoxCaminhos[w].second;
                     
                     
                     //desconsiderar 0 0
                      if(x || y)
                      {
                            Feromonios[x][y] += Q/dist;
                            Feromonios[y][x] += Q/dist;
                            Probabilidades[x][y] = CalculoProbabilidade(x, y, Grafo, Feromonios, alfa, beta);
                            Probabilidades[y][x] = CalculoProbabilidade(y, x, Grafo, Feromonios, alfa, beta);
                      }
                      
                }
            }
         }
      
         
      
         
     
    }
    
    cout<<"Melhor Caminho: "<<MelhorCaminho.second<<endl<<"Pior Caminho: "<<PiorCaminho.second<<endl;
 
}

signed main() {
    auto start = chrono::high_resolution_clock::now(); // Marca o tempo de início

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    srand(seed);
    //Inicialização e definição da função de atratividade como o inverso da distancia. (1/Dxy).
    DefinicaoDeParametros();
    
    auto end = chrono::high_resolution_clock::now(); // Marca o tempo de término
    double time_taken = chrono::duration_cast<chrono::milliseconds>(end - start).count(); // Calcule o tempo decorrido

    cout << "Tempo de execução: " << time_taken/1000 << " segundos" << endl;


}