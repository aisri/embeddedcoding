#include <vector>
#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;

int shortestWay(string src, string target) {
    int numSequences = 0;
    int tIdx = 0;

    while (tIdx < target.size()) {
        int i = 0, j = tIdx;

        while (i < src.size() && j < target.size()) {
            if (src[i++] == target[j]) {
                j++;
            }
        }
        if (j == tIdx)
            return -1;
        numSequences++;
        tIdx = j;
    }
    return numSequences;
}

int clearRange(int n, int s, int e) {
    int mask = (0x1 << (e - s + 1)) - 1;
    return n & (mask << s);
}

int coinChange(vector<int> coins, int amount) {
    vector<int> dp(amount + 1, amount + 1);

    dp[0] = 0;
    for (int i = 1; i <= amount; i++) {
        for (int &c: coins) {
            if (c <= i ) {
                dp[i] = std::min(dp[i], 1 + dp[i - c]);
            }
        }
    }
    return dp[amount] > amount ? -1 : dp[amount];
}

int uniquePaths(int m, int n) {
    std::function<int(int, int, int , int)> helper = [&](int r, int c, int m, int n) {
        if (r == m-1 && c == n-1) return 1;
        if (r >m || c > n) return 0;

        return helper(r + 1, c, m, n) + helper(r, c + 1, m, n);
    };

    return helper(0, 0, 3, 2);
}

void print_n_prime_numbers(int n)
{
    vector<int> primes;
    if (n <= 0)
        return;
    if (n == 1) {
        cout << "2 " << endl;
        return;
    }
    cout << "2 ";
    primes.push_back(2);
    n--;
    for (int i = 3; n; i++) {
        bool isprime = true;
        for (const auto& p : primes) {
            if (i % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            cout << i << " ";
            primes.push_back(i);
            n--;
        }
    }
    cout << endl;
}

using Matrix = vector<vector<int>>;

Matrix matmul(Matrix A, Matrix B) {
    if (!A.size() || !B.size())
        return {};

    if (A[0].size() != B.size())
        return {};

    const int npRows = A.size();
    const int npCols = B[0].size();
    Matrix product {A.size(), vector<int>(npCols, 0)};
    for (int r = 0; r < npRows; r++) {
        for (int c = 0; c < npCols; c++) {
            for (int k = 0; k < B.size(); k++) {
                product[r][c] += (A[r][k] * B[k][c]);
            }
        }
    }
    return product;
}

void inplaceRotate(Matrix &A)
{
    for (int top = 0, bottom = A.size() - 1; top < bottom; top++, bottom--) {
        for (int i = 0, left = top, right = bottom; i < (right - left); i++) {
            int t = A[top][left + i];
            A[top][left + i] = A[bottom - i][left];
            A[bottom - i][left] = A[bottom][right - i];
            A[bottom][right - i] = A[top + i][right];
            A[top + i][right] = t;
        }
    }
}

void kadans(vector<int> a) {
    int sum = a[0];
    int gSum = INT_MIN;
    for (int i = 1; i < a.size(); i++) {
        if (sum < 0) {
            sum = a[i];
        } else {
            sum += a[i];
        }

        if (sum > gSum)
            gSum = sum;
    }
    cout << gSum << " " << endl;

    int lmax = 0, gmax = INT32_MIN;
    for (int i = 0; i < a.size(); i++) {
        lmax = std::max(a[i], a[i] + lmax);
        if (lmax > gmax)
            gmax = lmax;
    }
    cout << gmax << " <<- gmax" << endl;
}

using BigNumber = vector<unsigned char>;
void add_big_numbers(BigNumber &a, BigNumber &b, BigNumber &result) {
    // assume a.size() <= b.size()
    if (b.size() < a.size())
        std::swap(a, b);

    auto addWithCarry = [&](unsigned aN, unsigned bN, unsigned &carry) {
        unsigned aD = UINT8_MAX - aN;
        unsigned sum = 0;
        cout << hex << aD << endl;
        // if we can fit bN in rest of aD value;
        if (bN < aD) {
            sum = aN + bN;
            cout << "fit" << endl;
        } else {
            carry = bN - aD;
            cout << "NO fit " << hex << carry << endl;
        }
        return sum;
    };

    unsigned carry = 0;
    for (int i = 0; i < a.size(); i++) {
        cout << "loop " << i << " " << hex << carry << endl;
        auto sum = addWithCarry(a[i], b[i], carry);
        result.push_back(sum);
    }
    for (int i = a.size(); i < b.size(); i++) {
        auto sum = addWithCarry(a[i], b[i] + carry, carry);
        result.push_back(sum);
    }
    result.push_back(carry);
}

void print_big_number(BigNumber a) {
    for(int it = a.size()-1; it >=0; it--)
        cout << hex << a[it];
    cout << endl;
}

void fibonacci(int n) {
    unsigned first = 1, second = 1;
    cout << first << " " << second << " ";
    while (n--) {
        auto sum = first + second;
        cout << sum << " ";
        first = second;
        second = sum;
    }
    cout << endl;
}

void telephone_number_combos( string &&num) {
    static vector<string> lookup = {
        " ", " ", "ABC", "DEF", "GHI",
        "JKL", "MNO", "PQRS", "TUV", "WXYZ"
    };

    std::function<void(string &n, int idx, string& res)> helper = nullptr;
    helper = [&](string& n, int idx, string& res) {
        if (res.size() == n.size()) {
            cout << res << endl;
            return;
        }
        string s = lookup[n[idx] - '0'];
        // cout << "permuting-" << s << endl;
        for (const auto& ch : s) {
            res.push_back(ch);
            // cout << "pushing-" << ch << " res= " << res << endl;
            helper(n, idx + 1, res);
            // cout << "popping-" << ch << endl;
            res.pop_back();
        }
    };

    string result;
    helper(num, 0, result);
}

void permute(string &&s) {
    std::function<void(string&, int)> helper = nullptr;

    helper = [&](string& str, int idx) {
        if (idx == str.size()) {
            cout << str << endl;
            return;
        }
        for (int i = idx; i < str.size(); i++) {
            std::swap(str[i], str[idx]);
            helper(s, idx + 1);
            std::swap(str[i], str[idx]);
        }
    };
    helper(s, 0);
}

int main()
{
    // permute("abcde");
    // telephone_number_combos("23456");

    // fibonacci(200);
    // BigNumber a = { UINT8_MAX, UINT8_MAX };
    // BigNumber b = { UINT8_MAX, UINT8_MAX };
    // BigNumber result;
    // add_big_numbers(a, b, result);
    // print_big_number(a);
    // print_big_number(b);
    // print_big_number(result);
    // unsigned char aa = 0xFF + 0xFF + 0xFF;
    // cout << hex << (-2>>0) << endl;

    // print_n_prime_numbers(0);
    // print_n_prime_numbers(1);
    // print_n_prime_numbers(2);
    // print_n_prime_numbers(2000);
    // Matrix A = {
    //     { 1, 2, 3, 0 },
    //     { 4, 5, 6, 9 },
    //     { 7, 8, 9, 5 },
    //     { 3, 4, 7, 6 }
    // };

    // Matrix B = { { 1 }, { 1 }, { 1 }, { 1 } };

    // auto printMatrix = [](Matrix m) {
    //     for (const auto& r : m) {
    //         for (const auto& c : r) {
    //             cout << c << " ";
    //         }
    //         cout << endl;
    //     }
    //     cout << endl;
    // };

    // printMatrix(matmul(A, B));
    // inplaceRotate(A);
    // printMatrix(A);
    kadans({ -2, 1, 2, 3, -1, 5, -9 });
}