#include <bits/stdc++.h>
using namespace std;

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

inline int calcDisplacements(Node *N, int G[][3])
{

    int c = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int tmp1 = N->puzzle[i][j];
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    if (G[k][l] != 0 && G[k][l] == tmp1)
                    {
                        c += abs(k - i) + abs(l - j);
                    }
                }
            }
        }
    }
    return c;
}

inline void expand(Node *N, list<Node *> &OL, int G[][3], list<Node *> &CL)
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
            child->h = calcDisplacements(N, G);
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
    cout << "Step = " << N->level << endl;
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

    cout << "Enter 0 to represent blank cell in matrix!" << endl;
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

    cout << "Enter goal matrix: " << endl;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cin >> G[i][j];
        }
    }

    S->g = 0;
    S->h = calcDisplacements(S, G);
    S->f = S->g + S->h;
    S->level = 0;
    S->parent = NULL;

    list<Node *> OL;
    list<Node *> CL;

    OL.push_back(S);

    Node *N = NULL;
    bool goalReached = false;
    int steps = 0;
    while (!OL.empty())
    {
        cout << "Step No. = " << ++steps << endl;
        int minval = INT_MAX;
        list<Node *>::iterator res;
        for (list<Node *>::iterator it = OL.begin(); it != OL.end(); it++)
        {
            if (minval > (*it)->f)
            {
                minval = (*it)->f;
                res = it;
            }
        }
        N = *res;
        expand(N, OL, G, CL);
        if (matchMatrix(N, G))
        {
            CL.push_back(N);
            goalReached = true;
            break;
        }
        OL.erase(res);
        CL.push_back(N);
    }

    if (goalReached)
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
