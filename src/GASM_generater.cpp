#include <iostream>
using namespace std;

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

int main () {
  string p1, p2, p3, p4;
  bool is_first_func = true;

  cout << ".section .data" << endl;
  cout << ".section .text" << endl;
  cout << ".global main" << endl;
  cout << endl;

  while (cin >> p1 >> p2 >> p3 >> p4) {
    if (p1 == "func") {
      if (is_first_func) 
        is_first_func = false;
      else {
        cout << "  leave" << endl;
        cout << "  ret" << endl;
        cout << endl;
      }

      if (p4 == "getchar")
        cout << "l_getchar" << ":" << endl;
      else if (p4 == "putchar")
        cout << "l_putchar" << ":" << endl;
      else
        cout << p4 << ":" << endl;

      cout << "  pushl %ebp" << endl;
      cout << "  movl  %esp, %ebp" << endl;
      continue;
    }
    cout << "  "; //just add some space for easy reading

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
      continue;
    }

    if (p1 == "init") {
      cout << "subl $" << p2 << ", %esp" << endl;
      continue;
    }

    cout << p1 << " ";
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
  cout << "  leave" << endl;
  cout << "  ret" << endl;
  cout << endl;
  return 0;
}
