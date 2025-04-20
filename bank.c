#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

// Structure to hold shared resources and their mutexes
typedef struct {
    int bank_account_balance;
    int stock_portfolio_value;
    pthread_mutex_t bank_mutex;
    pthread_mutex_t stock_mutex;
    int target_balance;
    bool done;
} AccountResources;

// Agent parameters structure
typedef struct {
    AccountResources *resources;
    char *name;
    int transfer_amount;
    int transfer_direction; // 1: bank->stock, -1: stock->bank
    int sleep_duration_ms;
} AgentParams;

// Function for agent to transfer money
void* agent_transfer(void* arg) {
    AgentParams *params = (AgentParams*)arg;
    AccountResources *res = params->resources;
    int transfer_amount = params->transfer_amount;
    
    printf("Agent %s started: Will transfer %d per operation (%s)\n", 
           params->name, 
           transfer_amount,
           (params->transfer_direction > 0) ? "bank->stock" : "stock->bank");
    
    while (!res->done) {
        // Lock both mutexes to ensure the entire operation is atomic
        // Always lock in the same order to prevent deadlocks!
        pthread_mutex_lock(&res->bank_mutex);
        printf("Agent %s: Acquired bank_mutex\n", params->name);
        
        pthread_mutex_lock(&res->stock_mutex);
        printf("Agent %s: Acquired stock_mutex - now holds both locks\n", params->name);
        
        // Check if we're done before making any transfers
        if (res->bank_account_balance <= res->target_balance) {
            res->done = true;
            printf("Agent %s: Target balance of %d reached or exceeded. Current balance: %d\n", 
                   params->name, res->target_balance, res->bank_account_balance);
            pthread_mutex_unlock(&res->stock_mutex);
            pthread_mutex_unlock(&res->bank_mutex);
            break;
        }
        
        // Critical section - accessing both protected resources
        if (params->transfer_direction > 0) {
            // Bank to stock transfer
            if (res->bank_account_balance >= transfer_amount) {
                res->bank_account_balance -= transfer_amount;
                res->stock_portfolio_value += transfer_amount;
                printf("Agent %s: Transferred %d from bank to stocks\n", 
                       params->name, transfer_amount);
            } else {
                printf("Agent %s: Insufficient bank funds for transfer\n", params->name);
            }
        } else {
            // Stock to bank transfer
            if (res->stock_portfolio_value >= transfer_amount) {
                res->stock_portfolio_value -= transfer_amount;
                res->bank_account_balance += transfer_amount;
                printf("Agent %s: Transferred %d from stocks to bank\n", 
                       params->name, transfer_amount);
            } else {
                printf("Agent %s: Insufficient stock value for transfer\n", params->name);
            }
        }
        
        printf("Agent %s: Bank balance: %d, Stock value: %d\n", 
               params->name, res->bank_account_balance, res->stock_portfolio_value);
        
        // Always release locks in the opposite order of acquisition
        pthread_mutex_unlock(&res->stock_mutex);
        printf("Agent %s: Released stock_mutex\n", params->name);
        
        pthread_mutex_unlock(&res->bank_mutex);
        printf("Agent %s: Released bank_mutex\n", params->name);
        
        // Sleep to simulate processing time and make output readable
        usleep(params->sleep_duration_ms * 1000);
    }
    
    printf("Agent %s: Terminating\n", params->name);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t agent1_thread, agent2_thread;
    int target_balance = 0;  // Default target balance
    int initial_bank_balance = 1000;
    int initial_stock_value = 500;
    
    // Process command line arguments
    if (argc > 1) {
        target_balance = atoi(argv[1]);
        printf("Target balance set to: %d\n", target_balance);
        if (argc > 2) {
            initial_bank_balance = atoi(argv[2]);
            printf("Initial bank balance set to: %d\n", initial_bank_balance);
            if (argc > 3) {
                initial_stock_value = atoi(argv[3]);
                printf("Initial stock value set to: %d\n", initial_stock_value);
            }
        }
    } else {
        printf("Usage: %s [target_balance] [initial_bank_balance] [initial_stock_value]\n", argv[0]);
        printf("Using default values: target_balance=%d, initial_bank_balance=%d, initial_stock_value=%d\n", 
               target_balance, initial_bank_balance, initial_stock_value);
    }
    
    // Initialize the shared resources structure
    AccountResources *resources = malloc(sizeof(AccountResources));
    resources->bank_account_balance = initial_bank_balance;
    resources->stock_portfolio_value = initial_stock_value;
    resources->target_balance = target_balance;
    resources->done = false;
    
    // Initialize mutexes using pthread_mutex_init
    pthread_mutex_init(&resources->bank_mutex, NULL);
    pthread_mutex_init(&resources->stock_mutex, NULL);
    
    // Create agent parameter structures
    AgentParams *agent1_params = malloc(sizeof(AgentParams));
    agent1_params->resources = resources;
    agent1_params->name = "BankToStock";
    agent1_params->transfer_amount = 50;
    agent1_params->transfer_direction = 1;  // bank to stock
    agent1_params->sleep_duration_ms = 300;
    
    AgentParams *agent2_params = malloc(sizeof(AgentParams));
    agent2_params->resources = resources;
    agent2_params->name = "StockToBank";
    agent2_params->transfer_amount = 30;
    agent2_params->transfer_direction = -1;  // stock to bank
    agent2_params->sleep_duration_ms = 500;
    
    printf("Initial bank balance: %d, Initial stock value: %d\n", 
           resources->bank_account_balance, resources->stock_portfolio_value);
    printf("Agents will run until bank balance reaches %d\n", target_balance);
    
    // Create two agent threads
    pthread_create(&agent1_thread, NULL, agent_transfer, (void*)agent1_params);
    pthread_create(&agent2_thread, NULL, agent_transfer, (void*)agent2_params);
    
    // Wait for both threads to finish
    pthread_join(agent1_thread, NULL);
    pthread_join(agent2_thread, NULL);
    
    printf("\nFinal Results:\n");
    printf("Final bank balance: %d, Final stock value: %d\n", 
           resources->bank_account_balance, resources->stock_portfolio_value);
    printf("Total assets: %d\n", 
           resources->bank_account_balance + resources->stock_portfolio_value);
    
    // Clean up
    pthread_mutex_destroy(&resources->bank_mutex);
    pthread_mutex_destroy(&resources->stock_mutex);
    
    // Free allocated memory
    free(agent1_params);
    free(agent2_params);
    free(resources);
    
    return 0;
}