#include "gomoku.h"

#define DEPTH 3

int main(int argc, char *argv[])
{
    Board board;
    string opponent_move;
    string my_move;

    bool is_first = true;
    int color = atoi(argv[argc - 1]);
    if (color == 1)
        is_first = false;
    while (board.game_status() == 1)
    {
        if (is_first)
        {
            minimax(board, &my_move, DEPTH, DEPTH, -oo, oo, true);
            defend_against_4_in_a_row(board, &my_move); // if we can defend against 4 in a row, do it
            // TODO: defend against 3 in a row
            board.make_move(my_move, self);
            // cerr << "AI: " << my_move << endl;
            cerr << my_move << endl;
            board.print();
        }

        int status = board.game_status();
        if (status != 1)
            return status;

        // cerr << "You: ";
        cin >> opponent_move;
        board.make_move(opponent_move, opponent);
        is_first = true;
        board.print();
        cout << endl;
    }
    return board.game_status();
}
