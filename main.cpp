#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>


class Ukkonen
{
private:
    class Node {
    public:
        int parent, suf, d;
        size_t lft;
        std::vector<int> bor;
        Node(){}
    };
    std::vector<Node> nodes_;
    int next_, left_, link_;
    size_t cur_;
    std::string s_;
    bool leaf_;
    size_t n_;

    void add(int right)
    {
        leaf_ = false;
        if (nodes_[cur_].d != right - left_)
        {
            size_t len = right - left_ - nodes_[nodes_[cur_].parent].d;
            leaf_ = s_[nodes_[cur_].lft + len] != s_[right];
            if (leaf_)
            {
                size_t u = create(nodes_[cur_].parent, nodes_[cur_].lft, right - left_);
                nodes_[cur_].parent = u;
                nodes_[cur_].lft += len;
                nodes_[u].bor[s_[nodes_[u].lft + len] - 'a'] = cur_;
                nodes_[nodes_[u].parent].bor[s_[nodes_[u].lft] - 'a'] = u;
                cur_ = u;
            }
        }
        if (nodes_[cur_].d == right - left_)
        {
            leaf_ = nodes_[cur_].bor[s_[right] - 'a'] == -1;
            if (leaf_)
                nodes_[cur_].bor[s_[right] - 'a'] = create(cur_, right, -1);
            cur_ = nodes_[cur_].bor[s_[right] - 'a'];
        }
    }

    int create(int p, size_t l, int depth) {
        nodes_[next_].parent = p;
        nodes_[next_].lft = l;
        nodes_[next_].suf = -1;
        nodes_[next_].d = depth;
        nodes_[next_].bor.assign(27, -1);
        return next_++;
    }

    void init(std::string& s) {
        s_ = s;
        n_ = s.size();
        nodes_.assign(2 * n_ + 10, Node());
        next_ = 0;
        create(0, 0, 0);
        nodes_[0].suf = 0;
        left_ = 0;
        cur_ = 0;
        link_ = -1;
    }

    void buildTree(std::string& s)
    {
        for (int right = 0; right < n_; right++)
        {
            while (left_ <= right)
            {
                add(right);
                if (link_ != -1) {
                    nodes_[link_].suf = nodes_[cur_].parent;
                    link_ = -1;
                }
                if (!leaf_)
                    break;

                left_++;

                cur_ = nodes_[cur_].parent;
                if (nodes_[cur_].suf == -1)
                {
                    link_ = cur_;
                    cur_ = nodes_[cur_].parent;
                }
                cur_ = nodes_[cur_].suf;
                while (nodes_[cur_].d != -1 && nodes_[cur_].d < right - left_)
                    add(left_ + nodes_[cur_].d);
            }
        }

    }

public:
    void build(std::string s) {
        init(s);
        buildTree(s);
    }

    Ukkonen(){}

    unsigned int substring()
    {
        unsigned int result = 0;
        for (size_t i = 0; i < next_; i++)
            if (nodes_[i].d != - 1)
                result += nodes_[i].d - nodes_[nodes_[i].parent].d;
            else
                result += n_ - nodes_[i].lft;
        return result;
    }


};

int main()
{
    std::string s;
    size_t k, l;
    std::cin >> k >> s;
    l = s.size();

    s += s;

    Ukkonen ukkonen;
    for (size_t i = 0; i < l; i++) {
        char t = s[i + k];
        s[i + k] = 'z' + 1;
        ukkonen.build(s.substr(i, k + 1));
        printf("%d ", ukkonen.substring() - k - 1);
        s[i + k] = t;
    }

    return 0;
}
