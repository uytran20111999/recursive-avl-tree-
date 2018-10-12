#include<iostream>
enum balfactor{LH,EH,RH};
using namespace std;
struct tree {
	int data;
	balfactor balance_factor;
	tree* left;
	tree* right;
};
tree* creat_tree(int data) {
	tree* temp = new tree();
	temp->right = temp->left = NULL;
	temp->balance_factor = EH;
	temp->data = data;
	return temp;
}
void rightrotate(tree*& x) {
	tree *y = x->left;
	x->left = y->right;
	y->right = x;
	switch (y->balance_factor)
	{
	case EH:y->balance_factor = RH; x->balance_factor = LH; break;
	case LH:y->balance_factor = EH; x->balance_factor = EH; break;
	}
	x = y;
}
void leftrotate(tree*& x){
	tree * y = x->right;
	x->right = y->left;
	y->left = x;
	switch (y->balance_factor)
	{
	case EH:y->balance_factor = LH; x->balance_factor = RH; break;
	case RH:y->balance_factor = EH; x->balance_factor = EH; break;
	}
	x = y;
}
void  LRrotare(tree*& x) {
	tree* y = x->left;
	tree* z = y->right;
	leftrotate(y);
	rightrotate(x);
	switch (z->balance_factor)
	{
	case EH:y->balance_factor = EH; x->balance_factor = EH; break;
	case LH:y->balance_factor = EH; x->balance_factor = RH; break;
	case RH:y->balance_factor = LH; x->balance_factor = EH; break;
	}
	z->balance_factor = EH;
	x = z;
}
void RLrotate(tree*& x) {
	tree* y = x->right;
	tree* z = y->left;
	rightrotate(y);
	leftrotate(x);
	switch (z->balance_factor)
	{
	case EH:y->balance_factor = EH; x->balance_factor = EH; break;
	case LH:y->balance_factor = RH; x->balance_factor = EH; break;
	case RH:y->balance_factor = EH; x->balance_factor = LH; break;
	}
	z->balance_factor = EH;
	x = z;
}
int fixleft(tree*&x) {// x lech trai
	tree*	y = x->left;
	switch (y->balance_factor)
	{
	case LH: rightrotate(x); return 2;
	case EH: rightrotate(x); return 1;
	case RH:LRrotare(x); return 2;
	}
}
int fixright(tree*& x) {
	tree* y = x->right;
	switch (y->balance_factor)
	{
	case LH:RLrotate(x); return 2;
	case EH:leftrotate(x); return 1;
	case RH: leftrotate(x); return 2;
	}
}
int insertNODE(tree* &root,int data) {//2 la do dai tang, //1 la da sua cay, // -1 la null
	if (root != NULL) {
		int rec=0;
		if (root->data > data) { rec=insertNODE(root->left, data); 
		if (rec < 2)return rec;
		switch (root->balance_factor)
		{
		case EH:root->balance_factor = LH; return 2;
		case RH:root->balance_factor = EH; return 1;
		case LH:fixleft(root); return 1;
		}
		}
		else {
			rec=insertNODE(root->right, data);
			if (rec < 2)return rec;
			switch (root->balance_factor)
			{
			case EH:root->balance_factor = RH; return 2;
			case RH:fixright(root); return 1;
			case LH:root->balance_factor = EH; return 1;
			}
		}
	}
	root = creat_tree(data);
	if (root == NULL)return -1;
	else return 2;
}
void delete_alltree(tree* root) {
	if (root != NULL) {
		delete_alltree(root->left);
		delete_alltree(root->right);
		delete root;
	}
}
void LNR(tree* root){
	if (root != NULL) {
		LNR(root->left);
		cout << root->data << '\t';
		LNR(root->right);
	}
}
int findminright(tree*& subroot,tree*& father){
	if (subroot->left != NULL) {
		int rec =findminright(subroot->left,father);
		if (rec < 2)return rec;
		switch (subroot->balance_factor)
		{
		case EH:subroot->balance_factor = RH; return 1;
		case LH:subroot->balance_factor = EH; return 2;
		case RH: return fixright(subroot);
		}
	}
	else {
		tree* temp = subroot;
		subroot = subroot->right;
		father->data = temp->data;
		delete temp;
		return 2;
	}
}
int deleteNODE(tree*& root, int data){//2 do dai giam,1 da sua,-1 khong tim thay
	if(root!=NULL){
		int rec = 0;
		if (root->data > data) {
			rec = deleteNODE(root->left, data);
			if (rec < 2)return rec;
			switch (root->balance_factor)
			{
			case EH: root->balance_factor = RH; return 1;
			case RH: return fixright(root);
			case LH: root->balance_factor = EH; return 2;
			}
		}
		else if (root->data < data) {
			rec = deleteNODE(root->right, data);
			if (rec < 2)return rec;
			switch (root->balance_factor)
			{
			case EH: root->balance_factor = LH; return 1;
			case RH: root->balance_factor = EH; return 2;
			case LH: return fixleft(root); 
			}
		}
		else {
			int rec = 0;
			tree* tem=NULL;
			if (root->left == NULL) {
				tem = root;
				root = root->right; rec = 2; delete tem;
			}
			else if (root->right == NULL) {
				tem = root;
				root = root->left;
				rec = 2;
				delete tem;
			}
			else {
				rec = findminright(root->right, root);
				if (rec < 2)return rec;
				switch (root->balance_factor)
				{
				case EH: root->balance_factor = LH; return 1;
				case RH: root->balance_factor = EH; return 2;
				case LH : return fixleft(root);
				}
			}
			return rec;

		}
	}
	else return -1;
}
void main() {
	int a[] = { 12,54,2,5,6,1,2,1 };
	tree* root = NULL;
	int n = sizeof(a) / 4;
	for (int i = 0; i < n; i++)insertNODE(root, a[i]);
	for (int i = 0; i < n; i++) {
		deleteNODE(root, a[i]); LNR(root); cout << endl;
}
	LNR(root);
	delete_alltree(root);
}