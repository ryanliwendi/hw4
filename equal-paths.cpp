#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool check(Node * root, int depth, int& firstLeafDepth);

bool equalPaths(Node * root)
{
    // Add your code below
    int depth = -1;
    return check(root, 0, depth);
}

bool check(Node * root, int depth, int& firstLeafDepth)
{
    if (root == nullptr)
        return true;
    if (root -> left == nullptr && root -> right == nullptr)
    {
        if (firstLeafDepth == -1)
        {
            firstLeafDepth = depth;
            return true;
        }
        return firstLeafDepth == depth;
    }
    return check(root -> left, depth + 1, firstLeafDepth) && check(root -> right, depth + 1, firstLeafDepth);
}



