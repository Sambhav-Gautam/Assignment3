# **Matrix Calculator Android App**

## **Overview**
This Android application performs matrix operations—**addition**, **subtraction**, **multiplication**, and **division**—using a native C++ library integrated via **JNI (Java Native Interface)**. Users can input two matrices, choose an operation, and view the result. **Matrix division** is implemented as multiplication by the inverse of the second matrix.

---

## **Features**

- **Matrix Input**  
  Users can input matrices in text format:  
  - Rows separated by **semicolons (;)** or **newlines**  
  - Elements separated by **spaces** or **commas**

- **Supported Operations**  
  - Addition  
  - Subtraction  
  - Multiplication  
  - Division (*A · B⁻¹*, where B is a square matrix)

- **Error Handling**  
  - Validates matrix dimensions  
  - Handles singular matrices during division

- **Native C++ Backend**  
  - Matrix operations are performed using C++ for efficiency  
  - Integrated via JNI

- **User Interface**  
  - Intuitive UI with input fields  
  - Operation selector (Spinner)  
  - Button for computation  
  - TextView for displaying the result

---

## **Project Structure**

### **C++ (Native Backend)**
`app/src/main/cpp/native-lib.cpp`  
Includes:
- Determinant calculation via recursive cofactor expansion
- Adjugate matrix computation
- Matrix addition, subtraction, multiplication, and division
- JNI interface for processing Java double arrays

### **Main Activity (Kotlin)**
`app/src/main/java/com/example/assignment3/MainActivity.kt`  
Handles:
- UI setup and event handling  
- Matrix input parsing  
- JNI function call  
- Result display

### **Layout (XML)**
`app/src/main/res/layout/activity_main.xml`  
Includes:
- `EditText` fields for matrix input  
- `Spinner` for selecting operations  
- `Button` to trigger computation  
- `TextView` for showing the result

---

## **Prerequisites**

- **Android Studio**: Version 4.0 or higher  
- **NDK**: Android Native Development Kit  
- **CMake**: For native library build  
- **Min SDK**: 21 (Android 5.0 Lollipop) or above

---

## **Setup Instructions**

### 1. Clone the Repository
```bash
git clone <repository-url>
```

### 2. Open in Android Studio
- Open the project in Android Studio  
- Ensure **NDK** and **CMake** are installed via **SDK Manager**

### 3. Configure Build
- Sync project with Gradle  
- Verify `native-lib` is included in `app/build.gradle`:
```groovy
externalNativeBuild {
    cmake {
        path "src/main/cpp/CMakeLists.txt"
    }
}
```

### 4. Build and Run
- Build the project to compile C++ code and generate the native library  
- Run the app on an emulator or physical device

---

## **Usage**

### 1. Input Matrices
- Enter Matrix A and Matrix B using text fields  
- **Format**:  
  - Rows separated by `;` or newlines  
  - Elements separated by spaces or commas  
  - Example (2×3 matrix):  
    ```
    1 2 3; 4 5 6
    ```

### 2. Select Operation
- Choose from **Add**, **Subtract**, **Multiply**, or **Divide**

### 3. Compute
- Tap the **"Compute"** button  
- Result is displayed as a matrix in the result `TextView`

### 4. Error Messages
- Errors (e.g., invalid dimensions, singular matrix) are shown in the result field

---

## **Matrix Operation Details**

| Operation      | Requirement                                                |
|----------------|-------------------------------------------------------------|
| Addition/Subtraction | Matrices must be of the same dimensions             |
| Multiplication        | Columns in A = Rows in B                            |
| Division              | B must be square and non-singular (det(B) ≠ 0)     |

- Inverse is calculated using **adjugate and determinant**

---

## **Limitations**

- **Matrix Size**: Limited by device memory and input format (practical limit ~100×100)  
- **Division**: B must be **square** and **non-singular**  
- **Input Validation**: Assumes numeric input  
- **Precision**: Uses `double`, with a small threshold (`1e-12`) for singularity checks

---

## **Example**

### **Input**
```
Matrix A: 1 2; 3 4  
Matrix B: 5 6; 7 8  
Operation: Multiply
```

### **Output**
```
19.00   22.00  
43.00   50.00
```

---

## **Troubleshooting**

- **JNI Errors**: Check that `System.loadLibrary("matrixcalc")` matches the native library name in `CMakeLists.txt`  
- **Build Issues**: Ensure NDK/CMake versions are compatible  
- **Input Errors**: Validate consistent column counts and numeric values in all rows

---

## **Future Improvements**

- Add operations like **transpose**, **eigenvalues**, etc.  
- Enhance input parsing flexibility  
- Optimize C++ code using **BLAS** or other libraries  
- Add **unit tests** for Kotlin and C++ components

---

## **License**
This project is licensed under the **MIT License**. See the `LICENSE` file for details.
