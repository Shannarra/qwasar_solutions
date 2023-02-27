#![allow(clippy::comparison_chain)]

pub mod list {
    use crate::redis_engine::RedisValue;
    type Result = super::super::Result;
    type List = std::collections::HashMap<String, Vec<RedisValue>>;

    pub fn llen(list: &mut List, args: Vec<&str>) -> Result {
        // https://redis.io/commands/llen/

        if args.len() != 1 {
            return Err("[ERROR]: llen accepts only one argument".to_string());
        }

        if !list.contains_key(args[0]) {
            // from docs:
            // If key does not exist, it is interpreted as an empty list and 0 is returned.
            Ok("0".to_string())
        } else {
            Ok(list[args[0]].len().to_string())
        }
    }

    pub fn lpush(list: &mut List, args: Vec<&str>) -> Result {
        // https://redis.io/commands/lpush/

        if args.len() < 2 {
            return Err("[ERROR]: At least two arguments required for lpush!".to_string());
        }

        let mut vec = vec![];
        for v in &args[1..] {
            vec.push(RedisValue::from_str(v));
        }

        if !list.contains_key(args[0]) {
            list.insert(args[0].to_string(), vec);
        } else {
            let mut deq = std::collections::VecDeque::from(list[args[0]].clone());
            for v in &args[1..] {
                deq.push_front(RedisValue::from_str(v));
            }

            let l = list.get_mut(args[0]).unwrap();
            l.clear();
            *l = Vec::from(deq);
        }

        Ok(format!("{}", args.len() - 1))
    }

    pub fn rpush(list: &mut List, args: Vec<&str>) -> Result {
        // https://redis.io/commands/rpush/

        if args.len() < 2 {
            return Err("[ERROR]: At least two arguments required for rpush!".to_string());
        }

        let mut vec = vec![];
        for v in &args[1..] {
            vec.push(RedisValue::from_str(v));
        }

        if !list.contains_key(args[0]) {
            list.insert(args[0].to_string(), vec);
        } else {
            for v in &args[1..] {
                list.get_mut(args[0]).unwrap().push(RedisValue::from_str(v));
            }
        }

        Ok(format!("{}", args.len() - 1))
    }

    pub fn lrem(list: &mut List, args: Vec<&str>) -> Result {
        // https://redis.io/commands/lrem/

        if args.len() < 3 {
            return Err("[ERROR]: At least three arguments required for lrem!".to_string());
        }

        let name = args[0];
        let mut count = 0;
        let key = args[2];

        if args[1].parse::<i32>().is_err() {
            return Err(format!("Second argument of lrem MUST be a whole number! Got {}", args[1]));
        } else {
            count = args[1].parse::<i32>().unwrap();
        }

        if !list.contains_key(name) {
            // Note that non-existing keys are treated like empty lists, so when key does not exist, the command will always return 0.
            return Ok("0".to_string());
        }

        let old_len = list[name].len();

        let mutable_list = list.get_mut(name).unwrap();
        let mut vec_cpy = vec![];

        if count == 0 {
            mutable_list
                .retain(|x| {
                    x.to_string() != key
                });
        } else if count < 0{
            let mut to_remove = -count;

            for item in mutable_list.iter().rev() {
                if to_remove > 0  && item.to_string() == key {
                    to_remove-=1;
                    continue;
                } else {
                    vec_cpy.push(item.clone());
                }
            }

            mutable_list.clear();
            // doing this:
            *mutable_list = vec_cpy.clone();
            // will save the list in reverse, so we need to reverse it back:
            mutable_list.reverse();
        } else {
            for item in mutable_list.iter() {
                if count > 0  && item.to_string() == key {
                    count-=1;
                    continue;
                } else {
                    vec_cpy.push(item.clone());
                }
            }

            mutable_list.clear();
            *mutable_list = vec_cpy.clone();
        }

        Ok(format!("{}", old_len - list[name].len()))
    }

    pub fn lindex(list: &mut List, args: Vec<&str>) -> Result {
        // https://redis.io/commands/lindex/

        if args.len() < 2 {
            return Err("[ERROR]: At least two arguments required for lindex!".to_string());
        }

        let name = args[0];
        let mut index = 0;

        if args[1].parse::<i32>().is_err() {
            return Err("[ERROR]: index value is not an integer".to_string());
        } else {
            index = args[1].parse::<i32>().unwrap();
        }

        if !list.contains_key(name) {
            return Ok("(nil)".to_string());
        }

        let vec = &list[name];

        if index > vec.len().try_into().unwrap() {
            return Ok("(nil)".to_string());
        }

        if index < 0 { // -1, -2, etc..
            index = (vec.len() as i32 + index) as i32;
        }

        Ok(format!("{:?}", vec[index as usize].to_string()))
    }


    pub fn lpop(list: &mut List, args: Vec<&str>) -> Result {
        // https://redis.io/commands/lpop/

        if args.is_empty() {
            return Err("[ERROR]: At least one argument required for lpop!".to_string());
        }

        let v = list.get_mut(args[0]).unwrap();

        if v.is_empty() {
            return Err(format!("[ERROR]: List {} is empty!", args[0]));
        }

        if args.len() > 1 {
            let mut count = 0;

            if args[1].parse::<i32>().is_err() {
                return Err("[ERROR]: count value is not an integer. Usage: lpop LISTNAME [COUNT]".to_string());
            } else {
                count = args[1].parse::<i32>().unwrap();
            }

            let mut r_v = vec![];
            for _ in 0..count {
                v.rotate_left(1);
                r_v.push(v.pop().unwrap().to_string());
            }

            return Ok(format!("{:?}", r_v));
        }

        v.rotate_left(1);
        Ok(v.pop().unwrap().to_string())
    }

     pub fn rpop(list: &mut List, args: Vec<&str>) -> Result {
        // https://redis.io/commands/rpop/

        if args.is_empty() {
            return Err("[ERROR]: At least one argument required for rpop!".to_string());
        }

        let v = list.get_mut(args[0]).unwrap();

        if v.is_empty() {
            return Err(format!("[ERROR]: List {} is empty!", args[0]));
        }

        if args.len() > 1 {
            let mut count = 0;

            if args[1].parse::<i32>().is_err() {
                return Err("[ERROR]: count value is not an integer. Usage: rpop LISTNAME [COUNT]".to_string());
            } else {
                count = args[1].parse::<i32>().unwrap();
            }

            let mut r_v = vec![];
            for _ in 0..count {
                r_v.push(v.pop().unwrap().to_string());
            }

            return Ok(format!("{:?}", r_v));
        }

        Ok(v.pop().unwrap().to_string())
     }

     pub fn lset(list: &mut List, args: Vec<&str>) -> Result {
         // https://redis.io/commands/lset/

         if args.len() < 3 {
             return Err("[ERROR]: At least three arguments required for lset!".to_string());
         }

         let key     =args[0];
         let mut index   = 0;
         let element = args[2];

         if args[1].parse::<i32>().is_err() {
             return Err("[ERROR]: index value is not an integer".to_string());
         } else {
             index = args[1].parse::<i32>().unwrap();
         }

         if !list.contains_key(key) {
             return Err("[ERROR]: no such key".to_string());
         }

         let vec = list.get_mut(key).unwrap();

         if index > vec.len().try_into().unwrap() {
             return Err("[ERROR]: Index is out of list range".to_string());
         }

         // clamp the index into the bounds of the vec
         if index < 0 { // -1, -2, etc..
             index = (vec.len() as i32 + index) as i32;
         }

         vec[index as usize] = RedisValue::from_str(element);
         Ok("Ok".to_string())
     }
}
