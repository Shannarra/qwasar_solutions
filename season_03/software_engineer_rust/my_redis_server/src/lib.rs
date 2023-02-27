pub mod redis_engine;

#[cfg(test)]
mod test {
    #[test]
    fn commands_execute_properly() {
        let commands = vec![
            "echo hi",
            "ping",
            "ping asd",
            "set kekw hi",
            "get kekw",
            "key kekw",
            "type kekw",
            "del namaikatiputkata",
            "del kekw",
            "set kekw sadeg",
            "set dummy val",
            "unlink dummy",
        ];
        let responses: Vec<core::result::Result<String, String>> = vec![
            Ok("hi".to_string()),
            Ok("pong".to_string()),
            Ok("asd".to_string()),
            Ok("Ok".to_string()),
            Ok("hi".to_string()),
            Ok("Ok".to_string()),
            Ok("string".to_string()),
            Ok("0".to_string()),
            Ok("1".to_string()),
            Ok("Ok".to_string()),
            Ok("Ok".to_string()),
            Ok("1".to_string()),
        ];

        let executor = crate::redis_engine::setup_executor(true);

        if executor.setup_properly {
            for i in 0..commands.len() {

                assert!(executor.exec(commands[i].to_string()) == responses[i])
            }
        } else {
            assert!(false);
        }
    }

    #[test]
    fn expire_delays_properly() {
        "
    // To test the \"expire\" functionality place the following code at the bottom of \"main\" or
    // anywhere OTUSIDE OF a #[test] cfg!
    let executor = crate::redis_engine::setup_executor(true);
    if executor.setup_properly {{
      assert_eq!(executor.exec(\"set dummy asd\".to_string()) , Ok(\"Ok\".to_string()));
      assert_eq!(executor.exec(\"expire dummy 1\".to_string()), Ok(\"1\".to_string()));
      assert_eq!(executor.exec(\"get dummy\".to_string())   , Ok(\"asd\".to_string()));
      std::thread::sleep(std::time::Duration::from_secs(2)); // wait 2s and check if \"dummy\" expired
      assert_ne!(executor.exec(\"get dummy\".to_string()), Ok(\"asd\".to_string()));
      assert_eq!(executor.exec(\"get dummy\".to_string()), Ok(\"(nil)\".to_string()));
      println!(\"Expiring works just fine!\");
    }} else {{
      assert!(false);
    }}
    ";
        assert!(true)
    }

    #[test]
    fn renaming_works_as_expected() {
        let commands = vec![
            "set dummy 123",
            "get dummy",
            "rename dummy henlo",
            "get henlo",
        ];

        let responses = vec![
            Ok("Ok".to_string()),
            Ok("123".to_string()),
            Ok("Ok".to_string()),
            Ok("123".to_string()),
        ];

        let executor = crate::redis_engine::setup_executor(true);

        if executor.setup_properly {
            for i in 0..commands.len() {
                assert!(executor.exec(commands[i].to_string()) == responses[i])
            }
        } else {
            assert!(false);
        }
    }

    #[test]
    fn basic_list_operations_work_properly() {
        let commands = vec![
            "llen list1",
            "llen list_that_doesnt_exist",
            "rpush new_list 1 2 3",
            "llen new_list",
        ];

        let responses = vec![
            Ok("2".to_string()),
            Ok("0".to_string()),
            Ok("3".to_string()),
            Ok("3".to_string()),
        ];

        let executor = crate::redis_engine::setup_executor(true);

        if executor.setup_properly {
            for i in 0..commands.len() {
                assert!(executor.exec(commands[i].to_string()) == responses[i])
            }
        } else {
            assert!(false);
        }
    }

    #[test]
    fn lrem_works_as_expected() {
        let executor = crate::redis_engine::setup_executor(true);

        assert_eq!(executor.exec("rpush list hello".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("rpush list foo".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("rpush list hello".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("rpush list hello".to_string()), Ok("1".to_string()));

        assert_eq!(executor.exec("lrem list -2 hello".to_string()), Ok("2".to_string()));
        assert_eq!(executor.exec("llen list".to_string()), Ok("2".to_string()));
    }

    #[test]
    fn lindex_works_as_expected() {
        let executor = crate::redis_engine::setup_executor(true);

        assert_eq!(executor.exec("rpush list hello".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("rpush list foo".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("rpush list hello2".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("rpush list hello3".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("llen list".to_string()), Ok("4".to_string()));

        assert_eq!(executor.exec("lindex list 1".to_string()), Ok("\"foo\"".to_string()));
        assert_eq!(executor.exec("lindex list 2".to_string()), Ok("\"hello2\"".to_string()));
        assert_eq!(executor.exec("lindex list -1".to_string()), Ok("\"hello3\"".to_string()));
        assert_eq!(executor.exec("lindex list -2".to_string()), Ok("\"hello2\"".to_string()));
    }

    #[test]
    fn lpop_works_as_expected() {
        let executor = crate::redis_engine::setup_executor(true);

        assert_eq!(executor.exec("rpush list hello".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("rpush list foo".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("rpush list hello2".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("rpush list hello3".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("llen list".to_string()), Ok("4".to_string()));

        assert_eq!(executor.exec("lpop list asd".to_string()), Err("[ERROR]: count value is not an integer. Usage: lpop LISTNAME [COUNT]".to_string()));
        assert_eq!(executor.exec("lpop list".to_string()), Ok("hello".to_string()));
        assert_eq!(executor.exec("lpop list 2".to_string()), Ok("[\"foo\", \"hello2\"]".to_string()));
        assert_eq!(executor.exec("llen list".to_string()), Ok("1".to_string()));
    }

    #[test]
    fn rpop_works_as_expected() {
        let executor = crate::redis_engine::setup_executor(true);

        assert_eq!(executor.exec("rpush list hello".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("rpush list foo".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("rpush list hello2".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("rpush list hello3".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("llen list".to_string()), Ok("4".to_string()));

        assert_eq!(executor.exec("rpop list asd".to_string()), Err("[ERROR]: count value is not an integer. Usage: rpop LISTNAME [COUNT]".to_string()));
        assert_eq!(executor.exec("rpop list".to_string()), Ok("hello3".to_string()));
        assert_eq!(executor.exec("rpop list 2".to_string()), Ok("[\"hello2\", \"foo\"]".to_string()));
        assert_eq!(executor.exec("llen list".to_string()), Ok("1".to_string()));
    }

    #[test]
    fn lpush_works_as_expected() {
        let executor = crate::redis_engine::setup_executor(true);

        assert_eq!(executor.exec("lpush list hello".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("lpush list foo".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("lpush list hello2".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("lpush list hello3".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("llen list".to_string()), Ok("4".to_string()));
    }

    #[test]
    fn lset_works_as_expected() {
        let executor = crate::redis_engine::setup_executor(true);

        assert_eq!(executor.exec("lpush list hello".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("lpush list foo".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("lset list 0 hello2".to_string()), Ok("Ok".to_string()));
        assert_eq!(executor.exec("lset list -2 hello3".to_string()), Ok("Ok".to_string()));
        assert_eq!(executor.exec("llen list".to_string()), Ok("2".to_string()));
        assert_eq!(executor.exec("lindex list 0".to_string()), Ok("\"hello3\"".to_string()));
    }

    #[test]
    fn hget_and_exists_work_as_expected() {
        let executor = crate::redis_engine::setup_executor(true);

        assert_eq!(executor.exec("hexists hash2 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash1 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash3 asd".to_string()), Ok("0".to_string()));
        assert_eq!(executor.exec("hget hash1 name".to_string()), Ok("Petar".to_string()));
        assert_eq!(executor.exec("hexists hash1 age".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hget hash1 age".to_string()), Ok("22".to_string()));
        assert_eq!(executor.exec("hget hash2 name".to_string()), Ok("this is a new hash".to_string()));
    }

    #[test]
    fn hdel_works_as_expected() {
        let executor = crate::redis_engine::setup_executor(true);

        assert_eq!(executor.exec("hexists hash2 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash1 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash1 age".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hdel hash1 name age".to_string()), Ok("2".to_string()));
        assert_eq!(executor.exec("hexists hash1 name".to_string()), Ok("0".to_string()));
        assert_eq!(executor.exec("hexists hash1 age".to_string()), Ok("0".to_string()));
        assert_eq!(executor.exec("hdel hash2 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash2 name".to_string()), Ok("0".to_string()));
    }

    #[test]
    fn hgetall_works_as_expected() {
        let executor = crate::redis_engine::setup_executor(true);

        assert_eq!(executor.exec("hexists hash2 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash1 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash1 age".to_string()), Ok("1".to_string()));

        // this is needed since hgetall will return the key-values array in an arbitrary element order
        // to see why read more here: https://doc.rust-lang.org/std/collections/struct.HashMap.html#method.iter
        let val = executor.exec("hgetall hash1".to_string());

        if val == Ok("[\"age\", \"22\", \"name\", \"Petar\"]".to_string()) ||
            val == Ok("[\"name\", \"Petar\", \"age\", \"22\"]".to_string()) {
                assert!(true);
            } else {
                assert!(false);
            }

        assert_eq!(executor.exec("hgetall hash2".to_string()), Ok("[\"name\", \"this is a new hash\"]".to_string()));
        assert_eq!(executor.exec("hgetall doesnt_exist".to_string()), Ok("[]".to_string()));
    }

    #[test]
    fn hkeys_works_as_expected() {
        let executor = crate::redis_engine::setup_executor(true);

        assert_eq!(executor.exec("hexists hash2 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash1 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash1 age".to_string()), Ok("1".to_string()));

        // this is needed since hkeys will return the key-values array in an arbitrary element order
        // to see why read more here: https://doc.rust-lang.org/std/collections/struct.HashMap.html#method.iter
        let val = executor.exec("hkeys hash1".to_string());

        if val == Ok("[\"age\", \"name\"]".to_string()) ||
            val == Ok("[\"name\", \"age\"]".to_string()) {
                assert!(true);
            } else {
                assert!(false);
            }

        assert_eq!(executor.exec("hkeys hash2".to_string()), Ok("[\"name\"]".to_string()));
        assert_eq!(executor.exec("hkeys doesnt_exist".to_string()), Ok("[]".to_string()));
    }

    #[test]
    fn hvals_works_as_expected() {
        let executor = crate::redis_engine::setup_executor(true);

        assert_eq!(executor.exec("hexists hash2 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash1 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash1 age".to_string()), Ok("1".to_string()));

        // this is needed since hkeys will return the key-values array in an arbitrary element order
        // to see why read more here: https://doc.rust-lang.org/std/collections/struct.HashMap.html#method.iter
        let val = executor.exec("hvals hash1".to_string());

        if val == Ok("[\"22\", \"Petar\"]".to_string()) ||
            val == Ok("[\"Petar\", \"22\"]".to_string()) {
                assert!(true);
            } else {
                assert!(false);
            }

        assert_eq!(executor.exec("hvals hash2".to_string()), Ok("[\"this is a new hash\"]".to_string()));
        assert_eq!(executor.exec("hvals doesnt_exist".to_string()), Ok("[]".to_string()));
    }

    #[test]
    fn hlen_works_as_expected() {
        let executor = crate::redis_engine::setup_executor(true);

        assert_eq!(executor.exec("hexists hash2 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash1 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash1 age".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hlen hash1".to_string()), Ok("2".to_string()));
        assert_eq!(executor.exec("hlen hash2".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hlen doesnt_exist".to_string()), Ok("0".to_string()));
    }

    #[test]
    fn hset_works_as_expected() {
        let executor = crate::redis_engine::setup_executor(true);

        assert_eq!(executor.exec("hexists hash2 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash1 name".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hexists hash1 age".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hmset hash1 name \"John\" age 25".to_string()), Ok("2".to_string()));
        assert_eq!(executor.exec("hset hash2 newfield 69".to_string()), Ok("1".to_string()));
        assert_eq!(executor.exec("hlen hash2".to_string()), Ok("2".to_string()));
        assert_eq!(executor.exec("hget hash2 newfield".to_string()), Ok("69".to_string()));
        assert_eq!(executor.exec("hget hash1 name".to_string()), Ok("\"John\"".to_string()));
    }
}
