#![allow(unused_assignments, clippy::inherent_to_string)]

use std::collections::HashMap;
use std::sync::{Arc, Mutex};

mod util;
use util::*;

mod command_execution;
use command_execution::hash_command::hash;
use command_execution::kvp_command::kvp;
use command_execution::list_command::list;
use command_execution::one_off_command::one_off;

pub const DUMP_FILE_NAME: &str = "dump.my_rdb";
pub const DEBUG_DUMP_FILE_NAME: &str = "debug_dump.my_rdb";
#[derive(Clone)]
pub struct ExecutionContext {
    pub key_value_pairs: Arc<Mutex<HashMap<String, RedisValue>>>,
    lists: Arc<Mutex<HashMap<String, Vec<RedisValue>>>>,
    // Full type here is:
    //std::sync::Arc<std::sync::Mutex<std::collections::HashMap<std::string::String, std::collections::HashMap<std::string::String, RedisValue>>>>
    hashes: Arc<Mutex<HashMap<String, HashMap<String, RedisValue>>>>,
}

impl ExecutionContext {
    fn new() -> Self {
        Self {
            key_value_pairs: Arc::new(Mutex::new(HashMap::new())),
            lists: Arc::new(Mutex::new(HashMap::new())),
            hashes: Arc::new(Mutex::new(HashMap::new())),
        }
    }

    fn from_file_contents(text: String) -> Self {
        let mut context = ExecutionContext::new();
        let mut key_value_pairs = HashMap::new();
        let mut lists = HashMap::new();
        let mut hashes = HashMap::new();

        if let Ok(val) = serde_json::from_str(&text) {
            key_value_pairs = get_kvps_from_json("key_value_pairs".to_string(), &val);
            lists           = get_lists_from_json(&val);
            hashes          = get_hashes_from_json(&val);
        } else {
            println!("[ERROR]: file {DUMP_FILE_NAME} has been corrupted.\nHINT: Please delete it and run the program again.");
            std::process::exit(1);
        }

        context.key_value_pairs = Arc::new(Mutex::new(key_value_pairs));
        context.lists = Arc::new(Mutex::new(lists));
        context.hashes = Arc::new(Mutex::new(hashes));

        context
    }

    pub fn to_string(&self) -> String {
        use serde_json::{from_str, to_string_pretty, Value};

        let kvps: &HashMap<String, RedisValue> = &self.key_value_pairs.lock().unwrap();
        let lists: &HashMap<String, Vec<RedisValue>> = &self.lists.lock().unwrap();
        let hashes: &HashMap<String, HashMap<String, RedisValue>> = &self.hashes.lock().unwrap();

        let mut final_str = String::from("{\n\t\"key_value_pairs\": ");
        if let Ok(s) = to_string_pretty(&kvps) {
            final_str.push_str(&s);
            final_str.push_str(",\n");
        }
        if let Ok(s) = to_string_pretty(&lists) {
            final_str.push_str("\"lists\": ");
            final_str.push_str(&s);
            final_str.push_str(",\n");
        }
        if let Ok(s) = to_string_pretty(&hashes) {
            final_str.push_str("\t\"hashes\": ");
            final_str.push_str(&s);
            final_str.push('\n');
        }
        final_str.push_str("\n}");

        if from_str::<Value>(&final_str).is_ok() {
            // falltru
        } else if kvps.len() != 0 && lists.len() != 0 && hashes.len() != 0 {
            println!("Data has been corrupted. Cannot save.");
            std::process::exit(1);
        }

        final_str
    }
}

#[derive(Clone)]
pub struct Executor {
    pub context: ExecutionContext,
    pub setup_properly: bool,
    dump_file_path: &'static str,
}

pub fn setup_executor(debug_mode: bool) -> Executor {
    use std::{fs, path};

    let mut setup_properly = false;
    let dump_file_path = if debug_mode {
        &DEBUG_DUMP_FILE_NAME
    } else {
        &DUMP_FILE_NAME
    };

    let path = path::Path::new(&dump_file_path);
    if !path.exists() {
        if fs::File::create(&dump_file_path).is_err() {
            eprintln!("[ERROR]: Cannot create a dump file! \nHINT: This can usually be resolved by running the program again.");
            std::process::exit(1);
        } else {
            let setup_data = r#"{
    "key_value_pairs": {

    },
    "lists": {

    },
    "hashes": {

    }
}"#;
            fs::write(&dump_file_path, setup_data).unwrap();
        }
    } else if let Ok(text) = fs::read_to_string(&dump_file_path) {
        let context = ExecutionContext::from_file_contents(text);
        setup_properly = true;

        if setup_properly {
            return Executor {
                context,
                setup_properly,
                dump_file_path,
            };
        }
    } else {
        eprintln!("[ERROR]: Cannot read dump file!");
    }

    Executor::error_default()
}

impl Executor {
    fn error_default() -> Self {
        Self {
            context: ExecutionContext::new(),
            setup_properly: false,
            dump_file_path: "",
        }
    }

    fn exec_one_off(&self, command: &str, args: Vec<&str>) -> command_execution::Result {
        match command {
            "echo" => one_off::echo(args),
            "ping" => one_off::ping(args),
            "flushall" => {
                /*
                Delete all the keys of all the existing databases, not just the currently selected one.
                This command never fails.
                 */
                Ok("(nil)".to_string())
            }
            _ => {
                panic!("This will never be reached");
            }
        }
    }

    fn exec_kvp_command(&self, command: &str, args: Vec<&str>) -> command_execution::Result {
        let ctx = &self.context.key_value_pairs;
        match command {
            "set" => kvp::set(&mut ctx.lock().unwrap(), args),
            "get" => kvp::get(&mut ctx.lock().unwrap(), args),
            "key" => kvp::key(&mut ctx.lock().unwrap(), args),
            "type" => kvp::r#type(&mut ctx.lock().unwrap(), args),
            "del" => kvp::del(&mut ctx.lock().unwrap(), args),
            "unlink" => kvp::unlink(ctx, args.iter().map(|x| x.to_string()).collect()),
            "expire" => kvp::expire(ctx, args.iter().map(|x| x.to_string()).collect()),
            "rename" => kvp::rename(&mut ctx.lock().unwrap(), args),
            _ => {
                panic!("This will never be reached");
            }
        }
    }

    fn exec_list_command(&self, command: &str, args: Vec<&str>) -> command_execution::Result {
        match command {
            "llen" => list::llen(&mut self.context.lists.lock().unwrap(), args),
            "lrem" => list::lrem(&mut self.context.lists.lock().unwrap(), args),
            "lindex" => list::lindex(&mut self.context.lists.lock().unwrap(), args),
            "lpop" => list::lpop(&mut self.context.lists.lock().unwrap(), args),
            "rpop" => list::rpop(&mut self.context.lists.lock().unwrap(), args),
            "lpush" => list::lpush(&mut self.context.lists.lock().unwrap(), args),
            "rpush" => list::rpush(&mut self.context.lists.lock().unwrap(), args),
            "lset" => list::lset(&mut self.context.lists.lock().unwrap(), args),
            _ => {
                panic!("This will never be reached");
            }
        }
    }

    fn exec_hash_command(&self, command: &str, args: Vec<&str>) -> command_execution::Result {
        match command {
            "hget" => hash::hget(&mut self.context.hashes.lock().unwrap(), args),
            "hexists" => hash::hexists(&mut self.context.hashes.lock().unwrap(), args),
            "hdel" => hash::hdel(&mut self.context.hashes.lock().unwrap(), args),
            "hgetall" => hash::hgetall(&mut self.context.hashes.lock().unwrap(), args),
            "hkeys" => hash::hkeys(&mut self.context.hashes.lock().unwrap(), args),
            "hlen" => hash::hlen(&mut self.context.hashes.lock().unwrap(), args),
            "hmset" => hash::hmset(&mut self.context.hashes.lock().unwrap(), args),
            "hset" => hash::hset(&mut self.context.hashes.lock().unwrap(), args),
            "hvals" => hash::hvals(&mut self.context.hashes.lock().unwrap(), args),
            _ => {
                panic!("This will never be reached");
            }
        }
    }

    pub fn exec(&self, command: String) -> command_execution::Result {
        let mut command_words = command.split(' ');
        let clone = command_words.next().unwrap().to_lowercase().clone();
        let cmd_name = clone.as_str();
        let cmd_args = command_words.collect::<Vec<_>>();

        match cmd_name {
            "echo" | "ping" | "flushall" => self.exec_one_off(cmd_name, cmd_args),
            "set" | "get" | "key" | "type" | "del" | "unlink" | "expire" | "rename" => {
                self.exec_kvp_command(cmd_name, cmd_args)
            }
            "llen" | "lrem" | "lindex" | "lpop" | "rpop" | "lpush" | "rpush" | "lset" => {
                self.exec_list_command(cmd_name, cmd_args)
            }
            "hget" | "hexists" | "hdel" | "hgetall" | "hkeys" | "hlen" | "hmset" | "hset" | "hvals" => {
                self.exec_hash_command(cmd_name, cmd_args)
            },
            _ => Err(format!("Unknown command \"{cmd_name}\" provided.")),
        }
    }

    pub fn expire_value(&self, key: &str) -> command_execution::Result {
        self.exec_kvp_command("del", vec![key])
    }

    pub fn save(&self) {
        let string_value = self.context.to_string();

        let path = std::path::Path::new(&self.dump_file_path);
        if !path.exists() {
            if std::fs::File::create(&self.dump_file_path).is_err() {
                eprintln!("[ERROR]: Cannot create a dump file!");
            }
        } else {
            std::fs::write(&self.dump_file_path, &string_value).unwrap();
        }
    }
}

pub static mut EXPIRY_LIST: Vec<ScheduledExpiry> = vec![];

#[derive(Debug)]
pub struct ScheduledExpiry {
    pub start: std::time::SystemTime,
    pub wait_time: u64,
    pub key: String,
}

impl ScheduledExpiry {
    pub fn create(start: std::time::SystemTime, wait_time: u64, key: String) {
        unsafe {
            EXPIRY_LIST.push(Self {
                start,
                wait_time,
                key,
            })
        }
    }
}
