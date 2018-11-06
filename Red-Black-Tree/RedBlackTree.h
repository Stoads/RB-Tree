#ifndef __RED_BLACK_TREE_H__
#define __RED_BLACK_TREE_H__
#ifndef NULL
#define __NULL_IS_DEFINED__
#define NULL (nd*)0
#endif//NULL
/* Header File Info
	작성 시작 : 2017 - 04 - 08 오후 8시 53분
	작성자 : Stoads
*/
/* RED-BLACK TREE의 성질
	1. 모든 노드는 검은색 혹은 빨간색을 갖는다
	2. 루트는 항상 검은색이다
	3. 모든 NULL LEAF는 검은색이다
	4. 어떤 노드 n이 빨간색이면, n의 두 자식은 항상 검은색이다(즉, 빨간색은 연속해서 있을 수 없다)
	5. 임의의 노드 n으로부터 내려갈 수 있는 모든 NULL LEAF까지는 항상 같은 수의 검은색 노드를 경유해서 지나간다(최단거리와 최장 거리는 항상 2배 이하로 차이난다)
*/
template <typename K, typename T>
class RedBlackTree {
private:
	//RED-BLACK TREE에서 노드의 색
	enum NodeColor { RED = 0, BLACK };
	//트리의 노드
	class RBNode {
		//RedBlackTree 객체에서는 Node의 private 속성의 접근도 자유로워야 함
		friend class RedBlackTree<K, T>;
	public:
		//연산자 오버로딩
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
		//Key값을 반환함
		K getKey() { return key; }
	private:
		//key값을 초기화하는 생성자, 정상적인 노드는 이 생성자를 통해 생성되야 함 (처음 생성되는 노드는 기본적으로 빨간색임)
		RBNode(K key) : key(key), p(NULL), l(NULL), r(NULL), b(NULL), n(NULL), col(RED) {}
		//key값을 초기화하지 않는 생성자, 비정상적인 노드(END, NULL LEAF) 등을 생성할 때 사용함 (NULL LEAF는 기본적으로 검은색임)
		RBNode() : p(NULL), l(NULL), r(NULL), b(NULL), n(NULL), col(BLACK) {}
		//노드의 값
		T data;
		//노드의 색
		int col;
		//노드의 키값
		K key;
		//부모 노드 포인터
		struct RBNode *p;
		//왼쪽 자식 노드 포인터
		struct RBNode *l;
		//오른쪽 자식 노드 포인터
		struct RBNode *r;
		//바로 왼쪽에 있는 노드를 가리키는 포인터
		struct RBNode *b;
		//바로 오른쪽에 있는 노드를 가리키는 포인터
		struct RBNode *n;
	};
	typedef RBNode nd;
	//트리의 루트
	nd* head = NULL;
	//트리의 가장 좌측 노드 - [a,b]에서 a에 해당하는 노드
	nd* first = NULL;
	//트리의 가장 우측 노드 - [a,b]에서 b에 해당하는 노드
	nd* last = NULL;
	//임의의 END 노드 [a,b)에서 b에 해당하는 노드
	nd* end = new RBNode;
	//트리의 크기
	size_t siz = 0;
	//루트를 재설정
	void setHead() {
		while (head->p != NULL) {
			head = head->p;
		}
	}
	//현재 노드의 조부모 노드
	nd* grandNode(nd* p) {
		if (p != NULL && p->p != NULL)
			return p->p->p;
		return NULL;
	}
	//현재 노드의 삼촌 노드
	nd* unclneNode(nd* p) {
		nd* g = grandNode(p);
		if (g == NULL)return NULL;
		if (p->p == g->l)
			return g->r;
		return g->l;
	}
	//현재 노드의 형제 노드
	nd* brotherNode(nd* p) {
		if (p->p == NULL)return NULL;
		return p->p->r == p ? p->p->l : p->p->r;
	}
	//p를 기준으로 왼쪽 회전
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
	//p를 기준으로 오른쪽 회전
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
	//추가된 노드가 어떤 규칙에 위반되는지 판단함
	void judgeInsertNodeCase(nd* p) {
		if (p == NULL)return;
		//u는 p의 삼촌 노드
		nd* u = unclneNode(p);
		//루트노드라면
		if (p->p == NULL)
			insertNodeCase1(p);
		//부모가 검은색이라면
		else if (p->p->col == BLACK)
			insertNodeCase2(p);
		//부모와 삼촌의 색이 빨간색이라면
		else if ((u != NULL) && (u->col == RED))
			insertNodeCase3(p);
		//그 외의 경우
		else
			insertNodeCase4(p);
	}
	//부모라면 검은색으로 현재 노드를 검은색으로 바꿈
	void insertNodeCase1(nd* p) {
		if (p->p == NULL)
			p->col = BLACK;
	}
	//부모가 검은색이면 별도의 처리가 필요 없음
	void insertNodeCase2(nd* p) {
		return;
	}
	//부모와 삼촌의 색이 빨간색이면 조부모 노드로부터 검은색을 물려받고 조부모 노드에 대해서 트리의 규칙을 다시 검사
	void insertNodeCase3(nd* p) {
		nd* u = unclneNode(p), *g;
		p->p->col = BLACK;
		u->col = BLACK;
		g = grandNode(p);
		g->col = RED;
		judgeInsertNodeCase(g);
		return;
	}
	//나머지 경우에 적당히 트리를 회전하고 색을 바꿈으로써 RED-BLACK-TREE의 4,5번 규칙을 만족시킴
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
	//p가 루트라면 별도의 처리를 할 필요가 없다
	void deleteNodeCase1(nd* p) {
		if (p->p != NULL) {
			deleteNodeCase2(p);
		}
	}
	//p의 형제 노드인 s가 빨간색인 경우 형제를 검은색으로 두기 위해서 트리를 회전한다
	void deleteNodeCase2(nd* p) {
		nd* s = brotherNode(p);
		//한번의 회전도 일어나지 않았으므로 반드시 s는 NULL이 될 수 없음
		//RED-BLACK TREE의 5번 성질에 의해서 p가 검은색 노드고 s가 빨간색 노드라면\
								 s는 반드시 검은색 노드를 가진 두 자식이 존재하는 것이 자명함
		if (s->col == RED) {
			p->p->col = RED;
			s->col = BLACK;
			if (p == p->p->l)
				leftRotate(p->p);
			else
				rightRotate(p->p);
		}
		//위 주석에 의해 회전이 일어나도 p의 형제 노드는 반드시 존재함
		deleteNodeCase3(p);
	}
	//부모노드가 검은색이고, 형제의 두 자식이 모두 검은색이라면 형제의 색을 빨간색으로 바꿈으로서 부모노드가 루트인 서브트리에서의 5번 성질을 만족 시킴\
				 전체 트리에 대해 5번 성질을 만족시키기 위해서 부모노드를 case1부터 재조정한다
	void deleteNodeCase3(nd* p) {
		nd* s = brotherNode(p);
		if (p->p->col == BLACK &&
			//s->col == BLACK &&		//2번에 의해서 형제노드는 반드시 존재하며 검은색이므로 자명한 문장임
			(s->l == NULL || s->l->col == BLACK) &&		//RED-BLACK Tree의 3번 성질에 의해 NULL노드는 검은색으로 취급함
			(s->r == NULL || s->r->col == BLACK)) {
			s->col = RED;
			deleteNodeCase1(p->p);
		}
		else
			deleteNodeCase4(p);
	}
	//부모가 빨간색이고, 형제와 형제의 자식노드가 모두 검은색이라면, 부모노드를 검은색으로, 형제 노드를 빨간색으로 바꾼다
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
	//형제의 한쪽 자식이 빨간색이고, 형제 노드에서 빨간 자식의 방향이 현재 노드의 부모로부터의 방향과 일치한다면\
				 형제를 회전해서 방향이 일치하지 않게 함으로써 case6을 수행할 수 있게 한다
	void deleteNodeCase5(nd* p) {
		nd* s = brotherNode(p);
		//if(s->col==BLACK) //s가 검은색이 아니면 s의 자식은 빨간색이 될 수 없으므로 자명한 문장임
		//왼쪽 방향일 경우 형제를 오른쪽으로 회전해서 방향을 다르게함
		if (p->p->l == p &&
			(s->r == NULL || s->r->col == BLACK) &&	//둘다 NULL인 경우 혹은, 한쪽이 검은색, 한쪽이 NULL인 경우는 2-4에서 다 처리되므로
			(s->l->col == RED)) {	//s->r이 검은색이라면 반드시 s->l은 빨간색으로 존재함
			s->col = RED;
			s->l->col = BLACK;
			rightRotate(s);
		}
		//오른쪽 방향일 경우 형제를 왼쪽으로 회전해서 회전해서 방향을 다르게함
		else if (p->p->r == p &&
			(s->l == NULL || s->l->col == BLACK) &&
			(s->r->col == RED)) {
			s->col = RED;
			s->r->col = BLACK;
			leftRotate(s);
		}
		deleteNodeCase6(p);
	}
	//진행하면서 깨진 RED-BLACK TREE의 성질 4,5번을 만족하게끔 트리를 회전, 색을 바꾼다
	void deleteNodeCase6(nd* p) {
		nd* s = brotherNode(p);
		s->col = p->p->col;
		p->p->col = BLACK;
		//case5에 의하여 현재 노드가 부모 노드의 왼쪽이면, 형제의 오른쪽 자식은 빨간색임이 자명함
		if (p->p->l == p) {
			s->r->col = BLACK;
			leftRotate(p->p);
		}
		//case5에 의하여 현재 노드가 부모 노드의 오른쪽이면, 형제의 왼쪽 자식은 빨간색임이 자명함
		else {
			s->l->col = BLACK;
			rightRotate(p->p);
		}
	}
	// NULL이 아닌 자식을 1개 이하로 가지는 노드를 트리에서 지운다
	void deleteOneChildNode(nd* p) {
		if (p == NULL)return;
		//현 노드의 자식을 구함
		nd* c = p->r == NULL ? p->l : p->r;
		//p가 NULL인 자식을 1개 갖고 있으면 false, 아니면 true
		bool isChildCreated = false;
		if (c == NULL) {
			//NULL은 검은색으로 처리하므로, 임의의 자식에 검은색 노드를 할당함
			c = new RBNode;
			isChildCreated = true;
		}
		if (p->p != NULL) {		//루트가 아니라면 c를 p의 부모 노드와 연결함
			if (p == p->p->l)
				p->p->l = c;
			else
				p->p->r = c;
		}
		c->p = p->p;	//p의 부모노드를 c와 연결함
						/*
						만약 p가 검은색이라면 RED-BLACK TREE의 5번 성질에 의해서 반드시 형제노드가 존재함
						*/
		if (p->col == BLACK) {
			if (c->col == RED) //지워진 노드의 자식이 빨간색이면 자식에게 검은색을 주고 추가적인 처리를 할 필요가 없음
				c->col = BLACK;
			else				//지워진 노드의 자식이 검은색이면
				deleteNodeCase1(c);
		}
		if (p == head) {	//루트가 지워진다면
			head = c;		//루트를 지워지는 노드의 자식으로 재설정
		}
		delete p;		//메모리의 누수를 막기 위해 p에 할당된 메모리를 해제함
		siz--;		//노드 p가 지워졌으므로 크기 1 감소
					//c가 임의로 만들어진 노드면 삭제함
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
		if (!siz) {		//트리의 크기가 0이 되었으면 트리를 초기 상태로 되돌림
			head = NULL;
			first = NULL;
			last = NULL;
		}
		else		//삭제에서 회전이 일어나 루트가 바뀌었을 가능성이 존재하므로 루트를 재설정함
			setHead();
	}
public:
	/* getFirst -> RBNode
	트리의 가장 좌측 노드를 반환한다
	*/
	RBNode getFirst() {
		return *first;
	}
	/* getLast -> RBNode
	트리의 가장 우측 노드를 반환한다
	*/
	RBNode getLast() {
		return *last;
	}
	/* getBegin -> RBNode
	트리의 가장 좌측 노드를 반환한다
	*/
	RBNode getBegin() {
		return *first;
	}
	/* getLast -> RBNode
	노드의 Before 값으로 트리의 가장 우측 노드를 갖는 END 노드를 반환한다
	*/
	RBNode getEnd() {
		return *end;
	}
	/* insert
	1. key값을 키로 갖고 값을 data 값을 갖는 노드를 트리에 삽입한다
	2. key값을 키로 갖는 노드에 data 값으로 값을 삽입(갱신)한다
	*/
	void insert(K key, T data) {
		operator[](key) = data;		//삽입에서의 노드찾기 및 생성의 처리는 연산자 오버로딩 한 것으로 대체함
	};
	/* erase
	key값을 갖는 노드를 트리에서 지운다
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
			k = p->b;			//양쪽 자식 모두 있기때문에 p의 바로 전이 있는 것은 자명함
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
	트리를 초기 상태로 되돌린다
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
	트리가 비어있는지를 반환한다
	*/
	bool isEmpty() {
		return siz == 0;
	}
	/* size -> int
	트리에 존재하는 노드의 개수를 반환한다
	*/
	int size() {
		return siz;
	}
	/* find -> RBNode&
	key값을 키로 갖는 노드를 찾아서 반환한다
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
	1. key값을 갖는 노드를 찾아서 반환한다
	2. key값을 갖는 노드가 없다면 만들어서 반환한다
	*/
	RBNode& operator[](K key) {
		//key값이 될 노드 p
		auto p = head;
		//2번의 노드 추가가 이루어질 때, 맨 처음 혹은 맨 뒤에 노드가 추가될 경우 first와 end가 갱신되어야함
		bool isFirst = true, isLast = true;
		while (p != NULL) {
			//현재 노드기준 왼쪽에 위치함
			if (p->key > key) {
				//key값을 가진 노드가 없다면 노드 추가
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
					//추가한 노드로 트리의 규칙을 다시 맞춤
					judgeInsertNodeCase(p->l);
					//루트가 회전해서 바뀌었을 수 있으므로 루트를 재설정
					setHead();
					return *p->l;
				}
				else {
					p = p->l;
					isLast = false;
				}
			}
			//현재 노드기준 오른쪽에 위치함
			else if (p->key < key) {
				//key값을 가진 노드가 없다면 노드 추가
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
					//추가한 노드로 트리의 규칙을 다시 맞춤
					judgeInsertNodeCase(p->r);
					//루트가 회전해서 바뀌었을 수 있으므로 루트를 재설정
					setHead();
					return *p->r;
				}
				else {
					p = p->r;
					isFirst = false;
				}
			}
			//현재 노드가 key값을 키로 가진다면, 현재 노드를 반환
			else
				return *p;
		}
		//트리가 비어있다면 head에 노드를 할당하고, head를 반환
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