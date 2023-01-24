// A [M N] B [N K]
#include <iostream>
#include <vector>

using namespace std;
using Matrix = vector<vector<int>>;

Matrix matmul(Matrix A, Matrix B)
{
    /*  2x2     2x1
      [[0 1]   [4]
      [2 3]]   [5]
      ijk
      00 = [00 * 00 + 01 * 10]
      10 = [10 * 00 + 11 * 10]
    */
    if (A.size() == 0 || B.size() == 0)
        return Matrix {};

    if (A[0].size() != B.size())
        return Matrix {};

    int sz = B[0].size();
    Matrix result { A.size(), vector<int>(sz, 0) };

    for (auto i = 0; i < A.size(); i++) {
        for (auto j = 0; j < B[0].size(); j++) {
            for (auto k = 0; k < B.size(); k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}

void rotateMatrix(Matrix& A)
{
    int left = 0, right = A.size() - 1;
    while (left < right) {
        int top = left, bottom = right, i = 0;
        while (i++ < bottom - top) {
            auto t = A[top][left + i];
            A[top][left + i ] = A[bottom - i][left];
            A[bottom - i][left] = A[bottom][right - i];
            A[bottom][right - i] = A[top + i][right];
            A[top + i][right] = t;
        }
        left++;
        right--;
    }
}

void transposeAndSwap(Matrix &A) {
    int sz = A.size();
    for (int r = 0; r < A.size(); r++) {
        for (int c = r; c < A[r].size(); c++) {
            if (r != c) {
                std::swap(A[r][c], A[c][r]);
            }
        }
    }

    for (int r = 0; r < A.size(); r++) {
        for (int c = 0; c < A[r].size() / 2; c++) {
            std::swap(A[r][c], A[r][sz - c - 1]);
        }
    }
}

int main()
{
    Matrix A = {
        { 1, 2, 3, 0 },
        { 4, 5, 6, 9 },
        { 7, 8, 9, 5 },
        { 3, 4, 7, 6 }
    };

    Matrix B = { { 1 }, { 1 }, { 1 }, { 1 } };

    auto printMatrix = [](Matrix m) {
        for (auto& r : m) {
            for (auto& c : r) {
                cout << c << " ";
            }
            cout << endl;
        }
        cout << endl;
    };
    printMatrix(matmul(A, B));

    printMatrix(matmul(A, B));
    transposeAndSwap(A);
    printMatrix(A);
    rotateMatrix(A);
    printMatrix(A);

    return 0;
}
