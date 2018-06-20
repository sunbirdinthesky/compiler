int add(int a, int b) {
  a = a+b;
  return a;
}

int mul(int a, int b) {
  a = add(a, b);
  a = a*b;
  return a;
}

int div(int a, int b) {
  a = a/b;
  return a;
}
int main() {
  int a;
  a = 3;
  int b;
  b = 7;
  a = mul(a, b);
  putchar(a);

  a = 142;
  b = 2; 
  a = div(a, b);
  putchar(a);

  char c;
  c = 'a';
  {
    char c;
    c = 'A';
    putchar(c);
  }
  putchar(c);
  c = getchar ();
  putchar(c);
  return 0;
}
