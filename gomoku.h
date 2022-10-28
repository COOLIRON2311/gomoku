#pragma once
#include <array>
#include <iostream>
#include <string>
#include <vector>

#define self 1
#define opponent -1
#define _empty 0
#define oo 1e5

using namespace std;
using cell = int8_t;

class Board
{
    array<array<cell, 15>, 15> board;

public:
    Board()
    {
        for (int i = 0; i < 15; i++)
            for (int j = 0; j < 15; j++)
                board[i][j] = _empty;
    }
    Board(const Board &other)
    {
        for (int i = 0; i < 15; i++)
            for (int j = 0; j < 15; j++)
                board[i][j] = other.board[i][j];
    }
    void print() const
    {
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
                if (board[i][j] == _empty)
                    cerr << ". ";
                else if (board[i][j] == self)
                    cerr << "X ";
                else
                    cerr << "O ";
            cerr << endl;
        }
    }
    inline cell operator()(int i, int j) const
    {
        return board[i][j];
    }
    inline cell &operator()(int i, int j)
    {
        return board[i][j];
    }
    static pair<int, int> str2idx(const string &move)
    {
        int row = 15 - stoi(move.substr(1));
        int col = move[0] - 'a';
        return {row, col};
    }
    static string idx2str(int col, int row)
    {
        return string(1, 'a' + row) + to_string(15 - col);
    }
    // bool is_valid_move(int col, int row)
    // {
    //     return col >= 0 && col < 15 && row >= 0 && row < 15 && board[col][row] == 0;
    // }
    // check if all elements in the vector differ by 1 pairwisely
    template <typename T>
    bool is_sequential(const vector<T> &seq) const
    {
        for (int i = 0; i < seq.size() - 1; i++)
            if (abs(seq[i + 1] - seq[i]) != 1)
                return false;
        return true;
    }
    static int analize_chunk(const vector<cell> &chunk)
    {
        if (chunk.size() < 5)
            return 0;

        int free = 0;
        int selfs = 0;
        int opponents = 0;
        for (int i = 0; i < chunk.size(); i++)
        {
            if (chunk[i] == _empty)
                free++;
            else if (chunk[i] == self)
            {
                if (opponents > 0) // opponent's stone in the middle
                    return 0;
                selfs++;
            }
            else if (chunk[i] == opponent)
                opponents++;
        }
        if (opponents > 1) // more than one opponent's stone
            return 0;

        if (free == 0) // no free cells
            return 0;

        else if (free == 1) // one free cell
        {
            if (selfs == 4)
                return 1000;
            else if (selfs == 3)
                return 100;
            else if (selfs == 2)
                return 10;
            else if (selfs == 1)
                return 1;
            else
                return 0;
        }
        else if (free == 2) // two free cells
        {
            if (selfs == 4)
                return 1000;
            else if (selfs == 3)
                return 100;
            else if (selfs == 2)
                return 10;
            else
                return 0;
        }
        else if (free == 3) // three free cells
        {
            if (selfs == 4)
                return 1000;
            else if (selfs == 3)
                return 100;
            else
                return 0;
        }
        else if (free == 4) // four free cells
        {
            if (selfs == 4)
                return 1000;
            else
                return 0;
        }
        else // five free cells
            return 0;
    }
    int open_ends() const
    {
        int count = 0;
        for (int i = 0; i < 15; i++)
        {
            vector<cell> row;
            vector<cell> col;
            for (int j = 0; j < 15; j++)
            {
                row.push_back(board[i][j]);
                col.push_back(board[j][i]);
            }
            count += analize_chunk(row);
            count += analize_chunk(col);
        }

        for (int k = 0; k < 15 * 2 - 1; k++)
        {
            vector<cell> diag1;
            for (int j = 0; j <= k; j++)
            {
                int i = k - j;
                if (0 <= i && i < 15 && 0 <= j && j < 15)
                    diag1.push_back(board[i][j]);
            }
            count += analize_chunk(diag1);
        }

        for (int k = 1; k < 15 * 2; k++)
        {
            vector<cell> diag2;
            for (int j = 0; j <= k; j++)
            {
                int i = 15 - (k - j);
                if (0 <= i && i < 15 && 0 <= j && j < 15)
                    diag2.push_back(board[i][j]);
            }
            count += analize_chunk(diag2);
        }
        return count;
    }
    // 0 - win, 1 - game in progress, 3 - lose, 4 - draw
    int game_status() const
    {
        static int analisis = 0;
        analisis++;
        // cout << "game_status:" << endl;
        // print();
        for (int i = 0; i < 15; i++)
        {
            vector<int> selfs_row;
            vector<int> selfs_col;
            vector<int> opponents_row;
            vector<int> opponents_col;
            for (int j = 0; j < 15; j++)
            {
                if (board[i][j] == self)
                    selfs_row.push_back(j);
                else if (board[i][j] == opponent)
                    opponents_row.push_back(j);
                if (board[j][i] == self)
                    selfs_col.push_back(j);
                else if (board[j][i] == opponent)
                    opponents_col.push_back(j);
            }
            if (selfs_row.size() >= 5 && is_sequential(selfs_row) || selfs_col.size() >= 5 && is_sequential(selfs_col))
                return 0;
            if (opponents_row.size() >= 5 && is_sequential(opponents_row) || opponents_col.size() >= 5 && is_sequential(opponents_col))
                return 3;
        }

        for (int k = 0; k < 15 * 2 - 1; k++)
        {
            vector<int> selfs_diag1;
            vector<int> opponents_diag1;
            for (int j = 0; j <= k; j++)
            {
                int i = k - j;
                if (0 <= i && i < 15 && 0 <= j && j < 15)
                {
                    if (board[i][j] == self)
                    {
                        selfs_diag1.push_back(i);
                    }
                    else if (board[i][j] == opponent)
                    {
                        opponents_diag1.push_back(i);
                    }
                }
            }
            if (selfs_diag1.size() >= 5 && is_sequential(selfs_diag1))
                return 0;
            if (opponents_diag1.size() >= 5 && is_sequential(opponents_diag1))
                return 3;
        }

        int filled = 0;
        for (int k = 1; k < 15 * 2; k++)
        {
            vector<int> selfs_diag2;
            vector<int> opponents_diag2;
            for (int j = 0; j <= k; j++)
            {
                int i = 15 - (k - j);
                if (0 <= i && i < 15 && 0 <= j && j < 15)
                {
                    if (board[i][j] == self)
                    {
                        filled++;
                        selfs_diag2.push_back(i);
                    }
                    else if (board[i][j] == opponent)
                    {
                        filled++;
                        opponents_diag2.push_back(i);
                    }
                }
            }
            if (selfs_diag2.size() >= 5 && is_sequential(selfs_diag2))
                return 0;
            if (opponents_diag2.size() >= 5 && is_sequential(opponents_diag2))
                return 3;
        }
        if (filled == 225)
            return 4;
        return 1;
    }
    void make_move(const string &move, int player)
    {
        auto [row, col] = str2idx(move);
        board[row][col] = player;
    }
};

int minimax(Board& b, string *best_move, int depth, int _depth, int alpha, int beta, bool isMax)
{
    if (depth == 0)
        return b.open_ends();

    if (isMax)
    {
        int best = -oo;
        for (int i = 0; i < 15; i++) // check all possible moves
        { // row
            for (int j = 0; j < 15; j++)
            { // col
                if (b(i, j) == _empty)
                {
                    b(i, j) = self; // make move
                    int value = minimax(b, best_move, depth - 1, _depth, alpha, beta, false);
                    b(i, j) = _empty; // undo move
                    if (value > best)
                    {
                        best = value;
                        if (depth == _depth)
                            *best_move = Board::idx2str(i, j);
                    }
                    alpha = max(alpha, best);
                    if (best >= beta)
                        return best; // beta cut-off
                }
            }
            return best;
        }
    }
    else
    {
        int best = oo;
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (b(i, j) == _empty)
                {
                    b(i, j) = opponent;
                    int value = minimax(b, best_move, depth - 1, _depth, alpha, beta, true);
                    b(i, j) = _empty;
                    if (value < best)
                    {
                        best = value;
                        if (depth == _depth)
                            *best_move = Board::idx2str(i, j);
                    }
                    beta = min(beta, best);
                    if (best <= alpha)
                        return best; // alpha cut-off
                }
            }
        }
        return best;
    }
    return 0;
}
