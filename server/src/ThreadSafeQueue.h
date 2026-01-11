#pragma once

// ThreadSafeQueue - Usage Example
// This project uses moodycamel::BlockingConcurrentQueue directly.
// No wrapper needed - the library provides all necessary functionality.
//
// Usage:
//   #include <blockingconcurrentqueue.h>
//
//   // Define your task type
//   struct NetworkTask {
//       int clientId;
//       std::string data;
//   };
//
//   // Create queues (dynamically sized, thread-safe)
//   moodycamel::BlockingConcurrentQueue<NetworkTask> networkToGameQueue;
//   moodycamel::BlockingConcurrentQueue<NetworkTask> gameToNetworkQueue;
//
//   // Network thread: push to game state
//   NetworkTask task{clientId, messageData};
//   networkToGameQueue.enqueue(task);
//
//   // Game state thread: blocking pop
//   NetworkTask task;
//   networkToGameQueue.wait_dequeue(task);
//   // Process task...
//
//   // Game state thread: pop with timeout (recommended)
//   NetworkTask task;
//   if (networkToGameQueue.wait_dequeue_timed(task, std::chrono::milliseconds(100))) {
//       // Got a task, process it
//   }
//
//   // Non-blocking try
//   NetworkTask task;
//   if (networkToGameQueue.try_dequeue(task)) {
//       // Got a task, process it
//   }
//
// That's it! No custom wrapper needed.
