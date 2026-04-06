#include "Transaction.hpp"

Transaction::Transaction(int buy_order_id, int sell_order_id, float execution_price)
    : buy_order_id(buy_order_id), sell_order_id(sell_order_id), execution_price(execution_price) {}

Transaction::~Transaction() {
    // Sem alocação dinâmica
}
