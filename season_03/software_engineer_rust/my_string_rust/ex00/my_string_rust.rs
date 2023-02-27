struct MyStringRust {
    buff: Vec<char>,
}

impl MyStringRust {
    fn new() -> Self {
        Self { buff: Vec::new() }        
    }

    fn len(&self) -> usize {
        self.buff.len()
    }

    fn push(&mut self, c: char) {
        self.buff.push(c)
    }

    fn push_str(&mut self, text: &str) {
        for x in text.chars() {
            self.push(x)
        }
    }
}

impl From<&str> for MyStringRust {
    fn from(value: &str) -> Self {
        Self {
            buff: value.chars().collect()
        }
    }
}

impl std::fmt::Display for MyStringRust {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        for x in &self.buff {
            write!(f, "{}", x).unwrap()
        }
        Ok(())
    }
}

impl PartialEq for MyStringRust {
    fn eq(&self, other: &Self) -> bool {
        self.buff == other.buff
    }
}

trait ToMyStringRust<'a> {
    fn to_my_string_rust(&'a self) -> MyStringRust;
}

impl<'a> ToMyStringRust<'a> for &'a str {
	fn to_my_string_rust(&'a self) -> MyStringRust {
        MyStringRust::from(*self)
    }
}

impl Clone for MyStringRust {
    fn clone(&self) -> Self {
        Self {
            buff: self.buff.clone()
        }
    }
}

impl std::ops::Add for MyStringRust {
    type Output = Self;

    fn add(mut self, other: Self) -> Self {
        for x in other.buff {
            self.push(x)
        }

        self
    }
}

impl std::iter::FromIterator<char> for MyStringRust {
    fn from_iter<I: std::iter::IntoIterator<Item=char>>(iter: I) -> Self {
        let mut c = MyStringRust::new();

        for i in iter {
            c.push(i);
        }

        c
    }
}

// To test the code just uncomment this block:
/*
fn main() {
    
    {
        println!("## CONSTRUCTOR TEST");

        println!("default constructor: ");
        let s1 = MyStringRust::new();

        println!("{}", s1.len() == 0);

        println!("&str from constructor: ");
        let s2 = MyStringRust::from("Trait PartialEq");

        println!("s2 'hello' = {}?", s2);

        println!("Trait PartialEq: ");
        println!("{}", s2 == "Trait PartialEq".to_my_string_rust());

        let mut s3 = MyStringRust::new();

        println!("push_str(): ");
        s3.push_str("push_str");
        println!("{}", s3 == "push_str".to_my_string_rust());

        println!("Copy string: ");
        let s4 = s2.clone();

        println!("{}", s4 == s2);
    }


    {
        println!("## SIZE");

        let u = "qwasar creating stars".to_my_string_rust();

        println!("size: ");
        println!("{}", u.len() == 21);
    }

    {
        println!("## OPERATOR+");

        let u = "qwasar ".to_my_string_rust();
        let v = "creating stars".to_my_string_rust();
        let w = u + v;

        println!("operator+: ");
        println!("{}", w == "qwasar creating stars".to_my_string_rust());
    }
   
}
 */ 