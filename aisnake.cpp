#include "aisnake.h"
#include <queue>
#include <set>

using namespace std;
Direction AISnake::go_to(Loc target) {

    if (this->direction == UP || this->direction == DOWN){
        if (target.first < this->body[0].first){
            return LEFT;
        }
        if (target.first > this->body[0].first){
            return RIGHT;
        }
        if (target.first == this->body[0].first){

            if (target.second > this->body[0].second && this->direction == UP) {
                return (this->body[0].first < 30) ? RIGHT : LEFT;
            }
            if (target.second < this->body[0].second && this->direction == DOWN) {
                return (this->body[0].first > 10) ? LEFT : RIGHT;
            }
            return this->direction;
        }

    }
    else {
        if (target.second < this->body[0].second) {
            return UP;
        }
        if (target.second > this->body[0].second) {
            return DOWN;
        }
        if (target.second == this->body[0].second) {
            if (target.first > this->body[0].first && this->direction == LEFT) {
                return (this->body[0].second < 30) ? DOWN : UP;
            }
            if (target.first < this->body[0].first && this->direction == RIGHT) {
                return (this->body[0].second > 10) ? UP : DOWN;
            }
            return this->direction;
        }
    }
    return this->direction;
}

bool AISnake::reachtarget(Loc target) {
    return this->body[0] == target;
}

pair<int, int> GreedyFood::getNearestFood() {
    queue<pair<int, int>> toSearch;
    set<pair<int, int>> searched;
    toSearch.push(this->body[0]);
    searched.insert(this->body[0]);
    while(!toSearch.empty()){
        pair<int,int> target = toSearch.front();
        toSearch.pop();
        if (item_map_ptr->at(target.first)[target.second] && item_map_ptr->at(target.first)[target.second]->getName() == FOOD){
            return target;
        } else{
            if (target.first > 0 && searched.find(make_pair(target.first-1, target.second)) == searched.end()){
                toSearch.push(make_pair(target.first-1, target.second));
                searched.insert(make_pair(target.first-1, target.second));
            }
            if (target.first < width-1 && searched.find(make_pair(target.first+1, target.second)) == searched.end()){
                toSearch.push(make_pair(target.first+1, target.second));
                searched.insert(make_pair(target.first+1, target.second));
            }
            if (target.second > 0 && searched.find(make_pair(target.first, target.second-1)) == searched.end()){
                toSearch.push(make_pair(target.first, target.second-1));
                searched.insert(make_pair(target.first, target.second-1));
            }
            if (target.second < height-1 && searched.find(make_pair(target.first, target.second+1)) == searched.end()){
                toSearch.push(make_pair(target.first, target.second+1));
                searched.insert(make_pair(target.first, target.second+1));
            }
        }

    }
    return make_pair(-1,-1);
}

Direction GreedyFood::act(Field *state) {
    pair<int,int> target = getNearestFood();
    if (target.first == -1) {
        return this->direction;
    }
    return this->go_to(target);
}

void WalkingSnake::setPath(std::queue<Loc> path) {
    this->path = path;
}

Direction WalkingSnake::act(Field *state) {
    if (next_target == make_pair(-1, -1)) {
        if (path.empty()) {
            return this->direction;
        }
        next_target = path.front();
        path.pop();
        path.push(next_target);
    }
    if (!this->reachtarget(next_target)) {
        return this->go_to(next_target);
    }
    else{
        next_target = path.front();
        path.pop();
        path.push(next_target);
        return this->go_to(next_target);
    }
}