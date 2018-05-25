int add ( int a, int b) {
  a = a + b;
  return a;
}

int mul ( int a, int b ) {
  int c;
  a = add ( a, b );
  a = a * b;
  return a;
}

int main () {
  int a;
  a = 1;
  int b;
  b = 4;
  a = mul ( a, b );
  
  char c;
  c = 'a';
  putchar ( c );
  c = getchar ();
  putchar ( c );
  return 0;
}
