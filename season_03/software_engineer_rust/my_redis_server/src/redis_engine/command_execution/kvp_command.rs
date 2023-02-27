#![allow(clippy::comparison_chain)]

pub mod kvp {
    use crate::redis_engine::RedisValue;

    type KVPHash = std::collections::HashMap<String, RedisValue>;
    type Result = super::super::Result;

    fn set_from_redis_value(kvp: &mut KVPHash, name: &str, value: RedisValue) -> Result {
        kvp.insert(name.to_string(), value);

        Ok("Ok".to_string())
    }

    pub fn set(kvp: &mut KVPHash, args: Vec<&str>) -> Result {
        // https://redis.io/commands/set/

        let mut text = String::new();

        if args.len() < 2 {
            return Err("[ERROR]: Too few arguments for set".to_string());
        } else if args.len() > 2 {
            for i in &args[1..] {
                text.push_str(i);
                text.push(' ');
            }
            //Err("[ERROR]: Too many arguments for set".to_string())
        } else {
            text.push_str(args[1]);
        }

        kvp.insert(args[0].to_string(), RedisValue::from_str(&text));

        Ok("Ok".to_string())
    }

    pub fn get(kvp: &mut KVPHash, args: Vec<&str>) -> Result {
        // https://redis.io/commands/get/

        if args.len() != 1 {
            return Err("[ERROR]: get requires only one argument!".to_string());
        }

        if kvp.contains_key(args[0]) {
            kvp[args[0]].to_result_value()
        } else {
            Ok("(nil)".to_string())
        }
    }

    pub fn key(kvps: &mut KVPHash, args: Vec<&str>) -> Result {
        /*
         * There's no command "key" in Redis. I assume "key" will return "Ok" if a key exists and "Nok" if a key
         * does not exist.
         */
        //https://redis.io/commands/keys/

        if args.len() != 1 {
            return Err("[ERROR]: key requires only one argument!".to_string());
        }

        if kvps.contains_key(args[0]) {
            Ok("Ok".to_string())
        } else {
            Ok("Nok".to_string())
        }
    }

    pub fn r#type(kvps: &mut KVPHash, args: Vec<&str>) -> Result {
        // https://redis.io/commands/type/

        if args.len() != 1 {
            return Err("[ERROR]: type requires only one argument!".to_string());
        }

        return if kvps.contains_key(args[0]) {
            let mut txt = "string".to_string();

            if kvps[args[0]].is_a_number() {
                txt = format!("(integer) {}", kvps[args[0]].int_value());
            }

            Ok(txt)
        } else {
            Err("(nil)".to_string())
        }
    }


    pub fn del(kvps: &mut KVPHash, args: Vec<&str>) -> Result {
        // https://redis.io/commands/del/

        if args.is_empty() {
            return Err("[ERROR]: del requires at least one argument!".to_string());
        }

        let mut affected = 0;

        for k in args {
            if kvps.contains_key(k) && kvps.remove(k).is_some() {
                    affected+=1;
            }
        }

        Ok(format!("{}", affected))
    }

    pub fn unlink(kvps_arc: &std::sync::Arc<std::sync::Mutex<KVPHash>>, args: Vec<String>) -> Result {
        // https://redis.io/commands/unlink/

        // This command is very similar to DEL: it removes the specified keys.
        // Just like DEL a key is ignored if it does not exist, but it does the
        // job in a separate thread

        if args.is_empty() {
            return Err("[ERROR]: unlink requires at least one argument!".to_string());
        }

        let affected = std::sync::Arc::new(std::sync::Mutex::new(0));
        let sub_arc = std::sync::Arc::clone(kvps_arc);
        let sub_aff = std::sync::Arc::clone(&affected);

        std::thread::spawn(move || {
            let mut kvps = sub_arc.lock().unwrap();
            let mut aff = sub_aff.lock().unwrap();

            for i in args {
                if kvps.contains_key(&i) {
                    kvps.remove(&i);
                    *aff += 1;

                    println!("Affected: {}", aff);
                }
            }
        }).join().unwrap();

        Ok(format!("{:?}", affected.lock().unwrap()))
    }

     pub fn expire(_: &std::sync::Arc<std::sync::Mutex<KVPHash>>, args: Vec<String>) -> Result {
         // https://redis.io/commands/expire/

         // Set a timeout on key. After the timeout has expired, the key will automatically be deleted.

         if args.len() < 2 {
             return Err("[ERROR]: expire requires at least two arguments!".to_string());
         }

         let cl = args[1].clone();
         if cl.parse::<u64>().is_err() {
             return Err(format!("[ERROR]: Expiry timeout must be a number! Got {}", cl));
         }

         crate::redis_engine::ScheduledExpiry::create(
             std::time::SystemTime::now(),
             cl.parse::<u64>().unwrap(),
             args[0].clone()
         );


         // let kvps_arc = std::sync::Arc::clone(kvps);
         // let cl = args[1].clone();

         // tokio::spawn(async move {
         //     tokio::time::sleep(tokio::time::Duration::from_secs(cl.parse::<u64>().unwrap())).await;
         //     self::del(&mut kvps_arc.lock().unwrap(), vec![args[0].as_str()]).unwrap();
         // });

         Ok(format!("{}", 1))
     }

    pub fn rename(kvps: &mut KVPHash, args: Vec<&str>) -> Result {
        // https://redis.io/commands/rename/

        if args.len() < 2 {
            return Err("[ERROR]: rename requires at least two arguments!".to_string());
        }

        let clone = kvps[args[0]].clone();
        self::del(kvps, vec![args[0]]).unwrap();

        //println!("\n\nClone: {clone:?}\n\n");

        self::set_from_redis_value(kvps, args[1], clone)
    }
}
