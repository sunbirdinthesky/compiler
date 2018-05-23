#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <cstring>
using namespace std;

class state_machine {
  public:
    string check (const char val) {
      if (cur_node == root) return "";
      string key;
      key += val;

      if (val == ' ' && cur_node != NULL && cur_node->child[97] == cur_node)
        return "";

      if (trims.find(key) != trims.end()) {
        if (pool.find(cur_node) == pool.end()) { 
          return "id " + buff;
        }
        return pool[cur_node] + " " + buff;
      }
      return "";
    }

    void next (const char val) {
      if (buff == "" && val == ' ') return;
      buff += val;
      if (cur_node == NULL) return;
      cur_node = cur_node->child[int(val)];
    }

    void reset () {
      buff = "";
      cur_node = root;
    }

    state_machine () {
      root = new node();

      insert("int");
      pool[cur_node] = "key";

      insert("char");
      pool[cur_node] = "char";

      insert("=");
      pool[cur_node] = "op";

      insert("+");
      pool[cur_node] = "op";

      insert("-");
      pool[cur_node] = "op";

      insert("*");
      pool[cur_node] = "op";

      insert("/");
      pool[cur_node] = "op";

      insert("{");
      pool[cur_node] = "op";

      insert("}");
      pool[cur_node] = "op";

      insert("(");
      pool[cur_node] = "op";

      insert(")");
      pool[cur_node] = "op";

      insert(",");
      pool[cur_node] = "op";

      insert(";");
      pool[cur_node] = "op";

      trims.insert("+");
      trims.insert("-");
      trims.insert("*");
      trims.insert("/");
      trims.insert("{");
      trims.insert("}");
      trims.insert("(");
      trims.insert(")");
      trims.insert(" ");
      trims.insert(",");
      trims.insert(";");

      str_self_loop();
      num_self_loop();

      reset();
    }

  private:
      class node {
        public:
          bool is_end;
          node *child[128];
          node () {
            is_end = false;
            memset(child, 0, sizeof(child));
          }
      };

      void insert (string str) {
        cur_node = root;
        for (short val : str) { 
          if (cur_node->child[val] == NULL) 
            cur_node->child[val] = new node();
          cur_node = cur_node->child[val];
        }
        cur_node->is_end = true;
      }

      void str_self_loop () {
        root->child['"'] = new node();
        cur_node = root->child['"'];

        for (int i = 0; i < 128; i++) cur_node->child[i] = cur_node;

        cur_node->child['"'] = new node();
        cur_node = cur_node->child['"'];
        cur_node->is_end = true;
        pool[cur_node] = "str";
      }

      void num_self_loop () {
        node *num = new node();
        int offset = int('0');
        for (int i = offset; i < offset+10; i++) {
          root->child[i] = num;
          num->child[i]  = num;
        }
        pool[num] = "num";
      }

      node   *root;
      node   *cur_node;
      string buff;
      set <string> trims;
      map <node*, string> pool;
};

int main (int argn, char** argv) {
  state_machine token_tree;
  ifstream fin;
  int n_line = 0;
  if (argn != 2) {
    cout << "Usage: " << argv[0] << " source file" << endl;
    return 0;
  }
  fin.open(argv[1]);

  while (!fin.eof()) {
    string str;
    getline(fin, str);
    cout << "line " << ++n_line << endl;
    str += ' ';
    for (char val : str) {
      string token = token_tree.check(val);
      if (token != "") {
        cout << token << endl;
        token_tree.reset();
      }
      token_tree.next(val);
    }
  }
}
