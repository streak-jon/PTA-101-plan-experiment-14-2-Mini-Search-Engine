#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<cstring>
#include<vector>
#include<queue>
#include<map>
#include<set>
#include<stack>
#include<cmath>
#include <functional> 
#include<algorithm>
using namespace std;

#define ll long long
#define pll pair<int,int>
#define lpll pair<int,ll>
#define maxsize 105
const int ini = 2e9 + 1;
const ll inf = 2e18 + 1;
const int mod = 998244353;
const int N = 1 << 18;

#define Order 3
#define MID_KEY (Order+1)/2
#define MIN_KEY (Order-1)/2


template<typename S,typename L>
struct disk_memory{
	S file_id;//文件id
	L line_num;//所在文件行数
	disk_memory *next;

	disk_memory() : file_id(S{}), line_num(L{}),next(nullptr){};
	disk_memory(S a,L b,disk_memory* p = nullptr):
		file_id(a),line_num(b),next(p){}
};
template <typename S, typename L>
using fptr = disk_memory<S,L>*;

template<typename T>
struct data_progress{
	static void process(T data){
		
	}
};
template<>
struct data_progress<int>{
	static void process(int &data){
		data = 0;
	}
};
template<>
struct data_progress<string>{
	static void process(string &data){
		data = "";
	}
};
template<typename S,typename L>
struct data_progress<fptr<S,L>>{
	static void process(fptr<S,L> &data){
		data = nullptr;
	}
};

template<typename T,typename K>//string p* strign int   S，L 是p*的数据类型
struct Rcd{
	T key;
	K data;
	// void reset(){
	// 	reset_impl(static_cast<T*>(nullptr),static_cast<K*>(nullptr));
	// }
	// void reset_impl(int*,int*){
	// 	data = key = 0;
	// }
	
	// void reset_impl(string*,K*){		
	// 	key = "";
	// 	disk_decrease();
	// }
	
	Rcd<T,K>& operator=(const Rcd<T,K>& s){//决定是否要用operator = 不然的话string和int之类的普通数据判断相等会有问题 
		cut();
		key = s.key;					// 好像string之类的判断没有问题
		data = s.data;
		//disk_decrease();//自身data先清空 data=x.data?
		// while(tem){
		// 	K new_node = tem;//data是一个指针，直接改变地址指向，而不是新建一个指针
		// 	disk_preinsert(new_node);
		// 	tem = tem->next;
		// }
		return *this;
	}
	void disk_preinsert(K &p){//应该要根据K型数据的不同
		p->next = data;
		data = p;
	}
	void disk_sufinsert(K p){//能不能只在K为指针时使用？
		K tem=data;
		while(tem && tem->next){
			tem = tem->next;
		}
		tem->next = p;//tem->next指向p指向的地址
		
	}
	void disk_decrease(){
		
		data_progress<T>::process(key);
		K current = data;
		
		while(current){
			
			data = data->next;
			delete current;
			current = data;
		}
		data = nullptr;
	}
	
	void cut(){
		data_progress<T>::process(key);
		data_progress<K>::process(data);
	}
	Rcd():key(T{}),data(K{}){}
	Rcd(T s1,K s2):
		key(s1),data(s2){}
};


template<typename T,typename K,typename S,typename L>
struct node{
	int size;
	bool leaf;
	node<T,K,S,L> *father;//指针也要记录键和值？

	
	Rcd<T,K> rcd[Order + 1];
	node<T,K,S,L> *next[Order + 1];
	node() = default;
	node(Rcd<T,K> x) : size(1), leaf(true), father(nullptr) {
		rcd[1] = x;
		for (int i = 0; i <= Order; i++)
			next[i] = nullptr; }
};
template<typename T,typename K,typename S,typename L>
using ptr = node<T,K,S,L> *;

template<typename T,typename K,typename S,typename L>
struct Result{
	int pos;
	bool tag;
	ptr<T,K,S,L> BNode;

	Result() = default;
	Result(int a,bool b,ptr<T,K,S,L> p):
		pos(a),tag(b),BNode(p){}
};

template<class T,class K,class S,class L>
class BTree{
	public:
		BTree():root(nullptr){}
		~BTree(){
			decrease_self(root);
		}
		void decrease_self(ptr<T, K, S, L> &p);

		void build();
		bool insert(ptr<T,K,S,L> &p,const Rcd<T,K> x);
		void insertBNode(ptr<T,K,S,L> &p,ptr<T,K,S,L> &child,int,Rcd<T,K> x);//child为左儿子
		void split(ptr<T,K,S,L> &p, ptr<T,K,S,L> &splitnode);
		void Newroot( ptr<T,K,S,L> &l, ptr<T,K,S,L> &r, Rcd<T,K> x);

		Result<T,K,S,L> search(const ptr<T,K,S,L>&, Rcd<T,K> x)const;
		int level_search(const ptr<T,K,S,L>, Rcd<T,K> x)const;

		void decrease(Rcd<T,K> x);
		void restore(ptr<T,K,S,L> &p, int pos);
		void insertrecode(ptr<T,K,S,L> &p, int pos, Rcd<T,K> x);
		void remove(ptr<T,K,S,L> &p, int pos);
		void combine(ptr<T,K,S,L> &l, ptr<T,K,S,L> &r);
		void Deleteroot(ptr<T,K,S,L> &p);

		bool precursor(ptr<T,K,S,L> &p, int &pos);
		bool successor(ptr<T,K,S,L> &p, int &pos);
		void pre_order(ptr<T,K,S,L> p);

		ptr<T,K,S,L> getroot()const{
			return root;
		}

	private:	
		ptr<T,K,S,L> root;
};
struct cmp{
	bool operator()(const pair<string, int> &a,const pair<string, int> &b)const{
		return a.second < b.second;
	}
};
struct file_cmp{
	bool operator()(int a,int b)const{
		return a < b;
	}
};
int file_num,num;
map<string, int> mapping,order_result;
vector<pair<string, int>> order_line;
vector<vector<string>> file;
map<string, int> file_id;
vector<string> file_name,file_line;
map<int, set<pair<string,int>,cmp>,file_cmp> ans;
//327和328行共同构成的结构，很明显是一种层次结构
//map<string, set<Rcd<string>>> s_map_line;//好奇怪的索引结构，二级索引是类似set的东西，不能重复 或者是仍使用vector,但想办法去重，如map<string,bool>
//map<string, vector<Rcd<int>>> line_map_file;//一级索引是类似vector的东西,同样的索引结构却能采用不同的处理，震惊
											//一个原因，后面的文件名不可能与前面的文件名相同,但后面的文件内容很有可能与前面的文件内容相同,
											//理论上文件名不会重复
void Tolower(string &s);

int main()
{
	int n;
	char c;
	string res;
	BTree<string,fptr<int,int>,int,int> p;
	p.build();
	cin >> n;
	getchar();
	Result<string,fptr<int,int>,int,int> result;
	vector<vector<int>> answer(file_num);
	
	for (int i = 0; i < n;i++){
		size_t number = 0;//一个查询文本行确实应该包含number个string，不管是否重复
		map<string, bool> visit;
		vector<int> cnt(file_num, 0);
		while(cin.get(c)){
			c = tolower(c);
			if(c == ' ' || c=='\n'){
				number++;
				if(visit[res])
					continue;
				visit[res] = true;
				
				result = p.search(p.getroot(),Rcd<string,fptr<int,int>>(res,nullptr));
				vector<bool> cnt_file(file_num, false);
				if(result.tag){
					fptr<int, int> current = result.BNode->rcd[result.pos].data;
					while(current){//可能出现一种情况，一个night映射到一个文件中多个文本行，导致answer.size增大，实际上应该只增加一次
						if(!cnt_file[current->file_id]){
							cnt[current->file_id]++;
							cnt_file[current->file_id] = true;
						}
						answer[current->file_id].push_back(current->line_num);//一个文本行为 last night i read book
						current = current->next;//难以避免重复								//query 文本行为 last night
					}
				}			
				res.clear();
			}
			else
				res += c;
			if(c=='\n')
				break;
		}
		int ans = 0;
		bool sign = false;
		for (int i = 0; i < file_num;i++){
			if(cnt[i]>=number){
				ans++;
				sign = true;
			}
		}
		if(!sign){
			cout << 0 << endl
				 << "Not Found" << endl;
		}
		else{
			cout << ans << endl;
			for (int i = 0; i < file_num;i++){
				if(cnt[i]>=number){
					vector<int> visit(file[i].size(), false);//预防同一个文件中出现两个以上相同的文本行,重复输出
					sort(answer[i].begin(), answer[i].end());
					cout << file_name[i] << endl;
					for(auto j:answer[i]){
						if(visit[j])
							continue;
						visit[j] = true;
						cout << file[i][j] << endl;
					}
				}
			}
		}
		for (int i = 0; i < file_num; i++)
			answer[i].clear();
	}

	return 0;
}
template<class T,class K,class S,class L>
void BTree<T,K,S,L>::build()
{
	int number = 0;
	string s,res;
	cin >> file_num;
	file.resize(file_num + 1);
	getchar();
	for (int i = 0; i < file_num;i++){
		int num = -1;
		
		while (getline(cin, s)){//一个文件出现同一行数据，影响大吗？
			if(s=="#")
				break; 
			if(num==-1){
				file_name.push_back(s);
				num++;
				continue;
			}
			file[i].push_back(s);//如果同一个文件出现相同文本行
			map<string, bool> visit;
			for (size_t j = 0; j <= s.length();j++){
				if(j==s.length()||s[j]==' '){
					if(visit[res])
						continue;
					visit[res] = true;//同一行文件行中不能有相同string
					Tolower(res);
					if(!mapping[res]){
						mapping[res] = ++number;
						file_line.push_back(res);
					}
					fptr<S, L> p = new disk_memory<S,L>(i, num);//内存分配会失败？
					if(!p){
						cerr<<"wromg allocate p memory"<<endl;
						exit(1);
					}
					insert(root, Rcd<T,K>(res,p));
					res.clear();
				}
				else
					res += s[j];
			}
			num++;
		}
	}
	sort(file_line.begin(), file_line.end());
	int order = 1;
	for(auto i:file_line){
		order_line.push_back({i, order++});
	}
	sort(order_line.begin(), order_line.end(), [](const pair<string, int> &a, const pair<string, int> &b)
		 { return mapping[a.first] < mapping[b.first]; });
	for(auto i:order_line){
		cout << i.second << ' ' << i.first << endl;
		order_result[i.first] = i.second;
	}
}
template<class T,class K,class S,class L>
bool BTree<T,K,S,L>::insert(ptr<T,K,S,L> &p,const Rcd<T,K> x)//要不插叶子
{
	if(!p){
		p = new node<T,K,S,L>(x);
		if(!p){
			cerr<<"wromg allocate first insert memory"<<endl;
			exit(1);
		}
		return true;
	}
	Result<T,K,S,L> result = search(p, x);//传递引用指针p的副本非引用指针ptr<T,K,S,L> p.
	if(result.tag){//只找键值，不找真实数据，如果一个键值存在，则必有对应的指针,因为文件是顺序读取的，所以数据前插还是后插的话，可能有影响
		Rcd<T, K> tem = x;
		result.BNode->rcd[result.pos].disk_sufinsert(tem.data);
		return false;
	}
	ptr<T,K,S,L> current = result.BNode, splitnode = nullptr;
	insertBNode(current, splitnode, result.pos, x);

	while(current->size>=Order){
		Rcd<T,K> k = current->rcd[MID_KEY];

		split(current, splitnode);
		current->rcd[MID_KEY].cut();//k上去的话，curretn->rcd[MId_KEY]应为空
		if(!current->father){
			Newroot(current, splitnode, k);
			break;
		}
		else{
			int pos = level_search(current->father, k);
			insertBNode(current->father, splitnode, pos,k);//唯一与之前不同的是rcd.data指针的移动，除此之外还能有什么地方呢？
		}
		
		current = current->father;
	}
	return true;
}
template<class T,class K,class S,class L>
void BTree<T,K,S,L>::Newroot(ptr<T,K,S,L> &l,ptr<T,K,S,L> &r,Rcd<T,K> x)
{
	ptr<T,K,S,L> p = new node<T,K,S,L>(x);
	if(!p){
		cerr<<"wromg allocate newroot memory"<<endl;
		exit(1);
	}
	p->next[0] = l;
	p->next[1] = r;
	if(l)
		l->father = p;
	if(r)
		r->father = p;
	root = p;
	p->leaf = false;
}
template<class T,class K,class S,class L>
void BTree<T,K,S,L>::insertBNode(ptr<T,K,S,L> &p,ptr<T,K,S,L> &child,int pos,Rcd<T,K> x)//在同一层BNode插入数据 应该要能移动BNode中数据位置
{
	for (int i = p->size; i >= pos; i--){
		p->rcd[i + 1] = p->rcd[i];
		p->next[i + 1] = p->next[i];
	}
	p->rcd[pos] = x;
	p->next[pos] = child;
	
	if(child)
		child->father = p;
	p->size++;
}
template<class T,class K,class S,class L>
void BTree<T,K,S,L>::split(ptr<T,K,S,L> &p,ptr<T,K,S,L> &splitnode)
{
	splitnode = new node<T,K,S,L>();
	if(!splitnode){
		cerr<<"wromg allocate splitnode memory"<<endl;
		exit(1);
	}
	splitnode->father = p->father;
	splitnode->next[0] = p->next[MID_KEY];
	if(splitnode->next[0])
		splitnode->next[0]->father = splitnode;
	for (int i = MID_KEY+1; i <= p->size; i++){
		int pos = i - (MID_KEY);
		insertBNode(splitnode,p->next[i], pos, p->rcd[i]);//是在第pos个位置插入吗 Rcd 赋值会有问题
		p->rcd[i].cut();
		p->next[i] = nullptr;
	}
	p->size = MID_KEY-1;
}
template<class T,class K,class S,class L>
int BTree<T,K,S,L>::level_search(const ptr<T,K,S,L> p,Rcd<T,K> x)const
{
	if(!p)
		return 1;
	int pos=1;
	while(pos<=p->size&&x.key>p->rcd[pos].key){
		pos++;
	}
	return pos;
}
template<class T,class K,class S,class L>
Result<T,K,S,L> BTree<T,K,S,L>::search(const ptr<T,K,S,L> &p, Rcd<T,K> x)const
{
	ptr<T,K,S,L> current = p, fa = nullptr;
	int pos=0;
	bool find = false;
	while(current){
		pos = level_search(current, x);//在这一层中找是否有等于x的值

		if(pos<=current->size&&current->rcd[pos].key==x.key){//current不可能为空，
			find = true;
			break;
		}
		fa = current;
		current = current->next[pos - 1];
	}
	return find ? Result<T,K,S,L>(pos, find, current) : Result<T,K,S,L>(pos, find, fa);
}
template<class T,class K,class S,class L>
void BTree<T,K,S,L>::pre_order(ptr<T,K,S,L> p)
{
	if(p){
		cout << "size=" << p->size << endl;
		for (int i = 1; i <= p->size;i++){
			cout << "key[" << i << "]=" << order_result[p->rcd[i].key]<<' '<<p->rcd[i].key;
			fptr<S, L> tem = p->rcd[i].data;
			cout << " :";
			while(tem){
				cout << tem->file_id << ' ' << tem->line_num << '|';
				tem = tem->next;
				if(++num==100)
					exit(1);
			}
			cout << endl;
		}
		cout << endl;
		for (int i = 0; i <= p->size; i++){
			pre_order(p->next[i]);
		}
	}

}
template<class T,class K,class S,class L>
void BTree<T,K,S,L>::decrease(Rcd<T,K> x)
{
	Result<T,K,S,L> result = search(root, x);
	if(!result.tag)
		return;
	cout << "Yes" << endl;
	ptr<T,K,S,L> current = result.BNode;
	bool sign=precursor(current, result.pos);
	if(!sign)
		sign=successor(current, result.pos);
	
	Rcd<T,K> k = current->rcd[result.pos];//前驱或后继的值

	remove(current, result.pos);//remove一定是叶子结点,则next移不移无所谓
	
	if(current->father&&current->size<MIN_KEY){
		int pos = level_search(current->father, k);
		restore(current,pos-1);
	}
}
template<class T,class K,class S,class L>
void BTree<T,K,S,L>::restore(ptr<T,K,S,L> &q,int pos)//pos是第几条边[0-size]
{
	ptr<T,K,S,L> brother,p=q,parent=p->father;
	//cout << "restore pos=" << pos << endl;
	if(pos&&parent->next[pos-1]->size>MIN_KEY){//右旋
		brother = parent -> next[pos - 1];
		//cout << "1" << endl;
		insertBNode(p, p->next[0], 1, parent->rcd[pos]);
		p->next[0] = brother->next[brother->size];
		if(p->next[0])
			p->next[0]->father = p;
		parent->rcd[pos] = brother->rcd[brother->size];
		remove(brother, brother->size);
	}
	else if(pos<parent->size&&parent->next[pos+1]->size>MIN_KEY){//左旋
		//cout << "2" << endl;
		brother = parent->next[pos+1];
		insertBNode(p, brother->next[0], p->size+1, parent->rcd[pos+1]);
		parent->rcd[pos+1] = brother->rcd[1];
		remove(brother, 1);
		for (int i = 0; i <= brother->size;i++)
			brother->next[i] = brother->next[i + 1];
	}
	else if(pos){//先右旋再左旋 RL
		//cout << "3" << endl;
		brother = parent -> next[pos - 1];
		insertBNode(p, p->next[0], 1, parent->rcd[pos]);
		combine(brother, p);
		remove(parent, pos);
		p = brother;
		for (int i = pos; i <= parent->size;i++)
			parent->next[i] = parent->next[i + 1];
		
		if(parent->size<MIN_KEY){
			if(!parent->father&&!parent->size){
				Deleteroot(root);
			}
			else if(parent->father){
				restore(parent, level_search(parent->father, p->rcd[1]) - 1);//如何确定p->rcd[1]就是要找的点
			}
		}
	}
	else{
		brother = parent->next[pos + 1];
		//cout << "4" << endl;
		insertBNode(p, brother->next[0], p->size + 1, parent->rcd[pos + 1]);
		combine(p,brother);
		
		remove(parent, pos+1);
		for (int i = pos+1; i <= parent->size;i++)
			parent->next[i] = parent->next[i + 1];

		if(parent->size<MIN_KEY){
			if(!parent->father&&!parent->size){
				Deleteroot(root);	
			}
			else if(parent->father){
				int positon = level_search(parent->father, p->rcd[p->size]) - 1;
				restore(parent, positon);//如何确定p->rcd[1]就是要找的点
			}
		}
	}
	
}
template<class T,class K,class S,class L>
void BTree<T,K,S,L>::Deleteroot(ptr<T,K,S,L> &p)//为什么一定是左儿子，一定是先有左儿子，再有右儿子,反之不然
{
	ptr<T,K,S,L> child = p->next[0];

	p->next[0] = child->next[0];//0儿子是它自已
	if(p->next[0])
		p->next[0]->father = p;
	
	for (int i = 1; i <= child->size;i++){
		insertBNode(p, child->next[i], i, child->rcd[i]);
	}
}
template<class T,class K,class S,class L>
void BTree<T,K,S,L>::combine(ptr<T,K,S,L> &l,ptr<T,K,S,L> &r)
{
	if(!l||!r){
		return;
	}
	for (int i = 1; i <= r->size;i++)
		insertBNode(l, r->next[i], l->size + i, r->rcd[i]); // next[zero]舍弃
	delete r;
	r = nullptr;
}
template<class T,class K,class S,class L>
void BTree<T,K,S,L>::remove(ptr<T,K,S,L> &p,int pos)
{
	if(!p)
		return;
	for (; pos < p->size;pos++){
		p->rcd[pos] = p->rcd[pos + 1];
	}
	p->rcd[p->size--].cut();
}
template<class T,class K,class S,class L>
void BTree<T,K,S,L>::insertrecode(ptr<T,K,S,L> &p,int pos,Rcd<T,K> x)
{
	if(!p)
		return;
	for (int i = p->size; i >= pos;i++){
		p->rcd[i+1] = p->rcd[i];
	}
	p->rcd[pos] = x;
	p->size++;
}
template<class T,class K,class S,class L>
bool BTree<T,K,S,L>::precursor(ptr<T,K,S,L> &p,int &pos)//如果一个点没有前驱或后继，则它为叶子
{
	if(!p||!p->next[pos-1])
		return false;
	ptr<T,K,S,L> pre = p->next[pos - 1];
	while(pre->next[pre->size])
		pre = pre->next[pre->size];
	p->rcd[pos] = pre->rcd[pre->size];

	p = pre;
	pos = pre->size;
	return true;
}
template<class T,class K,class S,class L>
bool BTree<T,K,S,L>::successor(ptr<T,K,S,L> &p,int &pos)
{
	if(!p||!p->next[pos])
		return false;

	ptr<T,K,S,L> suc = p->next[pos];
	while(suc->next[0])
		suc = suc->next[0];
	p->rcd[pos] = suc->rcd[0];

	p = suc;
	pos = 1;
	return true;
}
void Tolower(string &s)
{
	transform(s.begin(), s.end(), s.begin(), [](unsigned char c)
			  { return tolower(c); });
}
template<class T,class K,class S,class L>
void BTree<T,K,S,L>::decrease_self(ptr<T,K,S,L> &p)
{
	if(p){
		for (int i = 0; i <= p->size;i++){
			if(i)
				p->rcd[i].disk_decrease();
			decrease_self(p->next[i]);
		}
		delete p;
		p = nullptr;
	}
}