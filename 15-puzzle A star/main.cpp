#include "header.hpp"

Puzzle origin(true);
Puzzle order(false);

void Search(){
    auto compare = [](Puzzle a, Puzzle b) { return a.crow() > b.crow(); }; // crow
    std::priority_queue<Puzzle, std::vector<Puzzle>, decltype(compare)> Queue(compare);
    std::set<Puzzle> visited;

    Queue.push(origin);

    while(Queue.size() > 0){
        Puzzle curr = Queue.top();
        Queue.pop();
        visited.insert(curr);

        bool dirs[4][2] = {
            {0,0},
            {0,1},
            {1,0},
            {1,1}
        };


        if( curr.operator==(order)) break;

        for(int i=0; i < 4; i++){
            if(curr.legal(dirs[i][0], dirs[i][1])){
                Puzzle moved = curr.move(dirs[i][0], dirs[i][1]);
                
                auto find = visited.find(moved);
                if(find != visited.end()){ // znalazł -> ewentualny update
                    Puzzle old = *find;
                    if(old.moveCount > moved.moveCount){
                        old.moveCount = moved.moveCount;
                        old.prevX = moved.prevX;
                        old.prevY = moved.prevY;
                    }
                }else{
                    Queue.push(moved);
                }
            }
        }
    }
    //koniec
    auto find = visited.find(Puzzle(false));
    if(find != visited.end()){// jest


        // print
        Puzzle curr = *find;
        curr.print();
        
        while(curr.prevX != max+1){
            std::swap(curr.board[curr.zeroX][curr.zeroY], curr.board[curr.prevX][curr.prevY]);
            curr = *visited.find(curr);
            curr.print();
        }
        

        // std::cout<<"Jest \nkroków: "<<(int)find->moveCount<<std::endl;    
    }else{
        // std::cout<<"Nie ma \nkolejka: "<<Queue.size()<<std::endl;
    }
    // std::cout<<"Odwiedzonych: "<<visited.size()<<std::endl;
    // std::cout<<visited.size()<<std::endl;
}


void go(){
    origin = Puzzle(true);
    while(!origin.isSolvable())
        origin = Puzzle(true);
    
    Search();
}


int main(){
    // for(int i=0; i < 100; i++)
        go();
    return 0;
}