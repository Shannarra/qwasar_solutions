pub(crate) fn code_is_legal(code: &String) -> bool {
    for c in code.chars() {
        if !('0'..='8').contains(&c) {
            return false;
        }
    }

    code.len() == 4
}

pub(crate) fn generate_code() -> String {
    let mut code = String::new();

    use rand::Rng;

    while code.len() < 4 {
        let c: char = (rand::thread_rng().gen_range(0..9) as u8 + 48) as char;
        if !code.contains(c) {
            code.push(c);
        }
    }

    code
}


pub(crate) fn parse_code(args: &Vec<String>, idx: usize, code: &mut String) -> core::result::Result<(), &'static str> {
    if idx < args.len() -1 {
        if !code_is_legal(&args[idx+1].trim().to_string()) {
            return Err("[ERROR]: Code must consist of ONLY positive UNIQUE numbers and must not contain 9! Example: -c 1234");
        }

        if args[idx+1].len() == 4 {
            *code = args[idx+1].to_string();
            Ok(())
        }
        else {
            Err("[ERROR]: Code must have a length of 4!")
        }
    } else {
        Err("[ERROR]: No code provided for the \"-c\" option")
    }
}

pub(crate) fn parse_tries(args: &Vec<String>, idx: usize, tries: &mut u32) -> core::result::Result<(), &'static str> {
    if idx < args.len() -1 {
        if args[idx+1].parse::<u32>().is_ok() {
            let v = args[idx+1].parse::<u32>().unwrap();
            if v < 1 {
                return Err("[ERROR]: Tries must be a number greater than 0!");
            }
            *tries = v;
            Ok(())
        } else {
            Err("[ERROR]: Only positive whole number must be provided for -t")
        }
    } else {
        Err("[ERROR]: No code provided for the \"-t\" option")
    }
}
