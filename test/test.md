# Test Program: Printing Stars

### Program Description
Basically, the program solves the problem in this 
[link](https://www.acmicpc.net/problem/2446).
Given a natural number N (<= 1000), the task is to print stars in 2N-1 lines like the example below. If input N exceeds 40, it warns you about not being able to print all the stars in 80 columns.
> Input (Example)
```
5
```

> Output (Example)
```
*********
 *******
  *****
   ***
    *
   ***
  *****
 *******
*********
```

> Input (Example)
```
41
y

```

> Output (Example)
```
It will exceed 80 columns. Continue? (y/n)
[stars omitted]
```

### Pseudocode
```C
unsigned int atoi(char *s) {
    int n = 0;
    while(*s != 0) {
        n *= 10;
        /*
        tmp = n;
        tmp = tmp + tmp;
        tmp = tmp + tmp;
        tmp = tmp + tmp;
        tmp2 = n + n;
        tmp = tmp + tmp2;
        */
        n += ch[i] - '0';
    }
}

void main() {
    unsigned int i, N;
    unsigned char buf[64];
    gets(buf);
    N = atoi(buf);
    if(N > 40) {
        do{
            printf("It will exceed 80 columns. Continue? (y/n)\n");
            scanf("%s", buf);
        }while (buf[0] != 'y' && buf[0] != 'n');
        if(buf[0] == 'n') exit(0);
    }   
    for(i=0; i<2*N-1; ++i) {
        unsigned int tmp1, tmp2;
        if(i<N) {
            tmp1 = i;
            tmp2 = 2*N-2*i-1;
        }
        else {
            tmp1 = 2*N-i-2;
            tmp2 = 3+2*i-2*N;
        }
		while(tmp1--) putchar(' ');
		while(tmp2--) putchar('*');
        putchar('\n');
    }   
    exit(0);
}
```

### Short version (in python, just for fun)
```python
i=N=input()
while i>1-N:i-=1;x=abs(i)+1;print ' '*(N-x)+'*'*(2*x-1)
```

