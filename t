int a[10][5];
int sum(){
    int i, j, N = 10, M = 5;
    int result;
    while (i < 10)
    {
        while (j < 5)
        {
            result = result + a[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
    return result;
}
int main(){
    int i1, j1, s;

    while (i1 < 10){
        a[i1][0] = 1;
        a[i1][1] = 2 * i1;
        a[i1][2] = 1 + i1 * 2;
        a[i1][3] = i1;
        a[i1][4] = i1 * i1;
        i1 = i1 + 1;
    }
    
    return sum();
}