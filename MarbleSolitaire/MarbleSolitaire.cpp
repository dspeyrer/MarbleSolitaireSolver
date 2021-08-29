#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <windows.h>

using namespace std;

// Dimensions of the board
const int BOARD_SIZE = 7;

typedef int board_state[BOARD_SIZE][BOARD_SIZE];

board_state initial_data = {
    {-1,-1, 1, 1, 1,-1,-1},
    {-1, 1, 1, 1, 1, 1,-1},
    { 1, 1, 1, 1, 1, 1, 1},
    { 1, 1, 1, 0, 1, 1, 1},
    { 1, 1, 1, 1, 1, 1, 1},
    {-1, 1, 1, 1, 1, 1,-1},
    {-1,-1, 1, 1, 1,-1,-1}
};

enum Dir { Up, Down, Left, Right };

struct Move {
    int x;
    int y;
    Dir dir;

    explicit operator string() {
        return to_string(y) + string(", ") + to_string(x) + string(" ") + string(dir == Up ? "\x1e" : dir == Right ? "\x10" : dir == Down ? "\x1f" : "\x11");
    }

    // Returns the move passed in with the coordinates incremented in the direction specified in the move.
    // This may return an out-of-bounds coordinate.
    Move increment_direction();
};

Move Move::increment_direction() {
    Move m;
    m.dir = dir;
    m.x = x;
    m.y = y;
    switch (dir) {
    case Up:
        m.x--;
        break;
    case Right:
        m.y++;
        break;
    case Down:
        m.x++;
        break;
    case Left:
        m.y--;
        break;
    };
    return m;
};

typedef vector<Move> solution;


UINT64 searched_amount = 0;
int max_search = 0;




// Defines a board state with functions 
class Board {
public:
    Board(board_state state);
    void log();

    // Recursively solve the board and add the found solutions to the output vector, 
    // and keep track of the current stack and the recursed level
    void solve(vector<solution> &output, solution &stack);

    // Attempts to modify the board state to reflect the move passed in. If successful, will return true
    // and modify the board; otherwise, will return false.
    bool make_move(Move &mov);
private:
    board_state state;
};

Board::Board(board_state copy_state) {
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            state[x][y] = copy_state[x][y];
        }
    }
}

void Board::log() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            switch (state[i][j]) {
            case -1:
                cout << "  ";
                break;
            case 0:
                cout << "\xb0\xb0";
                break;
            case 1:
                cout << "\xb2\xb2";
                break;
            }
        }
        cout << "\n";
    }
}

bool Board::make_move(Move &m0) {
    Move m1 = m0.increment_direction();
    Move m2 = m1.increment_direction();

    bool oob = false;

    // This will make sure that no out of bounds locations will be accessed
    switch (m0.dir) {
    case Up:
        oob = m2.x < 0;
        break;
    case Right:
        oob = m2.y >= BOARD_SIZE;
        break; 
    case Down:
        oob = m2.x >= BOARD_SIZE;
        break; 
    case Left:
        oob = m2.y < 0;
        break;
    }


    // This will validate whether a move is possible: the origin marble should be there, the marble being jumped over should
    // be there, and there should be an empty space in the destination.
    if (
        oob ||
        state[m0.x][m0.y] != 1 ||
        state[m1.x][m1.y] != 1 ||
        state[m2.x][m2.y] != 0
        ) {
        return false;
    }


    // Make the modifications to the board state.
    state[m0.x][m0.y] = 0;
    state[m1.x][m1.y] = 0;
    state[m2.x][m2.y] = 1;

    return true;
}

void Board::solve(vector<solution>& output, solution &stack) {
    searched_amount++;

    if (stack.size() > max_search) {
        max_search = stack.size();
    }

    cout << "\r";

    cout << " | currently searching at : ";
    cout << stack.size();

    cout << " | total searched amount: ";
    cout << searched_amount;

    cout << " | farthest search at: ";
    cout << max_search;

    cout << " |";



    int counter = 0;

    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            // Iterate over all marbles in the board in order to look for valid moves
            if (state[x][y] == 1) {
                counter++;
                // Look in all directions for a valid move
                for (int dirInt = Up; dirInt < 4; dirInt++) {
                    Dir dir = static_cast<Dir>(dirInt);
                    // This created move does not have to be a valid move, but the X and Y coordinates of the 
                    // move origin will always have a marble.
                    Move mov;
                    mov.x = x;
                    mov.y = y;
                    mov.dir = dir;
                    // Copy the current state into another board instance so that the process can recurse
                    Board m(state);
                    // Try to make the move on the board. make_move will return true if the move successfully
                    // completed, and if it did, then recurse
                    if (m.make_move(mov)) {
                        solution newStack = stack;

                        newStack.push_back(mov);
                        m.solve(output, newStack);
                    }
                }
            }
        }
    }

    if (counter == 1) {
        output.push_back(stack);
        cout << "\n\nFound solution: [\n";
        for (int i = 0; i < stack.size(); i++) {
            cout << static_cast<string>(stack[i]);
            cout << "\n";
        }
        cout << "]\n";
    }
}

int main()
{
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    
    Board initial_state(initial_data);
    solution stack = {};
    vector<solution> solutions = {};
    initial_state.solve(solutions, stack);
}