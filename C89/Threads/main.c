#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double x;
    double y;
} vector_t;

typedef struct {
    double mass;
    vector_t position;
    vector_t velocity;
    vector_t acceleration;
} particle_t;

typedef struct {
    uint32_t index_start;
    uint32_t index_finish;
} thread_info_t;

//================================================================================

// Utility functions
void read_file(char* const filename);
vector_t add_vector(vector_t, vector_t);
vector_t substract_vector(vector_t, vector_t);
vector_t scale_vector(double, vector_t);
double mod(vector_t);

// Simulation functions
void* simulate(void* thread_info);
void computeAcceleration(uint32_t index);
void computeVelocity(uint32_t index);
void computePosition(uint32_t index);
void resolveCollisions();

// Constants
static double delta_time = 0.05;
static double gravity = 0.0;
static uint32_t steps = NULL;
static uint32_t particles_count = NULL;
static particle_t* particles = NULL;
static uint8_t thread_count;
static pthread_cond_t cv;
static pthread_mutex_t mutex;
static uint32_t cntr = 0;

//================================================================================

int main(const int argc, char* const argv[]) {
    // Parameters
    if (argc != 4) {
        printf("Usage: %s <input_filename> <output_filename> <thread_count>\n", argv[0]);
        return -1;
    }
    char* input_filename = argv[1];
    char* output_filename = argv[2];
    thread_count = atoi(argv[3]);

    read_file(input_filename);

    pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));
    thread_info_t* thread_args = malloc(thread_count * sizeof(thread_info_t));
    pthread_cond_init(&cv, NULL);
    pthread_mutex_init(&mutex, NULL);

    uint32_t chunk_size = particles_count / thread_count;
    for (int thread = 0; thread < thread_count; ++thread) {
        thread_args[thread].index_start = chunk_size * thread;
        thread_args[thread].index_finish = chunk_size * (thread + 1);
    }
    thread_args[thread_count - 1].index_finish = particles_count;

    FILE* file = fopen(output_filename, "w");
    fprintf(file, "t,");
    for (int i = 0; i < particles_count; ++i) {
        fprintf(file, "x%d,y%d,", i, i);
    }
    fprintf(file, "\n");

    for (int step = 0; step < steps; ++step) {
        for (int thread = 0; thread < thread_count; ++thread)
            pthread_create(&thread_handles[thread], NULL, &simulate, &thread_args[thread]);

        for (int thread = 0; thread < thread_count; ++thread) {
            pthread_join(thread_handles[thread], NULL);
        }
        cntr = 0;

        resolveCollisions();

        fprintf(file, "%d,", step);
        for (int i = 0; i < particles_count; ++i) {
            fprintf(file, "%lf,%lf,", particles[i].position.x, particles[i].position.y);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    // Finish
    pthread_cond_destroy(&cv);
    pthread_mutex_destroy(&mutex);
    free(thread_args);
    free(thread_handles);

    return 0;
}

//================================================================================

void read_file(char* const filename) {
    FILE* file = fopen(filename, "r");
    fscanf(file, "%lf %d %d", &gravity, &particles_count, &steps);

    particles = (particle_t*)malloc(particles_count * sizeof(particle_t));
    for (int i = 0; i < particles_count; ++i) {
        fscanf(file, "%lf", &particles[i].mass);
        fscanf(file, "%lf %lf", &particles[i].position.x, &particles[i].position.y);
        fscanf(file, "%lf %lf", &particles[i].velocity.x, &particles[i].velocity.y);
    }

    fclose(file);
}

vector_t add_vector(vector_t a, vector_t b) {
    vector_t result = {a.x + b.x, a.y + b.y};
    return result;
}

vector_t substract_vector(vector_t a, vector_t b) {
    vector_t result = {a.x - b.x, a.y - b.y};
    return result;
}

vector_t scale_vector(double b, vector_t a) {
    vector_t result = {a.x * b, a.y * b};
    return result;
}

double mod(vector_t a) {
    return sqrt(a.x * a.x + a.y * a.y);
}

//================================================================================

void* simulate(void* thread_info) {
    thread_info_t* info = thread_info;
    for (uint32_t index = info->index_start; index < info->index_finish; ++index) {
        computeAcceleration(index);
    }

    pthread_mutex_lock(&mutex);
    cntr++;
    while (cntr != thread_count) {
        pthread_cond_wait(&cv, &mutex);
    }
    pthread_cond_broadcast(&cv);
    pthread_mutex_unlock(&mutex);

    for (uint32_t index = info->index_start; index < info->index_finish; ++index) {
        computePosition(index);
        computeVelocity(index);
    }

    return NULL;
}

void computeAcceleration(uint32_t index) {
    vector_t sum = {0, 0};
    for (int k = 0; k < particles_count; ++k) {
        if (index != k) {
            vector_t R = substract_vector(particles[index].position, particles[k].position);
            double modR = mod(R);
            sum = add_vector(sum, scale_vector(particles[k].mass / (modR * modR * modR), R));
        }
    }
    vector_t F = scale_vector(gravity * particles[index].mass, sum);
    particles[index].acceleration = scale_vector(delta_time / particles[index].mass, F);
}

void computeVelocity(uint32_t index) {
    particles[index].velocity = add_vector(particles[index].velocity, scale_vector(delta_time, particles[index].acceleration));
}

void computePosition(uint32_t index) {
    particles[index].position = add_vector(particles[index].position, scale_vector(delta_time, particles[index].velocity));
}

void resolveCollisions() {
    for (int i = 0; i < particles_count - 1; ++i)
        for (int j = i + 1; j < particles_count; ++j) {
            if (fabs(particles[i].position.x - particles[j].position.x) < 0.000001 && fabs(particles[i].position.y - particles[j].position.y) < 0.000001) {
                vector_t temp = particles[i].velocity;
                particles[i].velocity = particles[j].velocity;
                particles[j].velocity = temp;
            }
        }
}