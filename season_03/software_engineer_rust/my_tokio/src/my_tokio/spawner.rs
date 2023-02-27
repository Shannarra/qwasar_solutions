use std::future::Future;
use std::sync::{
    Arc, Mutex, 
    mpsc::{
        SyncSender,
    }
};

use futures::FutureExt;

use super::task;

pub(crate) struct Spawner {
    pub(crate) task_sender: SyncSender<Arc<task::Task>>,
}

impl Spawner {
    pub(crate) fn spawn(&self, future: impl Future<Output = ()> + 'static + Send) -> core::result::Result<(), String> {
        let future = future.boxed();
        let task = Arc::new(task::Task {
            future: Mutex::new(Some(future)),
            task_sender: self.task_sender.clone(),
        });

        if self.task_sender.send(task).is_ok() {
            return Ok(())
        }

        Err("too many tasks queued".to_string())
    }
}