#include "Order.hpp"

Order::Order(int id, char type, float price, int timestamp): id(id), type(type), price(price), timestamp(timestamp) {}

Order::~Order() {
    // Sem alocação dinâmica
}
