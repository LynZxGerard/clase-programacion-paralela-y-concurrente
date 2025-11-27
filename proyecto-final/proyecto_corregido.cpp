// proyecto_corregido.cpp
// Compilar: g++ -std=c++17 -O3 -pthread -o proyecto_corregido proyecto_corregido.cpp
#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <atomic>

using namespace std;

const size_t ELEMENTS = 20'000'000;
const int DETECTED_THREADS = thread::hardware_concurrency() ? thread::hardware_concurrency() : 4;

double *work_buffer = nullptr;
double *local_results = nullptr;
atomic<int> metric_counter(0);
atomic<bool> done_flag(false);

const int RING_SZ = 8;
double ring[RING_SZ];
atomic<int> w_ptr(0);
atomic<int> r_ptr(0);

mutex ring_mutex;
condition_variable ring_cv;

void produce_items(int items);
void consume_items(int id);
void compute_chunk(int id);
void try_set_affinity(int id);

double now_seconds()
{
    using namespace std::chrono;
    return duration_cast<duration<double>>(high_resolution_clock::now().time_since_epoch()).count();
}

int main(int argc, char **argv)
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    work_buffer = new double[ELEMENTS];
    local_results = new double[DETECTED_THREADS];

    for (size_t i = 0; i < ELEMENTS; ++i)
    {
        work_buffer[i] = double(i % 100) * 0.001;
    }

    cout << "CPUs (detected): " << DETECTED_THREADS << "\n";
    cout << "Elements = " << ELEMENTS << "\n";

    thread producer([&]()
                    { produce_items(1000); });

    vector<thread> consumers;
    for (int i = 0; i < DETECTED_THREADS; ++i)
    {
        consumers.emplace_back(consume_items, i);
    }

    vector<thread> workers;
    for (int i = 0; i < DETECTED_THREADS; ++i)
    {
        workers.emplace_back(compute_chunk, i);
    }

    for (int i = 0; i < (int)workers.size(); ++i)
    {
        try_set_affinity(i);
    }

    this_thread::sleep_for(chrono::seconds(1));

    producer.join();
    for (auto &t : consumers)
        if (t.joinable())
            t.join();
    for (auto &t : workers)
        if (t.joinable())
            t.join();

    double total = 0.0;
    for (int i = 0; i < (int)(DETECTED_THREADS); ++i)
    {
        total += local_results[i];
    }

    cout << "Metric counter = " << metric_counter.load() << "\n";
    cout << "Total sum: " << total << "\n";

    delete[] work_buffer;
    delete[] local_results;

    return 0;
}

void produce_items(int items)
{
    for (int k = 0; k < items; ++k)
    {
        double v = double(k % 100) * 0.002;

        unique_lock<mutex> lock(ring_mutex);

        // Esperar hasta que haya espacio en el ring
        ring_cv.wait(lock, [&]() {
            return ((w_ptr.load() + 1) % RING_SZ) != r_ptr.load();
        });

        ring[w_ptr] = v;
        w_ptr = (w_ptr + 1) % RING_SZ;

        ring_cv.notify_all();
    }
    done_flag = true;
    ring_cv.notify_all();
}

void consume_items(int id)
{
    while (true)
    {
        unique_lock<mutex> lock(ring_mutex);

        // Si está vacío y el productor terminó, salir
        if (r_ptr == w_ptr && done_flag)
        {
            break;
        }

        // Si está vacío pero el productor aún produce, esperar
        if (r_ptr == w_ptr)
        {
            ring_cv.wait_for(lock, chrono::milliseconds(1));
            continue;
        }

        double v = ring[r_ptr];
        r_ptr = (r_ptr + 1) % RING_SZ;

        ring_cv.notify_all();

        // Hacer el cálculo FUERA del lock para no bloquear a otros
        lock.unlock();

        double out = v * 2.0; // "calculo" del proceso consumido del anillo
        (void)out;
        metric_counter++; // Arriba se hizo atomica
    }
}

void compute_chunk(int id)
{
    int workers = DETECTED_THREADS; // ya no se multiplican por 4
    size_t chunk = ELEMENTS / workers;
    size_t start = id * chunk;
    size_t end = (id == workers - 1) ? ELEMENTS : start + chunk;

    if (id % 3 == 0)
        this_thread::sleep_for(chrono::milliseconds(10));

    double sum = 0.0;
    for (size_t i = start; i < end; ++i)
    {
        sum += work_buffer[i];
        if ((i & 0x3FF) == 0)
            metric_counter++;
    }
    local_results[id] = sum;

    // Aqui habia un for que no hacia absolutamente nada
}

void try_set_affinity(int id)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    int cpu = id % (DETECTED_THREADS); // Ya no se crean mas hilos de los que maneja la pc
    CPU_SET(cpu, &cpuset);
    int rc = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    if (rc != 0)
    {
    }
}