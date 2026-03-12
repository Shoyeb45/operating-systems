# include<iostream>
# include<thread>
# include<string>
# include<system_error>

static volatile int counter = 0;

void task(std::string args) {
    std::cout << "Begin: " << args << "\n";
    int i;
    for (i = 0; i < 1e7; i++) {
        counter = counter + 1;
    }
    std::cout << "Done: " << args << "\n"; 
}

int main() {
    try
    {
        std::cout << "Main begin, counter = " << counter << "\n";
        std::thread thread1(task, "A");
        std::thread thread2(task, "B");

        thread1.join();
        thread2.join();

        std::cout << "Main Done with both, counter = " << counter << "\n";
    }
    catch(std::system_error const& err)
    {
        std::cerr << err.what() << '\n';
        return -1;
    }
    
    return 0;
}