#include<iostream>
#include<string>
#include<vector>

using namespace std;

//线性结构
void work_line() {
	char a[1000];
	int n;
	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> a[i];
	}
	for (int i = 0; i <= n-2; i++) {
		cout << "<" << a[i] << "," << a[i + 1] << ">";
		if (i < n - 2) {
			cout<< ", ";
		}
	}
	cout << endl;
}

//树
typedef char BTDataType; //使用字符

class BTNode {
public:
	BTDataType data;
	BTNode* left;
	BTNode* right;
};
BTNode* BinaryTreeCreate(string arr, int n, int* pi) {
	if (*pi >= n) {
		return  nullptr;
	}
	if (arr[*pi] == '#') {
		(*pi)++;
		return nullptr;
	}
	BTNode* root = new BTNode;
	root->data = arr[(*pi)++];
	//递归创建
	root->left = BinaryTreeCreate(arr, n, pi);
	root->right = BinaryTreeCreate(arr, n, pi);
	return root;
}
void BinaryTreePrevOrder(BTNode* root) {
	if (root == nullptr) {
		return;
	}
	if (root->left) {
		cout << "<" << root->data << "," << root->left->data << ">" << ", ";
	}
	if (root->right) {
		cout << "<" << root->data << "," << root->right->data << ">" << ", ";
	}
	BinaryTreePrevOrder(root->left);
	BinaryTreePrevOrder(root->right);
}
void work_tree() {
	//输入数据
	string arr;
	cin >> arr;
	int n = arr.size();
	int i = 0;

	BTNode* root = BinaryTreeCreate(arr, n, &i);

	BinaryTreePrevOrder(root);
}

//图形结构
void mapCreate(vector<vector<int>>& arr,int m) {
	char a, b;
	for (int i = 0; i < m; i++) {
		scanf("%c,%c", &a, &b);
		getchar();
		arr[a - 'a'][b - 'a'] = 1;
	}
}
void work_map() {
	vector<vector<int>> arr(10,vector<int>(10,0));
	int n = 0, m = 0;
	cin >> n >> m;  //n个点,m条边
	getchar();
	mapCreate(arr,m);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (arr[i][j] == 1) {
				cout << "<" << (char)(i + 'a') << "," << (char)(j + 'a') << ">" << ", ";
			}
		}
	}
}

int main(void) {
	work_line();
	work_tree();
	work_map();
	return 0;
}