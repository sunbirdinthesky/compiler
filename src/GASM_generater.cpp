#include <iostream>
#include <vector>
using namespace std;

vector <string> b1, b2, b3, b4;

string address_translater (string address) {
  int ascii = char(address[0]);
  if (ascii >= int('0') && ascii <= int('9')) {
    return "-" + address + "(%ebp)";
  }
  if (ascii == int('$'))
    return address;
  if (ascii == int('-'))
    return "";
  return "%" + address;
}

void translate(string p1, string p2, string p3, string p4) {
  if (p1 == "func") {
    if (p4 == "getchar")
      cout << "l_getchar" << ":" << endl;
    else if (p4 == "putchar")
      cout << "l_putchar" << ":" << endl;
    else
      cout << p4 << ":" << endl;

    cout << "  pushl %ebp" << endl;
    cout << "  movl  %esp, %ebp" << endl;
    return;
  }

  cout << "  ";

  if (p1 == "call") {
    if (p2 == "getchar")
      cout << "call l_getchar" << endl;
    else if (p2 == "gcc_getchar")
      cout << "call getchar" << endl;
    else if (p2 == "putchar")
      cout << "call l_putchar" << endl;
    else if (p2 == "gcc_putchar")
      cout << "call putchar" << endl;
    else 
      cout << "call " << p2 << endl;
    return;
  }

  if (p1 == "init") {
    cout << "subl $" << p2 << ", %esp" << endl;
    return;
  }

  cout <<  p1 << " ";
  p2 = address_translater(p2);
  p3 = address_translater(p3);
  p4 = address_translater(p4);
  if (p1.substr(0, 3) == "mov") 
    cout << p2 << ", " << p4 << endl;
  else {
    if (p3 == "") 
      cout << p2 << endl;
    else 
      cout << p2 << ", " << p4 << endl;
  }
}

void optimize() {
  int len = b1.size();
  bool is_have_func = false;
  vector <string> t1, t2, t3, t4;
  
  for (int idx = 0; idx < len; idx++) {
    if (b1[idx] == "func") 
      is_have_func = true;
    if (is_have_func) {
      t1.push_back(b1[idx]);
      t2.push_back(b2[idx]);
      t3.push_back(b3[idx]);
      t4.push_back(b4[idx]);

      if (idx < len-1 && b1[idx] == b1[idx+1] && b2[idx] == b4[idx+1] && 
          b3[idx] == b3[idx+1] && b4[idx] == b2[idx+1]) {
        idx += 1;
        cerr << "[info] : combined 1 line" << endl;
        continue;
      }
    } else {
      cerr << "[info] : removed 1 line" << endl;
    }
  }
  len = t1.size();
  for (int idx = 0; idx < len; idx++) {
    translate(t1[idx], t2[idx], t3[idx], t4[idx]);
  }
  cout << endl;
  b1.clear();
  b2.clear();
  b3.clear();
  b4.clear();
}

int main () {
  string p1, p2, p3, p4;

  cout << ".section .data" << endl;
  cout << ".section .text" << endl;
  cout << ".global main" << endl;
  cout << endl;
  
  while (cin >> p1 >> p2 >> p3 >> p4) {
    b1.push_back(p1);
    b2.push_back(p2);
    b3.push_back(p3);
    b4.push_back(p4);
    if (p1 == "ret")
      optimize();
  }
  optimize();

  return 0;
}
