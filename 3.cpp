#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>

#include <fcntl.h>
#include <sys/ioctl.h>

using namespace std ;

class GPIO
{
    private :
        string default_path = "/sys/class/gpio/" ;
        string path, export_path, unexport_path ;
        int num ;
        string num_str ;
        int f_temp, status ;
        int gpio_export() ;
        int gpio_unexport() ;
    public :
        GPIO(int init_num) ;
        int write_value(string value) ;
        int set_dir(string dir) ;
        ~GPIO() ;
} ;

GPIO::GPIO(int init_num) {
    path = default_path + "gpio" + to_string(init_num) ;
    num = init_num ;
    num_str = to_string(num) ;
    gpio_export() ;
}

GPIO::~GPIO() {
    gpio_unexport() ;
}

int GPIO::write_value(string value) {
    string value_path = path + "/value" ;
    f_temp = open(value_path.c_str(), O_WRONLY  | O_CLOEXEC) ;
    if (f_temp < 0) return f_temp ; // 파일 열기
    
    status = write(f_temp, value.c_str(), value.length()) ;
    if (status < 0) return status ; // 파일에 값 쓰기
    
    close(f_temp) ; // 파일 닫기
    return 0 ;
}


int GPIO::set_dir(string dir) {
    string dir_path = path + "/direction" ;
    f_temp = open(dir_path.c_str(), O_WRONLY  | O_CLOEXEC) ;
    cout << "f " << f_temp << endl ;
    if (f_temp < 0) return f_temp ; // 파일 열기
    
    status = write(f_temp, dir.c_str(), dir.length()) ;
    cout << "fs " << status << endl ;
    if (status < 0) return status ; // 파일에 값 쓰기
    
    close(f_temp) ; // 파일 닫기
    return 0 ;
}


int GPIO::gpio_export() {
    export_path = default_path + "export" ;
    
    f_temp = open(export_path.c_str(), O_WRONLY  | O_CLOEXEC) ;
    if (f_temp < 0) return f_temp ; // 파일 열기
    
    status = write(f_temp, num_str.c_str(), num_str.length()) ;
    if (status < 0) return status ; // 파일에 값 쓰기
    
    close(f_temp) ; // 파일 닫기
    return 0 ;
}

int GPIO::gpio_unexport() {
    unexport_path = default_path + "unexport" ;
    
    f_temp = open(unexport_path.c_str(), O_WRONLY  | O_CLOEXEC) ;
    if (f_temp < 0) return f_temp ; // 파일 열기
    
    status = write(f_temp, num_str.c_str(), num_str.length()) ;
    if (status < 0) return status ; // 파일에 값 쓰기
    
    close(f_temp) ; // 파일 닫기
    return 0 ;
}

int main(int argc, char* [])
{
    int led_num = 76 ;
    GPIO led = GPIO(led_num) ;
    led.set_dir("out") ;
    for (int i = 0; i < 10; i++) {
        led.write_value("1") ;
        usleep(500 * 1000) ;
        led.write_value("0") ;
        usleep(500 * 1000) ;
    }
    return 0 ;
}