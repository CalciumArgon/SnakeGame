//
// Created by lenovo on 2023/7/3.
//

#include "aisnake.h"
#include <queue>
#include <set>

using namespace std;
Direction AISnake::go_to(Loc target) {

    if (target.first < this->body[0].first){
        return LEFT;
    }
    if (target.first > this->body[0].first){
        return RIGHT;
    }
    if (target.second < this->body[0].second){
        return DOWN;
    }
    if (target.second > this->body[0].second){
        return UP;
    }
    return this->direction;
}

pair<int, int> GreedyFood::getNearestFood() {
    queue<pair<int, int>> toSearch;
    set<pair<int, int>> searched;
    toSearch.push(this->body[0]);
    searched.insert(this->body[0]);
    while(!toSearch.empty()){
        pair<int,int> target = toSearch.front();
        toSearch.pop();
        if (item_map_ptr->at(target.first)[target.second]){
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
    if (this->path.empty()){
        return this->direction;
    }
    Loc target = this->path.front();
    this->path.pop();
    this->path.push(target);
    return this->go_to(target);
}
