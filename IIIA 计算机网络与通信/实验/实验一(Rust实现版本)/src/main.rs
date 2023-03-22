use std::fs;
use std::io::prelude::*;
use std::net::TcpListener;
use std::net::TcpStream;
use regex::Regex;
use std::thread;
use std::sync::mpsc;
use std::sync::Arc;
use std::sync::Mutex;

// 运行的方法:
// cargo run
// 然后再浏览器输入your IP addreess:7878即可
pub struct ThreadPool {
    workers: Vec<Worker>,
    sender: mpsc::Sender<Job>,
}

impl ThreadPool {
    // --snip--
    pub fn new(size: usize) -> ThreadPool {
        assert!(size > 0);
    
        let (sender, receiver) = mpsc::channel();
    
        let receiver = Arc::new(Mutex::new(receiver));
    
        let mut workers = Vec::with_capacity(size);
    
        for id in 0..size {
            workers.push(Worker::new(id, Arc::clone(&receiver)));
        }
    
        ThreadPool {
            workers,
            sender,
        }
    }
    

}
    
struct Worker {
    id: usize,
    thread: thread::JoinHandle<()>,
}
    
impl Worker {
    fn new(id: usize, receiver: Arc<Mutex<mpsc::Receiver<Job>>>) -> Worker {
        let thread = thread::spawn(move || {
            loop {
                let job = receiver.lock().unwrap().recv().unwrap();

                println!("Worker {} got a job; executing.", id);

                job();
            }
        });

        Worker {
            id,
            thread,
        }
    }
}
type Job = Box<dyn FnOnce() + Send + 'static>;

impl ThreadPool {
    // --snip--

    pub fn execute<F>(&self, f: F)
        where
            F: FnOnce() + Send + 'static
    {
        let job = Box::new(f);

        self.sender.send(job).unwrap();
    }
}
fn main() {
    
    start();
    
}

fn start(){
    // TODO2:完成你的IP地址的查找
    let listener = TcpListener::bind("10.211.55.2:7878").unwrap();

    let pool = ThreadPool::new(4);

    for stream in listener.incoming() {
        let stream = stream.unwrap();

        pool.execute(|| {
            handle_connection(stream);
        });
    }
}

fn handle_connection(mut stream: TcpStream) {
    let mut buffer = [0; 1024];

    stream.read(&mut buffer).unwrap();

    let get = b"GET / HTTP/1.1\r\n";
    // TODO1:完成你的正则表达式
    let re = Regex::new("([A-Za-z]+) +(.*) +(HTTP/[0-9][.][0-9])").unwrap();

    let str = String::from_utf8_lossy(&buffer);

    if buffer.starts_with(get) {
        let contents = fs::read_to_string("hello.html").unwrap();
        
        let response = format!(
            "HTTP/1.1 200 OK\r\nContent-Length: {}\r\n\r\n{}",
            contents.len(),
            contents
        );

        stream.write(response.as_bytes()).unwrap();
        stream.flush().unwrap();
    } else {
        for cap in re.captures_iter(&*str){
            let path = &cap[2][1..];
            println!("path:{}",path);
            match fs::read(path){
                Ok(s) => {
                    if path.ends_with(".png"){
                        let response = format!("HTTP/1.1 200 OK\r\nContent-Length: {}\r\nContent-Type:image/x-icon\r\n\r\n", s.len());
                        stream.write(response.as_bytes()).unwrap();
                        stream.write_all(&s[..]).unwrap();
                        stream.flush().unwrap();
                    }
                    else{
                        let response = format!("HTTP/1.1 200 OK\r\nContent-Length: {}\r\n\r\n", s.len());
                        stream.write(response.as_bytes()).unwrap();
                        stream.write_all(&s[..]).unwrap();
                        stream.flush().unwrap();
                    }
                    
                }
                Err(s) => {
                    let status_line = "HTTP/1.1 404 NOT FOUND";
                    let contents = fs::read_to_string("404.html").unwrap();

                    let response = format!(
                        "{}\r\nContent-Length: {}\r\n\r\n{}",
                        status_line,
                        contents.len(),
                        contents
                    );

                    stream.write(response.as_bytes()).unwrap();
                    stream.flush().unwrap();
                }
            }
        }
        
    }
}