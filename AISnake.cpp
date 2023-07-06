//
// Created by lenovo on 2023/7/3.
//

#include "AISnake.h"
#include <set>
using namespace std;
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


Direction AISnake::go_to(pair<int, int> target) {
    if (target.first <= -1 || target.second <=-1){
        return this->direction;
    }
    if (target.first < this->body[0].first){
        return Direction::UP;
    } else if (target.first > this->body[0].first){
        return Direction::DOWN;
    } else if (target.second < this->body[0].second){
        return Direction::LEFT;
    } else if (target.second > this->body[0].second){
        return Direction::RIGHT;
    } else{
        return this->direction;
    }
}


Direction GreedyFood::act(Field &state) {
    pair<int,int> target = getNearestFood();
    return go_to(target);
}

Direction WalkingAI::act(Field &state) {
    if (trajectory.empty()){
        return this->direction;
    } else{
        pair<int, int> target = trajectory.front();
        trajectory.pop();
        trajectory.push(target);
        return go_to(target);
    }
}