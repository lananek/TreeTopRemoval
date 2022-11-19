#include <algorithm>
#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct NodeTree
{
	long long info = 0ll;
	long long pair_dist_halfways_v = 0ll;
	long long pair_dist_halfways = 0ll;
	long long way = 0ll;
	long long hight = 0ll;
	long long leaf = 0ll;
	long long number_of_ways = 0ll;
	NodeTree* left = nullptr;
	NodeTree* right = nullptr;

	explicit NodeTree(long long _info = 0ll, NodeTree* _left = 0, NodeTree* _right = 0, long long _way = 0ll, long long _hight = 0ll,
		long long _number_of_ways = 0ll, long long _leaf = 0ll)
		: info(_info), left(_left), right(_right), way(_way), hight(_hight), number_of_ways(_number_of_ways), leaf(_leaf) {}

	bool IsLeaf() { return left == 0 && right == 0; }
};


class KBinaryTree
{
private:
	long long max_of_ways = 0ll;
	NodeTree* pTree = nullptr;

public:
	KBinaryTree() : pTree(nullptr), max_of_ways(0) {}
	~KBinaryTree() { Erase(pTree); }

	void Insert(long long info);
	inline bool IsTwoSons(NodeTree* p);
	inline void DeleteLE1Sons(NodeTree* p, NodeTree** pp);
	bool Delete(long long info);
	long long Get_max_way();
	long long Get_max_way(NodeTree* p, long long& h);
	bool Way(long long& anwser);
	void Way(NodeTree* p, long long& anwser);

	friend istream& operator >> (istream& fi, KBinaryTree& a)
	{
		string s;
		while (fi)
		{
			if (getline(fi, s)) { a.Insert(stoll(s)); }
		}
		return fi;
	}

	std::ostream& PrintPreO(std::ostream& os) const;
	std::ostream& PrintPreO(std::ostream& os, NodeTree* t) const;

private:
	void Erase(NodeTree*& t);
	bool Leaf(NodeTree* t);
};

long long KBinaryTree::Get_max_way()
{
	long long h;
	return Get_max_way(pTree, h);
}

long long KBinaryTree::Get_max_way(NodeTree* p, long long& h)
{
	long long way = 0;
	if (p == nullptr) { h = 0; }
	else if (p->IsLeaf()) { h = 0; p->leaf = 1; }
	else
	{
		long long hr, hl;
		Get_max_way(p->right, hr);
		Get_max_way(p->left, hl);
		h = 1 + max(hl, hr);
		way = (p->right == nullptr ? 0 : 1) + (p->left == nullptr ? 0 : 1) + hl + hr;
		if (way > max_of_ways) { 
			max_of_ways = way; 
		}
		p->way = way; 
		p->hight = h;
		if (p->left == nullptr && p->right != nullptr) {
			p->leaf = p->right->leaf; 
		}
		else if (p->left != nullptr && p->right == nullptr) 
		{ p->leaf = p->left->leaf; 
		}
		else { 
			p->leaf = p->right->hight == p->left->hight ? p->left->leaf + p->right->leaf : (p->right->hight > p->left->hight ? p->right->leaf : p->left->leaf); }
	}
	return way;
}

bool KBinaryTree::Way(long long& anwser)
{
	anwser = INT_MIN;
	Way(pTree, anwser);
	if (anwser != INT_MIN) { return true; }
	return false;
}

void KBinaryTree::Way(NodeTree* p, long long& anwser)
{
	if (p)
	{
		if (p->way == max_of_ways)
		{
			if (p->left != nullptr && p->right != nullptr)
			{
				p->pair_dist_halfways = p->left->leaf * p->right->leaf;
			}
			else if (p->left != nullptr && p->right == nullptr)
			{
				p->pair_dist_halfways = p->left->leaf * 1;
			}
			else if (p->left == nullptr && p->right != nullptr)
			{
				p->pair_dist_halfways = p->right->leaf * 1;
			}
		}

		if (p->left != nullptr && p->right != nullptr)
		{
			if (p->left->hight > p->right->hight)
			{
				p->left->pair_dist_halfways_v = p->pair_dist_halfways_v + p->pair_dist_halfways;
				p->right->pair_dist_halfways_v = p->pair_dist_halfways;
			}
			else if (p->left->hight < p->right->hight)
			{
				p->left->pair_dist_halfways_v = p->pair_dist_halfways;
				p->right->pair_dist_halfways_v = p->pair_dist_halfways_v + p->pair_dist_halfways;
			}
			else
			{
				p->left->pair_dist_halfways_v = p->pair_dist_halfways + p->left->leaf * (p->pair_dist_halfways_v / p->leaf);
				p->right->pair_dist_halfways_v = p->pair_dist_halfways + p->right->leaf * (p->pair_dist_halfways_v / p->leaf);
			}
		}
		else if (p->left == nullptr && p->right != nullptr)
		{
			p->right->pair_dist_halfways_v = p->pair_dist_halfways_v + p->pair_dist_halfways;
		}
		else if (p->left != nullptr && p->right == nullptr)
		{
			p->left->pair_dist_halfways_v = p->pair_dist_halfways_v + p->pair_dist_halfways;
		}
		if ((p->pair_dist_halfways_v + p->pair_dist_halfways) != 0 && (p->pair_dist_halfways_v + p->pair_dist_halfways) % 2 != 0 && p->info > anwser)
		{
			anwser = p->info;
		}
		Way(p->left, anwser);
		Way(p->right, anwser);
	}
}

void KBinaryTree::Insert(long long info)
{
	NodeTree** pp = &pTree, * p = pTree;
	while (p)
	{
		if (info < p->info) { pp = &p->left; p = p->left; }
		else { pp = &p->right; p = p->right; }
	}
	*pp = new NodeTree(info);
}

inline bool KBinaryTree::IsTwoSons(NodeTree* p)
{
	return (p->left && p->right) ? true : false;
}

inline void KBinaryTree::DeleteLE1Sons(NodeTree* p, NodeTree** pp)
{
	if (p->left) *pp = p->left;
	else *pp = p->right;
	delete p;
}

bool KBinaryTree::Delete(long long info)
{
	NodeTree** pp = &pTree, * p = pTree;
	while (p)
	{
		if (info < p->info) { pp = &p->left; p = p->left; }
		else if (info > p->info) { pp = &p->right; p = p->right; }
		else break;
	}
	if (p == nullptr)
		return false;
	if (!IsTwoSons(p))
	{
		DeleteLE1Sons(p, pp);
	}
	else
	{
		pp = &p->right;
		NodeTree* p1 = p->right;
		while (p1->left) { pp = &p1->left; p1 = p1->left; }
		p->info = p1->info;
		DeleteLE1Sons(p1, pp);
	}
	return true;
}

bool KBinaryTree::Leaf(NodeTree* t)
{
	if (t)
		if (t->left == 0 && t->right == 0) return true;
	return false;
}

void KBinaryTree::Erase(NodeTree*& t)
{
	if (t) { Erase(t->left); Erase(t->right); delete t; }
}

std::ostream& KBinaryTree::PrintPreO(std::ostream& os) const
{
	return PrintPreO(os, pTree);
}

std::ostream& KBinaryTree::PrintPreO(std::ostream& os, NodeTree* t) const
{
	if (t) { os << t->info << endl; PrintPreO(os, t->left); PrintPreO(os, t->right); }
	return os;
}


int main()
{
	
	long long temp;
	KBinaryTree t;
	ifstream fin("tst.in", ios::out);
	ofstream fout("tst.out");
	fin >> t; 
	t.Get_max_way();
	if (t.Way(temp)) { t.Delete(temp); }
	t.PrintPreO(fout);
	fout.close();
	fin.close();
}