#include <bits/stdc++.h>
using namespace std;

typedef long long int lli;

struct Node
{
    int puzzle[3][3];
    int g;
    int h;
    int f;
    int level;
    int x;
    int y;
    struct Node *parent;
};

inline bool isVisited(Node *N, list<Node *> &CL)
{

    if (CL.empty())
    {
        return false;
    }
    int i, j;
    for (list<Node *>::iterator it = CL.begin(); it != CL.end(); it++)
    {
        Node *N2 = *it;
        bool flag = false;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (N->puzzle[i][j] != N2->puzzle[i][j])
                {
                    flag = true;
                    break;
                }
            }
        }
        if (!flag)
        {
            return true;
        }
    }
    return false;
}

inline void expand(Node *N, list<Node *> &OL, list<Node *> &CL)
{
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    for (int i = 0; i < 4; i++)
    {
        int x = N->x + dx[i];
        int y = N->y + dy[i];
        if (0 <= x && x <= 2 && 0 <= y && y <= 2)
        {
            Node *child = new Node();
            child->x = x;
            child->y = y;

            child->g = N->g + 1;
            child->h = 362880;
            child->level = N->level + 1;
            child->parent = N;
            child->f = child->g + child->h;

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    child->puzzle[i][j] = N->puzzle[i][j];
                }
            }
            child->puzzle[N->x][N->y] = N->puzzle[x][y];
            child->puzzle[x][y] = 0;

            if (!isVisited(child, CL))
            {
                OL.push_back(child);
            }
        }
    }
}

inline bool matchMatrix(Node *N, int G[][3])
{

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (N->puzzle[i][j] != G[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

inline void printPath(Node *N)
{

    if (N == NULL)
    {
        return;
    }
    printPath(N->parent);
    cout << "\n\nStep = " << N->level << endl;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << N->puzzle[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    Node *S = new Node();
    int G[3][3];

    cout << "Enter 0 to represent blank cell in matrix!\n"
         << endl;
    cout << "Enter start matrix: " << endl;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cin >> S->puzzle[i][j];

            if (S->puzzle[i][j] == 0)
            {
                S->x = i;
                S->y = j;
            }
        }
    }

    S->g = 0;
    S->h = 362881;
    S->f = S->g + S->h;
    S->level = 0;
    S->parent = NULL;

    cout << "Enter goal matrix: " << endl;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cin >> G[i][j];
        }
    }

    list<Node *> OL;
    list<Node *> CL;

    OL.push_back(S);

    Node *N = NULL;
    int steps = 0;
    while (!OL.empty())
    {
        cout << "steps = " << ++steps << endl;
        N = OL.front();
        expand(N, OL, CL);
        if (matchMatrix(N, G))
        {
            CL.push_back(N);
            break;
        }
        OL.pop_front();
        CL.push_back(N);
    }

    int i, j;
    bool reachedGoal = false;
    for (list<Node *>::iterator it = CL.begin(); it != CL.end(); it++)
    {
        Node *N2 = *it;
        bool flag = false;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (N->puzzle[i][j] != N2->puzzle[i][j])
                {
                    flag = true;
                    break;
                }
            }
        }
        if (!flag)
        {
            reachedGoal = true;
            break;
        }
    }

    if (reachedGoal)
    {
        cout << "\nSuccess! Path = \n";
        printPath(N);
    }
    else
    {
        cout << "\nFailure!\n";
    }

    return 0;
}
