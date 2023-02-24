#include <fmt/core.h>

void wrong_input() {
    fmt::print("\nPlease specify a command. Available commands are as follows:\n\n\tinit - Init a templtr project\n\tbuild <outdir> - Build project files to specified directory\n\n");
}

int init() {
    fmt::print("Init");
    return 0;
}

int build(const char* outdir) {
    fmt::print("Build");
    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        wrong_input();
        return -1;
    }

    if (std::strcmp(argv[1], "init") == 0) {
        init(); 
    } else if (std::strcmp(argv[1], "build") == 0) {
        if (argc == 3) {
            build(argv[2]); 
        } else {
            wrong_input();
        }
    } else {
        wrong_input();
        return -1;       
    }

    return 0;
}