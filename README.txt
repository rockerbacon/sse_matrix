All the code presented was implemented for a college work at Universidade Federal Rural do Rio de Janeiro.
The objective was to provide optimizations for the naive matrix multiplication and matrix identity check by using SSE instructions.
An additional matrix transposition optimization was developed during the proccess and all logic developed can be applied in an AVX implementation.
Both optimized implementations and naive implementations are provided for comparison.

Multiplication:

For the multiplication, given matrix A and matrix B, matrix B is transposed and the multiplication is done row x row instead of row x colum in order to optimize cache utilization.
For each index in the result matrix, a multiplication buffer is used to first store all the multiplication results and then the summation of this buffer is stored in the index.

Transposition:

For the transposition the matrix is sliced in 4x4 submatrices and all submatrices are individually transposed, then for every submatrix at index [i][j] where j > i the submatrix is swapped with the submatrix at [j][i].

Identity check:

For the identity check the matrix is sliced in 4x4 submatrices and for every submatrix at index [i][j] if i != j then all the submatrix values must be 0, otherwise the submatrix must be a 4x4 identity matrix.