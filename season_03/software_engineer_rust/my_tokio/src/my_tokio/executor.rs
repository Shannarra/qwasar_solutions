use std::sync::{
    Arc,
    mpsc::{
        Receiver
    },
};

use super::task;

pub(crate) struct Executor {
    pub(crate) ready_queue: Receiver<Arc<task::Task>>,
}

impl Executor {
    pub(crate) fn run(&self) -> core::result::Result<(), String> {
        while let Ok(task) = self.ready_queue.recv() {
            let mut next_future = task.future.lock().unwrap();
            if let Some(mut future) = next_future.take() {
                let waker = futures::task::waker_ref(&task);
                let context = &mut std::task::Context::from_waker(&*waker);

                // `BoxFuture<T>` == `Pin<Box<dyn Future<Output = T> + Send + 'static>>`.
                if future.as_mut().poll(context).is_pending() {
                    *next_future = Some(future);
                }
            }
        }

        Ok(())
    }
}
