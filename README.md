# Multithreaded Matrix Multiplier

## Compile

```shell
make
```

To enable debug statements, use

```shell
make debug
```

## Start `package`

```shell
./dist/package input_matrix_file_1 input_matrix_file_2 output_matrix_file seconds_between_thread_creation
```

To use the sample data, run

```shell
./dist/package sample-data/matrix1.dat sample-data/matrix2.dat output_matrix_file seconds_between_thread_creation
```

## Start `compute`

```shell
./dist/compute thread_pool_size [-n]
```
