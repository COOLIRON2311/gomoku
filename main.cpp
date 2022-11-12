#include "gomoku.h"

#define DEPTH 3

int main(int argc, char *argv[])
{
    Board board;
    string opponent_move;
    string my_move;
    bool mid_debut = true; // debut in the center of the board, 35 moves win guaranteed

    bool is_first = true;
    int color = atoi(argv[argc - 1]);
    if (color == 1)
        is_first = false;
    while (board.game_status() == 1)
    {
        if (is_first)
        {
            if (mid_debut)
            {
                my_move = "h8";
                mid_debut = false;
            }
            else
            {
                minimax(board, &my_move, DEPTH, DEPTH, -oo, oo, false);
                // defend_against_4_in_a_row(board, &my_move); // if we can defend against 4 in a row, do it
            }
            board.make_move(my_move, self);
            // cerr << "AI: " << my_move << endl;
            cerr << my_move << endl;
            board.print();
            cout << endl;
        }

        int status = board.game_status();
        if (status != 1)
            return status;

        // cerr << "You: ";
        cin >> opponent_move;
        board.make_move(opponent_move, opponent);
        is_first = true;
        mid_debut = false;
        board.print();
        cout << endl;
    }
    return board.game_status();
}
