#include "OrderBook.hpp"

#include <iostream>

// Ação de remover um Node da lista, ela faz o trabalho de "costurar" a lista
// ...-S5-S4-S3-... --> ...-S5-S3-...
OrderBook::Node::~Node() {
    this->next->prev = this->prev; // prev.S3 = S5
    this->prev->next = this->next; // next.S5 = S3
}

OrderBook::OrderBook()
    : transactionsHead(nullptr), transactionsTail(nullptr), buyCount(0), sellCount(0), transactionCount(0) {
    // Criando uma ordem fictícia.
    this->head = new Node(Order(0, 'H', 0, 0));
    // Queremos uma lista circular, vamos fazer head apontar pra si mesma
    this->head->prev = this->head;
    this->head->next = this->head;
}

OrderBook::~OrderBook() {
    // Esvazia a lista circular deletando os nós adjacentes ao head
    while (this->head->next != this->head) {
        delete this->head->next;
    }
    delete this->head;

    // Remove de head até tail
    TransactionNode* actual = this->transactionsHead;
    while (actual != nullptr) {
        TransactionNode* temp = actual;
        actual = actual->next;
        delete temp;
    }
}

// Se for do tipo 'S', percorre todos os Bn, se não encontrar correspondência, é inserido no fim dos Sn
// submit(S3): S2-S1-H-B1-B2-B3 -> S3-S2-S1-H-B1-B2-B3  ou S2-S1-H-B1-B3 (Se B2 corresponder com S3)
bool OrderBook::submit(Order order) {
    int orderprice = order.getPrice();

    if (order.getType() == 'S') {

        Node* chosen = nullptr;
        Node* actual = this->head->next;
        // Começa de B1, percorre todos os Bn, para ao encontrar um Node tipo 'S' ou 'H'
        for (; actual->order.getType() == 'B'; actual = actual->next) {
            float actualPrice = actual->order.getPrice();
            // Se for possível executar a venda
            if (actualPrice >= orderprice) {
                if (chosen == nullptr) {
                    chosen = actual;
                }
                // Se o preço de compra for maior, troque o best para este
                else if (chosen->order.getPrice() < actualPrice) {
                    chosen = actual;
                }
                // No Caso de empate e o timestamp de chosen já é menor (pois nossa estrutura é ordenado), não precisa
                // verificar.
            }
        }

        // Se não encontrou correspondência, coloque a ordem no registro
        // Por exemplo, se antes nosso registro era S2-S1-H-B1-B2-B3, então agora será
        // S3-S2-S1-H-B1-B2-B3
        // ^ novo Node a ser adicionado
        if (chosen == nullptr) {
            // Lembrando, actual = S2
            Node* newNode = new Node(order);
            newNode->prev = actual->prev; // prev.S3 = B3
            newNode->next = actual;       // next.S3 = S2
            actual->prev->next = newNode; // next.B3 = S3
            actual->prev = newNode;       // prev.S2 = S3

            this->sellCount += 1;
            return false;
        }
        // Se chegou aqui, então encontrou correspondência, remova o correspondido da lista e adicione a transação
        registerTransaction(chosen->order.getId(), order.getId(), chosen->order.getPrice());
        // O destrutor do Node já faz o trabalho de "costurar a lista"
        delete chosen;
        this->buyCount--;
        return true;

        // Processo Análogo
    } else if (order.getType() == 'B') {
        Node* chosen = nullptr;
        Node* actual = this->head->prev;
        // Começa de S1, percorre todos os Sn, para ao encontrar um Node tipo 'B' ou 'H'
        for (; actual->order.getType() == 'S'; actual = actual->prev) {
            float actualPrice = actual->order.getPrice();
            // Se for possível executar a venda
            if (actualPrice <= orderprice) {
                if (chosen == nullptr) {
                    chosen = actual;
                }
                // Se o preço de compra for menor, troque o best para este
                else if (chosen->order.getPrice() > actualPrice) {
                    chosen = actual;
                }
                // No Caso de empate e o timestamp de chosen já é menor (pois nossa estrutura é ordenado), não precisa
                // verificar.
            }
        }

        // Se não encontrou correspondência, coloque a ordem no registro
        // Por exemplo, se antes nosso registro era S2-S1-H-B1-B2-B3, então agora será
        // S2-S1-H-B1-B2-B3-B4
        //                   ^ novo Node a ser adicionado
        if (chosen == nullptr) {
            // Lembrando, actual = B3
            Node* newNode = new Node(order);
            newNode->next = actual->next; // next.B4 = S2
            newNode->prev = actual;       // prev.B4 = B3
            actual->next->prev = newNode; // prev.S2 = B4
            actual->next = newNode;       // next.B3 = B4

            this->buyCount += 1;
            return false;
        }
        // Se chegou aqui, então encontrou correspondência, remova o correspondido da lista e adicione a transação
        registerTransaction(order.getId(), chosen->order.getId(), chosen->order.getPrice());
        // O destrutor do Node já faz o trabalho de "costurar a lista"
        delete chosen;
        this->sellCount--;
        return true;
    } else {
        std::cerr << "Erro, não existe ordem com esse tipo" << std::endl;
        return false;
    }
}

bool OrderBook::cancel(int id) {
    // Percorre até chegar em H
    for (Node* actual = this->head->next; actual->order.getType() != 'H'; actual = actual->next) {
        if (actual->order.getId() == id) {
            if (actual->order.getType() == 'B') this->buyCount--;
            else if (actual->order.getType() == 'S') this->sellCount--;
            delete actual;
            return true;
        }
    }
    return false;
}

// Não esquecer do delete na main.
// ====================== Getters =================================================

Order* OrderBook::getBuyOrders(int* n) {
    *n = this->buyCount;
    Order* orders = new Order[this->buyCount];
    Node* actual = this->head->next;

    for (int index = 0; index < this->buyCount; index++) {
        orders[index] = actual->order;
        actual = actual->next;
    }
    return orders;
}
Order* OrderBook::getSellOrders(int* n) {
    *n = this->sellCount;
    Order* orders = new Order[this->sellCount];
    Node* actual = this->head->prev;

    for (int index = 0; index < this->sellCount; index++) {
        orders[index] = actual->order;
        actual = actual->prev;
    }
    return orders;
}

Transaction* OrderBook::getTransactions(int* n) {
    *n = this->transactionCount;
    Transaction* transactions = new Transaction[transactionCount];
    TransactionNode* actual = this->transactionsHead;

    for (int index = 0; index < this->transactionCount; index++) {
        transactions[index] = actual->transaction;
        actual = actual->next;
    }
    return transactions;
}
// ===============================================================================

// ================== Print ======================================================

void OrderBook::printBuyOrders() {
    std::cout << std::endl;
    std::cout << "Buy Orders:" << std::endl;

    if (buyCount == 0) {
        std::cout << "(empty)" << std::endl;
        return;
    }

    for (Node* actual = this->head->next; actual->order.getType() == 'B'; actual = actual->next) {
        std::cout << "[" << actual->order.getId() << " | " << actual->order.getPrice() << " | "
                  << actual->order.getTimestamp() << "]" << std::endl;
    }

    return;
}

void OrderBook::printSellOrders() {
    std::cout << std::endl;
    std::cout << "Sell Orders:" << std::endl;

    if (sellCount == 0) {
        std::cout << "(empty)" << std::endl;
        return;
    }

    for (Node* actual = this->head->prev; actual->order.getType() == 'S'; actual = actual->prev) {
        std::cout << "[" << actual->order.getId() << " | " << actual->order.getPrice() << " | "
                  << actual->order.getTimestamp() << "]" << std::endl;
    }

    return;
}

void OrderBook::printTransactions() {
    std::cout << std::endl;
    std::cout << "Transactions:" << std::endl;

    if (transactionCount == 0) {
        std::cout << "(empty)" << std::endl;
        return;
    }

    for (TransactionNode* actual = this->transactionsHead; actual != nullptr; actual = actual->next) {
        std::cout << "[" << actual->transaction.getBuyOrderId() << " | " << actual->transaction.getSellOrderId()
                  << " | " << actual->transaction.getExecutionPrice() << "]" << std::endl;
    }
    return;
}

// ===============================================================================

void OrderBook::registerTransaction(int buy_order_id, int sell_order_id, float execution_price) {
    if (this->transactionCount == 0) {
        this->transactionsHead = new TransactionNode(Transaction(buy_order_id, sell_order_id, execution_price));
        this->transactionsTail = this->transactionsHead;
    } else {
        // só faz sentido ter next se transaction_count > 0
        this->transactionsTail->next = new TransactionNode(Transaction(buy_order_id, sell_order_id, execution_price));
        this->transactionsTail = this->transactionsTail->next;
    }
    this->transactionCount += 1;
}
