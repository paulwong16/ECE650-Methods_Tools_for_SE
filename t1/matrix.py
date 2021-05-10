# Name: Zhijie Wang
# Student ID: 20856733
# Date: 2020/10/7
# Test codes for each sub question were contained in main function, writing questions were written as comments.


import sys


class Matrix:
    def __init__(self):
        self.m = []
        self.columns = 0
        self.rows = 0

    def get_columns(self):
        return self.columns

    def get_rows(self):
        return self.rows

    def get_value(self, i, j):
        return self.m[i][j]


def print_matrix(matrix):
    print(matrix.rows)
    for i in range(matrix.rows):
        print('| ' + str(matrix.m[i])[1:-1] + ' |')


def parse_matrix(inputs):
    return_matrix = Matrix()
    inputs_split = inputs.split('\n')
    return_matrix.rows = int(inputs_split[0])
    return_matrix.columns = len(inputs_split[1].strip('|').split(','))
    for i in range(return_matrix.rows):
        row = inputs_split[i+1].strip('|').split(',')
        row_list = []
        for j in range(return_matrix.columns):
            row_list.append(int(row[j]))
        return_matrix.m.append(row_list)
    return return_matrix


def matrix_mul(A, B):
    m, n = A.rows, B.columns
    C = [[0] * n for _ in range(m)]
    if A.columns != B.rows:
        raise Exception('multiplication between A and B is not possible')
    for i in range(m):
        for j in range(n):
            for k in range(A.columns):
                C[i][j] = C[i][j] + A.m[i][k]*B.m[k][j]

    return_matrix = Matrix()
    return_matrix.rows, return_matrix.columns, return_matrix.m = m, n, C
    return return_matrix


class SparseMatrix:
    def __init__(self):
        self.m = {}
        self.columns = 0
        self.rows = 0

    def get_columns(self):
        return self.columns

    def get_rows(self):
        return self.rows

    def get_value(self, i, j):
        if i in self.m and i < self.rows:
            if j in self.m[i] and j < self.columns:
                return self.m[i][j]
            else:
                return 0
        else:
            return 0


def sparse(M):
    s_m = SparseMatrix()
    s_m.rows, s_m.columns = M.rows, M.columns
    for i in range(M.rows):
        for j in range(M.columns):
            if M.m[i][j] != 0:
                if i not in s_m.m:
                    s_m.m[i] = {}
                s_m.m[i][j] = M.m[i][j]
    return s_m


def print_sparse_matrix(matrix):
    for i in matrix.m:
        for j in matrix.m[i]:
            print('({0},{1},{2})'.format(i, j, matrix.m[i][j]))


def sparse_matrix_mul(A, B):
    m, n = A.rows, B.columns
    if A.columns != B.rows:
        raise Exception('multiplication between A and B is not possible')
    return_matrix = Matrix()
    return_matrix.rows, return_matrix.columns, return_matrix.m = m, n, {}
    B_T = {}
    for i in B.m:
        for j in B.m[i]:
            if j not in B_T:
                B_T[j] = {}
            B_T[j][i] = B.m[i][j]
    for i in A.m:
        for j in B_T:
            tmp = 0
            for k in range(A.columns):
                if k in A.m[i] and k in B_T[j]:
                    tmp += A.m[i][k] * B.m[k][j]
            if tmp != 0:
                if i not in return_matrix.m:
                    return_matrix.m[i] = {}
                return_matrix.m[i][j] = tmp
    return return_matrix


if __name__ == '__main__':
    # Question A
    print('\n-----Question A-----\n')
    print('Create a matrix [[1,2,4], [4,5,8], [7,8,9]]')
    Q_a = Matrix()
    Q_a.m = [[1,2,4], [4,5,8], [7,8,9]]
    Q_a.rows = 3
    Q_a.columns = 3
    # Test access number of columns
    print('Test access number of columns')
    print(Q_a.get_columns())
    # Test access number of rows
    print('Test access number of rows')
    print(Q_a.get_rows())
    # Test access specific value
    print('Test access specific value (1,1)')
    print(Q_a.get_value(1, 1))

    # Question B
    print('\n-----Question B-----\n')
    print('Test printing a matrix [[1, 2, 4], [4, 5, 8], [7, 8, 9]]')
    Q_b = Matrix()
    Q_b.m = [[1, 2, 4], [4, 5, 8], [7, 8, 9]]
    Q_b.rows = 3
    Q_b.columns = 3
    print_matrix(Q_b)

    # Question C
    print('\n-----Question C-----\n')
    print('Test inputs: \n 3\n| 1, 2, 4 |\n| 4, 3, 8 |\n| 7, 3, 9 |')
    test_inputs = '3\n| 1, 2, 4 |\n| 4, 3, 8 |\n| 7, 3, 9 |'
    Q_c = parse_matrix(test_inputs)
    print('Print the matrix after parsing')
    print_matrix(Q_c)

    # Question D
    print('\n-----Question D-----\n')
    print('Test multiply two matrix [[1, 2, 3], [4, 5, 6], [7, 8, 9]] and [[1, 2, 4], [4, 5, 8], [7, 8, 9]]')
    A = Matrix()
    A.m = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
    A.rows = 3
    A.columns = 3
    B = Matrix()
    B.m = [[1, 2, 4], [4, 5, 8], [7, 8, 9]]
    B.rows = 3
    B.columns = 3
    try:
        print_matrix(matrix_mul(A, B))
    except Exception as e:
        print('Error: ' + str(e), file=sys.stderr)

    # Question E
    print('\n-----Question E-----\n')
    print('Create a matrix (0,2,3) (0,4,4) (1,2,5) (3,1,2) (3,2,6)')
    Q_e = SparseMatrix()
    Q_e.rows, Q_e.columns = 4, 5
    Q_e.m = {0: {2: 3, 4: 4}, 1: {2: 5}, 3: {1: 2, 2: 6}}
    # Test access number of columns
    print('Test access number of columns')
    print(Q_e.get_columns())
    # Test access number of rows
    print('Test access number of rows')
    print(Q_e.get_rows())
    # Test access specific value
    print('Test access specific value (0,2)')
    print(Q_e.get_value(0, 2))
    print('Test access specific value (1,3)')
    print(Q_e.get_value(1, 3))

    # Question F
    print('\n-----Question F-----\n')
    print('Test transfering a matrix [[0, 0, 3, 0, 4], [0, 0, 5, 0, 0], [0, 0, 0, 0, 0], [0, 2, 6, 0, 0]]')
    M = Matrix()
    M.rows, M.columns = 4, 5
    M.m = [[0, 0, 3, 0, 4], [0, 0, 5, 0, 0], [0, 0, 0, 0, 0], [0, 2, 6, 0, 0]]
    SM = sparse(M)
    print_sparse_matrix(SM)

    # Question G
    # Ans: sparse(M) is more efficient than M only when M contains any non-zero values.
    print('\n-----Question G-----\n')
    print('Ans: sparse(M) is more efficient than M only when M contains any non-zero values.')

    # Question H
    # Ans: No. I used different data structure (a list for Matrix and a dictionary for SparseMatrix) to store the matrix.
    #      Therefore, matrix_mul cannot be used directly.
    print('\n-----Question H-----\n')
    print('Ans: No. I used different data structure (a list for Matrix and a dictionary for SparseMatrix) '
          'to store the matrix. Therefore, matrix_mul cannot be used directly.')

    # Question I
    print('\n-----Question I-----\n')
    print('Test multiply two matrix (0,2,3) (0,4,4) (1,2,5) (3,1,2) (3,2,6) and '
          '(0,0,2) (0,2,3) (1,0,8) (1,2,5) (2,1,3) (3,1,2) (3,2,6) (4,3,1)')
    A = SparseMatrix()
    A.rows, A.columns = 4, 5
    A.m = {0: {2: 3, 4: 4}, 1: {2: 5}, 3: {1: 2, 2: 6}}
    B = SparseMatrix()
    B.rows, B.columns = 5, 4
    B.m = {0: {0: 2, 2: 3}, 1: {0: 8, 2: 5}, 2: {1: 3}, 3: {1: 2, 2: 6}, 4: {3: 1}}
    try:
        print_sparse_matrix(sparse_matrix_mul(A, B))
    except Exception as e:
        print('Error: ' + str(e), file=sys.stderr)
