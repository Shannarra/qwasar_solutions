#![allow(clippy::comparison_chain)]

pub mod one_off {
    type Result = super::super::Result;

    pub fn echo(args: Vec<&str>) -> Result {
        if args.is_empty() {
            Err("Too few arguments for echo".to_string())
        } else if args.len() > 1 {
            Err("Too many arguments for echo".to_string())
        } else {
            Ok(args[0].to_string())
        }
    }

    pub fn ping(args: Vec<&str>) -> Result {
        if args.len() == 1 {
            self::echo(args)
        } else if args.is_empty() {
            Ok("pong".to_string())
        } else {
            Err("Too many arguments for echo".to_string())
        }
    }
}
