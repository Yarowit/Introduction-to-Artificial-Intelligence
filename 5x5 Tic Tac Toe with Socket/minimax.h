// Jarosław Socha

#define MIN -100000
#define MAX  100000

#include "board.h"

int maxPlayer;
int minPlayer;

int dataBoard[5][5];

// anulje ruch
void reSetMove(int move){
  int i,j;
  i = (move/10)-1;
  j = (move%10)-1;
  board[i][j] = 0;
}

/**
 * Heurystyka:
 *  Max przegrywa : -10000
 *  Min przegrywa :  10000
 * 
 *  dla każdej kombinacji zwycięskiej:
 *      Max może tam jeszcze wgrać + [ 1 + (liczba pionków w czwórce)^2 ]
 *          jeżeli przeciwnik ma zablokowane pole gdzieś w środku czwórki + 5
 *      Min może tam jeszcze wgrać - [ 1 + (liczba pionków w czwórce)^2 ]
 *          jeżeli przeciwnik ma zablokowane pole gdzieś w środku czwórki + 5
 * 
 * Oparte na liczbie czwórek jakie jeszcze każdy gracz może ułożyć i postępie w ich układaniu
 * 
 * Ściąga:
 * Wartość - Znaczenie
 * 1 - 1
 * 2 - 2
 * 3 - 1 tam nie może
 * 4 - 2 tam nie może
 * 5 - nikt tam nie może
 * 
 *  > 1 blokada -> źle
 *  3 1 1 ? - \_ na boku blokada -> źle
 *  ? 1 1 3 - /
 *  1 3 1 1 +    \
 *  1 4 1 1 +++     blokada przeciwnika na tym polu lub 5 -> lepiej
 *  1 1 3 1 +       jedna blokada i to w środku -> dobrze
 *  1 1 4 1 +++  /
 *  blockageCount > 1 -
 */
int heur(int depth){
    if(winCheck(maxPlayer)) return 10000+depth;
    if(winCheck(maxPlayer%2+1)) return -10000-depth;
    if(loseCheck(maxPlayer)) return -10000-depth;
    if(loseCheck(maxPlayer%2+1)) return 10000+depth;

    // Sprawdzamy gdzie są zblokowane miejsca #################
    for(int x=0; x<5; x++)
    for(int y=0; y<5; y++)
        dataBoard[x][y] = board[x][y];
    
    for(int i=0;i<48;i++){
        int a,b,c;
        a = dataBoard[lose[i][0][0]][lose[i][0][1]];
        b = dataBoard[lose[i][1][0]][lose[i][1][1]];
        c = dataBoard[lose[i][2][0]][lose[i][2][1]];
        

        if(a != 0 && a < 3 && a == b)
            if(c == 0) dataBoard[lose[i][2][0]][lose[i][2][1]] = a+2;
            else if(c+a == 5) dataBoard[lose[i][2][0]][lose[i][2][1]] = 5;
        
        if(a != 0 && a < 3 &&  a == c)
            if(b == 0) dataBoard[lose[i][1][0]][lose[i][1][1]] = a+2;
            else if(b+a == 5) dataBoard[lose[i][1][0]][lose[i][1][1]] = 5;
            
        if(b != 0 && b < 3 &&  b == c)
            if(a == 0) dataBoard[lose[i][0][0]][lose[i][0][1]] = b+2;
            else if(a+b == 5) dataBoard[lose[i][0][0]][lose[i][0][1]] = 5;
    }
    //#########################################################

    int result = 0;

    int a,b,c,d;
    // dla każdej kombinacji
    for(int i=0; i<28; i++){
        a = dataBoard[win[i][0][0]][win[i][0][1]];
        b = dataBoard[win[i][1][0]][win[i][1][1]];
        c = dataBoard[win[i][2][0]][win[i][2][1]];
        d = dataBoard[win[i][3][0]][win[i][3][1]];
        
        // nadal maxPlayer może położyć linie
        if(a != minPlayer && b != minPlayer && c != minPlayer && d != minPlayer){
            int blockageCount = 0;
            if(a == maxPlayer+2 || a == 5) blockageCount++;
            if(b == maxPlayer+2 || b == 5) blockageCount++;
            if(c == maxPlayer+2 || c == 5) blockageCount++;
            if(d == maxPlayer+2 || d == 5) blockageCount++;

            if(blockageCount < 2){
                // jest tylko jedna blokada
                if(!(a == maxPlayer+2 || a == 5 || d == maxPlayer+2 || d == 5)){
                    // nie mamy bloku na bokach
                    result++; // mogę tam jeszcze coś ułożyć

                    int mypawns = (a == maxPlayer) + (b == maxPlayer) + (c == maxPlayer) + (d == maxPlayer);
                    //dodaj tyle ile już masz ^2
                    result += mypawns*mypawns;

                    if(mypawns > 2 && (b == 5 || b == minPlayer + 2 || c == 5 || d == minPlayer + 2 ))
                        result += 5; // środek przeciwnik ma zblokowany 
                }
            }

        }

        // nadal minPlayer może położyć linie
        if(a != maxPlayer && b != maxPlayer && c != maxPlayer && d != maxPlayer){
            int blockageCount = 0;
            if(a == minPlayer+2 || a == 5) blockageCount++;
            if(b == minPlayer+2 || b == 5) blockageCount++;
            if(c == minPlayer+2 || c == 5) blockageCount++;
            if(d == minPlayer+2 || d == 5) blockageCount++;
            if(blockageCount < 2){
                // jest tylko jedna blokada
                if(!(a == minPlayer+2 || a == 5 || d == minPlayer+2 || d == 5)){
                    // nie mamy bloku na bokach
                    result--;
                    
                    int mypawns = (a == minPlayer) + (b == minPlayer) + (c == minPlayer) + (d == minPlayer);
                    //dodaj tyle ile już masz ^2
                    result -= mypawns*mypawns;

                    if(mypawns > 2 && (b == 5 || b == maxPlayer + 2 || c == 5 || d == maxPlayer + 2))
                        result -= 5;// środek przeciwnik ma zblokowany 
                }
            }

        }

    }
    
    return result;
}


/**
 * Główny algorytm minimax
*/
int minimax(int depth, int player, int alpha, int beta){
    if(depth == 0 || winCheck(player) || loseCheck(player) || winCheck(player%2+1) || loseCheck(player%2+1))
        return heur(depth);
    
    int result;
    if(player == maxPlayer){
        // MAX
        result = MIN;
        for(int x=1; x <= 5; x++)
        for(int y=1; y <= 5; y++){
            int move = 10*x+y;
            if(setMove(move,player)){
                int val = minimax(depth - 1, player%2 +1, alpha, beta);
                reSetMove(move);

                if(val > result)
                    result = val;
                
                if(alpha < result)
                    alpha = result;
                
                if(beta <= alpha)
                    return result;
            }
        }
    }else{
        // MIN
        result = MAX;
        for(int x=1; x <= 5; x++)
        for(int y=1; y <= 5; y++){
            int move = 10*x+y;
            if(setMove(move,player)){
                int val = minimax(depth - 1, player%2 +1, alpha, beta);                
                reSetMove(move);

                if(val < result)
                    result = val;
                
                if(beta > result)
                    beta = result;
                
                if(beta <= alpha)
                    return result;
            }
        }
    }
    return result;
}

/**
 * Wywoływacz minimaxa
*/
int chooseAMove(int depth, int player){
    maxPlayer = player;
    minPlayer = player%2+1;

    int result = MIN;
    int bestMove = 0;
    int alpha = MIN;
    int beta = MAX;
    
    // MAX
    for(int x=1; x <= 5; x++)
    for(int y=1; y <= 5; y++){
        int move = 10*x+y;
        if(setMove(move,player)){
            int val = minimax(depth - 1, player%2 +1, alpha, beta);
            
            reSetMove(move);
            if(val > result){
                result = val;
                bestMove = move;
            }

            if(alpha < result)
                alpha = result;
            
            if(beta <= alpha)
                return bestMove;
        }
    }

    return bestMove;
}