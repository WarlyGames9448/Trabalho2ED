#include "OrderBook.hpp"

#include <iostream>

// Classe para melhor organização dos exemplos e para garantir que os Timestamps fiquem ordenados.
class System {
  public:
    void print() {
        ob.printBuyOrders();
        ob.printSellOrders();
        ob.printTransactions();
    }
    void println() {
        std::cout << "=======================================================================" << std::endl;
    }

    bool submit(int id, char type, float price) {
        bool status = ob.submit(Order(id, type, price, timestampCount));
        timestampCount++;
        return status;
    }

    bool cancel(int id) { return ob.cancel(id); }

    System() { timestampCount = 1; }

    OrderBook ob;
    int timestampCount;
};

int main() {
    System s;

    s.println();
    std::cout << "Exemplo 1: Ordem sem execução" << std::endl;
    s.submit(1, 'B', 20.0f);
    s.submit(10, 'B', 10.0f);
    s.submit(2, 'S', 35.0f);
    s.submit(12, 'B', 10.0f);
    s.submit(11, 'B', 10.0f);
    s.print();

    s.println();
    std::cout << "Exemplo 2: Execução imediata" << std::endl;
    s.submit(3, 'S', 12.0f);
    s.print();

    s.println();
    std::cout << "Exemplo 3: Escolha da melhor contraparte" << std::endl;
    s.submit(5, 'S', 10.0f);
    s.print();

    s.println();
    std::cout << "Exemplo 4: Contraparte não compatível" << std::endl;
    s.submit(400, 'S', 20.0f);
    s.print();

    s.println();
    std::cout << "Exemplo 5: Cancelamento" << std::endl;
    s.cancel(11);
    s.print();

    s.println();
    std::cout << "Exemplo 6: Fazendo Get" << std::endl;
    int n;
    Order* orders = s.ob.getSellOrders(&n);
    std::cout << "n = " << n << std::endl;


    delete[] orders;
    return 0;
}
