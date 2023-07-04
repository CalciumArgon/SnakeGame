#include "item.h"
#include "snake.h"

Item::Item(Loc location) : location(location) {}

Loc Item::getLoc()
{
    return location;
}

Snake *Item::hitHeadSnake(std::vector<Snake *> snakes)
{
    for (Snake* snake: snakes) {
        if (this->location == snake->getBody()[0]) {
            return snake;
        }
    }
    return nullptr;
}

Snake *Item::hitBodySnake(std::vector<Snake *> snakes)
{
    for (Snake* snake: snakes) {
        for (int i=1; i<snake->getLength(); ++i) {
            if (snake->getBody()[i] == this->location) {
                return snake;
            }
        }
    }
    return nullptr;
}


ItemType Item::getName()
{
    return name;
}

Food::Food(Loc location, int add_length):
    Item(location),
    add_length(add_length) {}

void Food::action(Snake *snake)
{
    if (snake == nullptr) { return; }
    snake->addLength(add_length);
    snake->incEaten();
}

ItemType Food::getName()
{
    return FOOD;
}

void Food::changeAddLength(int new_len)
{
    add_length = new_len;
}
