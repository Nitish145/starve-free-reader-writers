#include <pthread.h>
#include <semaphore>
#include <iostream>
#include <vector>
#include <thread>

// critical data to be shared
int data = 1;
// semaphore declarations
sem_t rw_queue, res_variable, read_mutex;
// counter for the reader
int reader_counter = 0;

bool reader(const int rid)
{
    // Entry section
    sem_wait(&rw_queue);
    sem_wait(&read_mutex);
    reader_counter++;
    if (reader_counter == 1)
        sem_wait(&res_variable);

    sem_post(&rw_queue);
    sem_post(&read_mutex);

    // Critical section
    std::cout << "Reader: " + std::to_string(rid) +" -> Value read as: " + std::to_string(data);

    // Exit section
    sem_wait(&read_mutex);
    reader_counter--;
    if (reader_counter == 0)
        sem_post(&res_variable);

    sem_post(&read_mutex);

    return true;
}

bool writer(const int wid)
{
    // Entry section
    sem_wait(&rw_queue);
    sem_wait(&res_variable);
    sem_post(&rw_queue);

    // Critical section
    data = data * 2;
    std::cout << "Writer: " + std::to_string(wid) +" -> Value updated to: " + std::to_string(data);

    // Exit section
    sem_post(&res_variable);

    return true;
}

using namespace std;
int main()
{
    int reader_count, writer_count, MAX_ID, i;
    cout << "Enter the number of Readers" << endl;
    cin >> reader_count;

    cout << "Enter the number of Writers" << endl;
    cin >> writer_count;

    MAX_ID = reader_count >= writer_count ? reader_count : writer_count;

    int ids[MAX_ID];
    for (i = 0; i < MAX_ID; i++)
        ids[i] = i + 1;

    // initialise semaphores
    sem_init(&rw_queue, 0, 1);
    sem_init(&read_mutex, 0, 1);
    sem_init(&res_variable, 0, 1);

    vector<thread> threads;

    for (i = 0; i < reader_count; i++)
        threads.emplace_back(thread(reader, ids[i]));

    for (i = 0; i < writer_count; i++)
        threads.emplace_back(thread(writer, ids[i]));

    for (i = 0; i < writer_count + reader_count; i++)
        threads[i].join();

    // destroying threads at the end of execution
    sem_destroy(&read_mutex);
    sem_destroy(&res_variable);
    sem_destroy(&rw_queue);

    return 0;
}
