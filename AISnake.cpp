//
// Created by lenovo on 2023/7/3.
//

#include "AISnake.h"
#include <queue>
#include <set>
pair<int, int> GreedyFood::getNearestFood(const Field &state) {
    queue<pair<int, int>> toSearch;
    set<pair<int, int>> searched;
    toSearch.push(this->body[0]);
    searched.insert(this->body[0]);
    while(!toSearch.empty()){
        pair<int,int> target = toSearch.front();
        toSearch.pop();
        if (state.itemMap[target.first][target.second]->name == "food"){
            return target;
        } else{
            if (target.first > 0 && searched.find(make_pair(target.first-1,target.second)) == searched.end()){
                toSearch.push(make_pair(target.first-1,target.second));
                searched.insert(make_pair(target.first-1,target.second));
            }
            if (target.first < state.height-1 && searched.find(make_pair(target.first+1,target.second)) == searched.end()){
                toSearch.push(make_pair(target.first+1,target.second));
                searched.insert(make_pair(target.first+1,target.second));
            }
            if (target.second > 0 && searched.find(make_pair(target.first,target.second-1)) == searched.end()){
                toSearch.push(make_pair(target.first,target.second-1));
                searched.insert(make_pair(target.first,target.second-1));
            }
            if (target.second < state.width-1 && searched.find(make_pair(target.first,target.second+1)) == searched.end()){
                toSearch.push(make_pair(target.first,target.second+1));
                searched.insert(make_pair(target.first,target.second+1));
            }
        }

    }
    return make_pair(-1,-1);
}

Direction GreedyFood::act(Field &state) {
    pair<int,int> target = getNearestFood(state);
    if (target.first == -1){
        return this->direction;
    }
    if (target.first < this->body[0].first){
        return Direction::up;
    } else if (target.first > this->body[0].first){
        return Direction::down;
    } else if (target.second < this->body[0].second){
        return Direction::left;
    } else if (target.second > this->body[0].second){
        return Direction::right;
    }
    return this->direction;
}