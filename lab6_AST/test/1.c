int main(){
    int a;
    a = 0;
    int b;
    b = 0;
    while (a < 100) {
    if (a == 5) {
        b = 25;
    }else if (a == 10) {
        b = 42;  
    }else {
        b = a * 2;
    }
    a = a + 1;
    } 
    printf("%d\n",a);
    printf("%d\n",b); 
    return 0;
}