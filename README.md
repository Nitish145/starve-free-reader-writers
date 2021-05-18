# CSN 232 : Assignment

Name : Nitish Aggarwal <br/>
Enr. No. : 18117064

## Starve Free Readers Writers Problem

### Sempahores 

- *rw_queue* : Maintains the order in which readers and writers arrive.
- *read_mutex* : Provides mutual exclusion to *reader_count* variable which counts the number of readers in critical section.
- *res_variable* : Prevents readers and writers or multiple writers to be there in critical section simultaneously.

### Shared Variables

- *reader_count* : Counts number of readers present in critical section at any point of time.
- *data* : critical data shared between various threads.

### Reader function

- **Entry Section** : Acquires required semaphores to enter critical section.
  - Reader tries to acquires *rw_queue* semaphore initially, if it is unavailable, reader is added to queue for the given semaphore.

  - After aquiring *rw_queue*, it tries to aquire *read_mutex* semaphore to modify *reader_count*.

  - If it is the first reader, it tries to aquire *res_variable* semaphore to verify that there are no writers in the critical section.

  - It releases *rw_queue* and *read_mutex* before entering critical section.

- **Critical Section** : Shared data is required by the reader.

- **Exit Section** : Acquired semaphores are released.
  - Reader tries to aquire *read_mutex* to modify *read_count*.

  - If it is the last reader, it releases *res_variable* semaphore.

  - It releases *read_mutex* in the end.

### Writer function

- **Entry Section** : Acquires required semaphores to enter critical section.
  - Writer tries to acquires *rw_queue* semaphore initially, if not available, writer is added to queue for the concerned semaphore.

  - After aquiring *rw_queue*, it tries to aquire *res_variable* semaphore and enter critical section.

  - Releases *rw_queue* before entering critical section.
- **Critical Section** : Writer modifies the shared *data*.

- **Exit Section** : Releases the acquired semaphores.
  - Writer releases the *res_variable* semaphore.

### Main (main) function

- Inputs number of readers and writers from the user.
- Creates the number of threads required followed by initialisation of semaphores.
- Initialises reader threads (using *reader*) and writer threads (using *writer*).
- All threads created are joined to the parent thread.
- Completion of execution calls for the desctruction of the created semaphores.
