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
