#include <iostream>
#include <map>
#include <vector>
#include <set>
using namespace std;

//语法分析状态机，状态图详见笔记
class stage_machine{
  public:
    stage_machine () {  //初始化，生成状态机
      root = new node();
      reset();

      //paras -> op:, -> para
      node *s11 = new node();
      node *s12 = new node();
      node *s13 = new node("paras");

      node *s21 = new node();
      node *s22 = new node("init");

      node *s32 = new node();
      node *s34 = new node("func");

      node *s41 = new node();
      node *s42 = new node("para");

      node *s51 = new node();
      node *s52 = new node();
      node *s54 = new node();
      node *s55 = new node("call_func");

      node *s511 = new node();
      node *s512 = new node();
      node *s513 = new node("ass");
      node *s514 = new node();
      node *s515 = new node();
      node *s516 = new node("oper");

      node *s60 = new node();
      node *s61 = new node();
      node *s62 = new node("ids");

      // paras  init  func
      root -> add("paras", s11);
      root -> add("para",  s21);
      s11  -> add("op ,", s12);
      s21  -> add("op ,", s12);
      s12  -> add("para", s13);  //paras
      s21  -> add("op ;", s22);  //init
      s21  -> add("op (", s32);
      s32  -> add("paras", s32);
      s32  -> add("para", s32);
      s32  -> add("op )", s34); //func

      root -> add("key int",  s41);
      root -> add("key char",  s41);
      s41  -> add("id",   s42); //para

      root -> add("id",  s51);
      s51  -> add("op (", s52);
      s52  -> add("id", s52);
      s52  -> add("ids", s52);
      s52  -> add("op )", s54);
      s54  -> add("op ;", s55); //call_func

      s51  -> add("op =", s511); 
      s511 -> add("id", s512); 
      s511 -> add("num", s512); 
      s511 -> add("str", s512); 
      s511 -> add("oper", s513); 
      s512 -> add("op ;", s513); 
      s511 -> add("call_func", s513); //ass 

      s51  -> add("op +", s514); 
      s51  -> add("op -", s514); 
      s51  -> add("op *", s514); 
      s51  -> add("op /", s514); 
      s514 -> add("id", s515); 
      s515 -> add("op ;", s516); //oper

      root -> add("ids",  s60);
      s51  -> add("op ,", s61);
      s60  -> add("op ,", s61);
      s61  -> add("id", s62); //ids
    }

    void reset () {    //状态恢复到最初
      ptr = root;
    }

    void match (string val) { //匹配下一个词素，检查状态转移
      if (ptr == NULL) return;
      ptr = ptr->match(val);
    }

    string get_type () {
      if (ptr == NULL) 
        return "INVALID";
      return ptr->get_type ();
    }

  private:
    class node {  //状态节点
      public:
        node () { //初始化节点，初始标记为not define，意为：此字串不可归约
          captial = 0; 
          type    = "NOT DEFINE";
        }

        node (string val) {  //初始化节点，标记为给定值
          captial = 0; 
          type = val;
        }

        string get_type () {   //获取当前状态名
          return type;
        }

        node *get_node (int i) {  //获取第i个转换条件所指向的节点
          return next[i];
        }

        node *match (string val) { //匹配下一个词素，返回下个状态的指针
          for (int i = 0; i < captial; i++)
            if (value[i] == val)
              return next[i];
          return NULL;
        }

        void add (string val, node *target) {  //添加节点，转移条件val，转移目标为target
          value[captial] = val;
          next[captial]  = target;
          captial++;
        }

        int size() { //获取当前节点的转移条件数
          return captial;
        }

      private:
        int captial; //节点存储的转移条件数
        string type; //节点的类型，即A -> aB中的A
        string value[10]; //存储转移触发条件
        node   *next[10]; //存储转移出发后，要转移到的目标状态
    };

    node *root; //指向初始状态的常量指针
    node *ptr;  //指向当前状态的动态指针
};

//symbols 
//    func a - paras
//    int  b address - 
//    char c address -
class symbol_list {
  private:
    class domain {
      public:
        domain () {
          n_shift = 0;
        }

        vector <string> find (string name) {
          vector <string> res;
          auto it = index.find(name);
          if (it != index.end()) { 
            int index = it->second;
            res.push_back(op1[index]);
            res.push_back(op2[index]);
            res.push_back(address[index]);
            res.push_back(func[index]);
          }
          return res;
        }

        void add (string a, string b, string func_para_type) {
          op1.push_back(a);
          op2.push_back(b);
          index[b] = op1.size();
          this->func.push_back(func_para_type);
          if (a == "int")  n_shift += 4;
          if (a == "char") n_shift += 1;
          address.push_back(to_string(n_shift));
        }
      private:
        int n_shift;
        map <string, int> index;
        vector <string> op1, op2, address, func; 
      public:
        domain *father;
    };
  public:
    symbol_list () {
      cur = new domain();
      cur -> father = NULL;
    }
//定义域上浮一层
    void new_level () {
      domain* tmp = cur;
      cur = new domain();
      cur -> father = tmp;
    }
//定义域下沉一层
    bool last_level () {
      if (cur->father == NULL) 
        return false;
      domain *tmp = cur;
      cur = cur -> father;
      delete tmp;
      return true;
    }

    vector <string> find (string name) {
      vector <string> res;
      domain *tmp = cur;
      while (tmp != NULL) {
        res = tmp-> find(name);
        if (res.size() == 0) 
          tmp = tmp->father;
        else 
          return res;
      }
      return res;
    }

    bool add (string a, string b, string func_para_type) {
      if (find(b).size() == 0) {
        cur->add (a, b, func_para_type);
        return true;
      } else {
        cout << "ERROR : Multi define of " << a << " " << b << " ";
        return false;
      }
    }
  private:
    domain *cur;
};

class info { //用于存储规约前和规约后的节点信息，节点信息用于翻译成中间语言
  public:
    info (string str) {
      idx = 0;
      // special for: id, num, str, char
      if (str.substr(0,2) == "id" && str.substr(0,3) != "ids") {
        type = "id";
        val.push_back("unknow");
        val.push_back(str.substr(3));
        return;
      }
      if (str.substr(0,3) == "num") {
        type = "num";
        val.push_back("num");
        val.push_back(str.substr(4));
        return;
      }
      if (str.substr(0,3) == "str") {
        type = "str";
        val.push_back("str");
        val.push_back(str.substr(4));
        return;
      }
      if (str.substr(0,4) == "char") {
        type = "char";
        val.push_back("char");
        val.push_back(str.substr(5));
        return;
      }
      type = str;
    }

    string get_type () {
      return type;
    }

    string get_val () {
      if (idx != val.size())
        return val[idx++];
      idx = 0;
      return "";
    }

    string get_val (int idx) {
      return val[idx];
    }
    
    void add_val (string str) {
      val.push_back(str);
    }
  private:
    unsigned int idx;
    string type;
    vector <string> val;
};

class IR {
  public:
    void add (string a, string b, string c, string d) {
      op0.push_back(a);
      op1.push_back(b);
      op2.push_back(c);
      op3.push_back(d);
    }

    void debug () {
      for (auto i = 0u; i < op1.size(); i++) {
        cout << "[IR line " << i << "] : <" << op0[i] << "> <" << op1[i];
        cout << "> <" << op2[i] << "> <" << op3[i] << ">" << endl;
      }
    }
  private:
    vector <string> op0, op1, op2, op3;
} ir;

void reduce (vector <info> &info_list, stage_machine &dfa) {
  int len = info_list.size();
  for (int n_elm = 1; n_elm < len; n_elm ++) {
    for (int loop = n_elm-1; loop >= 0; loop--) {
      dfa.reset();
      cout << "[info] : total, loaded, ptr : " << len << " " << n_elm << " " << loop << endl;
      cout << "[info] : str processing: ";
      for (int idx = loop; idx <= n_elm; idx++) {
        cout << "<" << info_list[idx].get_type() << ">  ";
        dfa.match(info_list[idx].get_type());
      }
      cout << endl;
      cout << "[info] : stage of dfa : " << dfa.get_type() << endl << endl;
      string type = dfa.get_type();

      if (type != "INVALID" && type != "NOT DEFINE") {
        string val;

        if (type == "paras") {
          info tmp("paras");
          //first elm
          while(true) {
            val = info_list[loop].get_val();
            if (val == "") break;
            tmp.add_val(val);
          }
          //second elm
          while(true) {
            val = info_list[loop+2].get_val();
            if (val == "") break;
            tmp.add_val(val);
          }
          info_list.erase(info_list.begin()+loop+1, info_list.begin()+n_elm+1);
          info_list[loop] = tmp;
        }

        if (type == "para") {
          info tmp("para");
          //first elm
          val = info_list[loop].get_type();
          tmp.add_val(val.substr(4));
          //second elm
          val = info_list[loop+1].get_val(1);
          tmp.add_val(val);

          info_list.erase(info_list.begin()+loop+1, info_list.begin()+n_elm+1);
          info_list[loop] = tmp;
        }

        if (type == "init") {
          info tmp("init");
          //first elm
          val = info_list[loop].get_val(0);
          tmp.add_val(val);
          val = info_list[loop].get_val(1);
          tmp.add_val(val);

          info_list.erase(info_list.begin()+loop+1, info_list.begin()+n_elm+1);
          info_list[loop] = tmp;
        }

        if (type == "func") {
          info tmp("func");
          //first elm
          val = info_list[loop].get_val(0);
          tmp.add_val(val);
          val = info_list[loop].get_val(1);
          tmp.add_val(val);
          //second elm
          while(true) {
            val = info_list[loop+2].get_val();
            if (val == "") break;
            tmp.add_val(val);
          }

          info_list.erase(info_list.begin()+loop+1, info_list.begin()+n_elm+1);
          info_list[loop] = tmp;
        }

        if (type == "call_func") { 
          info tmp("call_func");
          //first elm
          val = info_list[loop].get_val(1);
          tmp.add_val("unknow");
          tmp.add_val(val);
          //second elm
          while(true) {
            val = info_list[loop+2].get_val();
            if (val == "") break;
            tmp.add_val(val);
          }

          info_list.erase(info_list.begin()+loop+1, info_list.begin()+n_elm+1);
          info_list[loop] = tmp;
        }
          
        if (type == "ids") { 
          info tmp("ids");
          //first elm
          while(true) {
            val = info_list[loop].get_val();
            if (val == "") break;
            tmp.add_val(val);
          }
          //second elm
          val = info_list[loop+2].get_val(1);
          tmp.add_val("unknow");
          tmp.add_val(val);

          info_list.erase(info_list.begin()+loop+1, info_list.begin()+n_elm+1);
          info_list[loop] = tmp;
        }

        if (type == "oper") { 
          info tmp("oper");
          //first elm
          val = info_list[loop].get_val(1);
          tmp.add_val("unknow");
          tmp.add_val(val);
          //second elm : operactor
          val = info_list[loop+1].get_type().substr(3);
          tmp.add_val(val);
          //thrid  elm
          val = info_list[loop+2].get_val(1);
          tmp.add_val("unknow");
          tmp.add_val(val);

          info_list.erase(info_list.begin()+loop+1, info_list.begin()+n_elm+1);
          info_list[loop] = tmp;
        }

        if (type == "ass") { 
          info tmp("ass");
          //second elm
          val = info_list[loop+2].get_type();
          if (val == "id") {
            tmp.add_val("var");
            tmp.add_val("unknow");
            tmp.add_val(info_list[loop+2].get_val(1));
          }
          if (val == "num") {
            tmp.add_val("var");
            tmp.add_val("num");
            tmp.add_val(info_list[loop+2].get_val(1));
          }
          if (val == "str") {
            tmp.add_val("var");
            tmp.add_val("str");
            tmp.add_val(info_list[loop+2].get_val(1));
          }
          if (val == "oper") {
            tmp.add_val("oper");
            while(true) {
              val = info_list[loop+2].get_val();
              if (val == "") break;
              tmp.add_val(val);
            }
          }
          if (val == "call_func") {
            tmp.add_val("call_func");
            while(true) {
              val = info_list[loop+2].get_val();
              if (val == "") break;
              tmp.add_val(val);
            }
          }
          //second elm
          tmp.add_val("unknow");
          tmp.add_val(info_list[loop].get_val(1));

          info_list.erase(info_list.begin()+loop+1, info_list.begin()+n_elm+1);
          info_list[loop] = tmp;
        }

        len   = len - n_elm + loop;
        n_elm = loop;
      }
    }
  }
}

bool generate_ir (symbol_list &table, vector <info> &info_list) {
  info data = info_list[0];
  vector <string> var_info;

  if (data.get_type() == "init") {
    if (!table.add(data.get_val(0), data.get_val(1), ""))
      return false;
    var_info = table.find(data.get_val(1));

    if (data.get_val(0) == "int")
      ir.add("init", "4", "", var_info[2]); 
    if (data.get_val(0) == "char")
      ir.add("init", "1", "", var_info[2]);
  }

  if (data.get_type() == "func") {
    string ret_type  = data.get_val();
    string func_name = data.get_val();
    string para;
    vector <string> var_type;
    vector <string> var_name;
    while (true) {
      string val0 = data.get_val();
      if (val0 == "") 
        break;
      var_type.push_back(val0);
      var_name.push_back(data.get_val());
      if (para.size() == 0) 
        para = val0;
      else
        para = para + " " + val0;
    }

    if (!table.add(ret_type, func_name, para))
      return false;
    table.new_level();
    ir.add("func", "", "", func_name);

    for (auto i = 0u; i < var_type.size(); i++) {
      if (var_type[i] == "int") {
        table.add(var_type[i], var_name[i], "");
        var_info = table.find(var_name[i]);
        switch (i) {
          case 0:
            ir.add("movl", "eax", "", var_info[2]);
            break;
          case 1:
            ir.add("movl", "ebx", "", var_info[2]);
            break;
          case 2:
            ir.add("movl", "ecx", "", var_info[2]);
            break;
          case 3:
            ir.add("movl", "edx", "", var_info[2]);
            break;
        }
      }
      if (var_type[i] == "char") {
        table.add(var_type[i], var_name[i], "");
        var_info = table.find(var_name[i]);
        switch (i) {
          case 0:
            ir.add("movb", "al", "", var_info[2]);
            break;
          case 1:
            ir.add("movb", "bl", "", var_info[2]);
            break;
          case 2:
            ir.add("movb", "cl", "", var_info[2]);
            break;
          case 3:
            ir.add("movb", "dl", "", var_info[2]);
            break;
        }
      }
    }
  }

  info_list.clear();
  ir.debug();
  return true;
}


int main () {
  stage_machine dfa;
  vector <info> info_list; //用于存储未规约完毕的词素序列
  symbol_list table;
  set <string> trim_expr;
  trim_expr.insert("init");
  trim_expr.insert("func");
  trim_expr.insert("call_func");
  trim_expr.insert("ass");

  string buff1, buff2;
  int n_line;

  while (cin >> buff1) {
    if (buff1 == "line") {
      cin >> n_line;
      continue;
    }
    cin >> buff2;

    cout << "[info] : input " << buff1 << " " << buff2 << endl;

    if (buff2 == "{") {
      if (info_list.size() == 0) {
        table.new_level();
        continue;
      }
      reduce (info_list, dfa);
      if (info_list.size() != 1 || trim_expr.find(info_list[0].get_type()) == trim_expr.end()){
        cout << "ERROR #1 : expression error at line " << n_line << endl;
        return 1;
      }
      if (generate_ir(table, info_list)) {
        cout << "at line " << n_line << endl;
        return 1;
      }
      continue;
    }

    if (buff2 == "}") {
      if (info_list.size() != 0) {
        cout << "ERROR #2 : need ; at line " << n_line << endl;
        return 1;
      }
      if (!table.last_level()) {
        cout << "ERROR #2 : need '{' for character '}' at line " << n_line << endl;
        return 1;
      }
      continue;
    }

    info tmp_info(buff1 + ' ' + buff2);
    info_list.push_back(tmp_info);

    if (buff2 == ";") {
      reduce (info_list, dfa);
      if (info_list.size() != 1 || trim_expr.find(info_list[0].get_type()) == trim_expr.end()){
        cout << "ERROR #3 : expression error at line " << n_line << endl;
        return 1;
      }
      if (generate_ir(table, info_list)) {
        cout << "at line " << n_line << endl;
        return 1;
      }
    }
  }
  
  return 0;
}
