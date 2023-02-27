mod my_tokio;

fn main() -> core::result::Result<(), String>{

    let my_tokio = my_tokio::MyTokio::new();

    my_tokio.spawn(async {
        println!("hello world!");
    })?;

    my_tokio.spawn(async {
        println!("Task 1");
    })?;

    my_tokio.spawn(async {
        println!("Task 2");
    })?;

    my_tokio.spawn(async {
        println!("Task 3");
    })?;

    println!("Run my tokio");

    my_tokio.spawn(async {
        println!("Goodbye world!");
    })?;

    my_tokio.run()
}
