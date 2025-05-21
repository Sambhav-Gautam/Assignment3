# Matrix Calculator Android App


*Perform matrix operations with ease using native C++ power!*

---

## Overview

This Android app enables users to perform **matrix operations** (addition, subtraction, multiplication, and division) with a sleek interface and a high-performance **C++ backend** integrated via **JNI (Java Native Interface)**. Matrix division is computed as multiplication by the inverse of the second matrix.

---

## Table of Contents

- [Features](#features)
- [Demo Video](#demo-video)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Setup Instructions](#setup-instructions)
- [Usage](#usage)
- [Matrix Operations](#matrix-operations)
- [Limitations](#limitations)
- [Example](#example)
- [Troubleshooting](#troubleshooting)
- [Future Improvements](#future-improvements)
- [License](#license)

---

## Features

- **Flexible Matrix Input**  
  Input matrices using:  
  - Rows separated by **semicolons (;)** or **newlines**  
  - Elements separated by **spaces** or **commas**

- **Supported Operations**  
  - Addition  
  - Subtraction  
  - Multiplication  
  - Division (A · B⁻¹, where B is a square matrix)

- **Robust Error Handling**  
  - Validates matrix dimensions  
  - Detects singular matrices for division

- **High-Performance C++ Backend**  
  - Efficient matrix computations via native C++  
  - Seamless JNI integration

- **Intuitive UI**  
  - Input fields for matrices  
  - Spinner for operation selection  
  - Compute button and result display

---

## Demo Video

https://github.com/user-attachments/assets/5d637fe6-2fa2-4dba-a837-c4cba4f985c9

---

## Project Structure

```
MatrixCalculator
├── app
│   ├── src
│   │   ├── main
│   │   │   ├── cpp
│   │   │   │   └── native-lib.cpp    # C++ logic for matrix operations
│   │   │   ├── java
│   │   │   │   └── MainActivity.kt   # Kotlin UI and JNI handling
│   │   │   ├── res
│   │   │   │   └── layout
│   │   │   │       └── activity_main.xml  # UI layout
```

### C++ (Native Backend)
- **File**: `app/src/main/cpp/native-lib.cpp`  
- **Functionality**:  
  - Recursive determinant calculation  
  - Adjugate matrix computation  
  - Matrix addition, subtraction, multiplication, division  
  - JNI interface for Java double arrays

### Main Activity (Kotlin)
- **File**: `app/src/main/java/com/example/assignment3/MainActivity.kt`  
- **Functionality**:  
  - UI setup and event handling  
  - Matrix input parsing  
  - JNI function calls  
  - Result display

### Layout (XML)
- **File**: `app/src/main/res/layout/activity_main.xml`  
- **Components**:  
  - `EditText` for matrix input  
  - `Spinner` for operation selection  
  - `Button` to compute  
  - `TextView` for results

---

## Prerequisites

- **Android Studio**: Version 4.0+  
- **NDK**: Android Native Development Kit  
- **CMake**: For building native libraries  
- **Min SDK**: 21 (Android 5.0 Lollipop)

---

## Setup Instructions

1. **Clone the Repository**  
   ```bash
   git clone https://github.com/Sambhav-Gautam/Assignment3.git
   ```

2. **Open in Android Studio**  
   - Launch Android Studio and open the cloned project  
   - Install **NDK** and **CMake** via **SDK Manager**

3. **Configure Build**  
   - Sync project with Gradle  
   - Ensure `native-lib` is included in `app/build.gradle`:  
     ```groovy
     externalNativeBuild {
         cmake {
             path "src/main/cpp/CMakeLists.txt"
         }
     }
     ```

4. **Build and Run**  
   - Build the project to compile C++ code  
   - Run on an emulator or device

---

## Usage

1. **Input Matrices**  
   - Enter matrices in `EditText` fields  
   - **Format Example** (2×3 matrix):  
     ```
     1 2 3; 4 5 6
     ```

2. **Select Operation**  
   - Choose **Add**, **Subtract**, **Multiply**, or **Divide** from the spinner

3. **Compute Result**  
   - Press the **Compute** button  
   - View the result in the `TextView`

4. **Handle Errors**  
   - Errors (e.g., invalid dimensions, singular matrix) appear in the result field

---

## Matrix Operations

| Operation      | Requirement                                      |
|----------------|-------------------------------------------------|
| Addition       | Same dimensions                                 |
| Subtraction    | Same dimensions                                 |
| Multiplication | Columns of A = Rows of B                        |
| Division       | B must be square and non-singular (det(B) ≠ 0) |

*Note*: Division uses the inverse of B (A · B⁻¹), computed via adjugate and determinant.

---

## Limitations

- **Matrix Size**: Limited by device memory (~100×100 practical limit)  
- **Division**: Requires B to be square and non-singular  
- **Input**: Assumes numeric values  
- **Precision**: Uses `double` with a `1e-12` threshold for singularity

---

## Example

**Input**:  
```
Matrix A: 1 2; 3 4
Matrix B: 5 6; 7 8
Operation: Multiply
```

**Output**:  
```
19.00  22.00
43.00  50.00
```

---

## Troubleshooting

- **JNI Errors**: Ensure `System.loadLibrary("matrixcalc")` matches `CMakeLists.txt`  
- **Build Issues**: Verify NDK/CMake compatibility  
- **Input Errors**: Check for consistent column counts and numeric values

---

## Future Improvements

- Add **transpose**, **eigenvalues**, and other operations  
- Improve input parsing flexibility  
- Optimize C++ with **BLAS** libraries  
- Introduce **unit tests** for Kotlin and C++

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

```
       _          
      | |         
      | |__   ___ 
      | '_ \ / __|
      | | | | (__ 
      |_| |_|____|

Happy Matrix Computing!
```
