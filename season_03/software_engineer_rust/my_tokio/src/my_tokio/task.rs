use std::sync::{
    Arc, 
    Mutex,
    mpsc::{
        SyncSender
    },    
};
use futures::future::BoxFuture;

pub(crate) struct Task {
    pub(crate) future: Mutex<Option<BoxFuture<'static, ()>>>,
    pub(crate) task_sender: SyncSender<Arc<Task>>,
}

impl futures::task::ArcWake for Task {
    fn wake_by_ref(arc_self: &Arc<Self>) {
        let cloned = arc_self.clone();
        arc_self
            .task_sender
            .send(cloned)
            .expect("too many tasks queued");
    }
}
