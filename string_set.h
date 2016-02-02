/*------------------------------
* string_set.h
* Description: Class to outline string_set.cpp
-------------------------------*/

using namespace std;
class string_set {

public:
	string_set();
	void add(const char *s);
	void remove(const char *s);
	int contains(const char *s);
	void reset();
	const char *next();
	~string_set();

private:

	int hash_function(const char *s);

	enum {HASH_TABLE_SIZE = 100};

	class node {
		public:
		char *s;
		node *next;
	};

	node *hash_table[HASH_TABLE_SIZE];

	int iterator_index; // index in hash_table
	node *iterator_node; // node in hash_table[iterator_index];
};

class duplicate_exception { };
class memory_exception { };
class not_found_exception { };
