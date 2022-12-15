import random
import subprocess
import matplotlib.pyplot as plt

CPP_EXEC = '../../../cmake-build/lab_02_execution_time'

RES_FILE = '/Users/yaroslavbairamgalin/Desktop/Studies/BMSTU/5 term/aa/' \
           'aa_labs/lab_02/time_results.tmp.txt'

TEST_FILE = '/Users/yaroslavbairamgalin/Desktop/Studies/BMSTU/5 term/aa/' \
            'aa_labs/lab_02/time_test_data.tmp.txt'

SIZES = [
    (10, 10),
    (50, 50),
    (100, 100),
    (100, 100),
    (200, 200),
    (300, 300),
    (400, 400),
    (500, 500),
]


def main():
    generate_test_data()
    if run() != 0:
        return 1
    graph()


def generate_test_data():
    print('Generation of random data started...')

    test_file = open(TEST_FILE, 'w')
    test_file.write(f'{len(SIZES)}\n')
    for index, size in enumerate(SIZES):
        first_matrix = generate_matrix(size)
        write_matrix_to_file(test_file, first_matrix)
        second_matrix = generate_matrix(size)
        write_matrix_to_file(test_file, second_matrix)
        print(f'DONE {index+1}/{len(SIZES)}')
    print('Generation of random data completed')


def generate_matrix(size):
    matrix = [[str(random.randint(-100, 100)) for _ in range(size[1])]
              for _ in range(size[0])]
    return matrix


def write_matrix_to_file(test_file, matrix):
    size = (len(matrix), len(matrix[0]))
    test_file.write(f'{size[0]} {size[1]} ')
    for row in matrix:
        test_file.write(f'{" ".join(list(map(str, row)))}\n')


def graph():
    result_file = open(RES_FILE, 'r')
    result = result_file.readlines()
    result = list(map(int, result))

    res_simple = result[::3]
    res_vinograd = result[1::3]
    res_vinograd_optimized = result[2::3]

    print(res_simple)
    print(res_vinograd)
    print(res_vinograd_optimized)
    sz = [s[0] for s in SIZES]

    plt.plot(sz, res_simple, label='simple')
    plt.plot(sz, res_vinograd, label='Vinograd')
    plt.plot(sz, res_vinograd_optimized, label='Vinograd optimized')
    plt.legend()
    plt.grid()
    plt.title("Временные характеристики алгоритмов умножения матриц")
    plt.ylabel("Затраченное время, с")
    plt.xlabel("Рзамер матрицы, элемент.")
    plt.show()


def run():
    print('Run subprocess to measure time...')
    execution = subprocess.run(
        [f'{CPP_EXEC} \'{TEST_FILE}\' \'{RES_FILE}\''],
        capture_output=True,
        shell=True,
    )
    print(f'Subprocess ended with code {execution.returncode}')
    if execution.returncode != 0:
        print(execution.stdout)
        print(execution.stderr)

    return execution.returncode


if __name__ == '__main__':
    exit(main())
