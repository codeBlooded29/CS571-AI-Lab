#include <bits/stdc++.h>
using namespace std;

struct Node
{
    int puzzle[3][3];
    int g;
    int h;
    int x;
    int y;
    Node *parent;
};

struct Result
{
    int total_number_of_states_explored;
    int optimal_path_length;
    Node *goal;
    double execution_time;
    bool success;
};

struct compareFscore
{
    bool operator()(Node *const &n1, Node *const &n2)
    {
        if (n1->h > n2->h)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

inline int calcDisplacements(Node *N, int G[][3])
{
    int c = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (N->puzzle[i][j] != G[i][j])
            {
                c++;
            }
        }
    }
    return c;
}

inline bool matchMatrices(int A[][3], int B[][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (A[i][j] != B[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

inline bool isVisited(Node *N, list<Node *> &CL)
{

    if (CL.empty())
    {
        return false;
    }

    for (list<Node *>::iterator it = CL.begin(); it != CL.end(); it++)
    {
        if (matchMatrices(N->puzzle, (*it)->puzzle))
        {
            return true;
        }
    }
    return false;
}

inline int calcManhattan(Node *N, int G[][3])
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

inline void expand(Node *N, priority_queue<Node *, vector<Node *>, compareFscore> &OL,
                   list<Node *> &CL, int G[][3], int heuristic)
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

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    child->puzzle[i][j] = N->puzzle[i][j];
                }
            }
            child->puzzle[N->x][N->y] = N->puzzle[x][y];
            child->puzzle[x][y] = 0;

            child->g = N->g + 1;
            if (heuristic == 2)
            {
                child->h = calcDisplacements(child, G);
            }
            else if (heuristic == 3)
            {
                child->h = calcManhattan(child, G);
            }
            else if (heuristic == 4)
            {
                child->h = (3 * calcDisplacements(child, G)) - (2 * calcManhattan(child, G));
            }

            child->parent = N;

            if (!isVisited(child, CL))
            {
                OL.push(child);
            }
            else
            {
                free(child);
            }
        }
    }
}

inline void printMatrix(int M[][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
}

inline void printPath(Node *N)
{

    if (N == NULL)
    {
        return;
    }
    printPath(N->parent);
    cout << "\n\nStep = " << N->g << endl;
    printMatrix(N->puzzle);
}

inline void runH2heuristic(Node *S, Result &result, int G[][3])
{
    cout << "\nRunning for h2(n) = number of tiles displaced from their destined position.\n";
    S->g = 0;
    S->h = calcDisplacements(S, G);
    S->parent = NULL;

    priority_queue<Node *, vector<Node *>, compareFscore> OL;
    list<Node *> CL;

    OL.push(S);

    Node *N = NULL;
    int steps = 0;
    bool reachedGoal = false;
    while (!OL.empty())
    {
        // cout << "steps = " << ++steps << endl;
        steps++;
        N = OL.top();
        if (steps >= 5000)
        {
            break;
        }

        // printMatrix(N->puzzle);
        OL.pop();
        expand(N, OL, CL, G, 2);
        CL.push_back(N);

        if (matchMatrices(N->puzzle, G))
        {
            CL.push_back(N);
            reachedGoal = true;
            break;
        }
    }

    if (reachedGoal)
    {
        cout << "\nSuccess! Path = \n";
        printPath(N);

        cout << "\nTotal States explored = " << steps << endl;
        cout << "\nOptimal Path Length = " << N->g << endl;

        result.goal = N;
        result.optimal_path_length = N->g;
        result.total_number_of_states_explored = steps;
        result.success = true;
    }
    else
    {
        cout << "\nFailure!\n";
        cout << "\nTotal States Explored = very large( > 1,00,000).\n";
        result.success = false;
    }
}

inline void runH3heuristic(Node *S, Result &result, int G[][3])
{
    cout << "\nRunning for h3(n) = sum of Manhattan distance of each tiles from the goal position.\n";
    S->g = 0;
    S->h = calcManhattan(S, G);
    S->parent = NULL;

    priority_queue<Node *, vector<Node *>, compareFscore> OL;
    list<Node *> CL;

    OL.push(S);

    Node *N = NULL;
    int steps = 0;
    bool reachedGoal = false;
    while (!OL.empty())
    {
        // cout << "steps = " << ++steps << endl;
        steps++;
        N = OL.top();
        if (steps >= 5000)
        {
            break;
        }

        // printMatrix(N->puzzle);
        OL.pop();
        expand(N, OL, CL, G, 3);
        CL.push_back(N);

        if (matchMatrices(N->puzzle, G))
        {
            CL.push_back(N);
            reachedGoal = true;
            break;
        }
    }

    if (reachedGoal)
    {
        cout << "\nSuccess! Path = \n";
        printPath(N);

        cout << "\nTotal States explored = " << steps << endl;
        cout << "\nOptimal Path Length = " << N->g << endl;

        result.goal = N;
        result.optimal_path_length = N->g;
        result.total_number_of_states_explored = steps;
        result.success = true;
    }
    else
    {
        cout << "\nFailure!\n";
        cout << "\nTotal States Explored = very large( > 1,00,000).\n";
        result.success = false;
    }
}

inline void runH4heuristic(Node *S, Result &result, int G[][3])
{
    cout << "\nRunning for h4(n) = 3h2(n) - 2h2(n).\n";
    S->g = 0;
    S->h = (3 * calcDisplacements(S, G)) - (2 * calcManhattan(S, G));
    S->parent = NULL;

    priority_queue<Node *, vector<Node *>, compareFscore> OL;
    list<Node *> CL;

    OL.push(S);

    Node *N = NULL;
    int steps = 0;
    bool reachedGoal = false;
    while (!OL.empty())
    {
        // cout << "steps = " << ++steps << endl;
        steps++;
        N = OL.top();
        if (steps >= 5000)
        {
            break;
        }

        // printMatrix(N->puzzle);
        OL.pop();
        expand(N, OL, CL, G, 4);
        CL.push_back(N);

        if (matchMatrices(N->puzzle, G))
        {
            CL.push_back(N);
            reachedGoal = true;
            break;
        }

        CL.push_back(N);
        if (OL.size() > 5000)
            break;
    }

    if (reachedGoal)
    {
        cout << "\nSuccess! Path = \n";
        printPath(N);

        cout << "\nTotal States explored = " << steps << endl;
        cout << "\nOptimal Path Length = " << N->g << endl;

        result.goal = N;
        result.optimal_path_length = N->g;
        result.total_number_of_states_explored = steps;
        result.success = true;
    }
    else
    {
        cout << "\nFailure!\n";
        cout << "\nTotal States Explored = very large( > 1,00,000).\n";
        result.success = false;
    }
}

int main()
{
    Node *S = new Node();
    int G[3][3];

    cout << "\nEnter 0 to represent blank cell in matrix!\n"
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

    cout << "Enter goal matrix: " << endl;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cin >> G[i][j];
        }
    }

    // G[0][0]=1,G[0][1]=2,G[0][2]=3;
    // G[1][0]=4,G[1][1]=5,G[1][2]=6;
    // G[2][0]=7,G[2][1]=8,G[2][2]=0;

    vector<Result> results(4);

    clock_t start, end;
    double execution_time;

    start = clock();
    runH2heuristic(S, results[1], G);
    end = clock();
    results[1].execution_time = (end - start) / double(CLOCKS_PER_SEC);

    start = clock();
    runH3heuristic(S, results[2], G);
    end = clock();
    results[2].execution_time = (end - start) / double(CLOCKS_PER_SEC);

    start = clock();
    runH4heuristic(S, results[3], G);
    end = clock();
    results[3].execution_time = (end - start) / double(CLOCKS_PER_SEC);

    if (results[1].success || results[2].success || results[3].success)
    {
        cout << "\nComparison Table - \n\n";
        cout << "\nHeuristic\t\tTime Taken\t\tTotal Steps\tOptimal Cost\n";

        cout << "\th1\t\t" << results[1].execution_time << "\t\t";
        cout << results[1].total_number_of_states_explored << "\t\t";
        cout << results[1].optimal_path_length << endl;

        cout << "\th2\t\t" << results[2].execution_time << "\t\t";
        cout << results[2].total_number_of_states_explored << "\t\t";
        cout << results[2].optimal_path_length << endl;

        cout << "\th3\t\t" << results[2].execution_time << "\t\t";
        cout << results[3].total_number_of_states_explored << "\t\t";
        cout << results[3].optimal_path_length << endl;
    }

    return 0;
}
