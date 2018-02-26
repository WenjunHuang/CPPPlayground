extern crate libc;

extern {
    fn say_hello();
}

fn main(){
    unsafe{
        say_hello();
    }
}