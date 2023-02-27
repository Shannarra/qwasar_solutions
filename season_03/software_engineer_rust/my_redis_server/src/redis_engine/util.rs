#![allow(clippy::inherent_to_string)]

type IntegerType = i64;

#[derive(Clone)]
pub struct RedisValue {
    is_int: bool,
    value_t1: Option<IntegerType>,
    value_t2: Option<String>,
}

impl RedisValue {
    pub fn from_string(value: String) -> Self {
        Self {
            is_int: false,
            value_t1: None,
            value_t2: Some(value)
        }
    }

    pub fn from_int(value: IntegerType) -> Self {
        Self {
            is_int: true,
            value_t1: Some(value),
            value_t2: None
        }
    }

    pub fn from_str(value: &str) -> Self {
        match value.parse::<IntegerType>() {
            Ok(n) =>  RedisValue::from_int(n),
            Err(_) => RedisValue::from_string(value.to_string())
        }
    }

    pub fn from_json(value: &serde_json::Value) -> Self {
        let typ = value.is_number();
        let mut is_int = false;
        let mut value_t1 = None;
        let mut value_t2 = None;

        if typ {
            is_int = true;
            value_t1 = value.as_i64();
        } else {
            value_t2 = Some(value.as_str().unwrap().to_string());
        }

        Self {
            is_int,
            value_t1,
            value_t2
        }
    }

    pub fn to_string(&self) -> String {
        if self.is_int {
            format!("{}", &self.value_t1.unwrap())
        } else {
            let clone = &self.value_t2.as_ref().unwrap();
            clone.to_string()
        }
    }


    pub fn to_result_value(&self) -> crate::redis_engine::command_execution::Result {
        Ok(self.to_string())
    }

    pub fn is_a_number(&self) -> bool {
        self.is_int
    }

    pub fn int_value(&self) -> IntegerType {
        self.value_t1.unwrap()
    }

    pub fn string_value(&self) -> String {
        let cl = self.value_t2.as_ref().unwrap();

        cl.to_string()
    }
}

impl std::fmt::Debug for RedisValue {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "RedisValue {{").unwrap();

        if self.is_int {
            write!(f, " {} (Integer) }}", self.value_t1.as_ref().unwrap())
        } else {
            write!(f, " \"{}\" (String) }}", self.value_t2.as_ref().unwrap())
        }
    }
}

use serde::ser::{Serialize, Serializer};

impl Serialize for RedisValue {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        if self.is_a_number() {
            serializer.serialize_i64(self.value_t1.unwrap())
        } else {
            let clone = &self.value_t2.as_ref().unwrap().clone();
            serializer.serialize_str(clone)
        }
    }
}

use std::collections::HashMap;

pub fn get_kvps_from_json(key: String, value: &serde_json::Value) -> HashMap<String, RedisValue> {
    let mut map = HashMap::new();

    for (k,v) in value[key].as_object().unwrap() {
        map.insert(k.clone(), RedisValue::from_json(v));
    }

    map
}

pub fn get_lists_from_json(value: &serde_json::Value) -> HashMap<String, Vec<RedisValue>> {
    let mut map = HashMap::new();

    for (k,v) in value["lists"].as_object().unwrap() {
        if let Some(arr) = v.as_array() {
            let mut vector = Vec::new();
            for i in arr {
                vector.push(RedisValue::from_json(i))
            }
            map.insert(k.clone(), vector);
        } else {
            panic!("Invalid value or format for list!");
        }
    }

    map
}

pub fn get_hashes_from_json(value: &serde_json::Value) -> HashMap<String, HashMap<String, RedisValue>> {
    let mut map = HashMap::new();

    for (k,v) in value["hashes"].as_object().unwrap() {
        if let Some(obj) = v.as_object() {
            let mut m = HashMap::new();
            for (_k, _v) in obj {
                m.insert(_k.to_string(), RedisValue::from_json(_v));
            }

            map.insert(k.to_string(), m);
        } else {
            panic!("Invalid value or format for hash!");
        }
    }

    map
}
