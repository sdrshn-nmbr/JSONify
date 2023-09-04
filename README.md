JSON Parser and Writer
This is a JSON parsing and writing library written in C. It provides functions to parse JSON strings and files into a custom data structure and write JSON data to files. The library supports parsing JSON integers, strings, lists, booleans, and null values.

Table of Contents
Prerequisites
Usage
Examples
Contributing
License
Prerequisites
To use this library, you need:

A C compiler (e.g., GCC)
Basic knowledge of C programming
Usage
Clone the repository:

shell
Copy code
git clone https://github.com/yourusername/json-parser.git
Include the necessary header files in your C code:

c
Copy code
#include "json.h"
Compile your C program with the library source files:

shell
Copy code
gcc your_program.c json.c -o your_program
Use the functions provided in json.c to parse and manipulate JSON data.

Examples
Here's a simple example of how to use this library to parse a JSON file and access its elements in your C code:

c
Copy code
#include "json.h"

int main() {
    Element root_element;
    const char *filename = "example.json";

    if (read_json(filename, &root_element)) {
        // JSON parsing successful
        // Access and manipulate JSON elements using the 'root_element' object
        // For example:
        // Element *value = get_element(root_element.as_object, "key");
        // Access the value as value->as_int or value->as_string, etc.
    } else {
        // JSON parsing failed
        fprintf(stderr, "Failed to parse JSON from %s\n", filename);
    }

    return 0;
}
Contributing
Contributions are welcome! If you'd like to contribute to this project, please follow these steps:

Fork the repository on GitHub.
Create a new branch for your feature or bug fix.
Make your changes and test them thoroughly.
Create a pull request to merge your changes into the main branch.
