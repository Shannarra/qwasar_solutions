#![allow(clippy::if_same_then_else)] // for lines 18 and 20

pub mod hash {
    use crate::redis_engine::RedisValue;
    type Result = super::super::Result;
    type Hash = std::collections::HashMap<String, std::collections::HashMap<String, RedisValue>>;

    pub fn hget(hash: &mut Hash, args: Vec<&str>) -> Result {
        // https://redis.io/commands/hget/

        if args.len() < 2 {
            return Err("[ERROR]: At least two arguments required for hget!".to_string());
        }

        let key = args[0];
        let field = args[1];

        if !hash.contains_key(key) {
            return Ok("(nil)".to_string());
        } else if !hash[key].contains_key(field) {
            return Ok("(nil)".to_string());
        }

        Ok(hash[key][field].to_string())
    }

    pub fn hexists(hash: &mut Hash, args: Vec<&str>) -> Result {
        // https://redis.io/commands/hexists/

        if args.len() < 2 {
            return Err("[ERROR]: At least two arguments required for hexists!".to_string());
        }

        let key = args[0];
        let field = args[1];

        if !hash.contains_key(key) {
            return Ok("0".to_string());
        } else if !hash[key].contains_key(field) {
            return Ok("0".to_string());
        }

        Ok("1".to_string())
    }

    pub fn hdel(hash: &mut Hash, args: Vec<&str>) -> Result {
        // https://redis.io/commands/hdel/

        if args.len() < 2 {
            return Err("[ERROR]: At least two arguments required for hdel!".to_string());
        }

        let mut affected = 0;
        let key = args[0];

        if !hash.contains_key(key) {
            return Ok(format!("{}", affected));
        }

        for f in &args[1..] {
            if hash.get_mut(key).unwrap().remove(&f.to_string()).is_some() {
                affected+=1;
            }
        }

        Ok(format!("{}", affected))
    }

    pub fn hgetall(hash: &mut Hash, args: Vec<&str>) -> Result {
        // https://redis.io/commands/hgetall/

        if args.is_empty() {
            return Err("[ERROR]: At least two arguments required for hgetall!".to_string());
        }

        let mut vec = vec![];
        let key = args[0];

        if hash.contains_key(key) {
            for k in hash[key].keys() {
                vec.push(k.to_string());
                let clone = hash[key][k].to_string().clone();
                vec.push(clone);
            }
        }

        Ok(format!("{:?}", vec))
    }


    pub fn hkeys(hash: &mut Hash, args: Vec<&str>) -> Result {
        // https://redis.io/commands/hkeys/

        if args.is_empty() {
            return Err("[ERROR]: At least one argument required for hkeys!".to_string());
        }

        let mut vec = vec![];
        let key = args[0];

        if hash.contains_key(key) {
            for k in hash[key].keys() {
                vec.push(k.to_string())
            }

            // this can be done like so as well:
            // vec = hash[key].clone().into_keys().collect();
            // but will produce keys in arbitrary order
            // for more info see: https://doc.rust-lang.org/std/collections/struct.HashMap.html#method.into_keys
        }

        Ok(format!("{:?}", vec))
    }


    pub fn hvals(hash: &mut Hash, args: Vec<&str>) -> Result {
        // https://redis.io/commands/hvals/

        if args.is_empty() {
            return Err("[ERROR]: At least one argument required for hvals!".to_string());
        }

        let mut vec = vec![];
        let key = args[0];

        if hash.contains_key(key) {
            for k in hash[key].values() {
                vec.push(k.to_string())
            }

            // this can be done like so as well:
            // vec = hash[key].clone().into_values().collect();
            // but will produce keys in arbitrary order
            // for more info see: https://doc.rust-lang.org/std/collections/struct.HashMap.html#method.into_values
        }

        Ok(format!("{:?}", vec))
    }

    pub fn hlen(hash: &mut Hash, args: Vec<&str>) -> Result {
        // https://redis.io/commands/hlen/

        if args.is_empty() {
            return Err("[ERROR]: At least one argument required for hlen!".to_string());
        }

        let key = args[0];

        if !hash.contains_key(key) {
            return Ok("0".to_string())
        }

        return Ok(format!("{}", hash[key].keys().len()))
    }

    pub fn hset(hash: &mut Hash, args: Vec<&str>) -> Result {
        // https://redis.io/commands/hset/

        if args.len() < 3 {
            return Err("[ERROR]: At least three arguments required for hset!".to_string());
        }

        let key = args[0];
        let mut fields = vec![];
        let mut values = vec![];
        let mut affected = 0;

        // get all fields and values
        let mut idx = 1;
        for item in &args[1..] {
            if idx % 2 != 0 {
                fields.push(item);
            } else {
                values.push(item);
            }
            idx+=1;
        }

        if fields.len() != values.len() {
            return Err("[ERROR]: Not enough values corresponding to fields. Aborting.".to_string())
        }

        let mut sub_hash = std::collections::HashMap::<String, RedisValue>::new();

        for i in 0..fields.len() {
            sub_hash.insert(fields[i].to_string(), RedisValue::from_str(values[i]));
        }

        if !hash.contains_key(key) {
            affected = sub_hash.len();
            hash.insert(key.to_string(), sub_hash);
        } else {
            for (k,v) in sub_hash {
                hash
                    .get_mut(key)
                    .unwrap()
                    .insert(k.to_string(), v);
                affected+=1;
            }
        }
        return Ok(format!("{}", affected))
    }

     pub fn hmset(hash: &mut Hash, args: Vec<&str>) -> Result {
         // https://redis.io/commands/hmset/

         // See the link above.
         /*
          * As of Redis version 4.0.0, this command is regarded as deprecated.
          * It can be replaced by HSET with multiple field-value pairs when migrating or writing new code.
          */
         self::hset(hash, args)
     }
}
