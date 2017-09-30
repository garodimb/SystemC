#include <iostream>
#include <vector>


class List {
 public:

 	List() : MyMemberspace(*this) {}
 	typedef std::vector<int> collection;

 	struct MyMemberspace
 	{
 		friend List;
 		typedef collection::iterator iterator;

 		iterator begin() { return m_list_.mycollection_.begin(); }

 		iterator end() { return m_list_.mycollection_.end(); }

 	private:
 		MyMemberspace(List& list) : m_list_(list) {

 		}

 		List& m_list_;
 	}MyMemberspace;

 private:
 	collection mycollection_;
};

int main(int argc, char const *argv[])
{
	List my_list;
	List::MyMemberspace::iterator iter = my_list.MyMemberspace.begin();
	return 0;
}