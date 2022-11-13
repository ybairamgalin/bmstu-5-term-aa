import matplotlib.pyplot as plot
import matplotlib.pyplot as plt

MARKER = 'o-'


def main():
    radix_times = []
    beads_times = []
    binary_tree_times = []

    sort_time_results_file = open('sort_time_result.txt')
    array_lengths = list(map(int, sort_time_results_file.readline().split()))

    for _ in range(3):
        radix_times.append(
            list(map(int, sort_time_results_file.readline().split())))
        beads_times.append(
            list(map(int, sort_time_results_file.readline().split())))
        binary_tree_times.append(
            list(map(int, sort_time_results_file.readline().split())))

    figure_sorted = plot.figure()
    plot_sorted = figure_sorted.add_subplot()
    plot_sorted.plot(array_lengths, radix_times[0], MARKER, label='radix sort')
    plot_sorted.plot(array_lengths, beads_times[0], MARKER, label='beads sort')
    plot_sorted.plot(
        array_lengths,
        binary_tree_times[0],
        MARKER,
        label='binary tree sort'
    )
    plot.legend()
    plot.grid()
    plot.title("Временные характеристики алгоритмов сортировок")
    plot.ylabel("Затраченное время, 10*с")
    plot.xlabel("Длина, ед.")

    figure_reversed = plot.figure()
    plot_reversed = figure_reversed.add_subplot()
    plot_reversed.plot(
        array_lengths,
        radix_times[1],
        MARKER,
        label='radix sort'
    )
    plot_reversed.plot(
        array_lengths,
        beads_times[1],
        MARKER,
        label='beads sort'
    )
    plot_reversed.plot(
        array_lengths, binary_tree_times[1], MARKER, label='binary tree sort')
    plot.legend()
    plot.grid()
    plot.title("Временные характеристики алгоритмов сортировок")
    plot.ylabel("Затраченное время, 10*с")
    plot.xlabel("Длина, ед.")

    figure_random = plot.figure()
    plot_random = figure_random.add_subplot()
    plot_random.plot(array_lengths, radix_times[2], MARKER, label='radix sort')
    plot_random.plot(array_lengths, beads_times[2], MARKER, label='beads sort')
    plot_random.plot(
        array_lengths,
        binary_tree_times[2],
        MARKER,
        label='binary tree sort'
    )
    plot.legend()
    plot.grid()
    plot.title("Временные характеристики алгоритмов сортировок")
    plot.ylabel("Затраченное время, 10*с")
    plot.xlabel("Длина, ед.")

    generate_latex_table(
        array_lengths,
        binary_tree_times,
        radix_times,
        beads_times,
    )

    for index, figure in enumerate(
            [figure_sorted, figure_reversed, figure_random]):
        figure.set_size_inches(12, 7)
        figure.savefig(f'../../docs/inc/img/sort_time_graph_{index}', dpi=300)


def generate_latex_table(lengths, binary_tree, radix, beads):
    output_file = open('letex_sort_time_tables.txt', 'w')

    output_file.write(
        'Размер & Бинарным деревом &  Побитовая &  Бусинами \\\\\n')
    for length, bt, r, b in zip(lengths, binary_tree[0], radix[0], beads[0]):
        output_file.write('\\hline\n')
        output_file.write(f'{length} & {bt} & {r} & {b}\\\\\n')

    output_file.write(
        'Размер & Бинарным деревом &  Побитовая &  Бусинами \\\\\n')
    for length, bt, r, b in zip(lengths, binary_tree[1], radix[1], beads[1]):
        output_file.write('\\hline\n')
        output_file.write(f'{length} & {bt} & {r} & {b}\\\\\n')

    output_file.write(
        'Размер & Бинарным деревом &  Побитовая &  Бусинами \\\\\n')
    for length, bt, r, b in zip(lengths, binary_tree[1], radix[1], beads[1]):
        output_file.write('\\hline\n')
        output_file.write(f'{length} & {bt} & {r} & {b}\\\\\n')


if __name__ == '__main__':
    main()
