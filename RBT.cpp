#include <iostream>
//2015112147 �赵��
using namespace std;

// node Ŭ����
class node 
{
public:
	node(int key, node* left, node* right, char c) //������.
		:key(key), left(left), right(right), color(c) {}
	
private:
	int key;	// ������
	node* parent;
	node* left;
	node* right;
	char color; // false = black, true = red
	friend class RBTree;
};

class RBTree 
{
// ����Ž��Ʈ���� �����ϴ� Ŭ����
public:

	RBTree() 
	{ 
		head = 0;
	}


	void RB_Insert(int data)
	{
		node* new_node = new node(data, 0, 0, 'r'); // ������ ��� ����
		node* tmp = head; // ���� �� ��ġ
		node* parent_tmp = 0; // ������ ����� �θ� ������

		if (head == 0) //Ʈ���� ������� ���
		{
			head = new_node;
			new_node->parent = 0; 
		}
		else //Ʈ���� ������� ���
		{
			while (tmp != 0) // ����ִ� ��ġ�� Ž��
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

		insert_fix(new_node);// Ʈ�� ���� ����
	}
	
	void left_rotate(node*& x)
	{
		node* y = x->right;	
		x->right = y->left; //x�� �������� y�� ������ ����Ű�� ��
		if (y->left != 0) // y�� ������ ���� �ʾ��� ��� �θ� ������
		{
			y->left->parent = x;
		}

		y->parent = x->parent; // x�� ��ġ�� y�� ���Ƿ�, �θ� ������
		
		if (x->parent != 0) // �θ��� �ڽ� �����͸� ������
		{
			if (x == x->parent->left) 
				x->parent->left = y;
			else
				x->parent->right = y;
		}
		else // �θ� ���� ���, �� x�� ��Ʈ���� ���
		{
			head = y;
		}

		y->left = x; // y�� ������ x�� ����Ű����.
		x->parent = y; // x�� �θ������͸� ���� 
	}

	// left rotate�� ��Ī
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
		 node* p_p;	// �θ�������
		 node* g_p; // ���θ� ������
		 node* u_p; // ���� ������
		if (p == head) // �ش� ��尡 ��Ʈ�� ���, ���� �ٲ��ְ� ����.
		{
			p->color = 'b';
			return;
		}

		// ��尡 ��Ʈ�� �ƴϰ�, �������̰�, ����� �θ�� ���� �������� ���
		while ((p != head) && (p->color == 'r')&&(p->parent->color == 'r'))
		{		
			p_p = p->parent;
			g_p = p->parent->parent;

			if (p_p == g_p->left) // �θ� ���� �ڽ��� ���
			{
				u_p = g_p->right; // ������ ������ �ڽ��̴�.

				// ������ �����ϰ�, �������� ��� = case1.
				if ((u_p != 0) && (u_p->color == 'r'))
				{
					p->parent->color = 'b';
					u_p->color = 'b';
					g_p->color = 'r';
					p = g_p;
				}
				else
				{
					// ��尡 �θ��� ������ �ڽ��� ���. = case 2.
					if (p == p_p->right)
					{
						// �·� ���� �� Ŀ���� ���� �θ� ����Ű����.
						left_rotate(p_p);
						p = p_p;
						p_p = p->parent; // p_p�� �θ��� �θ�
					}

					// ��尡 �θ��� ���� �ڽ��� ��� �ٷ� case3����
					right_rotate(g_p);
					p->parent->color = 'b';
					g_p->color = 'r';
					p = p_p;
				}
			}
			else // �θ� ������ �ڽ��� ���. �����ڽ��� ����� ��Ī�̴�.
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
		head->color = 'b'; // ��Ʈ�� ���� ����������.
	}
	
	
	// ���� ���
	void show_sub(node* p, int level)
	{

		if (p != 0)
		{ 
			// �Ű������� ���� �����Ͱ� �ΰ��� �ƴ� ���
			show_sub(p->right, level + 1); // ���� �ڽ� ����
			for (int i = 0; i < level; i++) 
			{ // ������ ���� ��ŭ tab 
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
			show_sub(p->left, level + 1); // ���� �ڽ� ����
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