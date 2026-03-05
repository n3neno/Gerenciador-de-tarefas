#include<iostream>
#include<vector>
#include<memory>
#include<string>
#include<cctype>
#include<algorithm>
#include <fstream>
#include <sstream>
#include <clocale>

class tarefas {
private:
    bool conclusao = false;
    std::string descricao;
    int prioridade;
    double tempo;
public:
    ~tarefas() = default;
    tarefas(std::string d, int p, double t) : descricao(d), prioridade(p), tempo(t){}

    std::string to_csv(){
        return descricao + "," + std::to_string(prioridade) + "," + std::to_string(tempo) + "," + (conclusao ? "1" : "0");
    }
    std::string desc() const{
        return descricao;
    }

    void concluir_tarefa(bool estado){
        conclusao = estado;
    }

    void get_info(){
        std::cout << "Descrição: " << descricao << "\n";
        if (prioridade == 1){
            std::cout << "Prioridade: Baixa " << "\n";
        } else if (prioridade == 2){
             std::cout << "Prioridade: Média " << "\n";
        } else if (prioridade == 3){
            std::cout << "Prioridade: Alta" << "\n";
        }
        std::cout << "Tempo: " << tempo << "h" << "\n";
        if (conclusao == false){
            std::cout << "Conclusão: Pendente\n";
        } else {
            std::cout << "Conclusão: Concluída\n";
        }
        
    }

};

void salvar_csv(const std::vector<std::unique_ptr<tarefas>>& lista){
    std::ofstream arquivo("tarefas.csv");

    if (arquivo.is_open()){
        arquivo << "Descrição,Prioridade,Tempo,Concluído\n";

        for (const auto& tarefa : lista){
            arquivo << tarefa->to_csv() << "\n";
        }
        arquivo.close();
        std::cout << "Dados salvos com sucesso em 'tarefas.csv'!\n";
    } else {
        std::cerr << "Erro ao abrir o arquivo para salvar.\n";
    }
}
void carregar_csv(std::vector<std::unique_ptr<tarefas>>& lista){
    std::ifstream arquivo("tarefas.csv");
    std::string linha;

    
    if (!arquivo.is_open()){
        std::cout << "Nenhum arquivo de tarefas encontrado. Começando lista vazia.\n";
        return;
    }

    std::getline(arquivo, linha);
    
    while(std::getline(arquivo, linha)){
        std::stringstream ss(linha);
        std::string d, p_str, t_str, c_str;

        std::getline(ss, d, ',');
        std::getline(ss, p_str, ',');
        std::getline(ss, t_str, ',');
        std::getline(ss, c_str, ',');

        if (!d.empty()){
            int prioridade = std::stoi(p_str); 
            double tempo = std::stod(t_str);
            bool concluida = (c_str == "1");

            auto nova_tarefa = std::make_unique<tarefas>(d, prioridade, tempo);
            nova_tarefa->concluir_tarefa(concluida);
            lista.push_back(std::move(nova_tarefa));
        }
    }
    arquivo.close();
    std::cout << "Tarefas carregadas com sucesso!\n";
}

void concluir_t(std::vector<std::unique_ptr<tarefas>>& lista){
    std::string buscar;
    std::cout << "Digite a descrição exata da tarefa que deseja concluir: ";
    std::getline(std::cin >> std::ws, buscar);

    bool encontrada = false;

    for (const auto& tarefa: lista){
        if (tarefa->desc() == buscar){
            tarefa->concluir_tarefa(true);
            encontrada = true;
            break;
        }
    }
    if (!encontrada) {
        std::cout << "Tarefa não encontrada. Verifique se digitou corretamente.\n";
    }
}

void remover_t(std::vector<std::unique_ptr<tarefas>>& lista) {
    std::string buscar;
    std::cout << "Digite a descrição exata da tarefa que deseja REMOVER: ";
    std::getline(std::cin >> std::ws, buscar);

    
    for (auto it = lista.begin(); it != lista.end(); ++it) {
        if ((*it)->desc() == buscar) {
            lista.erase(it); 
            std::cout << "Tarefa '" << buscar << "' removida com sucesso!\n";
            return; 
        }
    }

    std::cout << "Tarefa não encontrada para remoção.\n";
}
int main(){
    setlocale(LC_ALL, "Portuguese");
    std::vector<std::unique_ptr<tarefas>> lista;
    carregar_csv(lista);
    int resposta = 0, opcao;
    do{

    std::cout << "======= *GERENCIADOR DE TAREFAS* =======\n";
    std::cout << "O que você deseja fazer ?[Opçoes: Adicionar tarefa(1), Ver tarefas existentes(2), Concluir uma tarefa(3), Remover uma tarefa]\n";
    std::cin >> opcao;
    std::cin.ignore(1000, '\n');


    } while (opcao != 1 && opcao != 2 && opcao != 3);
    
    if (opcao == 1){
        do{
        std::string de;
        double te;
        int pr;

        std::cout << "Adicione uma descrição: \n";
        std::getline(std::cin >> std::ws, de);
        
        
        
        while (true) {
            std::cout << "Adicione a prioridade [1 - Baixa / 2 - Media / 3- Alta]: \n";
            if (std::cin >> pr && (pr >= 1 && pr <= 3)) {
                break; 
            } else {
                std::cout << "Erro!!! Digite um valor válido: ): ";
                std::cin.clear(); 
                std::cin.ignore(1000, '\n'); 
            }
        }
        

        std::cout << "Adicione o tempo(h): \n";
        while (!(std::cin >> te)) {
        std::cout << "Erro!!! Digite um valor válido: ";
        std::cin.clear(); 
        std::cin.ignore(1000, '\n'); 
        }
         
        

        auto nova_tarefa = std::make_unique<tarefas>(de, pr, te);
        
        lista.push_back(std::move(nova_tarefa));

        std::cout << "Gostaria de adicionar uma tarefa? [Sim - 0 // Não - Qualquer outra tecla]" << "\n";
        if (!(std::cin >> resposta)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            resposta = 1; 
        }
        

    } while(resposta == 0);

    } else if (opcao == 3){
        concluir_t(lista);
    } else if (opcao == 2){
        std::cout << "Exibindo tarefas carregadas..\n";
    } else if (opcao == 4){
        remover_t(lista);
    }
    

    std::cout << "======= LISTA DE TAREFAS =======\n";
    for (const auto& t : lista){
        t->get_info();

        std::cout << "--------------------------------\n";

    }
    salvar_csv(lista);

    

    

}
