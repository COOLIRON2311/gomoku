#pragma once
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

#define self 1
#define opponent -1
#define _empty 0
#define oo 1e9

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
        cout << "     a b c d e f g h i j k l m n o" << endl;
        cout << "    ------------------------------" << endl;
        for (int i = 0; i < 15; i++)
        {
            cout << 15 - i << (15 - i < 10 ? "  | " : " | ");
            for (int j = 0; j < 15; j++)
                if (board[i][j] == _empty)
                    cout << ". ";
                else if (board[i][j] == self)
                    cout << "X ";
                else
                    cout << "O ";
            cout << endl;
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
        return string(1, 'a' + col) + to_string(15 - row);
    }
    inline bool is_valid_move(int col, int row)
    {
        return col >= 0 && col < 15 && row >= 0 && row < 15 && board[col][row] == _empty;
    }
    cell _is_a_win(const array<cell, 5> &v) const
    {
        int self_count = 0;
        int opponent_count = 0;
        for (int i = 0; i < 5; i++)
        {
            if (v[i] == self)
                self_count++;
            else if (v[i] == opponent)
                opponent_count++;
        }
        if (self_count == 5)
            return self;
        if (opponent_count == 5)
            return opponent;
        return 0;
    }
    int _analize_chunk(const array<cell, 5> &chunk) const
    {
        int score = 0;
        int free = 0;
        int selfs = 0;
        int opponents = 0;

        for (int i = 0; i < chunk.size(); i++)
        {
            if (chunk[i] == _empty)
                free++;
            else if (chunk[i] == self)
                selfs++;
            else if (chunk[i] == opponent)
                opponents++;
        }

        if (selfs == 5)
            score += 10000;

        else if (selfs == 4 && free == 1)
            score += 500;

        else if (selfs == 3 && free == 2)
            score += 200;

        else if (selfs == 2 && free == 3)
            score += 100;

        else if (selfs == 1 && free == 4)
            score += 10;

        else if (opponents == 3 && free == 2)
            score -= 200;

        else if (opponents == 4 && free == 1)
            score -= 500;

        else if (opponents == 5)
            score -= 10000;

        return score;
    }
    int heuristic() const
    {
        int count = 0;
        for (int i = 0; i < 15; i++)
        {
            array<cell, 5> row;
            array<cell, 5> col;
            for (int j = 0; j < 15 - 4; j++)
            {
                for (int k = 0; k < 5; k++)
                {
                    row[k] = board[i][j + k];
                    col[k] = board[j + k][i];
                }
                count += _analize_chunk(row);
                count += _analize_chunk(col);
            }
        }
        // for (int i = 0; i < 15 - 4; i++)
        // {
        //     for (int j = 0; j < 15 - 4; j++)
        //     {
        //         array<cell, 5> diag1;
        //         array<cell, 5> diag2;
        //         for (int k = 0; k < 5; k++)
        //         {
        //             diag1[k] = board[i + k][j + k];
        //             diag2[k] = board[i + k][j + 4 - k];
        //         }
        //         count += _analize_chunk(diag1);
        //         count += _analize_chunk(diag2);
        //     }
        // }
        for (int i = 0; i < 15 - 4; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                array<cell, 5> diag1;
                for (int k = 0; k < 5; k++)
                {
                    diag1[k] = board[i + k][j + k];
                }
                count += _analize_chunk(diag1);
            }
        }
        for (int i = 0; i < 15 - 4; i++)
        {
            for (int j = 15 - 1; j >= 4; j--)
            {
                array<cell, 5> diag2;
                for (int k = 0; k < 5; k++)
                {
                    diag2[k] = board[i + k][j - k];
                }
                count += _analize_chunk(diag2);
            }
        }
        return count;
    }
    // 0 - win, 1 - game in progress, 3 - lose, 4 - draw
    int game_status() const
    {
        for (int i = 0; i < 15; i++)
        {
            array<cell, 5> row;
            array<cell, 5> col;
            for (int j = 0; j < 15 - 4; j++)
            {
                for (int k = 0; k < 5; k++)
                {
                    row[k] = board[i][j + k];
                    col[k] = board[j + k][i];
                }
                cell win1 = _is_a_win(row);
                cell win2 = _is_a_win(col);
                if (win1 == self || win2 == self)
                    return 0;
                if (win1 == opponent || win2 == opponent)
                    return 3;
            }
        }

        for (int i = 0; i < 15 - 4; i++)
        {
            for (int j = 0; j < 15 - 4; j++)
            {
                array<cell, 5> diag1;
                array<cell, 5> diag2;
                for (int k = 0; k < 5; k++)
                {
                    diag1[k] = board[i + k][j + k];
                    diag2[k] = board[i + k][j + 4 - k];
                }
                cell win1 = _is_a_win(diag1);
                cell win2 = _is_a_win(diag2);
                if (win1 == self || win2 == self)
                    return 0;
                if (win1 == opponent || win2 == opponent)
                    return 3;
            }
        }

        for (int i = 0; i < 15; i++)
            for (int j = 0; j < 15; j++)
                if (board[i][j] == _empty)
                    return 1;
        return 4;
    }
    void make_move(const string &move, cell player)
    {
        auto [row, col] = str2idx(move);
        if (is_valid_move(row, col))
        {
            board[row][col] = player;
        }
    }
    inline void test_move(int i, int j, bool isAI)
    {
        board[i][j] = isAI ? self : opponent;
    }
    inline void undo_move(int i, int j)
    {
        board[i][j] = _empty;
    }
};

int minimax(Board &b, string *best_move, int depth, int _depth, int alpha, int beta, bool isAI)
{
    if (depth == 0)
    {
        if (isAI)
            return b.heuristic();
        else
            return -b.heuristic();
    }

    if (isAI)
    {
        int best = -oo;
        for (int i = 0; i < 15; i++) // check all possible moves
        {                            // row
            for (int j = 0; j < 15; j++)
            { // col
                if (b(i, j) == _empty)
                {
                    b.test_move(i, j, isAI); // make move
                    int value = minimax(b, best_move, depth - 1, _depth, alpha, beta, !isAI);
                    b.undo_move(i, j); // undo move
                    if (value > best)
                    {
                        best = value;
                        if (depth == _depth)
                            *best_move = Board::idx2str(j, i);
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
                    b.test_move(i, j, isAI);
                    int value = minimax(b, best_move, depth - 1, _depth, alpha, beta, !isAI);
                    b.undo_move(i, j);
                    if (value < best)
                    {
                        best = value;
                        if (depth == _depth)
                            *best_move = Board::idx2str(j, i);
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
bool defend_against_4_in_a_row(Board &b, string *best_move)
{
    static const set<array<cell, 5>> patterns = {
        {opponent, opponent, opponent, opponent, _empty},
        {opponent, opponent, opponent, _empty, opponent},
        {opponent, opponent, _empty, opponent, opponent},
        {opponent, _empty, opponent, opponent, opponent},
        {_empty, opponent, opponent, opponent, opponent}};

    for (int i = 0; i < 15; i++)
    {
        array<cell, 5> row;
        array<cell, 5> col;
        for (int j = 0; j < 15 - 4; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                row[k] = b(i, j + k);
                col[k] = b(j + k, i);
            }
            if (patterns.find(row) != patterns.end())
            {
                for (int k = 0; k < 5; k++)
                {
                    if (row[k] == _empty)
                    {
                        *best_move = Board::idx2str(j + k, i);
                        return true;
                    }
                }
            }
            if (patterns.find(col) != patterns.end())
            {
                for (int k = 0; k < 5; k++)
                {
                    if (col[k] == _empty)
                    {
                        *best_move = Board::idx2str(i, j + k);
                        return true;
                    }
                }
            }
        }
    }
    for (int i = 0; i < 15 - 4; i++)
    {
        for (int j = 0; j < 15 - 4; j++)
        {
            array<cell, 5> diag1;
            array<cell, 5> diag2;
            for (int k = 0; k < 5; k++)
            {
                diag1[k] = b(i + k, j + k);
                diag2[k] = b(i + k, j + 4 - k);
            }
            if (patterns.find(diag1) != patterns.end())
            {
                for (int k = 0; k < 5; k++)
                {
                    if (diag1[k] == _empty)
                    {
                        *best_move = Board::idx2str(j + k, i + k);
                        return true;
                    }
                }
            }
            if (patterns.find(diag2) != patterns.end())
            {
                for (int k = 0; k < 5; k++)
                {
                    if (diag2[k] == _empty)
                    {
                        *best_move = Board::idx2str(j + 4 - k, i + k);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
