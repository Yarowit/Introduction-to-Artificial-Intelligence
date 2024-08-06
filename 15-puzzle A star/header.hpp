#include <iostream>
#include <algorithm>
#include <random>
#include <queue>
#include <set>
#include <iterator>
#include <cstdlib>
#include <chrono>
#include <math.h>

#define max 4

class Puzzle{
    public:
    char board[max][max];
    char prevX=max+1, prevY=max+1;
    char zeroX=0, zeroY=0;
    char moveCount = 0;

    void print() const{
        std::cout<<"########\n";
        for(auto & row : board){
            for(char tile : row)
                if(tile+97 == 'a')
                    std::cout<<"  ";
                else
                    std::cout<<(char)(tile + 97)<<" ";
            std::cout<<std::endl;
        }
        std::cout<<"########\n";
    }

    bool operator<(const Puzzle& other) const {
        for(int i=0;i<max;i++)
            for(int j=0;j<max;j++)
                if(board[i][j] != other.board[i][j])
                    return board[i][j]<other.board[i][j];
        return false; 
    }

    bool operator==(const Puzzle& other) const {
        for(int i=0;i<max;i++)
            for(int j=0;j<max;j++)
                if(board[i][j] != other.board[i][j])
                    return false;
        return true; 
    }
    
    float crow(){
        float count = 0;
        for(int i=0;i<max;i++)
            for(int j=0;j<max;j++){
                count+=std::sqrt( (float)(i-board[i][j]/max)*(i-board[i][j]/max) + (float)(j-board[i][j]%max)*(j-board[i][j]%max) );
            }
            
        return count+moveCount*3/8;
    }

    unsigned int manhattan(){
        unsigned int count = 0;
        for(int i=0;i<max;i++)
            for(int j=0;j<max;j++){
                count+=std::abs(i-board[i][j]/max) + std::abs(j-board[i][j]%max);
            }
            
        return count+moveCount*3/8;
    }

    Puzzle(bool rand){ //prawy dolny pusty - nie działa, działa lewy górny pusty
        if(!rand)
            for(int i=0;i<max;i++)
                for(int j=0;j<max;j++)
                    board[i][j] = max*i+j;
        else{
            int perm[max*max];
            for(int i=0;i<max*max;i++)
                perm[i] = i;

            std::random_device rd;
            std::mt19937 g(rd());
        
            std::shuffle(perm+1, perm + max*max , g);

            for(int i=0;i<max;i++)
                for(int j=0;j<max;j++)
                    board[i][j] = perm[max*i+j];

        }

    };

    void print(){
        std::cout<<"########\n";
        for(auto & row : board){
            for(char tile : row)
                if(tile+97 == 'a')
                    std::cout<<"  ";
                else
                    std::cout<<(char)(tile + 97)<<" ";
            std::cout<<std::endl;
        }
        std::cout<<"########\n";
    }

    Puzzle(Puzzle* father){
        for(int i=0;i<max;i++)
            for(int j=0;j<max;j++)
                board[i][j] = father->board[i][j];
        zeroX = father->zeroX;
        prevX = father->zeroX;
        zeroY = father->zeroY;
        prevY = father->zeroY;
    }

    //
    /**  00
     * 10  01
     *   11
     * @param one leftDown
     * @param two mid
    */
    bool legal(bool one, bool two){
        return !(!one && !two &&  zeroY == max-1
        || !one && two  &&  zeroX == max-1
        ||  one && two &&  zeroY == 0
        ||  one && !two  &&  zeroX == 0  );
    }

    //TYLKO jak sprawdzony legal!!!
    Puzzle move(bool one, bool two){
        Puzzle child(this);

        child.zeroY+=(!one && !two) - (one && two);
        child.zeroX+=(!one && two) - (one && !two);
        
        child.moveCount = moveCount+1;
        std::swap(child.board[child.zeroX][child.zeroY], child.board[zeroX][zeroY]);
        return child;
    }

    //SOLVABILITY
    // This function returns true if given
    // instance of max*max - 1 puzzle is solvable
    bool isSolvable()
    {
        // Count inversions in given puzzle
        int inv_count = 0;
        for (int i = 0; i < max * max - 1; i++)
        {
            for (int j = i + 1; j < max * max; j++)
            {
                // count pairs(arr[i], arr[j]) such that
                // i < j but arr[i] > arr[j]
                if (board[i/max][i%max] > board[j/max][j%max])
                    inv_count++;
            }
        }
        // If grid is odd, return true if inversion
        // count is even.
        if (max & 1)
            return !(inv_count & 1);
    
        else     // grid is even
        {
            return (zeroY % 2 == inv_count%2);
            // int pos = findXPosition(puzzle);
            // if (pos & 1)
            //     return !(invCount & 1);
            // else
            //     return invCount & 1;
        }
    }
};