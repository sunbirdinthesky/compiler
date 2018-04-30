#include <iostream>
#include <cstring>
using namespace std;

int main () {
  bool odd[1000];
  int list[100];
  int n_odd = 0;
  memset(odd, 1, sizeof(odd));
  for (int i = 2; i < 1000; i++) {
    if (odd[i] == false) continue;
    list[n_odd++] = i;
    int tmp = i+i;
    while (tmp < 1000) {
      odd[tmp] = false;
      tmp += i;
    }
  }

  int input;
  cin >> input;
  
  int pos = 0;
  while (input != 1) {
    if (input % list[pos] == 0) {
      input = input / list[pos];
      cout << list[pos] << endl;
    }
    else 
      pos ++;
  }

  return 0;
}


