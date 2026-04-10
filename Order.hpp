#pragma once

class Order {

  private:
    int id;
    char type; // 'B' ou 'S'
    float price;
    int timestamp;

  public:
    Order() = default; // Construtor padrão para usar no get
    Order(int id, char type, float price, int timestamp);
    ~Order();

    inline int getId() { return id; }
    inline char getType() { return type; }
    inline float getPrice() { return price; }
    inline int getTimestamp() { return timestamp; }
};
