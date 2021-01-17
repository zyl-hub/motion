#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int m = 10;
    int n = 8;
    vector<vector<double>> test(m, vector<double>(n, 0));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            test[i][j] = i*j;
            cout << test[i][j]<<'\n';
        }
    }
}
