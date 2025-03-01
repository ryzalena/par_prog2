#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;

void readMatrixFromFile(const string& filename, vector<vector<int>>& matrix) {
    ifstream infile(filename);
    int rows, cols;

    infile >> rows >> cols;
    matrix.resize(rows, vector<int>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            infile >> matrix[i][j];
        }
    }

    infile.close();
}

void writeMatrixToFile(const string& filename, const vector<vector<int>>& matrix) {
    ofstream outfile(filename);

    int rows = matrix.size();
    int cols = matrix[0].size();

    outfile << rows << " " << cols << endl;
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            outfile << elem << " ";
        }
        outfile << endl;
    }

    outfile.close();
}

vector<vector<int>> multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int A_rows = A.size();
    int A_cols = A[0].size();
    int B_cols = B[0].size();

    vector<vector<int>> C(A_rows, vector<int>(B_cols, 0));

     #pragma omp parallel for collapse(2)
     for (int i = 0; i < A_rows; ++i) {
         for (int j = 0; j < B_cols; ++j) {
             int sum = 0;
             for (int k = 0; k < A_cols; ++k) {
                 sum += A[i][k] * B[k][j];
             }
             C[i][j] = sum;
         }
     }

    return C;
}

int main() {
    setlocale(LC_ALL, "RUS");
    vector<vector<int>> A, B;

    cout << "Чтение файлов начато." << endl;

    readMatrixFromFile("matrixA.txt", A);
    readMatrixFromFile("matrixB.txt", B);

    cout << "Чтение файлов закончено." << endl;

    if (A[0].size() != B.size()) {
        cerr << "Ошибка: количество столбцов первой матрицы должно быть равно количеству строк второй матрицы." << endl;
        return 1;
    }

    auto start = chrono::high_resolution_clock::now();
    vector<vector<int>> C = multiplyMatrices(A, B);
    auto end = chrono::high_resolution_clock::now();

    writeMatrixToFile("result.txt", C);

    chrono::duration<double> elapsed = end - start;
    cout << "Время выполнения: " << elapsed.count() << " секунд" << endl;
    cout << "Размер первой матрицы: " << A.size() << "x" << A[0].size() << endl;
    cout << "Размер второй матрицы: " << B.size() << "x" << B[0].size() << endl;
    cout << "Размер результирующей матрицы: " << C.size() << "x" << C[0].size() << endl;

    system("C:\\Users\\456\\PycharmProjects\\matrix\\venv\\Scripts\\python.exe C:\\Users\\456\\source\\repos\\pp-prog-2\\pp-prog-2\\verify_result.py result.txt matrixA.txt matrixB.txt");

    return 0;
}
