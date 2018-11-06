#ifndef __RED_BLACK_TREE_H__
#define __RED_BLACK_TREE_H__
#ifndef NULL
#define __NULL_IS_DEFINED__
#define NULL (nd*)0
#endif//NULL
/* Header File Info
	�ۼ� ���� : 2017 - 04 - 08 ���� 8�� 53��
	�ۼ��� : Stoads
*/
/* RED-BLACK TREE�� ����
	1. ��� ���� ������ Ȥ�� �������� ���´�
	2. ��Ʈ�� �׻� �������̴�
	3. ��� NULL LEAF�� �������̴�
	4. � ��� n�� �������̸�, n�� �� �ڽ��� �׻� �������̴�(��, �������� �����ؼ� ���� �� ����)
	5. ������ ��� n���κ��� ������ �� �ִ� ��� NULL LEAF������ �׻� ���� ���� ������ ��带 �����ؼ� ��������(�ִܰŸ��� ���� �Ÿ��� �׻� 2�� ���Ϸ� ���̳���)
*/
template <typename K, typename T>
class RedBlackTree {
private:
	//RED-BLACK TREE���� ����� ��
	enum NodeColor { RED = 0, BLACK };
	//Ʈ���� ���
	class RBNode {
		//RedBlackTree ��ü������ Node�� private �Ӽ��� ���ٵ� �����ο��� ��
		friend class RedBlackTree<K, T>;
	public:
		//������ �����ε�
		operator T() {
			return data;
		}
		RBNode& operator=(T data) {
			this->data = data;
			return *this;
		}
		RBNode& operator++() {
			RBNode* p = this;
			*p = *p->n;
			return *p;
		};
		RBNode& operator++(int) {
			RBNode *p = this;
			*p = *p->n;
			return *p->b;
		};
		RBNode& operator--() {
			RBNode* p = this;
			*p = *p->b;
			return *p;
		};
		RBNode& operator--(int) {
			RBNode* p = this;
			*p = *p->b;
			return *p->n;
		};
		bool operator==(RBNode& desc) {
			return this->key == desc.key && this->b==desc.b;
		}
		bool operator==(RBNode&& desc) {
			return this->key == desc.key && this->b == desc.b;
		}
		bool operator!=(RBNode& desc) {
			return this->key != desc.key || this->b != desc.b;
		}
		bool operator!=(RBNode&& desc) {
			return this->key != desc.key || this->b != desc.b;
		}
		//Key���� ��ȯ��
		K getKey() { return key; }
	private:
		//key���� �ʱ�ȭ�ϴ� ������, �������� ���� �� �����ڸ� ���� �����Ǿ� �� (ó�� �����Ǵ� ���� �⺻������ ��������)
		RBNode(K key) : key(key), p(NULL), l(NULL), r(NULL), b(NULL), n(NULL), col(RED) {}
		//key���� �ʱ�ȭ���� �ʴ� ������, ���������� ���(END, NULL LEAF) ���� ������ �� ����� (NULL LEAF�� �⺻������ ��������)
		RBNode() : p(NULL), l(NULL), r(NULL), b(NULL), n(NULL), col(BLACK) {}
		//����� ��
		T data;
		//����� ��
		int col;
		//����� Ű��
		K key;
		//�θ� ��� ������
		struct RBNode *p;
		//���� �ڽ� ��� ������
		struct RBNode *l;
		//������ �ڽ� ��� ������
		struct RBNode *r;
		//�ٷ� ���ʿ� �ִ� ��带 ����Ű�� ������
		struct RBNode *b;
		//�ٷ� �����ʿ� �ִ� ��带 ����Ű�� ������
		struct RBNode *n;
	};
	typedef RBNode nd;
	//Ʈ���� ��Ʈ
	nd* head = NULL;
	//Ʈ���� ���� ���� ��� - [a,b]���� a�� �ش��ϴ� ���
	nd* first = NULL;
	//Ʈ���� ���� ���� ��� - [a,b]���� b�� �ش��ϴ� ���
	nd* last = NULL;
	//������ END ��� [a,b)���� b�� �ش��ϴ� ���
	nd* end = new RBNode;
	//Ʈ���� ũ��
	size_t siz = 0;
	//��Ʈ�� �缳��
	void setHead() {
		while (head->p != NULL) {
			head = head->p;
		}
	}
	//���� ����� ���θ� ���
	nd* grandNode(nd* p) {
		if (p != NULL && p->p != NULL)
			return p->p->p;
		return NULL;
	}
	//���� ����� ���� ���
	nd* unclneNode(nd* p) {
		nd* g = grandNode(p);
		if (g == NULL)return NULL;
		if (p->p == g->l)
			return g->r;
		return g->l;
	}
	//���� ����� ���� ���
	nd* brotherNode(nd* p) {
		if (p->p == NULL)return NULL;
		return p->p->r == p ? p->p->l : p->p->r;
	}
	//p�� �������� ���� ȸ��
	void leftRotate(nd* p) {
		nd *c = p->r, *s = p->p;
		if (c->l != NULL)
			c->l->p = p;
		p->r = c->l;
		p->p = c;
		c->l = p;
		c->p = s;
		if (s != NULL)
			if (s->l == p)
				s->l = c;
			else
				s->r = c;
	}
	//p�� �������� ������ ȸ��
	void rightRotate(nd* p) {
		nd *c = p->l, *s = p->p;
		if (c->r != NULL)
			c->r->p = p;
		p->l = c->r;
		p->p = c;
		c->r = p;
		c->p = s;
		if (s != NULL)
			if (s->r == p)
				s->r = c;
			else
				s->l = c;
	}
	//�߰��� ��尡 � ��Ģ�� ���ݵǴ��� �Ǵ���
	void judgeInsertNodeCase(nd* p) {
		if (p == NULL)return;
		//u�� p�� ���� ���
		nd* u = unclneNode(p);
		//��Ʈ�����
		if (p->p == NULL)
			insertNodeCase1(p);
		//�θ� �������̶��
		else if (p->p->col == BLACK)
			insertNodeCase2(p);
		//�θ�� ������ ���� �������̶��
		else if ((u != NULL) && (u->col == RED))
			insertNodeCase3(p);
		//�� ���� ���
		else
			insertNodeCase4(p);
	}
	//�θ��� ���������� ���� ��带 ���������� �ٲ�
	void insertNodeCase1(nd* p) {
		if (p->p == NULL)
			p->col = BLACK;
	}
	//�θ� �������̸� ������ ó���� �ʿ� ����
	void insertNodeCase2(nd* p) {
		return;
	}
	//�θ�� ������ ���� �������̸� ���θ� ���κ��� �������� �����ް� ���θ� ��忡 ���ؼ� Ʈ���� ��Ģ�� �ٽ� �˻�
	void insertNodeCase3(nd* p) {
		nd* u = unclneNode(p), *g;
		p->p->col = BLACK;
		u->col = BLACK;
		g = grandNode(p);
		g->col = RED;
		judgeInsertNodeCase(g);
		return;
	}
	//������ ��쿡 ������ Ʈ���� ȸ���ϰ� ���� �ٲ����ν� RED-BLACK-TREE�� 4,5�� ��Ģ�� ������Ŵ
	void insertNodeCase4(nd* p) {
		nd* g = grandNode(p);
		nd* s = p->p;
		if (g != NULL) {
			if (g->l != NULL && p == g->l->r) {
				leftRotate(s);
				p = p->l;
			}
			else if (g->r != NULL && p == g->r->l) {
				rightRotate(s);
				p = p->r;
			}
		}
		g = grandNode(p);
		s = p->p;
		if (g != NULL) {
			if (p == s->l) {
				rightRotate(g);
			}
			else leftRotate(g);
			g->col = RED;
		}
		s->col = BLACK;
	}
	//p�� ��Ʈ��� ������ ó���� �� �ʿ䰡 ����
	void deleteNodeCase1(nd* p) {
		if (p->p != NULL) {
			deleteNodeCase2(p);
		}
	}
	//p�� ���� ����� s�� �������� ��� ������ ���������� �α� ���ؼ� Ʈ���� ȸ���Ѵ�
	void deleteNodeCase2(nd* p) {
		nd* s = brotherNode(p);
		//�ѹ��� ȸ���� �Ͼ�� �ʾ����Ƿ� �ݵ�� s�� NULL�� �� �� ����
		//RED-BLACK TREE�� 5�� ������ ���ؼ� p�� ������ ���� s�� ������ �����\
								 s�� �ݵ�� ������ ��带 ���� �� �ڽ��� �����ϴ� ���� �ڸ���
		if (s->col == RED) {
			p->p->col = RED;
			s->col = BLACK;
			if (p == p->p->l)
				leftRotate(p->p);
			else
				rightRotate(p->p);
		}
		//�� �ּ��� ���� ȸ���� �Ͼ�� p�� ���� ���� �ݵ�� ������
		deleteNodeCase3(p);
	}
	//�θ��尡 �������̰�, ������ �� �ڽ��� ��� �������̶�� ������ ���� ���������� �ٲ����μ� �θ��尡 ��Ʈ�� ����Ʈ�������� 5�� ������ ���� ��Ŵ\
				 ��ü Ʈ���� ���� 5�� ������ ������Ű�� ���ؼ� �θ��带 case1���� �������Ѵ�
	void deleteNodeCase3(nd* p) {
		nd* s = brotherNode(p);
		if (p->p->col == BLACK &&
			//s->col == BLACK &&		//2���� ���ؼ� �������� �ݵ�� �����ϸ� �������̹Ƿ� �ڸ��� ������
			(s->l == NULL || s->l->col == BLACK) &&		//RED-BLACK Tree�� 3�� ������ ���� NULL���� ���������� �����
			(s->r == NULL || s->r->col == BLACK)) {
			s->col = RED;
			deleteNodeCase1(p->p);
		}
		else
			deleteNodeCase4(p);
	}
	//�θ� �������̰�, ������ ������ �ڽĳ�尡 ��� �������̶��, �θ��带 ����������, ���� ��带 ���������� �ٲ۴�
	void deleteNodeCase4(nd* p) {
		nd* s = brotherNode(p);
		if (p->p->col == RED &&
			s->col == BLACK &&
			(s->l == NULL || s->l->col == BLACK) &&
			(s->r == NULL || s->r->col == BLACK)) {
			s->col = RED;
			p->p->col = BLACK;
		}
		else
			deleteNodeCase5(p);
	}
	//������ ���� �ڽ��� �������̰�, ���� ��忡�� ���� �ڽ��� ������ ���� ����� �θ�κ����� ����� ��ġ�Ѵٸ�\
				 ������ ȸ���ؼ� ������ ��ġ���� �ʰ� �����ν� case6�� ������ �� �ְ� �Ѵ�
	void deleteNodeCase5(nd* p) {
		nd* s = brotherNode(p);
		//if(s->col==BLACK) //s�� �������� �ƴϸ� s�� �ڽ��� �������� �� �� �����Ƿ� �ڸ��� ������
		//���� ������ ��� ������ ���������� ȸ���ؼ� ������ �ٸ�����
		if (p->p->l == p &&
			(s->r == NULL || s->r->col == BLACK) &&	//�Ѵ� NULL�� ��� Ȥ��, ������ ������, ������ NULL�� ���� 2-4���� �� ó���ǹǷ�
			(s->l->col == RED)) {	//s->r�� �������̶�� �ݵ�� s->l�� ���������� ������
			s->col = RED;
			s->l->col = BLACK;
			rightRotate(s);
		}
		//������ ������ ��� ������ �������� ȸ���ؼ� ȸ���ؼ� ������ �ٸ�����
		else if (p->p->r == p &&
			(s->l == NULL || s->l->col == BLACK) &&
			(s->r->col == RED)) {
			s->col = RED;
			s->r->col = BLACK;
			leftRotate(s);
		}
		deleteNodeCase6(p);
	}
	//�����ϸ鼭 ���� RED-BLACK TREE�� ���� 4,5���� �����ϰԲ� Ʈ���� ȸ��, ���� �ٲ۴�
	void deleteNodeCase6(nd* p) {
		nd* s = brotherNode(p);
		s->col = p->p->col;
		p->p->col = BLACK;
		//case5�� ���Ͽ� ���� ��尡 �θ� ����� �����̸�, ������ ������ �ڽ��� ���������� �ڸ���
		if (p->p->l == p) {
			s->r->col = BLACK;
			leftRotate(p->p);
		}
		//case5�� ���Ͽ� ���� ��尡 �θ� ����� �������̸�, ������ ���� �ڽ��� ���������� �ڸ���
		else {
			s->l->col = BLACK;
			rightRotate(p->p);
		}
	}
	// NULL�� �ƴ� �ڽ��� 1�� ���Ϸ� ������ ��带 Ʈ������ �����
	void deleteOneChildNode(nd* p) {
		if (p == NULL)return;
		//�� ����� �ڽ��� ����
		nd* c = p->r == NULL ? p->l : p->r;
		//p�� NULL�� �ڽ��� 1�� ���� ������ false, �ƴϸ� true
		bool isChildCreated = false;
		if (c == NULL) {
			//NULL�� ���������� ó���ϹǷ�, ������ �ڽĿ� ������ ��带 �Ҵ���
			c = new RBNode;
			isChildCreated = true;
		}
		if (p->p != NULL) {		//��Ʈ�� �ƴ϶�� c�� p�� �θ� ���� ������
			if (p == p->p->l)
				p->p->l = c;
			else
				p->p->r = c;
		}
		c->p = p->p;	//p�� �θ��带 c�� ������
						/*
						���� p�� �������̶�� RED-BLACK TREE�� 5�� ������ ���ؼ� �ݵ�� ������尡 ������
						*/
		if (p->col == BLACK) {
			if (c->col == RED) //������ ����� �ڽ��� �������̸� �ڽĿ��� �������� �ְ� �߰����� ó���� �� �ʿ䰡 ����
				c->col = BLACK;
			else				//������ ����� �ڽ��� �������̸�
				deleteNodeCase1(c);
		}
		if (p == head) {	//��Ʈ�� �������ٸ�
			head = c;		//��Ʈ�� �������� ����� �ڽ����� �缳��
		}
		delete p;		//�޸��� ������ ���� ���� p�� �Ҵ�� �޸𸮸� ������
		siz--;		//��� p�� ���������Ƿ� ũ�� 1 ����
					//c�� ���Ƿ� ������� ���� ������
		if (isChildCreated) {
			if (c->p != NULL) {
				if (c->p->l == c)
					c->p->l = NULL;
				else
					c->p->r = NULL;
			}
			c->p = NULL;
			delete c;
		}
		if (!siz) {		//Ʈ���� ũ�Ⱑ 0�� �Ǿ����� Ʈ���� �ʱ� ���·� �ǵ���
			head = NULL;
			first = NULL;
			last = NULL;
		}
		else		//�������� ȸ���� �Ͼ ��Ʈ�� �ٲ���� ���ɼ��� �����ϹǷ� ��Ʈ�� �缳����
			setHead();
	}
public:
	/* getFirst -> RBNode
	Ʈ���� ���� ���� ��带 ��ȯ�Ѵ�
	*/
	RBNode getFirst() {
		return *first;
	}
	/* getLast -> RBNode
	Ʈ���� ���� ���� ��带 ��ȯ�Ѵ�
	*/
	RBNode getLast() {
		return *last;
	}
	/* getBegin -> RBNode
	Ʈ���� ���� ���� ��带 ��ȯ�Ѵ�
	*/
	RBNode getBegin() {
		return *first;
	}
	/* getLast -> RBNode
	����� Before ������ Ʈ���� ���� ���� ��带 ���� END ��带 ��ȯ�Ѵ�
	*/
	RBNode getEnd() {
		return *end;
	}
	/* insert
	1. key���� Ű�� ���� ���� data ���� ���� ��带 Ʈ���� �����Ѵ�
	2. key���� Ű�� ���� ��忡 data ������ ���� ����(����)�Ѵ�
	*/
	void insert(K key, T data) {
		operator[](key) = data;		//���Կ����� ���ã�� �� ������ ó���� ������ �����ε� �� ������ ��ü��
	};
	/* erase
	key���� ���� ��带 Ʈ������ �����
	*/
	void erase(K key) {
		if (isEmpty())return;
		auto p = head;
		while (p != NULL && p->key != key) {
			if (p->key > key) {
				if (p->l == NULL)return;
				p = p->l;
			}
			else if (p->key < key) {
				if (p->r == NULL)return;
				p = p->r;
			}
		}
		auto k = p;
		if (p->l != NULL && p->r != NULL) {
			k = p->b;			//���� �ڽ� ��� �ֱ⶧���� p�� �ٷ� ���� �ִ� ���� �ڸ���
			p->key = k->key;
			p->data = k->data;
		}
		if(k->n!=NULL)
			k->n->b = k->b;
		if(k->b!=NULL)
			k->b->n = k->n;
		deleteOneChildNode(k);
		if (head != NULL) {
			first = head;
			while (first->l != NULL)first = first->l;
			last = head;
			while (last->r != NULL)last = last->r;
			end->b = last;
			last->n = end;
		}
	}
	/* clear
	Ʈ���� �ʱ� ���·� �ǵ�����
	*/
	void clear() {
		auto p = first;
		while (p != end) {
			auto k = p;
			p = p->n;
			delete k;
		}
		siz = 0;
		head = NULL;
		first = NULL;
		last = NULL;
		end->b = NULL;
	}
	/* isEmpty -> bool
	Ʈ���� ����ִ����� ��ȯ�Ѵ�
	*/
	bool isEmpty() {
		return siz == 0;
	}
	/* size -> int
	Ʈ���� �����ϴ� ����� ������ ��ȯ�Ѵ�
	*/
	int size() {
		return siz;
	}
	/* find -> RBNode&
	key���� Ű�� ���� ��带 ã�Ƽ� ��ȯ�Ѵ�
	*/
	RBNode& find(K key) {
		nd* p = head;
		while (p != NULL) {
			if (p->key > key)
				p = p->l;
			else if (p->key < key)
				p = p->r;
			else
				return *p;
		}
		return end;
	}
	/* operator[key] -> RBNode&
	1. key���� ���� ��带 ã�Ƽ� ��ȯ�Ѵ�
	2. key���� ���� ��尡 ���ٸ� ���� ��ȯ�Ѵ�
	*/
	RBNode& operator[](K key) {
		//key���� �� ��� p
		auto p = head;
		//2���� ��� �߰��� �̷���� ��, �� ó�� Ȥ�� �� �ڿ� ��尡 �߰��� ��� first�� end�� ���ŵǾ����
		bool isFirst = true, isLast = true;
		while (p != NULL) {
			//���� ������ ���ʿ� ��ġ��
			if (p->key > key) {
				//key���� ���� ��尡 ���ٸ� ��� �߰�
				if (p->l == NULL) {
					p->l = new nd(key);
					p->l->p = p;
					if (p->b != NULL) {
						p->b->n = p->l;
						p->l->b = p->b;
					}
					siz++;
					p->b = p->l;
					p->l->n = p;
					if (isFirst) {
						first = p->l;
					}
					//�߰��� ���� Ʈ���� ��Ģ�� �ٽ� ����
					judgeInsertNodeCase(p->l);
					//��Ʈ�� ȸ���ؼ� �ٲ���� �� �����Ƿ� ��Ʈ�� �缳��
					setHead();
					return *p->l;
				}
				else {
					p = p->l;
					isLast = false;
				}
			}
			//���� ������ �����ʿ� ��ġ��
			else if (p->key < key) {
				//key���� ���� ��尡 ���ٸ� ��� �߰�
				if (p->r == NULL) {
					p->r = new nd(key);
					p->r->p = p;
					if (p->n != NULL) {
						p->n->b = p->r;
						p->r->n = p->n;
					}
					siz++;
					p->n = p->r;
					p->r->b = p;
					if (isLast) {
						last = p->r;
						last->n = end;
						end->b = last;
					}
					//�߰��� ���� Ʈ���� ��Ģ�� �ٽ� ����
					judgeInsertNodeCase(p->r);
					//��Ʈ�� ȸ���ؼ� �ٲ���� �� �����Ƿ� ��Ʈ�� �缳��
					setHead();
					return *p->r;
				}
				else {
					p = p->r;
					isFirst = false;
				}
			}
			//���� ��尡 key���� Ű�� �����ٸ�, ���� ��带 ��ȯ
			else
				return *p;
		}
		//Ʈ���� ����ִٸ� head�� ��带 �Ҵ��ϰ�, head�� ��ȯ
		head = new nd(key);
		siz++;
		judgeInsertNodeCase(head);
		first = head;
		last = head;
		last->n = end;
		end->b = last;
		return *head;
	}
};
#endif//__RED_BLACK_TREE_H__
#ifdef __NULL_IS_DEFINED__
#undef NULL
#endif//__NULL_IS_DEFINED__