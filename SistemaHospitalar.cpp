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


