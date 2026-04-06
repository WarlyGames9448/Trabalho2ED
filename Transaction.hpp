class Transaction {

  private:
    int buy_order_id;
    int sell_order_id;
    float execution_price;

  public:
    Transaction(); // Construtor padrão para usar no get
    Transaction(int buy_order_id, int sell_order_id, float execution_price);
    ~Transaction();

    inline int getBuyOrderId() { return buy_order_id; }
    inline int getSellOrderId() { return sell_order_id; }
    inline float getExecutionPrice() { return execution_price; }
};
