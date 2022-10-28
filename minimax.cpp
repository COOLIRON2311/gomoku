#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <queue>

#define oo 1e9
using namespace std;

int nodes_visited = 0;

class Node
{
public:
    int value;
    vector<Node *> children;
    Node *parent;
    Node(int value, Node *parent)
    {
        this->value = value;
        this->parent = parent;
    }
    void print(int depth = 0)
    {
        for (int i = 0; i < children.size() / 2; i++)
            children[i]->print(depth + 1);
        for (int i = 0; i < depth; i++)
            cerr << "   ";
        if (value == oo)
            cerr << "*" << endl;
        else
            cerr << value << endl;
        for (int i = children.size() / 2; i < children.size(); i++)
            children[i]->print(depth + 1);
    }
};

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int minimax(Node *node, int depth, bool isMax)
{
    nodes_visited++;
    if (depth == 0)
        return node->value;
    if (isMax)
    {
        int best = -oo;
        for (int i = 0; i < node->children.size(); i++)
            best = max(best, minimax(node->children[i], depth - 1, !isMax));
        return best;
    }
    else
    {
        int best = oo;
        for (int i = 0; i < node->children.size(); i++)
            best = min(best, minimax(node->children[i], depth - 1, !isMax));
        return best;
    }
}

int minimaxab(Node *node, int depth, int alpha, int beta, bool isMax)
{
    nodes_visited++;
    if (depth == 0)
        return node->value;
    if (isMax)
    {
        int best = -oo;
        for (int i = 0; i < node->children.size(); i++)
        {
            best = max(best, minimaxab(node->children[i], depth - 1, alpha, beta, !isMax));
            alpha = max(alpha, best);
            if (best >= beta)
                break; // beta cut-off
        }
        return best;
    }
    else
    {
        int best = oo;
        for (int i = 0; i < node->children.size(); i++)
        {
            best = min(best, minimaxab(node->children[i], depth - 1, alpha, beta, !isMax));
            beta = min(beta, best);
            if (best <= alpha)
                break; // alpha cut-off
        }
        return best;
    }
}

Node *make_tree(const string &leafs, int d, int b)
{
    istringstream iss(leafs);
    Node *root = new Node(0, nullptr);
    queue<Node *> q;

    q.push(root);
    for (int i = 0; i < d; i++)
    {
        Node *node = q.front();
        vector<int> values;
        int value = oo;

        q.push(nullptr); // null node to mark the end of the level

        while (node != nullptr)
        {
            q.pop();
            for (int j = 0; j < b; j++)
            {
                if (i == d - 1)
                    iss >> value;
                Node *child = new Node(value, node);
                node->children.push_back(child);
                q.push(child);
            }
            node = q.front();
        }
        q.pop();
    }
    return root;
}

int main()
{
    int d;
    int b;
    cin >> d >> b;
    cin.ignore();
    string leafs;
    getline(cin, leafs);

    Node *root = make_tree(leafs, d, b);

    // root->print();

    int result = minimaxab(root, d, -oo, oo, true);
    cout << result << " " << nodes_visited << endl;

    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    // cout << "0 0" << endl;
}
