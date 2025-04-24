// app/src/main/cpp/native-lib.cpp

#include <jni.h>
#include <vector>
#include <stdexcept>
#include <valarray>
#include <cmath>    // for std::fabs()

// Helpers: element access in a flat valarray
inline double get(const std::valarray<double>& M, int r, int c, int C) {
    return M[r*C + c];
}
inline void set(std::valarray<double>& M, int r, int c, int C, double v) {
    M[r*C + c] = v;
}

// Recursive determinant for an n×n matrix stored in row-major A
double det(const std::vector<double>& A, int n) {
    if (n == 1) return A[0];
    if (n == 2) return A[0]*A[3] - A[1]*A[2];
    double D = 0.0;
    std::vector<double> minor((n-1)*(n-1));
    for (int col = 0; col < n; ++col) {
        // build minor excluding row 0 and column col
        for (int i = 1; i < n; ++i) {
            int mj = 0;
            for (int j = 0; j < n; ++j) {
                if (j == col) continue;
                minor[(i-1)*(n-1) + mj] = A[i*n + j];
                ++mj;
            }
        }
        double cofactor = ((col % 2) == 0 ? 1 : -1) * det(minor, n-1);
        D += A[col] * cofactor;
    }
    return D;
}

// Compute the adjugate of an n×n matrix A
std::vector<double> adjugate(const std::vector<double>& A, int n) {
    std::vector<double> adj(n*n);
    std::vector<double> minor((n-1)*(n-1));
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            // build minor excluding row r and column c
            int mi = 0;
            for (int i = 0; i < n; ++i) {
                if (i == r) continue;
                int mj = 0;
                for (int j = 0; j < n; ++j) {
                    if (j == c) continue;
                    minor[mi*(n-1) + mj] = A[i*n + j];
                    ++mj;
                }
                ++mi;
            }
            double cof = ((r + c) % 2 == 0 ? 1 : -1) * det(minor, n-1);
            // place in the transposed position
            adj[c*n + r] = cof;
        }
    }
    return adj;
}

extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_com_example_assignment3_MainActivity_processMatrices(
        JNIEnv* env, jobject /* this */,
        jint op, jint rA, jint cA, jint rB, jint cB,
        jdoubleArray aArr, jdoubleArray bArr
) {
    try {
        // Read dimensions
        int rowsA = rA, colsA = cA, rowsB = rB, colsB = cB;
        int sizeA = rowsA * colsA;
        int sizeB = rowsB * colsB;

        // Copy Java arrays into C++ valarrays
        jdouble* aPtr = env->GetDoubleArrayElements(aArr, nullptr);
        jdouble* bPtr = env->GetDoubleArrayElements(bArr, nullptr);
        std::valarray<double> A(aPtr, sizeA), B(bPtr, sizeB);

        std::valarray<double> C;  // will hold the result

        // ADDITION or SUBTRACTION
        if (op == 0 || op == 1) {
            if (rowsA != rowsB || colsA != colsB)
                throw std::runtime_error("Add/Sub requires matrices of the same dimensions");
            if (op == 0) {
                C = A + B;
            } else {
                C = A - B;
            }
        }
            // MULTIPLICATION
        else if (op == 2) {
            if (colsA != rowsB)
                throw std::runtime_error("Multiply requires A.cols == B.rows");
            C = std::valarray<double>(rowsA * colsB);
            for (int i = 0; i < rowsA; ++i) {
                for (int j = 0; j < colsB; ++j) {
                    double sum = 0;
                    for (int k = 0; k < colsA; ++k) {
                        sum += get(A, i, k, colsA) * get(B, k, j, colsB);
                    }
                    set(C, i, j, colsB, sum);
                }
            }
            rowsB = colsB;  // adjust for return shape
        }
            // DIVISION (A · B⁻¹)
        else if (op == 3) {
            if (rowsB != colsB)
                throw std::runtime_error("Divide requires B to be square");
            // Copy B into a vector for det/adj
            std::vector<double> Bv(sizeB);
            for (int i = 0; i < sizeB; ++i) Bv[i] = B[i];
            double D = det(Bv, rowsB);
            if (std::fabs(D) < 1e-12)
                throw std::runtime_error("Divide error: B is singular");
            auto adjB = adjugate(Bv, rowsB);
            std::valarray<double> invB(sizeB);
            for (int i = 0; i < sizeB; ++i) invB[i] = adjB[i] / D;

            if (colsA != rowsB)
                throw std::runtime_error("Divide requires A.cols == B.rows");
            C = std::valarray<double>(rowsA * colsB);
            for (int i = 0; i < rowsA; ++i) {
                for (int j = 0; j < colsB; ++j) {
                    double sum = 0;
                    for (int k = 0; k < colsA; ++k) {
                        sum += get(A, i, k, colsA) * get(invB, k, j, colsB);
                    }
                    set(C, i, j, colsB, sum);
                }
            }
            rowsB = colsB;
        }
        else {
            throw std::runtime_error("Unknown operation");
        }

        // Prepare the Java result array
        int outRows = rowsA;
        int outCols = (op < 2 ? colsA : rowsB);
        int outSize = outRows * outCols;
        jdoubleArray outArr = env->NewDoubleArray(outSize);
        env->SetDoubleArrayRegion(outArr, 0, outSize, &C[0]);

        // Release native arrays
        env->ReleaseDoubleArrayElements(aArr, aPtr, 0);
        env->ReleaseDoubleArrayElements(bArr, bPtr, 0);

        return outArr;
    }
    catch (const std::runtime_error& e) {
        // Throw IllegalArgumentException in Java
        jclass iae = env->FindClass("java/lang/IllegalArgumentException");
        env->ThrowNew(iae, e.what());
    }
    catch (...) {
        // Fallback for any other exception
        jclass re = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(re, "Native code error");
    }
    return nullptr;  // signal that an exception was thrown
}
