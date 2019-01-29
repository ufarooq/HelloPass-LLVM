
void test(int a, int b, int c, int d, int e, int f) {
        a = b * c;
        d = b;
        b = b +1;
        e = d * c;
        f = b * c;
}

int main(int argc, char** argv) {
	test(10, 11, 25, 33, 43,26);
  return 0;
}