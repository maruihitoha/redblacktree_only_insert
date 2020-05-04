#include <iostream>
//2015112147 김도훈
using namespace std;

// node 클래스
class node 
{
public:
	node(int key, node* left, node* right, char c) //생성자.
		:key(key), left(left), right(right), color(c) {}
	
private:
	int key;	// 데이터
	node* parent;
	node* left;
	node* right;
	char color; // false = black, true = red
	friend class RBTree;
};

class RBTree 
{
// 이진탐색트리를 관리하는 클래스
public:

	RBTree() 
	{ 
		head = 0;
	}


	void RB_Insert(int data)
	{
		node* new_node = new node(data, 0, 0, 'r'); // 삽입할 노드 생성
		node* tmp = head; // 삽입 할 위치
		node* parent_tmp = 0; // 삽입할 노드의 부모 포인터

		if (head == 0) //트리가 비어있을 경우
		{
			head = new_node;
			new_node->parent = 0; 
		}
		else //트리가 비어있을 경우
		{
			while (tmp != 0) // 비어있는 위치를 탐색
			{
				parent_tmp = tmp;
				if (tmp->key < new_node->key)
					tmp = tmp->right;
				else
					tmp = tmp->left;
			}

			new_node->parent = parent_tmp;

			if (parent_tmp->key > new_node->key)
				parent_tmp->left = new_node;
			else
				parent_tmp->right = new_node;
		}

		insert_fix(new_node);// 트리 구조 수정
	}
	
	void left_rotate(node*& x)
	{
		node* y = x->right;	
		x->right = y->left; //x의 오른쪽이 y의 왼쪽을 가리키게 함
		if (y->left != 0) // y의 왼쪽이 비지 않았을 경우 부모를 맞춰줌
		{
			y->left->parent = x;
		}

		y->parent = x->parent; // x의 위치에 y가 오므로, 부모를 맞춰줌
		
		if (x->parent != 0) // 부모의 자식 포인터를 맞춰줌
		{
			if (x == x->parent->left) 
				x->parent->left = y;
			else
				x->parent->right = y;
		}
		else // 부모가 없는 경우, 즉 x가 루트였을 경우
		{
			head = y;
		}

		y->left = x; // y의 왼쪽이 x를 가리키도록.
		x->parent = y; // x의 부모포인터를 맞춤 
	}

	// left rotate와 대칭
	void right_rotate(node*& x)
	{

		node* y = x->left;
		x->left = y->right;

		if (x->left != 0)
		{
			x->left->parent = x;
		}

		y->parent = x->parent;

		if (x->parent != 0)
		{
			if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;
		}
		else
		{
			head = y;
		}

		y->right = x;
		x->parent = y;
	}
	

	void insert_fix(node*& p) 
	{
		 node* p_p;	// 부모포인터
		 node* g_p; // 조부모 포인터
		 node* u_p; // 삼촌 포인터
		if (p == head) // 해당 노드가 루트일 경우, 색만 바꿔주고 리턴.
		{
			p->color = 'b';
			return;
		}

		// 노드가 루트가 아니고, 빨간색이고, 노드의 부모색 또한 빨간색일 경우
		while ((p != head) && (p->color == 'r')&&(p->parent->color == 'r'))
		{		
			p_p = p->parent;
			g_p = p->parent->parent;

			if (p_p == g_p->left) // 부모가 왼쪽 자식일 경우
			{
				u_p = g_p->right; // 삼촌은 오른쪽 자식이다.

				// 삼촌이 존재하고, 빨간색일 경우 = case1.
				if ((u_p != 0) && (u_p->color == 'r'))
				{
					p->parent->color = 'b';
					u_p->color = 'b';
					g_p->color = 'r';
					p = g_p;
				}
				else
				{
					// 노드가 부모의 오른쪽 자식일 경우. = case 2.
					if (p == p_p->right)
					{
						// 좌로 돌린 뒤 커서가 원래 부모를 가리키도록.
						left_rotate(p_p);
						p = p_p;
						p_p = p->parent; // p_p는 부모의 부모
					}

					// 노드가 부모의 왼쪽 자식일 경우 바로 case3진입
					right_rotate(g_p);
					p->parent->color = 'b';
					g_p->color = 'r';
					p = p_p;
				}
			}
			else // 부모가 오른쪽 자식일 경우. 왼쪽자식일 경우의 대칭이다.
			{
				u_p = g_p->left;
				if ((u_p!=0)&&(u_p->color == 'r'))
				{
					p_p->color = 'b';
					u_p->color = 'b';
					g_p->color = 'r';
					p = g_p;
				}
				else
				{
					if (p == p_p->left)
					{
						right_rotate(p_p);
						p = p_p;
						p_p = p->parent;
					}		
					left_rotate(g_p);
					p->parent->color = 'b';
					g_p->color = 'r';
					p = p_p;
				}
			}
		}
		head->color = 'b'; // 루트의 색을 검은색으로.
	}
	
	
	// 구조 출력
	void show_sub(node* p, int level)
	{

		if (p != 0)
		{ 
			// 매개변수로 받은 포인터가 널값이 아닐 경우
			show_sub(p->right, level + 1); // 우측 자식 실행
			for (int i = 0; i < level; i++) 
			{ // 현재의 레벨 만큼 tab 
				cout << '\t';
			}
			
			cout << " " << p->key << " " << p->color; 
			
			if ((p->left != 0) && (p->right != 0)) 
			{
				cout << "<";
			}
			else if (p->right != 0)
			{
				cout << "/";
			}
			else if (p->left != 0) 
			{
				cout << "\\";
			}
			cout << endl;
			show_sub(p->left, level + 1); // 좌측 자식 실행
		}
	}


	void show_structure() 
	{
		if (head == 0) 
		{
			cout << "empty" << endl;
		}
		else 
		{
			cout << endl;
			show_sub(head, 1);
			cout << endl;
		}
	}


	
private:
	node* head;
};

int main()
{
	RBTree test;
	node* t_node;
	int input;
	char cmd;

	cout << endl << "Commands:" << endl;
	cout << "  +key : Insert (or update) element" << endl;

	do 
	{
		cout << endl;
		test.show_structure();
		cout << endl << "command : ";
		cin >> cmd;
		if (cmd == '+')
			cin >> input;
		switch (cmd) 
		{

		case '+':
			cout << "insert : key = " << input << endl;
			test.RB_Insert(input);
			break;

		case 'Q': case 'q':
			break;

		default:
			cout << "invalid command" << endl;

		}

	} while ((cmd != 'Q') && (cmd != 'q'));

}