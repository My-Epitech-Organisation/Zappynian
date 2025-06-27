/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for main.cpp functionality
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <iostream>
#include <sstream>

// Mock the main function for testing argument parsing logic
namespace MainTest {
    int test_main(int argc, char **argv) {
        if (argc < 5) {
            std::cerr << "Usage: " << argv[0] << " -p <port> -h <host>\n";
            return -1;
        }
        if (std::string(argv[1]) != "-p" || std::string(argv[3]) != "-h") {
            std::cerr << "Usage: " << argv[0] << " -p <port> -h <host>\n";
            return -1;
        }
        if (std::string(argv[2]).empty() || std::string(argv[4]).empty()) {
            std::cerr << "Host and port must not be empty.\n";
            return -1;
        }
        
        int port = std::stoi(argv[2]);
        if (port <= 0 || port > 65535) {
            std::cerr << "Port must be between 1 and 65535.\n";
            return -1;
        }
        
        if (argc > 5) {
            std::cerr << "Too many arguments.\n";
            return -1;
        }
        
        // If we get here, arguments are valid
        return 0;
    }
}

TestSuite(main_args);

Test(main_args, valid_arguments) {
    char* argv[] = {
        (char*)"./zappy_gui",
        (char*)"-p",
        (char*)"4242",
        (char*)"-h",
        (char*)"localhost"
    };
    int argc = 5;
    
    int result = MainTest::test_main(argc, argv);
    cr_assert_eq(result, 0, "Valid arguments should return 0");
}

Test(main_args, too_few_arguments) {
    char* argv[] = {
        (char*)"./zappy_gui",
        (char*)"-p",
        (char*)"4242"
    };
    int argc = 3;
    
    cr_redirect_stderr();
    int result = MainTest::test_main(argc, argv);
    cr_assert_eq(result, -1, "Too few arguments should return -1");
}

Test(main_args, wrong_flag_order) {
    char* argv[] = {
        (char*)"./zappy_gui",
        (char*)"-h",
        (char*)"localhost",
        (char*)"-p",
        (char*)"4242"
    };
    int argc = 5;
    
    cr_redirect_stderr();
    int result = MainTest::test_main(argc, argv);
    cr_assert_eq(result, -1, "Wrong flag order should return -1");
}

Test(main_args, empty_port) {
    char* argv[] = {
        (char*)"./zappy_gui",
        (char*)"-p",
        (char*)"",
        (char*)"-h",
        (char*)"localhost"
    };
    int argc = 5;
    
    cr_redirect_stderr();
    int result = MainTest::test_main(argc, argv);
    cr_assert_eq(result, -1, "Empty port should return -1");
}

Test(main_args, empty_host) {
    char* argv[] = {
        (char*)"./zappy_gui",
        (char*)"-p",
        (char*)"4242",
        (char*)"-h",
        (char*)""
    };
    int argc = 5;
    
    cr_redirect_stderr();
    int result = MainTest::test_main(argc, argv);
    cr_assert_eq(result, -1, "Empty host should return -1");
}

Test(main_args, port_zero) {
    char* argv[] = {
        (char*)"./zappy_gui",
        (char*)"-p",
        (char*)"0",
        (char*)"-h",
        (char*)"localhost"
    };
    int argc = 5;
    
    cr_redirect_stderr();
    int result = MainTest::test_main(argc, argv);
    cr_assert_eq(result, -1, "Port 0 should return -1");
}

Test(main_args, port_negative) {
    char* argv[] = {
        (char*)"./zappy_gui",
        (char*)"-p",
        (char*)"-1",
        (char*)"-h",
        (char*)"localhost"
    };
    int argc = 5;
    
    cr_redirect_stderr();
    int result = MainTest::test_main(argc, argv);
    cr_assert_eq(result, -1, "Negative port should return -1");
}

Test(main_args, port_too_high) {
    char* argv[] = {
        (char*)"./zappy_gui",
        (char*)"-p",
        (char*)"65536",
        (char*)"-h",
        (char*)"localhost"
    };
    int argc = 5;
    
    cr_redirect_stderr();
    int result = MainTest::test_main(argc, argv);
    cr_assert_eq(result, -1, "Port too high should return -1");
}

Test(main_args, port_max_valid) {
    char* argv[] = {
        (char*)"./zappy_gui",
        (char*)"-p",
        (char*)"65535",
        (char*)"-h",
        (char*)"localhost"
    };
    int argc = 5;
    
    int result = MainTest::test_main(argc, argv);
    cr_assert_eq(result, 0, "Port 65535 should be valid");
}

Test(main_args, port_min_valid) {
    char* argv[] = {
        (char*)"./zappy_gui",
        (char*)"-p",
        (char*)"1",
        (char*)"-h",
        (char*)"localhost"
    };
    int argc = 5;
    
    int result = MainTest::test_main(argc, argv);
    cr_assert_eq(result, 0, "Port 1 should be valid");
}

Test(main_args, too_many_arguments) {
    char* argv[] = {
        (char*)"./zappy_gui",
        (char*)"-p",
        (char*)"4242",
        (char*)"-h",
        (char*)"localhost",
        (char*)"extra"
    };
    int argc = 6;
    
    cr_redirect_stderr();
    int result = MainTest::test_main(argc, argv);
    cr_assert_eq(result, -1, "Too many arguments should return -1");
}

Test(main_args, different_valid_hosts) {
    char* valid_hosts[][5] = {
        {(char*)"./zappy_gui", (char*)"-p", (char*)"4242", (char*)"-h", (char*)"127.0.0.1"},
        {(char*)"./zappy_gui", (char*)"-p", (char*)"4242", (char*)"-h", (char*)"192.168.1.1"},
        {(char*)"./zappy_gui", (char*)"-p", (char*)"4242", (char*)"-h", (char*)"example.com"}
    };
    
    for (int i = 0; i < 3; i++) {
        int result = MainTest::test_main(5, valid_hosts[i]);
        cr_assert_eq(result, 0, "Valid host should return 0");
    }
}

Test(main_args, different_valid_ports) {
    char* valid_ports[][5] = {
        {(char*)"./zappy_gui", (char*)"-p", (char*)"1024", (char*)"-h", (char*)"localhost"},
        {(char*)"./zappy_gui", (char*)"-p", (char*)"8080", (char*)"-h", (char*)"localhost"},
        {(char*)"./zappy_gui", (char*)"-p", (char*)"12345", (char*)"-h", (char*)"localhost"}
    };
    
    for (int i = 0; i < 3; i++) {
        int result = MainTest::test_main(5, valid_ports[i]);
        cr_assert_eq(result, 0, "Valid port should return 0");
    }
}
