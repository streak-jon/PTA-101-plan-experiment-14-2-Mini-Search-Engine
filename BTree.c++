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

/*
	A01
	in this night i sleep well 
	last night i saw s sea ans stand in sky star thinking who we are and where to go
	night after night

	night
	NiGht
	saw

	

	
	1、一个单词可能对应到多个语句，一个语句可能对应到多个文件，每个点都是一个出发点，由这个点可以导向无数点
	2、如果保持一个文件行的完整性，那么要判断这一行有没有要找的string，就要耗费时间
	***如果把一个文件行以每个单词划分，就一定要给每个单词记录其文件行(应该是文件行编号，且可能不止有一个文件行编号),一旦多个文件行都有同一个单词，
	即文件的相同率或说这个单词的出现频率太高的话，可能这种方法更好，但这种方法的索引结构：string->文件行->文件编号如何处理？


	大概有三种建树形式
	1.把整个文件的内容按size大小(bits)划分，在文件内部组成一个B树结构，除非文件内容本身是有序的(即划分是有序的,一行的数据大小会限制B树节点大小)
	2.把一定数量的文件看成一个节点，多文件构成整颗树，其键和值又是什么，如果文件只是一个编号，而编号索引到文件具体存储位置(即可访问到文件内容)
	如果能根据索引得到的东西，不管是直接对应的数据，还是一个对应数据的地址(间接)，后者带来的损失有多大？
	3.以所有文件的内容为一个节点，

	文件编号本身作为一个节点，其对搜索，没有帮助(如果搜索对象是某个文件的具体值),应对不同的搜索对象构造不同的树
	也就不难理解网站的搜索系统是搜索一个关键词(string)对应到所有网站名


	1.要把数据放到磁盘中
	2.B树存入该数据的关键字（映射）及磁盘地址（索引）
	3.每一个数据带上文件名的编号（映射）这样做会增大空间，map<string,int> <- -> map<int,string>
	4.关键字是否有序
	5.B树内部结点是否就是索引结点，且只有索引功能？（索引功能是什么，是指数据在内存中实际位置吗）
	6.B树叶子结点存放真正的数据？这像B+树的定义
	7.磁盘中的地址是如何产生的(如何构造文本行的磁盘信息?)
	8.B树的磁盘访问
	9.一个指针如何对应一个磁盘地址
	10.插入后节点关键字数量超过磁盘块容量（如m-1个关键字）(磁盘块容量是什么？)
	11.具体需考虑对齐和元数据开销？对齐是什么？
	12.索引结点的键值（仅存储键值范围和子节点指针（无实际数据））和子结点键值（存储完整键值和指向真实数据的指针）有什么差别
		即为什么说B树中非叶子结点是索引结点？叶子结点是数据结点？

	磁盘空闲块列表
	内存模拟磁盘
	在MySQL或PostgreSQL中分析B树索引的实际存储结构（通过EXPLAIN命令或系统表）
	与其说每个B树节点需存储在一个或多个固定大小的磁盘块中，不如说每个B树节点对应一个磁盘块
	用户程序和操作系统
	到底要不要操作磁盘文件？
	键和值究竟是什么关系？
	有必要把string映射到int吗？

	
	不管怎么想，都只有一个结构，就是 string->文本行->文件编号,这三种数据形式
	在同一个文件中，如果出现两行以上文本行相同，s_map_line要去重，即便string一样，但所在行数不同，所以仍会输入，这一点在ans插入时一样
	为什么都要用visit去重?这种去重方式会产生多大的内存消耗？有很多小的内存消耗，碎片化的
	感觉B树和s_map_line之类完全是两种不同的结构，两者之间根本没有关联，这可能是时间复杂度高的原因
	怎么感觉s_map_line和line_map_file组成一个完全二分图，换句话说就是图太满了
	
	
	fptr默认构造为零都不行(意思是要重构默认构造函数)
	splitnode->rcd[1]的地址怎么可能在进入函数内是一个值，出函数后又是另一个值？
	可能提高时间复杂度部分：
		在B树插入时，对每个文件的每个文本行进行拆分
		在B树插入本身
		查询
		result从低到高层用map索引到真正数据

	为什么要有s_map_line和line_map_file这种东西呢？因为一个string会对应多个文本行，一个文本行会对应多个文件,这是采用string作为树节点必须要
	解决的问题
	如果多级索引结构的每一级很密，恐怕不能发挥出该结构的快速索引能力，在这种情况下，如果只求叶子到树顶的索引关系，不如直接在这两者建立关联，
	去掉所有间接关联
	从叶子到根的理解不行

	真正的问题是把每个文件的每个文本行分解为string,只要知道每个文件是否有查询的文本行的所有string就行，输出有点奇怪，输出该string所在文本行
	100000个string
	从文件向下搜索出string，或者从string向上导出文件，两者差别
	如果索引指向一个指针，这个指针以链表形式存储数据，其访问是顺序访问,如果指向一个杂乱无章的空间，其时间复杂度也不可观。由此可见
	考虑如何组织数据结构以优化访问速度是重要的。而不假思索就随意的处理数据，这是一个灾难。
	//如果有两个相同string映射到同一个文件，应该只算一次映射
	//每个文件只能通过一次
	//插入string的同时插入其文件行编号
	//唯一能去除的就是在两个文件中同一行有两个同样的文本行，这概率有多大？
	//键是string,但值又是什么？如果值是对应的文件行，但文件行不唯一，所以值应该可以映射到对应的所有文件行.值是指向真实数据存放的地址
	清楚变量定义 Rcd只要保持T,K(string ,p*)就行
	p*对应一个地址，改地址如果建立
	一旦引入指针，Rcd的处理就变的很复杂,准确来说，就是指针的处理很复杂
	如果一个指针本身就是nullptr,它指向一个指针
	绝对是splitnode->rcd[pos].data与原来的P->rcd[i]有关联，导致后者被删除时，前者为野指针
	两个指针指向同一个地址
	因为是前插，所以地址会变
	p->rcd[i].reset();//这个地方不会与splitnod关联吧 浅拷贝 赋值难道是浅拷贝？不对，是两个指针(splitnode-<rcd[pos]和p-<rcd[i])指向同一个地方
	不用rcd->reset(),直接切断p->rcd[i]对指针的联系
	说一个笑话，tem->next = p 与 p=tem->next 分不清. 好吧，原来是result.BNode->rcd[result.pos]和p分不清
	段错误,使用指针极有可能遇到的问题，也是我最怕的问题之一，所有有指针的地方都可能出错，应该是发生在rcd->data(链表)
	这有问题，为什么出来就有问题？这个地方不会与splitnod关联吧 浅拷贝 赋值难道是浅拷贝？
	终究还是回到rcd->data尾插的不稳定问题
	忘了一个点，只有动态内存分配的点才能delete
	如果两个指针(动态内存分配)指向同一个地方，delate其中一个指针,则另一个指针会变成野指针。
	
	好，最后一个问题(超时)是，原本用answer.size记录query string打到文本行的次数，之所以错误，因为单单一条string就可能不止一次打到该文本行
	一个段错误，卡了我6天，我先前一直以为是树中rcd.data指针的问题，使得我花了很多时间在上面，但实际上，却是在405行数组越界访问，先前401行visit
	数组开到answer[i].size(),一个显然的错误，answer.size只是string映射到文件的次数(我原先也这样认为)，如果只映射了3次，但实际上，该文件行数
	可能大于3，所以应该以该文件共有多少行为准，即file[i].size()
	下一个错误，运行时错误，解决办法和上面一样，我原本把answer[i].size()开到11，因为我以为一个文件最多只有10行(理解错题意)，但实际上最大可能为
	100行.
	如一个文件有last night i read book 和 this night i stand here,一个night就可以映射两次，所以应该对每一个string有一个对文件的访问限制，
	即一个string只能映射一个文件一次，用vector<bool> cnt_file(file_num, false)作限制; 用vector<int>cnt,记录访问次数
	交了整整40次
	最后要感谢浩宇，没有他帮我解决编译器问题，我还深陷段错误中不能自拔(删除了旧)
	Note:
		如果有两个文件有相同的string，则应只输出前一个文件对应的内容，也就是说后面的string根本不用插入，但string对应的信息应添加对应文件编号
		一个string映射一个文件行：
			1、一个string会映射到不同的文件行(即会有很多的文件行都有这个string)，在这种情况下，内存中处理用vector(一个类似链表的东西)
		，那么在磁盘中？
		虽然都是数据，但数据的层次不同
		一个文件中相同的文本行不应插入两次
		***必须要始终牢记的一点就是，K可能是指针，也可能不是，所以，对rcd的操作，必须要分情况讨论.
	采用结构：
		一个文件行总是应该拆分为具体的string以遍查找该文件行是否有对应string
	先前问题
		要解决同一个文件有两个以上相同文本行问题
		如果记录文件行对应的文件行号,是否能根据这个行号索引到文件的第几行的内容，文件是要分行的
		要不把文件行给编号化，然后B树存int,不然直接存文件行,实际上两者可以比较大小，不妨直接用string,
		不存文件号，存文件行号，通过文件行号索引文件号						如果string很大，用编号更好	
		在同一层BNode插入数据 应该要能移动BNode中数据位置	
	难点：
		1.索引结构是什么？(即如何由索引结构导向真实数据存储地？)
		2.在完成索引结构的基础上,想办法提高索引速度，以及尽可能降低存储索引的空间(数据压缩)
		3.如何解决rcd.data是一个指针从而引起的一系列问题？ 指针内存分配，赋值(=)，释放？ 
			而且要保证不能与rcd.data为string等普通数据类型时产生混乱
		
	思想：
	1.数据是真实的，索引是虚拟的(真实数据的存储空间无法避免)
		如果只有一个单词就能索引到这条语句，就无所谓这究竟是一个单词，还是多个单词索引的结果
		插入B树的时候小写(这棵树要保持小写)，查询的时候也小写，只要保持文件行输出时是原来样子
		一个完整的数据行究竟是什么，我根本不在意，重要的是保留关键的部分(既能由此推出整个数据行)
	2.对不同的数据类型，要采取不同的操作,由结果导向数据结构的构造，而不是反过来.
*/
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