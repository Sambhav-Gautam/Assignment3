// app/src/main/java/com/example/assignment3/MainActivity.kt
package com.example.assignment3

import android.os.Bundle
import android.widget.ArrayAdapter
import androidx.appcompat.app.AppCompatActivity
import com.example.assignment3.databinding.ActivityMainBinding
import kotlin.math.sqrt

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding

    companion object {
        init { System.loadLibrary("matrixcalc") }
    }

    // Updated to include both matrix shapes
    external fun processMatrices(
        opCode: Int,
        rowsA: Int, colsA: Int,
        rowsB: Int, colsB: Int,
        arrA: DoubleArray,
        arrB: DoubleArray
    ): DoubleArray

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar?.hide()
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val ops = listOf("Add", "Subtract", "Multiply", "Divide")
        binding.spinnerOp.adapter = ArrayAdapter(
            this, android.R.layout.simple_spinner_dropdown_item, ops
        )

        binding.btnCompute.setOnClickListener {
            try {
                val matA = parse2D(binding.etMatrixA.text.toString())
                val matB = parse2D(binding.etMatrixB.text.toString())

                // Flatten row-major
                val flatA = matA.flatten().toDoubleArray()
                val flatB = matB.flatten().toDoubleArray()

                val resultFlat = processMatrices(
                    binding.spinnerOp.selectedItemPosition,
                    matA.size, matA[0].size,
                    matB.size, matB[0].size,
                    flatA, flatB
                )

                // Infer result dims from opCode:
                val (rR, cR) = when (binding.spinnerOp.selectedItemPosition) {
                    0,1 -> matA.size to matA[0].size           // Add/Sub: same shape
                    2    -> matA.size to matB[0].size           // Multiply
                    3    -> matA.size to matB.size              // Divide: A·B⁻¹ (B square)
                    else -> 0 to 0
                }

                binding.tvResult.text = format2D(resultFlat, rR, cR)

            } catch (e: Exception) {
                binding.tvResult.text = "Error: ${e.message}"
            }
        }
    }

    // Parses lines separated by newlines or semicolons;
    // entries separated by spaces or commas.
    private fun parse2D(input: String): List<List<Double>> {
        val rows = input
            .trim()
            .split("[;\n]".toRegex())
            .map { it.trim() }
            .filter { it.isNotEmpty() }
            .map { row ->
                row.split("[,\\s]+".toRegex())
                    .filter { it.isNotBlank() }
                    .map { it.toDoubleOrNull()
                        ?: throw NumberFormatException("‘$it’ is not a number") }
            }

        require(rows.isNotEmpty()) { "Matrix cannot be empty" }
        val cols = rows[0].size
        require(cols > 0) { "Matrix must have at least one column" }
        // all rows same length
        require(rows.all { it.size == cols }) {
            "All rows must have the same number of columns"
        }
        return rows
    }

    private fun format2D(flat: DoubleArray, r: Int, c: Int): String {
        return flat.mapIndexed { i, v ->
            "%.2f".format(v) + if ((i+1)%c==0) "\n" else "\t"
        }.joinToString("")
    }
}
