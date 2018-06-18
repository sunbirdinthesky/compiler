int main () {
  char a;
  a = 'A';
  a = a + 1;
  {
    putchar( a );
    char a;
    a = 'k';
    putchar( a );
  }
  putchar( a );
  return 0;
}
  
