//Compilar:  g++ -std=c++17 -Wall -o atendimento atendimento_template.cpp 
//Rodar:     ./atendimento
#include <iostream>
#include <string>
using namespace std;

//ESTRUTURA BASE
struct No {
    string nome;
    int    urgencia;   // 1=baixa, 2=media, 3=alta (usado apenas por Paciente)
    No*    prox;
    No(const string& n, int u = 0) : nome(n), urgencia(u), prox(nullptr) {}
};

//R1. FILA DE PACIENTES (estrutura FIFO com 2 pointers)
struct FilaPacientes {
    No* frente = nullptr;
    No* tras   = nullptr;

    //TODO 1: implemente enqueue(nome, urgencia)
    void enqueue(const string& nome, int urgencia) {
        No* novo = new No(nome, urgencia);
        if (isEmpty()) {
            frente = novo;
            tras = novo;
        } else {
            tras->prox = novo;
            tras = novo;
        }
    }

    //TODO 2: Implemente dequeue()
    string dequeue() {
        if (isEmpty()) return "";
        
        No* temp = frente;
        string nome = temp->nome;
        frente = frente->prox;
        
        if (frente == nullptr) {
            tras = nullptr;
        }
        
        delete temp;
        return nome;
    }

    //TODO 3: Implemente frente_peek() — ver quem está na frente sem remover
    string frente_peek() const {
        if (isEmpty()) return "";
        return frente->nome;
    }

    bool isEmpty() const { return frente == nullptr; }

    //BÔNUS: Implemente tamanho() que conta os nós na fila
    int tamanho() const {
        int contador = 0;
        No* atual = frente;
        while (atual != nullptr) {
            contador++;
            atual = atual->prox;
        }
        return contador;
    }

    ~FilaPacientes() {
        while (!isEmpty()) dequeue();
    }
};



//R2. HISTÓRICO COM PILHA (estrutura LIFO)
struct PilhaHistorico {
    No* topo = nullptr;

    //TODO 4: implemente push(nome)
    void push(const string& nome) {
        No* novo = new No(nome);
        novo->prox = topo;
        topo = novo;
    }

    //TODO 5: Implemente pop()
    string pop() {
        if (isEmpty()) return "";
        
        No* temp = topo;
        string nome = temp->nome;
        topo = topo->prox;
        
        delete temp;
        return nome;
    }

    //TODO 6: implemente peek()
    string peek() const {
        if (isEmpty()) return "";
        return topo->nome;
    }

    bool isEmpty() const { return topo == nullptr; }

    ~PilhaHistorico() {
        while (!isEmpty()) pop();
    }
};


//R3. LISTA DE MÉDICOS DISPONÍVEIS (lista encadeada)

struct ListaMedicos {
    No* head = nullptr;

    //TODO 7: implemente adicionarMedico(nome)
    void adicionarMedico(const string& nome) {
        No* novo = new No(nome);
        if (head == nullptr) {
            head = novo;
        } else {
            No* atual = head;
            while (atual->prox != nullptr) {
                atual = atual->prox;
            }
            atual->prox = novo;
        }
    }

    //TODO 8: implemente chamarMedico()
    string chamarMedico() {
        if (!temMedico()) return "";
        
        No* temp = head;
        string nome = temp->nome;
        head = head->prox;
        
        delete temp;
        return nome;
    }

    //TODO 9. implemente percorrer()
    void percorrer(const string& prefixo = "") const {
        if (!prefixo.empty()) cout << prefixo << ": ";
        if (!temMedico()) {
            cout << "(lista vazia)\n";
            return;
        }
        
        No* p = head;
        while (p != nullptr) {
            cout << p->nome;
            if (p->prox != nullptr) cout << " -> ";
            p = p->prox;
        }
        cout << "\n";
    }

    bool temMedico() const { return head != nullptr; }

    ~ListaMedicos() {
        while (head) {
            No* t = head;
            head  = head->prox;
            delete t;
        }
    }
};


// R4. RELATÓRIO E HISTÓRICO COMPLETO


//TODO 10 implemente a função imprimirHistorico(PilhaHistorico&)
void imprimirHistorico(PilhaHistorico& hist) {
    cout << "\n=== Relatório de Atendimentos ===\n";
    if (hist.isEmpty()) {
        cout << "(nenhum atendimento registrado)\n";
        return;
    }

    PilhaHistorico aux;

    //desempilha da pilha original e empilha na auxiliar, imprimindo na ordem (topo à base)
    while (!hist.isEmpty()) {
        string at = hist.pop();
        cout << "- " << at << "\n";
        aux.push(at);
    }

    //restaura a pilha original desempilhando da auxiliar
    while (!aux.isEmpty()) {
        hist.push(aux.pop());
    }
}


//PROGRAMA PRINCIPAL — integra as 4 estruturas
int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8"); // Ou setlocale(LC_ALL, "Portuguese");
    cout << "\n";
    cout << "Sistema de Atendimento — Squad\n";
    cout << "\n\n";

    //Criar as estruturas
    FilaPacientes   fila;
    PilhaHistorico historico;
    ListaMedicos    medicos;

    //Cadastrar médicos (R3) ────────────────────────────────
    medicos.adicionarMedico("Dra. Ana Lima");
    medicos.adicionarMedico("Dr. Bruno Costa");
    medicos.adicionarMedico("Dra. Carla Ramos");
    medicos.percorrer("Médicos disponíveis");

    //Pacientes chegam na fila (R1) ─────────────────────────
    cout << "\nPacientes chegando:\n";
    fila.enqueue("Eduardo (urgencia=3)", 3);
    fila.enqueue("Fernanda (urgencia=1)", 1);
    fila.enqueue("Gabriel (urgencia=2)", 2);
    fila.enqueue("Helena (urgencia=1)", 1);

    cout << "Próximo a ser atendido: " << fila.frente_peek() << "\n";
    cout << "Total na fila: " << fila.tamanho() << "\n";

    //Simulação de atendimentos
    cout << "\nSimulação de atendimentos:\n";
    int atend = 1;
    while (!fila.isEmpty() && medicos.temMedico()) {
        string paciente = fila.dequeue();
        string medico   = medicos.chamarMedico();

        cout << "  [" << atend++ << "] " << medico
             << " atende " << paciente << "\n";

        //Registra no histórico (R2)
        historico.push(medico + " → " + paciente);

        //Médico volta para a fila depois de atender
        medicos.adicionarMedico(medico);
    }

    //Último atendimento (peek na pilha)
    cout << "\nÚltimo atendimento registrado (peek): " << historico.peek() << "\n";

    //Relatório final (R4)
    imprimirHistorico(historico);

    return 0;
}