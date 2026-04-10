#pragma once

#include "Order.hpp"
#include "Transaction.hpp"

class OrderBook {

  private:
    struct Node {
        Order order; // 16 bytes
        Node* prev;  // 8 bytes
        Node* next;  // 8 bytes

        // Inicializando de modo mais eficiente.
        Node(const Order& o): order(o), prev(nullptr), next(nullptr) {}

        ~Node();
    };

    struct TransactionNode {
        Transaction transaction; // 12 bytes
        TransactionNode* next;   // 8 bytes

        // Inicializando de modo mais eficiente.
        TransactionNode(const Transaction& t): transaction(t), next(nullptr) {}

        ~TransactionNode() = default;
    };

  private:
    // usaremos uma lista encadeada para representar ambas ordens de venda e compra.
    // A lista será do tipo S3-S2-S1-H-B1-B2-B3-B4
    // onde Sn são as ordens de venda, Bn as de compra e H nossa head.
    // Ordens inseridas primeiro (menor timestamp) estará mais próximo de H (para manter ordenação)
    // a lista será circular, ou seja, o prev do S mais a esquerda aponta para o B mais a direita e vise-versa.
    // Escolhemos esse modelo para processamento mais rápido de inserção e aplicar o que foi visto em aula.
    // A escolha de uma lista circular é para maior velocidade de inserção no "fim" da lista caso não haja
    // correspondencia sem precisar percorrer uma outra lista caso usássemos duas listas separadas.

    Node* head;

    // precisamos saber quantas ordens temos para melhor organização
    int buyCount = 0, sellCount = 0;

    // Um lista encadeada, usando Tail para rápida inserção e Head para ler na ordem certa
    TransactionNode* transactionsHead;
    TransactionNode* transactionsTail;
    int transactionCount = 0;

  public:
    OrderBook();
    ~OrderBook();

    bool submit(Order order);
    bool cancel(int id);

    Order* getBuyOrders(int* n);
    Order* getSellOrders(int* n);
    Transaction* getTransactions(int* n);

    void printBuyOrders();
    void printSellOrders();
    void printTransactions();

  private:
    void registerTransaction(int buy_order_id, int sell_order_id, float execution_price);
};
