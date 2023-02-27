use std::sync::mpsc::sync_channel;

mod spawner;
mod task;
mod executor;

pub const MAX_QUEUED_TASKS: usize = 10_000;

pub struct MyTokio {
    executor: executor::Executor,
    spawner: spawner::Spawner,
}

impl MyTokio {
    pub fn new() -> Self {
        let (task_sender, ready_queue) = sync_channel(MAX_QUEUED_TASKS);
        Self {
            executor: executor::Executor { ready_queue },
            spawner: spawner::Spawner { task_sender }
        }
    }

    pub fn spawn(&self, future: impl std::future::Future<Output = ()> + 'static + Send) -> core::result::Result<(), String> {
        self.spawner.spawn(future)
    }

    pub fn run(self) -> core::result::Result<(), String> {
        // running means we won't spawn any more futures AND
        // we don't need the runtime context after use, therefore we
        drop(self.spawner);

        // ... and execute the runtime, clearing all the memory in the process
        self.executor.run()
    }
}

impl Default for MyTokio {
    fn default() -> Self {
        Self::new()
    }
}
