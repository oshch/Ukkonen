#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>


using namespace std;

class Ukkonen
{
private:
    class Node {
    public:
        int parent, d, suf, lft;
        vector<int> bor;
        Node(){}
    };
    vector<Node> nodes;
    int next, left, cur, link;
    char * s;
    bool leaf;
    int n;

    void add(int right)
    {
        leaf = false;
        if (nodes[cur].d != right - left)
        {
            int len = right - left - nodes[nodes[cur].parent].d;
            leaf = s[nodes[cur].lft + len] != s[right];
            if (leaf)
            {
                int u = create(nodes[cur].parent, nodes[cur].lft, right - left);
                nodes[cur].parent = u;
                nodes[cur].lft += len;
                nodes[u].bor[s[nodes[u].lft + len] - 'a'] = cur;
                nodes[nodes[u].parent].bor[s[nodes[u].lft] - 'a'] = u;
                cur = u;
            }
        }
        if (nodes[cur].d == right - left)
        {
            leaf = nodes[cur].bor[s[right] - 'a'] == -1;
            if (leaf)
                nodes[cur].bor[s[right] - 'a'] = create(cur, right, -1);
            cur = nodes[cur].bor[s[right] - 'a'];
        }
    }

    int create(int p, int l, int depth) {
        nodes[next].parent = p;
        nodes[next].lft = l;
        nodes[next].suf = -1;
        nodes[next].d = depth;
        nodes[next].bor.assign(27, -1);
        return next++;
    }

public:
    void build(char* _s, int len) {
        s = _s;
        n = len;
        int m = 2 * n + 10;
        while (nodes.size() < m) {
            nodes.push_back(Node());
        }
        next = 0;
        create(0, 0, 0);
        nodes[0].suf = 0;
        left = 0;
        cur = 0;
        link = -1;

        for (int right = 0; right < n; right++)
        {
            while (left <= right)
            {
                add(right);

                if (link != -1) {
                    nodes[link].suf = nodes[cur].parent;
                    link = -1;
                }
                if (!leaf)
                    break;

                left++;

                cur = nodes[cur].parent;
                if (nodes[cur].suf == -1)
                {
                    link = cur;
                    cur = nodes[cur].parent;
                }
                cur = nodes[cur].suf;
                while (nodes[cur].d != -1 && nodes[cur].d < right - left)
                    add(left + nodes[cur].d);
            }
        }
    }

    Ukkonen(char * s, int len)
    {
        build(s, len);
    }

    int substring()
    {
        int result = 0;
        for (int i = 0; i < next; i++)
                if (nodes[i].d != - 1)
                        result += nodes[i].d - nodes[nodes[i].parent].d;
                else
                        result += n - nodes[i].lft;

        return result;
    }


};

int main()
{
    char* s = new char[8001];
    int k, l;
    scanf("%d%s", &k, s);
    l = strlen(s);
    memcpy(s + l, s, l);
    s[2 * l] = 0;
    Ukkonen ukkonen(s, 0);
    for (int i = 0; i < l; i++) {
        char t = s[i + k];
        s[i + k] = 'z' + 1;
        ukkonen.build(s + i, k + 1);
        printf("%d ", ukkonen.substring() - k - 1);
        s[i + k] = t;
    }


    return 0;
}
