/*
 *  copyright: chunqi.wang
 *  you can use these code for free
 */

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <sstream>
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
      
      node *s70 = new node();
      node *s71 = new node();
      node *s72 = new node("ret");

      // paras  init  func
      root -> add("paras", s11);
      root -> add("para",  s21);
      s11  -> add("ops ,", s12);
      s21  -> add("ops ,", s12);
      s12  -> add("para", s13);  //paras
      s21  -> add("ops ;", s22);  //init
      s21  -> add("ops (", s32);
      s32  -> add("paras", s32);
      s32  -> add("para", s32);
      s32  -> add("ops )", s34); //func

      root -> add("key int",  s41);
      root -> add("key char",  s41);
      s41  -> add("id",   s42); //para

      root -> add("id",  s51);
      s51  -> add("ops (", s52);
      s52  -> add("id", s52);
      s52  -> add("ids", s52);
      s52  -> add("ops )", s54);
      s54  -> add("ops ;", s55); //call_func

      s51  -> add("ops =", s511); 
      s511 -> add("id", s512); 
      s511 -> add("num", s512); 
      s511 -> add("str", s512); 
      s511 -> add("character", s512); 
      s511 -> add("oper", s513); 
      s512 -> add("ops ;", s513); 
      s511 -> add("call_func", s513); //ass 

      s51  -> add("ops +", s514); 
      s51  -> add("ops -", s514); 
      s51  -> add("ops *", s514); 
      s51  -> add("ops /", s514); 
      s514 -> add("id", s515); 
      s514 -> add("num", s515); 
      s515 -> add("ops ;", s516); //oper

      root -> add("ids",  s60);
      s51  -> add("ops ,", s61);
      s60  -> add("ops ,", s61);
      s61  -> add("id", s62); //ids


      root -> add("cmd return", s70);
      s70  -> add("id", s71);
      s70  -> add("str", s71);
      s70  -> add("num", s71);
      s70  -> add("character", s71);
      s71  -> add("ops ;", s72);  //return id/num/char/str
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

        domain (int shift) {
          n_shift = shift;
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
          index[b] = op1.size();
          op1.push_back(a);
          op2.push_back(b);
          this->func.push_back(func_para_type);
          if (a == "int")  n_shift += 4;
          if (a == "char") n_shift += 1;
          address.push_back(to_string(n_shift));
        }

        int get_shift() {
          return n_shift;
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

    void new_special_level () {
      domain* tmp = cur;
      cur = new domain(tmp->get_shift());
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
        res = tmp->find(name);
        if (res.size() == 0) 
          tmp = tmp->father;
        else 
          return res;
      }
      return res;
    }

    vector <string> find_in_current (string name) {
      return cur->find(name);
    }

    bool addi_in_current (string a, string b, string func_para_type) {
      if (find_in_current(b).size() == 0) {
        cur->add (a, b, func_para_type);
        return true;
      } else {
        cerr << "ERROR : Multi define of " << a << " " << b << " ";
        return false;
      }
    }

    bool add (string a, string b, string func_para_type) {
      if (find(b).size() == 0) {
        cur->add (a, b, func_para_type);
        return true;
      } else {
        cerr << "ERROR : Multi define of " << a << " " << b << " ";
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
      if (str.substr(0,9) == "character") {
        type = "character";
        val.push_back("character");
        val.push_back(str.substr(11,1));
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
    IR () {
    }

    void add (string a, string b, string c, string d) {
      op0.push_back(a);
      op1.push_back(b);
      op2.push_back(c);
      op3.push_back(d);
    }

    void debug () {
      for (auto i = 0u; i < op1.size(); i++) {
        cerr << "[IR line " << i << "] : <" << op0[i] << "> <" << op1[i];
        cerr << "> <" << op2[i] << "> <" << op3[i] << ">" << endl;
      }
    }

    void output () {
      for (auto i = 0u; i < op1.size(); i++) {
        if (op0[i] == "") cout << "-";
        else cout << op0[i];
        cout << " ";
        if (op1[i] == "") cout << "-";
        else cout << op1[i];
        cout << " ";
        if (op2[i] == "") cout << "-";
        else cout << op2[i];
        cout << " ";
        if (op3[i] == "") cout << "-";
        else cout << op3[i];
        cout << endl;
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
      cerr << "[info] : total, loaded, ptr : " << len << " " << n_elm << " " << loop << endl;
      cerr << "[info] : str processing: ";
      for (int idx = loop; idx <= n_elm; idx++) {
        cerr << "<" << info_list[idx].get_type() << ">  ";
        dfa.match(info_list[idx].get_type());
      }
      cerr << endl;
      cerr << "[info] : stage of dfa : " << dfa.get_type() << endl << endl;
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
          tmp.add_val(info_list[loop].get_val(0));
          tmp.add_val(info_list[loop].get_val(1));
          //second elm : operactor
          val = info_list[loop+1].get_type().substr(4);
          tmp.add_val(val);
          //thrid  elm
          tmp.add_val(info_list[loop+2].get_val(0));
          tmp.add_val(info_list[loop+2].get_val(1));

          info_list.erase(info_list.begin()+loop+1, info_list.begin()+n_elm+1);
          info_list[loop] = tmp;
        }

        if (type == "ret") { 
          info tmp("ret");

          //first elm
          val = info_list[loop+1].get_type();
          if (val == "id") {
            tmp.add_val("unknow");
            tmp.add_val(info_list[loop+1].get_val(1));
          }
          if (val == "num") {
            tmp.add_val("num");
            tmp.add_val(info_list[loop+1].get_val(1));
          }
          if (val == "str") {
            tmp.add_val("str");
            tmp.add_val(info_list[loop+1].get_val(1));
          }
          if (val == "character") {
            tmp.add_val("character");
            tmp.add_val(info_list[loop+1].get_val(1));
          }

          info_list.erase(info_list.begin()+loop+1, info_list.begin()+n_elm+1);
          info_list[loop] = tmp;
        }

        if (type == "ass") { 
          info tmp("ass");
          //second elm
          val = info_list[loop+2].get_type();
          if (val == "id") {
            tmp.add_val("var");
            tmp.add_val("id");
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
          if (val == "character") {
            tmp.add_val("var");
            tmp.add_val("character");
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

  auto res = data.get_val();
  while (res != "") {
    cerr << res << endl;
    res = data.get_val();
  }

  if (data.get_type() == "init") {
    if (!table.addi_in_current(data.get_val(0), data.get_val(1), ""))
      return false;
    var_info = table.find_in_current(data.get_val(1));

    if (data.get_val(0) == "int")
      ir.add("init", "4", "", var_info[2]); 
    if (data.get_val(0) == "char")
      ir.add("init", "1", "", var_info[2]);
  }

  if (data.get_type() == "func") {
    string ret_type  = data.get_val();
    string func_name = data.get_val();
    string para      = "func";
    vector <string> var_type;
    vector <string> var_name;
    while (true) {
      string val0 = data.get_val();
      if (val0 == "") 
        break;
      var_type.push_back(val0);
      var_name.push_back(data.get_val());
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
        ir.add("init", "4", "", var_info[2]); 
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
        ir.add("init", "1", "", var_info[2]); 
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

  if (data.get_type() == "call_func") {
    var_info = table.find(data.get_val(1));
    string ret_type  = var_info[0];
    string func_name = var_info[1];
    vector <string> para_type;
   
    if (var_info[3] == "") {
      cerr << "ERROR : "  << func_name << " is not a func, error ";
      return false;
    }

    string tmp;
    istringstream iss(var_info[3]);
    iss >> tmp;
    while (!iss.eof()) {
      iss >> tmp;
      para_type.push_back(tmp);
    }

    data.get_val();
    data.get_val();

    unsigned int  n_para = 0;
    while (true) {
      if (data.get_val() == "")
        break;
      n_para++;
      if (n_para > para_type.size()) {
        cerr << "ERROR : too many parameters for function " << func_name << ", error "; 
        return false;
      }
      string name = data.get_val();
      var_info    = table.find(name);
      
      // checking
      if (var_info.size() == 0) {
        cerr << "ERROR : variable " << name << " not found, error ";
        return false;
      }
      if (var_info[3].size() != 0) {
        cerr << "ERROR : " << name << " is a function, not variable, error ";
        return false;
      }

      // adding IR
      if (var_info[0] == "int") {
        switch (n_para) {
          case 1:
            ir.add("movl", var_info[2], "", "eax");
            break;
          case 2:
            ir.add("movl", var_info[2], "", "ebx");
            break;
          case 3:
            ir.add("movl", var_info[2], "", "ecx");
            break;
          case 4:
            ir.add("movl", var_info[2], "", "edx");
            break;
        }
      } 
      if (var_info[0] == "char") {
        switch (n_para) {
          case 1:
            ir.add("movzbl", var_info[2], "", "eax");
            break;
          case 2:
            ir.add("movzbl", var_info[2], "", "ebx");
            break;
          case 3:
            ir.add("movzbl", var_info[2], "", "ecx");
            break;
          case 4:
            ir.add("movzbl", var_info[2], "", "edx");
            break;
        }
      } 
    }
    if (n_para < para_type.size()) {
      cerr << "ERROR : too less parameters for function " << func_name << ", error "; 
      return false;
    }
    ir.add("call", func_name, "", "");
  }

  // ass 
  if (data.get_type() == "ass") {
    string sub_type = data.get_val();
    if (sub_type == "var") {
      //pre check
      sub_type = data.get_val();
      if (sub_type == "id") {
        string name = data.get_val(2);
        var_info    = table.find(name);
        //checking 
        if (var_info.size() == 0) {
          cerr << "ERROR : variable " << name << " not found, error ";
          return false;
        }
        if (var_info[3].size() != 0) {
          cerr << "ERROR : " << name << " is a function, not variable, error ";
          return false;
        }
        if (var_info[0] == "int")  ir.add("movl",   var_info[2], "", "eax");
        if (var_info[0] == "char") ir.add("movzbl", var_info[2], "", "eax");
      }
      if (sub_type == "str") {
        //sorry, i can`t finish it yet
      }
      if (sub_type == "num") {
        ir.add("movl", "$" + data.get_val(2), "", "eax");
      }
      if (sub_type == "character") {
        char tmp = data.get_val(2)[0];
        int  ascii = int(tmp);
        string code = to_string(ascii);
        ir.add("movl", "$" + code, "", "eax");
      }

      //start assigning
      string name = data.get_val(4);
      var_info = table.find(name);

      //checking 
      if (var_info.size() == 0) {
        cerr << "ERROR : variable " << name << " not found, error ";
        return false;
      }
      if (var_info[3].size() != 0) {
        cerr << "ERROR : " << name << " is a function, not variable, error ";
        return false;
      }
      // assign
      if (var_info[0] == "int")  ir.add("movl", "eax", "", var_info[2]);
      if (var_info[0] == "char") ir.add("movb", "al",  "", var_info[2]);
    }

    // for a = a +-*/ b
    if (sub_type == "oper") {
      string name1 = data.get_val(2);
      auto   info1 = table.find(name1);
      //check var
      if (info1.size() == 0) {
        cerr << "ERROR : variable " << name1 << " not found, error ";
        return false;
      }
      if (info1[3].size() != 0) {
        cerr << "ERROR : " << name1 << " is a function, not variable, error ";
        return false;
      }

      if (info1[0] == "int") 
        ir.add("movl", info1[2], "", "eax");
      if (info1[0] == "char") 
        ir.add("movzbl", info1[2], "", "eax");

      if (data.get_val(4) == "unknow") {
        string name2 = data.get_val(5);
        auto   info2 = table.find(name2);
        //check var
        if (info2.size() == 0) {
          cerr << "ERROR : variable " << name2 << " not found, error ";
          return false;
        }
        if (info2[3].size() != 0) {
          cerr << "ERROR : " << name2 << " is a function, not variable, error ";
          return false;
        }

        if (info2[0] == "int") 
          ir.add("movl", info2[2], "", "ebx");
        if (info2[0] == "char") 
          ir.add("movzbl", info2[2], "", "ebx");
      }
      if (data.get_val(4) == "num") {
        string num = data.get_val(5);
        ir.add("movl", "$" + num, "", "ebx");
      }
      if (data.get_val(4)  == "character") {
        char tmp = data.get_val(5)[0];
        int  ascii = int(tmp);
        string code = to_string(ascii);
        ir.add("movl", "$" + code, "", "eax");
      }

      if (data.get_val(3) == "+") {
        ir.add("addl", "ebx", "eax", "eax");
      }
      if (data.get_val(3) == "-") {
        ir.add("subl", "ebx", "eax", "eax");
      }
      if (data.get_val(3) == "*") {
        ir.add("imull", "ebx", "", "eax");
      }
      if (data.get_val(3) == "/") {
        ir.add("cltd", "", "", "");
        ir.add("idivl", "ebx", "", "eax");
      }
      //start assigning
      string name = data.get_val(7);
      var_info = table.find(name);

      //checking 
      if (var_info.size() == 0) {
        cerr << "ERROR : variable " << name << " not found, error ";
        return false;
      }
      if (var_info[3].size() != 0) {
        cerr << "ERROR : " << name << " is a function, not variable, error ";
        return false;
      }
      // assign
      if (var_info[0] == "int")  ir.add("movl", "eax", "", var_info[2]);
      if (var_info[0] == "char") ir.add("movb", "al",  "", var_info[2]);
    }

    // for a = add ( a + b );
    if (sub_type == "call_func") {
      vector <string> swap;
      while (true) {
        string val = data.get_val();
        if (val == "") break;
        swap.push_back(val);
      }
      // made a tmp func type info
      {
        info data("call_func");
        for (auto i = 0u; i < swap.size()-2; i++)
          data.add_val(swap[i]);

        var_info = table.find(data.get_val(1));
        string ret_type  = var_info[0];
        string func_name = var_info[1];
        vector <string> para_type;
       
        if (var_info[3] == "") {
          cerr << "ERROR : "  << func_name << " is not a func, error ";
          return false;
        }

        string tmp;
        istringstream iss(var_info[3]);
        iss >> tmp;
        while (!iss.eof()) {
          iss >> tmp;
          para_type.push_back(tmp);
        }

        data.get_val();
        data.get_val();

        unsigned int  n_para = 0;
        while (true) {
          if (data.get_val() == "")
            break;
          n_para++;
          if (n_para > para_type.size()) {
            cerr << "ERROR : too many parameters for function " << func_name << ", error "; 
            return false;
          }
          string name = data.get_val();
          var_info    = table.find(name);
          
          // checking
          if (var_info.size() == 0) {
            cerr << "ERROR : variable " << name << " not found, error ";
            return false;
          }
          if (var_info[3].size() != 0) {
            cerr << "ERROR : " << name << " is a function, not variable, error ";
            return false;
          }

          // adding IR
          if (var_info[0] == "int") {
            switch (n_para) {
              case 1:
                ir.add("movl", var_info[2], "", "eax");
                break;
              case 2:
                ir.add("movl", var_info[2], "", "ebx");
                break;
              case 3:
                ir.add("movl", var_info[2], "", "ecx");
                break;
              case 4:
                ir.add("movl", var_info[2], "", "edx");
                break;
            }
          } 
          if (var_info[0] == "char") {
            switch (n_para) {
              case 1:
                ir.add("movzbl", var_info[2], "", "eax");
                break;
              case 2:
                ir.add("movzbl", var_info[2], "", "ebx");
                break;
              case 3:
                ir.add("movzbl", var_info[2], "", "ecx");
                break;
              case 4:
                ir.add("movzbl", var_info[2], "", "edx");
                break;
            }
          } 
        }
        if (n_para < para_type.size()) {
          cerr << "ERROR : too less parameters for function " << func_name << ", error "; 
          return false;
        }
        ir.add("call", func_name, "", "");
      }

      string name = swap[swap.size()-1];
      var_info = table.find(name);

      //checking 
      if (var_info.size() == 0) {
        cerr << "ERROR : variable " << name << " not found, error ";
        return false;
      }
      if (var_info[3].size() != 0) {
        cerr << "ERROR : " << name << " is a function, not variable, error ";
        return false;
      }
      if (var_info[0] == "int") {
        ir.add("movl", "eax", "", var_info[2]);
      }
      if (var_info[0] == "char") {
        ir.add("movb", "al", "", var_info[2]);
      }
    }

    //end of ass operation
  }

  if (data.get_type() == "ret") {
    string sub_type = data.get_val();
    if (sub_type == "unknow") { // to do
      string name = data.get_val(1);
      var_info    = table.find(name);
      //checking 
      if (var_info.size() == 0) {
        cerr << "ERROR : variable " << name << " not found, error ";
        return false;
      }
      if (var_info[3].size() != 0) {
        cerr << "ERROR : " << name << " is a function, not variable, error ";
        return false;
      }
      if (var_info[0] == "int")  ir.add("movl",   var_info[2], "", "eax");
      if (var_info[0] == "char") ir.add("movzbl", var_info[2], "", "eax");
    }
    if (sub_type == "str") {
      //sorry, i can`t finish it yet
    }
    if (sub_type == "num") {
      ir.add("movl", "$" + data.get_val(1), "", "eax");
    }
    if (sub_type == "character") {
      char tmp = data.get_val(1)[0];
      int  ascii = int(tmp);
      string code = to_string(ascii);
      ir.add("movl", "$" + code, "", "eax");
    }
    ir.add("leave", "", "", "");
    ir.add("ret", "", "", "");
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
  trim_expr.insert("ret");
  {
    table.add ("char", "getchar", "func");
    ir.add("func", "", "", "getchar");
    ir.add("movzbl", "al", "", "eax");
    ir.add("call", "gcc_getchar", "", "");
    ir.add("leave", "", "", "");
    ir.add("ret", "", "", "");

    table.add ("char", "putchar", "func char");  
    ir.add("func", "", "", "putchar");
    ir.add("init", "1", "", "1");
    ir.add("movb", "al", "", "1");
    ir.add("call", "gcc_putchar", "", "");
    ir.add("leave", "", "", "");
    ir.add("ret", "", "", "");
  }

  string buff1, buff2;
  int n_line;

  while (cin >> buff1) {
    if (buff1 == "line") {
      cin >> n_line;
      continue;
    }
    cin >> buff2;

    cerr << "[info] : input " << buff1 << " " << buff2 << endl;

    if (buff2 == "{") {
      if (info_list.size() == 0) {
        table.new_special_level();
        continue;
      }
      reduce (info_list, dfa);
      if (info_list.size() != 1 || trim_expr.find(info_list[0].get_type()) == trim_expr.end()){
        cerr << "ERROR #1 : expression error at line " << n_line << endl;
        return 1;
      }
      if (!generate_ir(table, info_list)) {
        cerr << "at line " << n_line << endl;
        return 1;
      }
      continue;
    }

    if (buff2 == "}") {
      if (info_list.size() != 0) {
        cerr << "ERROR #2 : need ; at line " << n_line << endl;
        return 1;
      }
      if (!table.last_level()) {
        cerr << "ERROR #2 : need '{' for character '}' at line " << n_line << endl;
        return 1;
      }
      continue;
    }

    info tmp_info(buff1 + ' ' + buff2);
    info_list.push_back(tmp_info);

    if (buff2 == ";") {
      reduce (info_list, dfa);
      if (info_list.size() != 1 || trim_expr.find(info_list[0].get_type()) == trim_expr.end()){
        cerr << "ERROR #3 : expression error at line " << n_line << endl;
        return 1;
      }
      if (!generate_ir(table, info_list)) {
        cerr << "at line " << n_line << endl;
        return 1;
      }
    }
  }

  if (table.last_level()) {
    cerr << "ERROR #2: expect } at line " << n_line << endl;
  }
  if (table.find("main").size() == 0) {
    cerr << "ERROR #4: cat find main function" << endl;
    return 1;
  }
  ir.output();  
  
  return 0;
}
