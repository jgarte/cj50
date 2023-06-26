#include <cj50.h>

float average(const float* vs, size_t len) {
    float total = 0;
    for (size_t i = 0; i < len; i++) {
        total += vs[i];
    }
    return total / len;
}


int main() {
    if (!getenv("NONAME")) {
        print_string("What is your name? ");
        string name = get_string();
        print_string("Hi ");
        print_string(name);
        print_string("!\n");
        print_string("What is your age? ");
        nat0 age = get_nat0();
        print_string("In a year, you will be ");
        print_nat0(age + 1);
        print_string("!\n");
        drop(name);
    }

    if (!getenv("NOFLOAT")) {
        print_string("Width: ");
        float w = get_float();
        print_string("Height: ");
        float h = get_float();
        print_string("Area = ");
        print_float(w * h);
        print_string("\n");
    }

    if (!getenv("NOARRAY")) {
        print_string("How many tests have you done? ");
        nat0 n = get_nat0();
        float* grades = new_floats(n);
        for (int i = 0; i < n; i++) {
            print_string("What was your grade for test no. ");
            print_int(i + 1);
            print_string("? ");
            float grade = get_float();
            grades[i] = grade;
        }
        float avg = average(grades, n);
        print_string("average(");
        print_debug_floats(grades, n);
        print_string(") = ");
        print_float(avg);
        print_string("\n");
        drop(grades);
    }

    if (!getenv("NONAMES")) {
        print_string("How many people do we have? ");
        nat0 n = get_nat0();
        string* ss = new_strings(n);
        for (int i = 0; i < n; i++) {
            print_string("What is the name of person no. ");
            print_int(i + 1);
            print_string("? ");
            ss[i] = get_string();
        }
        print_string("Our people are:\n");
        print_debug_strings(ss, n);
        print_string("\n");
        drop_array(ss, n);
    }
}
