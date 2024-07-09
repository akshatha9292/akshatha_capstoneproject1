#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>

#define PORT 9090
#define MAX_CLIENTS 1
#define SHM_NAME "/shm_random_numbers"
#define SHM_SIZE sizeof(int)

sem_t *semaphore;
int server_fd;
int client_count = 0;

// Function to generate random numbers and store them in shared memory
void *generate_random_numbers(void *arg) {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    ftruncate(shm_fd, SHM_SIZE);
    int *shared_memory = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    printf("Shared memory ID: %d\n", shm_fd);

    while (1) {
        int random_number = rand() % 90 + 10;
        sem_wait(semaphore);  // Wait for semaphore to ensure exclusive access
        *shared_memory = random_number;  // Write random number to shared memory
        sem_post(semaphore);  // Release semaphore
        sleep(1);  // Generate a new number every second
    }
    return NULL;
}

// Function to handle multiple clients
void *handle_clients(void *arg) {
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int clients[MAX_CLIENTS];
    int num_clients = 0;

    // Continuously accept and handle clients
    while (1) {
        if (num_clients < MAX_CLIENTS) {
            // Accept new client connections
            int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
            if (new_socket < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            // Store client socket in the array
            clients[num_clients++] = new_socket;
            client_count++;
            printf("\nAdding client on %d\n", new_socket);
        } else {
            // If maximum clients reached, create a new thread to handle additional clients
            pthread_t tid_client;
            pthread_create(&tid_client, NULL, handle_clients, NULL);
            num_clients = 0; // Reset the count for the new thread
            printf("\nCreating new thread group\n");
        }

        for (int i = 0; i < num_clients; i++) {
            // Handle client connection
            sleep(1); // Just to simulate some work with the client
        }
    }
    return NULL;
}

int main() {
    struct sockaddr_in address;
    int opt = 1;

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attach socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("\nServer is listening to port: %d\n", PORT);

    // Initialize semaphore
    semaphore = sem_open("/semaphore", O_CREAT, 0644, 1);
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
  
  }

    // Create threads for generating random numbers and accepting clients
    pthread_t tid_random, tid_client;
    pthread_create(&tid_random, NULL, generate_random_numbers, NULL);
    pthread_create(&tid_client, NULL, handle_clients, NULL);

    pthread_join(tid_random, NULL);
    pthread_join(tid_client, NULL);

    // Cleanup
    sem_close(semaphore);
    sem_unlink("/semaphore");
    shm_unlink(SHM_NAME);

    return 0;
}
