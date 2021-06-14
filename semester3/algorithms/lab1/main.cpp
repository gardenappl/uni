#include <iostream>
#include <fstream>
#include <limits>
#include <random>
#include <algorithm>
#include <string>
#include <queue>

using namespace std;

const int MAX_INTS_MEMORY = 100000;
const int FILE_COUNT = 4;
const int INPUT_SIZE = MAX_INTS_MEMORY * 10 + 5000;


void print_buffer(int* buffer, size_t size)
{
    cout << "{";
    for(size_t i = 0; i < size; i++)
    {
        cout << ' ' << buffer[i] << ' ';
    }
    cout << "}" << endl;
}

void read_buffer(int* buffer, size_t& size, fstream& file)
{
    file.read((char*)(&size), sizeof(size_t));
    file.read((char*)buffer, size * sizeof(int));
}

void write_buffer(int* buffer, size_t size, fstream& file)
{
    file.write((char*)(&size), sizeof(size_t));
    file.write((char*)buffer, size * sizeof(int));
    file.flush();
}

void generate_random_sequence(int* buffer, size_t length)
{
    static default_random_engine generator;
    //static uniform_int_distribution<int> distribution(0, numeric_limits<int>::max());
    static uniform_int_distribution<int> distribution(0, 10000);

    for(size_t i = 0; i < length; i++)
    {
        buffer[i] = distribution(generator);
    }
}

void generate_input_file(fstream& file)
{
    int* buffer = new int[MAX_INTS_MEMORY];
    for(int i = 0; i < INPUT_SIZE / MAX_INTS_MEMORY + (INPUT_SIZE % MAX_INTS_MEMORY != 0); i++)
    {
        int size = min(MAX_INTS_MEMORY, INPUT_SIZE - MAX_INTS_MEMORY * i);
        generate_random_sequence(buffer, size);
        file.write((char*)buffer, size * sizeof(int));
    }
    delete buffer;
}

//this merge_sort is for internal sorting only!

void merge(int* result_array, size_t start, size_t end, int* array1, size_t start1, size_t end1,
           int* array2, size_t start2, size_t end2)
{
//    cout << "Merging ";
//    print_buffer(array1 + start1, end1 - start1);
//    cout << "with ";
//    print_buffer(array2 + start2, end2 - start2);
    size_t i = start;
    size_t i1 = start1;
    size_t i2 = start2;

    while(i < end)
    {
        if(i1 != end1 && (i2 == end2 || array1[i1] < array2[i2]))
        {
            result_array[i] = array1[i1];
            i1++;
        }
        else
        {
            result_array[i] = array2[i2];
            i2++;
        }
        i++;
    }
}

void merge_sort(int* array, int* extra_array, size_t size)
{
    int* original_array_ptr = array;
    for(size_t merge_size = 1; merge_size < size; merge_size *= 2)
    {
        for(size_t i = 0; i < size; i += 2 * merge_size)
        {
            int middle = min(i + merge_size, size);
            int end = min(i + 2 * merge_size, size);
            merge(extra_array, i, end, array, i, middle, array, middle, end);
        }
        swap(array, extra_array);
//        print_buffer(array, size);
    }
    if(array != original_array_ptr)
    {
        copy(array, array + size, original_array_ptr);
    }
}

void check_is_sorted(int* buffer, size_t size)
{
    for(size_t i = 1; i < size; i++)
        if(buffer[i - 1] > buffer[i])
            throw std::logic_error("Array not sorted! " + to_string(buffer[i - 1]) + " > " + to_string(buffer[i]));
    cout << "Array is sorted!" << endl;
}


void polyphase_merge_get_runs(int*& run_distribution, int& buffer_size, int& output_file_num,
                              int input_size, int file_count, int max_memory, int& merges_count)
{
    merges_count = 0;
    run_distribution = new int[file_count];
    for(int i = 0; i < file_count; i++)
        run_distribution[i] = 0;
    run_distribution[0] = 1;
    int prev_output_file_num = 0;

    int total_runs;
    while(true)
    {
        int runs_to_merge = run_distribution[prev_output_file_num];
        for(int i = 0; i < file_count; i++)
        {
            if(i == prev_output_file_num)
                run_distribution[i] = 0;
            else
                run_distribution[i] += runs_to_merge;
        }
        prev_output_file_num = (prev_output_file_num + 1) % file_count;

        total_runs = 0;
        for(int i = 0; i < file_count; i++)
            total_runs += run_distribution[i];

        merges_count++;

        if(input_size < total_runs * max_memory)
            break;
    }
    buffer_size = input_size / total_runs + (input_size % total_runs != 0); //divide integers, round up

    output_file_num = (prev_output_file_num + file_count - 1) % file_count;
}

void insertion_sort_min_array(int* array, size_t size, int** buffers, size_t* buffer_positions)
{
    for(size_t i = 1; i < size; i++)
    {
        int buffer_index = array[i];
        int element = buffers[array[i]][buffer_positions[array[i]]];
        int j = i - 1;
        while(j >= 0 && buffers[array[j]][buffer_positions[array[j]]] > element)
        {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = buffer_index;
    }
}

void polyphase_merge_sort_do_merge(int* run_distribution, int output_file_num, int file_count,
                                   fstream* file_streams, int** buffers, size_t* buffer_sizes, size_t max_buffer_size)
{
    int runs_to_do = run_distribution[(output_file_num + file_count - 1) % file_count];

    cout << "to do: " << runs_to_do << endl;

    file_streams[output_file_num].close();
    file_streams[output_file_num].open("file_" + to_string(output_file_num) + ".bin", fstream::binary | fstream::out);

    for(int run_num = 0; run_num < runs_to_do; run_num++)
    {
        cout << "RUN NUMBER " << run_num << endl;
        //array of indexes of buffers with smallest numbers, sorted
        int* min_array = new int[file_count - 1];
        int min_array_end = 0;

        //our position inside the input buffer
        size_t* buffer_positions = new size_t[file_count];
        //how many buffers have we read?
        int* run_positions = new int[file_count];
        //how many buffers inside a run?
        int* run_sizes = new int[file_count];
        int output_run_size = 0;

        for(int i = 0; i < file_count; i++)
        {
            if(i != output_file_num)
            {
                cout << "Reading from file " << to_string(i) << endl;
                run_positions[i] = 0;

                char run_size;
                file_streams[i].read(&run_size, 1);
                cout << "Run size: " << (int)run_size << endl;
                run_sizes[i] = run_size;

                output_run_size += run_sizes[i];
                read_buffer(buffers[i], buffer_sizes[i], file_streams[i]);
                cout << "Buffer size: " << buffer_sizes[i] << endl;

                min_array[min_array_end] = i;
                min_array_end++;
                cout << "Min array: " << endl;
                print_buffer(min_array, min_array_end);

            }
            buffer_positions[i] = 0;

        }

        file_streams[output_file_num] << (char)output_run_size;
        cout << "Output run size: " << output_run_size << endl;
        file_streams[output_file_num].flush();

        //writing
        while(true)
        {
            insertion_sort_min_array(min_array, min_array_end, buffers, buffer_positions);
            print_buffer(min_array, min_array_end);
            int min_buffer_index = min_array[0];

            cout << "Minimal buffer: " << min_buffer_index << endl;
            cout << "Minimal buffer position: " << buffer_positions[min_buffer_index] << endl;

            //if input buffer is full
            if(buffer_positions[output_file_num] == max_buffer_size)
            {
                cout << "Input buffer is full" << endl;
                cout << max_buffer_size << endl;
                cout << output_file_num << endl;
                write_buffer(buffers[output_file_num], max_buffer_size, file_streams[output_file_num]);
                buffer_positions[output_file_num] = 0;
            }

            int min_element = buffers[min_buffer_index][buffer_positions[min_buffer_index]];
            buffers[output_file_num][buffer_positions[output_file_num]] = min_element;
            buffer_positions[output_file_num]++;
            buffer_positions[min_buffer_index]++;

            //if output buffer depleted
            if(buffer_positions[min_buffer_index] == buffer_sizes[min_buffer_index])
            {
                cout << "Output buffer depleted" << endl;
                run_positions[min_buffer_index]++;
                if(run_positions[min_buffer_index] == run_sizes[min_buffer_index])
                {
                    cout << "Output file depleted" << endl;

                    //get rid of buffer
                    min_array_end--;

                    if(min_array_end == 0)
                        break; //we're done with the run

                    swap(min_array[0], min_array[min_array_end]);
                }
                else
                {
                    //refill buffer
                    read_buffer(buffers[min_buffer_index], buffer_sizes[min_buffer_index], file_streams[min_buffer_index]);
                    buffer_positions[min_buffer_index] = 0;
                }
            }
        }

        cout << "Run done!" << endl;

        delete run_positions;
        delete run_sizes;
        delete min_array;
        delete buffer_positions;
    }
}

void polyphase_merge_sort(fstream& input_file, int input_size, int file_count, size_t max_buffer_size)
{
    int buffer_size;
    int* run_distribution;
    int output_file_num;
    int merges_count;
    polyphase_merge_get_runs(run_distribution, buffer_size, output_file_num, input_size, file_count, max_buffer_size, merges_count);

    print_buffer(run_distribution, file_count);

    int total_runs = 0;
    for(int i = 0; i < file_count; i++)
        total_runs += run_distribution[i];
    cout << "Must perform " << merges_count << " merge steps.\n";
    cout << "Buffer size: " << buffer_size << '\n';
    cout << "(" << buffer_size << " * " << total_runs << " = " << buffer_size * total_runs << " >= " << input_size << ")\n";
    cout << "Output file num: " << output_file_num << '\n';

    int** buffers = new int*[file_count];
    size_t* buffer_sizes = new size_t[file_count];
    for(int i = 0; i < file_count; i++)
    {
        buffers[i] = new int[buffer_size];
        buffer_sizes[i] = 0;
    }

    //Initial split
    fstream* file_streams = new fstream[file_count];
    for(int i = 0; i < file_count; i++)
    {
        file_streams[i].open("file_" + to_string(i) + ".bin", fstream::binary | fstream::out);
        for(int run_num = 0; run_num < run_distribution[i]; run_num++)
        {
            std::cout << "Reading " << buffer_size << " integers...\n";
            input_file.read((char*)buffers[0], buffer_size * sizeof(int));
            int read_count = input_file.gcount() / sizeof(int);
            std::cout << "Read " << read_count << " integers.\n";

            merge_sort(buffers[0], buffers[1], read_count);
            check_is_sorted(buffers[0], read_count);

            //Initial run size = 1 buffer
            file_streams[i] << (char)1;
            write_buffer(buffers[0], read_count, file_streams[i]);
            cout << "Wrote to file " << to_string(i) << endl;
        }
        file_streams[i].close();
    }

    for(int i = 0; i < file_count; i++)
        file_streams[i].open("file_" + to_string(i) + ".bin", fstream::binary | fstream::in);


    for(int merge_num = 0; merge_num < merges_count; merge_num++)
    {
        polyphase_merge_sort_do_merge(run_distribution, output_file_num, file_count, file_streams,
                                      buffers, buffer_sizes, buffer_size);

        for(int i = 0; i < file_count; i++)
        {
            if(i == output_file_num)
                run_distribution[i] += run_distribution[(output_file_num + file_count - 1) % file_count];
            else
                run_distribution[i] -= run_distribution[(output_file_num + file_count - 1) % file_count];
        }

        file_streams[output_file_num].close();
        file_streams[output_file_num].open("file_" + to_string(output_file_num) + ".bin", fstream::binary | fstream::in);
        output_file_num = (output_file_num + file_count - 1) % file_count;
    }


    for(int i = 0; i < file_count; i++)
        file_streams[i].close();

    for(int i = 0; i < file_count; i++)
        delete buffers[i];
    delete buffers;
    delete buffer_sizes;
}

int main()
{
    size_t size = 100;
    int* buffer = new int[size];
    generate_random_sequence(buffer, size);
//    print_buffer(buffer, size);

    int* extra_buffer = new int[size];
    merge_sort(buffer, extra_buffer, size);
    delete extra_buffer;
    print_buffer(buffer, size);
    check_is_sorted(buffer, size);


    fstream file;

    file.open("test.bin", fstream::binary | fstream::out);

    int num = 70;
    file.write(reinterpret_cast<char*>(&num), sizeof(num));
    print_buffer(buffer, size);
    write_buffer(buffer, size, file);

    buffer[0] = 69;
    size = 2;

    file.close();
    file.open("test.bin", fstream::binary | fstream::in);
    int random;
    file.read(reinterpret_cast<char*>(&random), sizeof(num));
    cout << random << endl;
    read_buffer(buffer, size, file);
    cout << "\nSize: " << size << endl;
    print_buffer(buffer, size);

    file.close();
    delete buffer;

    fstream input_file("input.bin", fstream::binary | fstream::out);
    cout << "generating " << INPUT_SIZE << " elements...\n";
    generate_input_file(input_file);

    input_file.close();
    input_file.open("input.bin", fstream::binary | fstream::in);
    polyphase_merge_sort(input_file, INPUT_SIZE, FILE_COUNT, MAX_INTS_MEMORY);

    input_file.close();

    return 0;
}

