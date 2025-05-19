/*
 * Title: How to implement an OrderBook in HFT
 * Article link: https://mp.weixin.qq.com/s/nALnUfJneJZ61Hl31gl9gg
 */

#include <map>
#include <deque>
#include <unordered_map>
#include <string>
#include <iostream>

enum class Side { BUY, SELL };

struct Order {
    std::string id;
    Side side;
    int price;
    int quantity;
};

class OrderBook {
public:
    void add_order(const Order& order);
    void cancel_order(const std::string& order_id);
    void match();

private:
    using OrderQueue = std::deque<Order>;
    using PriceMap = std::map<int, OrderQueue>;  // price -> queue of orders

    // Two trees: buy (descending), sell (ascending)
    PriceMap buy_tree;                          // Highest bid comes last
    PriceMap sell_tree;                         // Lowest ask comes first

    // One table: order ID -> {tree side, price, iterator}
    struct OrderRef {
        Side side;
        int price;
        OrderQueue::iterator it;
    };
    std::unordered_map<std::string, OrderRef> order_index;

    PriceMap& get_tree(Side side) {
        return (side == Side::BUY) ? buy_tree : sell_tree;
    }
};

// Adds a new order to the book
void OrderBook::add_order(const Order& order) {
    auto& tree = get_tree(order.side);
    auto& queue = tree[order.price];
    queue.push_back(order);

    order_index[order.id] = {order.side, order.price, std::prev(queue.end())};
}

// Cancels an existing order
void OrderBook::cancel_order(const std::string& order_id) {
    auto it = order_index.find(order_id);
    if (it == order_index.end()) return;

    OrderRef ref = it->second;
    auto& tree = get_tree(ref.side);
    auto& queue = tree[ref.price];
    queue.erase(ref.it);
    if (queue.empty()) {
        tree.erase(ref.price);
    }

    order_index.erase(it);
}

// Matches best bid and best ask if prices cross
void OrderBook::match() {
    while (!buy_tree.empty() && !sell_tree.empty()) {
        auto best_bid = std::prev(buy_tree.end());
        auto best_ask = sell_tree.begin();

        if (best_bid->first < best_ask->first) break;

        auto& bid_queue = best_bid->second;
        auto& ask_queue = best_ask->second;

        Order& buy_order = bid_queue.front();
        Order& sell_order = ask_queue.front();

        int trade_qty = std::min(buy_order.quantity, sell_order.quantity);
        std::cout << "Matched " << trade_qty << " @ price " << sell_order.price << "\n";

        buy_order.quantity -= trade_qty;
        sell_order.quantity -= trade_qty;

        if (buy_order.quantity == 0) {
            cancel_order(buy_order.id);
        }
        if (sell_order.quantity == 0) {
            cancel_order(sell_order.id);
        }
    }
}



int main() {
    OrderBook book;
    book.add_order({"o1", Side::BUY, 100, 10});
    book.add_order({"o2", Side::SELL, 99, 5});
    book.add_order({"o3", Side::SELL, 100, 10});

    book.match();  // Should match orders o1 and o3
    return 0;
}
