

#include "item.h"

Item::Item(int x, int y): location(make_pair(x, y)) {}

Snake* Item::hitHead(vector<Snake*> snakes) {
    for (Snake* snake: snakes) {
        if (this->location == snake->body[0]) {
            return snake;
        }
    }
    return nullptr;
}

Snake* Item::hitBody(vector<Snake*> snakes) {
    for (Snake* snake: snakes) {
        for (int i=1; i<snake->body.size(); ++i) {
            if (snake->body[i] == this->location) {
                return snake;
            }
        }
    }
    return nullptr;
}