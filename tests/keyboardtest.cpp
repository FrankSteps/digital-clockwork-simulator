<<<<<<< HEAD
=======
/*

*/

>>>>>>> feat/alarm-system
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>


int main(){
    int fd = open("/dev/input/event4", O_RDONLY | O_NONBLOCK);

    if(fd < 0){
<<<<<<< HEAD
        std::cerr << "error\n";
        return EXIT_FAILURE;
=======
        std::cerr << "\n";
        return 1;
>>>>>>> feat/alarm-system
    }

    struct libevdev* dev = nullptr;
    libevdev_new_from_fd(fd, &dev);


    while(true){
        struct input_event ev;

        int rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);

        if(rc == 0 && ev.type == EV_KEY){
            if(ev.value == 1){
<<<<<<< HEAD
                std::cout << "Pressionado: " << libevdev_event_code_get_name(EV_KEY, ev.code) << "\n";
            } else if(ev.value == 0) {
                std::cout << "Solto: "       << libevdev_event_code_get_name(EV_KEY, ev.code) << "\n";
=======
                std::cout << "Pressed: " << libevdev_event_code_get_name(EV_KEY, ev.code) << "\n";
            } else if(ev.value == 0) {
                std::cout << ": "       << libevdev_event_code_get_name(EV_KEY, ev.code) << "\n";
>>>>>>> feat/alarm-system
            }
        }
    }

    libevdev_free(dev);
    close(fd);

<<<<<<< HEAD
    return EXIT_SUCCESS;
=======
    return 0;
>>>>>>> feat/alarm-system
}