counter extract

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define DATA_SIZE 768
#define MAX_FIELD_NAME 50
#define MAX_FIELDS 200

typedef struct {
    int start;
    int end;
    char name[MAX_FIELD_NAME];
} FieldInfo;

FieldInfo fields[MAX_FIELDS];
int field_count = 0;

void initialize_fields() {
    // Header
    fields[field_count++] = (FieldInfo){0, 1, "Header"};
    fields[field_count++] = (FieldInfo){2, 2, "Measurement counter"};
    fields[field_count++] = (FieldInfo){3, 3, "Navigation State"};
    
    // Antenna Status
    fields[field_count++] = (FieldInfo){4, 4, "PVT status | Availability"};
    fields[field_count++] = (FieldInfo){5, 5, "No.of satellites GPS used"};
    fields[field_count++] = (FieldInfo){6, 6, "No.of satellites NAVICused"};
    fields[field_count++] = (FieldInfo){7, 7, "GPS Week number"};
    
    // GPS data
    fields[field_count++] = (FieldInfo){10, 17, "GPS TOW"};
    fields[field_count++] = (FieldInfo){18, 25, "Position X"};
    fields[field_count++] = (FieldInfo){26, 33, "Position Y"};
    fields[field_count++] = (FieldInfo){34, 41, "Position Z"};
    fields[field_count++] = (FieldInfo){42, 45, "Velocity X"};
    fields[field_count++] = (FieldInfo){46, 49, "Velocity Y"};
    fields[field_count++] = (FieldInfo){50, 53, "Velocity Z"};
    fields[field_count++] = (FieldInfo){54, 57, "GDOP"};
    fields[field_count++] = (FieldInfo){58, 61, "PDOP"};
    fields[field_count++] = (FieldInfo){62, 65, "Delta Time"};
    
    // NAVIC data
    fields[field_count++] = (FieldInfo){66, 67, "Checksum"};
    fields[field_count++] = (FieldInfo){68, 69, "NAVIC Week number"};
    fields[field_count++] = (FieldInfo){70, 77, "NAVIC TOW"};
    fields[field_count++] = (FieldInfo){78, 114, "Type 22 data"};
    fields[field_count++] = (FieldInfo){115, 130, "SV status of each Channel"};
    fields[field_count++] = (FieldInfo){131, 146, "SV ID Note2"};
    fields[field_count++] = (FieldInfo){147, 162, "C/N0 Note2"};
    fields[field_count++] = (FieldInfo){163, 290, "Pseudorange"};
    
    // Measurements
    fields[field_count++] = (FieldInfo){291, 418, "Deltarange"};
    fields[field_count++] = (FieldInfo){419, 546, "Carrier Phase Measurements"};
    fields[field_count++] = (FieldInfo){547, 554, "Clock bias"};
    fields[field_count++] = (FieldInfo){555, 562, "Clock drift"};
    fields[field_count++] = (FieldInfo){563, 633, "Ephemeris Note2"};
    fields[field_count++] = (FieldInfo){634, 635, "Message reception counter"};
    
    // Debug info
    fields[field_count++] = (FieldInfo){636, 657, "AST debug info for all channel"};
    fields[field_count++] = (FieldInfo){658, 671, "SV IDNote2"};
    fields[field_count++] = (FieldInfo){672, 685, "C/N0 Note2"};
    fields[field_count++] = (FieldInfo){686, 693, "ODP Position X"};
    fields[field_count++] = (FieldInfo){694, 701, "ODP Position Y"};
    fields[field_count++] = (FieldInfo){702, 709, "ODP Position Z"};
    fields[field_count++] = (FieldInfo){710, 713, "Velocity X"};
    fields[field_count++] = (FieldInfo){714, 717, "Velocity Y"};
    fields[field_count++] = (FieldInfo){718, 721, "Velocity Z"};
    
    // Counters and status
    fields[field_count++] = (FieldInfo){722, 729, "Estimated Clock Bias"};
    fields[field_count++] = (FieldInfo){730, 743, "Validation flag"};
    fields[field_count++] = (FieldInfo){744, 745, "SPS Update Counter"};
    fields[field_count++] = (FieldInfo){746, 747, "UART transmission counter"};
    fields[field_count++] = (FieldInfo){748, 749, "UART reception counter"};
    fields[field_count++] = (FieldInfo){750, 750, "UART checksum failure counter"};
    fields[field_count++] = (FieldInfo){751, 751, "I2C IC Rec counter"};
    fields[field_count++] = (FieldInfo){752, 752, "IC counter"};
    fields[field_count++] = (FieldInfo){753, 753, "IC update counter"};
    fields[field_count++] = (FieldInfo){754, 754, "AST reset Counter"};
    fields[field_count++] = (FieldInfo){755, 755, "SMAV71 Reset Counter"};
    fields[field_count++] = (FieldInfo){756, 756, "SPS Reset Counter"};
    fields[field_count++] = (FieldInfo){757, 757, "Reset ID"};
    fields[field_count++] = (FieldInfo){758, 765, "SMAV71 health debug info"};
    fields[field_count++] = (FieldInfo){766, 767, "Checksum"};
}

void display_data(int data[]) {
    printf("\033[2J\033[H");  // Clear screen and move cursor to top
    printf("=== Real-Time Data Display ===\n\n");
    
    for (int i = 0; i < field_count; i++) {
        printf("%s: ", fields[i].name);
        
        for (int j = fields[i].start; j <= fields[i].end; j++) {
            printf("%d ", data[j]);
        }
        printf("\n");
    }
    printf("\nLast updated: %s", ctime(&(time_t){time(NULL)}));
    fflush(stdout);
}

int main() {
    FILE *file;
    int data[DATA_SIZE];
    int row_count = 0;
    
    // Initialize field information
    initialize_fields();
    
    // Open data file
    file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open data file.\n");
        return 1;
    }
    
    // Read and display data
    while (fscanf(file, "%d", &data[0]) != EOF) {
        // Read the rest of the data points
        for (int i = 1; i < DATA_SIZE; i++) {
            if (fscanf(file, "%d", &data[i]) == EOF) {
                printf("Error: Incomplete data row.\n");
                fclose(file);
                return 1;
            }
        }
        
        row_count++;
        printf("Displaying row %d:\n", row_count);
        display_data(data);
        
        sleep(1);  // Wait for 1 second before showing next row
    }
    
    fclose(file);
    printf("End of data file reached. Total rows: %d\n", row_count);
    
    return 0;
}



----------_------------------


convert and then extract

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#define DATA_SIZE 768
#define MAX_FIELD_NAME 50
#define MAX_FIELDS 100
#define LINE_BUFFER_SIZE 5000

typedef struct {
    int start;
    int end;
    char name[MAX_FIELD_NAME];
    int is_float; // 0 for integer, 1 for float, 2 for raw bytes
} FieldInfo;

FieldInfo fields[MAX_FIELDS];
int field_count = 0;

void initialize_fields() {
    // Header
    fields[field_count++] = (FieldInfo){0, 1, "Header", 2};
    fields[field_count++] = (FieldInfo){2, 2, "Measurement counter", 0};
    fields[field_count++] = (FieldInfo){3, 3, "Navigation State", 0};
    
    // Antenna Status
    fields[field_count++] = (FieldInfo){4, 4, "PVT status | Availability", 0};
    fields[field_count++] = (FieldInfo){5, 5, "No.of satellites GPS used", 0};
    fields[field_count++] = (FieldInfo){6, 6, "No.of satellites NAVICused", 0};
    fields[field_count++] = (FieldInfo){7, 7, "GPS Week number", 0};
    
    // GPS data (these are likely floating point values)
    fields[field_count++] = (FieldInfo){10, 17, "GPS TOW", 1};
    fields[field_count++] = (FieldInfo){18, 25, "Position X", 1};
    fields[field_count++] = (FieldInfo){26, 33, "Position Y", 1};
    fields[field_count++] = (FieldInfo){34, 41, "Position Z", 1};
    fields[field_count++] = (FieldInfo){42, 45, "Velocity X", 1};
    fields[field_count++] = (FieldInfo){46, 49, "Velocity Y", 1};
    fields[field_count++] = (FieldInfo){50, 53, "Velocity Z", 1};
    fields[field_count++] = (FieldInfo){54, 57, "GDOP", 1};
    fields[field_count++] = (FieldInfo){58, 61, "PDOP", 1};
    fields[field_count++] = (FieldInfo){62, 65, "Delta Time", 1};
    
    // NAVIC data
    fields[field_count++] = (FieldInfo){66, 67, "Checksum", 2};
    fields[field_count++] = (FieldInfo){68, 69, "NAVIC Week number", 0};
    fields[field_count++] = (FieldInfo){70, 77, "NAVIC TOW", 1};
    fields[field_count++] = (FieldInfo){78, 114, "Type 22 data", 2};
    fields[field_count++] = (FieldInfo){115, 130, "SV status of each Channel", 2};
    fields[field_count++] = (FieldInfo){131, 146, "SV ID Note2", 2};
    fields[field_count++] = (FieldInfo){147, 162, "C/N0 Note2", 2};
    fields[field_count++] = (FieldInfo){163, 290, "Pseudorange", 2};
    
    // Measurements
    fields[field_count++] = (FieldInfo){291, 418, "Deltarange", 2};
    fields[field_count++] = (FieldInfo){419, 546, "Carrier Phase Measurements", 2};
    fields[field_count++] = (FieldInfo){547, 554, "Clock bias", 1};
    fields[field_count++] = (FieldInfo){555, 562, "Clock drift", 1};
    fields[field_count++] = (FieldInfo){563, 633, "Ephemeris Note2", 2};
    fields[field_count++] = (FieldInfo){634, 635, "Message reception counter", 0};
    
    // Debug info
    fields[field_count++] = (FieldInfo){636, 657, "AST debug info for all channel", 2};
    fields[field_count++] = (FieldInfo){658, 671, "SV IDNote2", 2};
    fields[field_count++] = (FieldInfo){672, 685, "C/N0 Note2", 2};
    fields[field_count++] = (FieldInfo){686, 693, "ODP Position X", 1};
    fields[field_count++] = (FieldInfo){694, 701, "ODP Position Y", 1};
    fields[field_count++] = (FieldInfo){702, 709, "ODP Position Z", 1};
    fields[field_count++] = (FieldInfo){710, 713, "Velocity X", 1};
    fields[field_count++] = (FieldInfo){714, 717, "Velocity Y", 1};
    fields[field_count++] = (FieldInfo){718, 721, "Velocity Z", 1};
    
    // Counters and status
    fields[field_count++] = (FieldInfo){722, 729, "Estimated Clock Bias", 1};
    fields[field_count++] = (FieldInfo){730, 743, "Validation flag", 2};
    fields[field_count++] = (FieldInfo){744, 745, "SPS Update Counter", 0};
    fields[field_count++] = (FieldInfo){746, 747, "UART transmission counter", 0};
    fields[field_count++] = (FieldInfo){748, 749, "UART reception counter", 0};
    fields[field_count++] = (FieldInfo){750, 750, "UART checksum failure counter", 0};
    fields[field_count++] = (FieldInfo){751, 751, "I2C IC Rec counter", 0};
    fields[field_count++] = (FieldInfo){752, 752, "IC counter", 0};
    fields[field_count++] = (FieldInfo){753, 753, "IC update counter", 0};
    fields[field_count++] = (FieldInfo){754, 754, "AST reset Counter", 0};
    fields[field_count++] = (FieldInfo){755, 755, "SMAV71 Reset Counter", 0};
    fields[field_count++] = (FieldInfo){756, 756, "SPS Reset Counter", 0};
    fields[field_count++] = (FieldInfo){757, 757, "Reset ID", 0};
    fields[field_count++] = (FieldInfo){758, 765, "SMAV71 health debug info", 2};
    fields[field_count++] = (FieldInfo){766, 767, "Checksum", 2};
}

// Convert hex string to byte
unsigned char hex_to_byte(const char *hex) {
    unsigned char byte = 0;
    sscanf(hex, "%2hhx", &byte);
    return byte;
}

// Convert little-endian bytes to integer
int32_t bytes_to_int(const unsigned char *bytes, int count) {
    int32_t value = 0;
    for (int i = 0; i < count; i++) {
        value |= (bytes[i] << (8 * i));
    }
    return value;
}

// Convert little-endian bytes to float
float bytes_to_float(const unsigned char *bytes, int count) {
    if (count != 4) return 0.0f; // Only support 4-byte floats
    
    union {
        float f;
        uint32_t i;
    } converter;
    
    converter.i = 0;
    for (int i = 0; i < count; i++) {
        converter.i |= (bytes[i] << (8 * i));
    }
    
    return converter.f;
}

// Convert little-endian bytes to double
double bytes_to_double(const unsigned char *bytes, int count) {
    if (count != 8) return 0.0; // Only support 8-byte doubles
    
    union {
        double d;
        uint64_t i;
    } converter;
    
    converter.i = 0;
    for (int i = 0; i < count; i++) {
        converter.i |= ((uint64_t)bytes[i] << (8 * i));
    }
    
    return converter.d;
}

void display_data(const unsigned char *data) {
    printf("\033[2J\033[H");  // Clear screen and move cursor to top
    printf("=== Real-Time Data Display ===\n\n");
    
    for (int i = 0; i < field_count; i++) {
        int start = fields[i].start;
        int end = fields[i].end;
        int count = end - start + 1;
        
        printf("%s: ", fields[i].name);
        
        if (fields[i].is_float == 0) { // Integer
            int32_t value = bytes_to_int(&data[start], count);
            printf("%d", value);
        } 
        else if (fields[i].is_float == 1) { // Float/Double
            if (count == 4) {
                float value = bytes_to_float(&data[start], count);
                printf("%f", value);
            } 
            else if (count == 8) {
                double value = bytes_to_double(&data[start], count);
                printf("%lf", value);
            } 
            else {
                printf("[Unsupported float size: %d bytes]", count);
            }
        } 
        else { // Raw bytes
            for (int j = start; j <= end; j++) {
                printf("%02X ", data[j]);
            }
        }
        printf("\n");
    }
    printf("\nLast updated: %s", ctime(&(time_t){time(NULL)}));
    fflush(stdout);
}

int main() {
    FILE *file;
    unsigned char data[DATA_SIZE];
    char line[LINE_BUFFER_SIZE];
    char *token;
    int row_count = 0;
    int byte_index;
    
    // Initialize field information
    initialize_fields();
    
    // Open data file
    file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open data file.\n");
        return 1;
    }
    
    // Read and display data
    while (fgets(line, LINE_BUFFER_SIZE, file) != NULL) {
        byte_index = 0;
        token = strtok(line, " \t\n");
        
        while (token != NULL && byte_index < DATA_SIZE) {
            if (strlen(token) == 2) { // Expecting 2-character hex values
                data[byte_index++] = hex_to_byte(token);
            }
            token = strtok(NULL, " \t\n");
        }
        
        if (byte_index != DATA_SIZE) {
            printf("Warning: Row %d has only %d bytes (expected %d)\n", 
                   row_count + 1, byte_index, DATA_SIZE);
        }
        
        row_count++;
        printf("Displaying row %d:\n", row_count);
        display_data(data);
        
        sleep(1);  // Wait for 1 second before showing next row
    }
    
    fclose(file);
    printf("End of data file reached. Total rows: %d\n", row_count);
    
    return 0;
}



---------------------+++++++++++++++++++++-------------


bytes direct extract


I understand. You want to directly display the raw byte data without any conversion. Here's the simplified C code that extracts and displays the byte data as-is:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define DATA_SIZE 768
#define MAX_FIELD_NAME 50
#define MAX_FIELDS 100
#define LINE_BUFFER_SIZE 5000

typedef struct {
    int start;
    int end;
    char name[MAX_FIELD_NAME];
} FieldInfo;

FieldInfo fields[MAX_FIELDS];
int field_count = 0;

void initialize_fields() {
    // Header
    fields[field_count++] = (FieldInfo){0, 1, "Header"};
    fields[field_count++] = (FieldInfo){2, 2, "Measurement counter"};
    fields[field_count++] = (FieldInfo){3, 3, "Navigation State"};
    
    // Antenna Status
    fields[field_count++] = (FieldInfo){4, 4, "PVT status | Availability"};
    fields[field_count++] = (FieldInfo){5, 5, "No.of satellites GPS used"};
    fields[field_count++] = (FieldInfo){6, 6, "No.of satellites NAVICused"};
    fields[field_count++] = (FieldInfo){7, 7, "GPS Week number"};
    
    // GPS data
    fields[field_count++] = (FieldInfo){10, 17, "GPS TOW"};
    fields[field_count++] = (FieldInfo){18, 25, "Position X"};
    fields[field_count++] = (FieldInfo){26, 33, "Position Y"};
    fields[field_count++] = (FieldInfo){34, 41, "Position Z"};
    fields[field_count++] = (FieldInfo){42, 45, "Velocity X"};
    fields[field_count++] = (FieldInfo){46, 49, "Velocity Y"};
    fields[field_count++] = (FieldInfo){50, 53, "Velocity Z"};
    fields[field_count++] = (FieldInfo){54, 57, "GDOP"};
    fields[field_count++] = (FieldInfo){58, 61, "PDOP"};
    fields[field_count++] = (FieldInfo){62, 65, "Delta Time"};
    
    // NAVIC data
    fields[field_count++] = (FieldInfo){66, 67, "Checksum"};
    fields[field_count++] = (FieldInfo){68, 69, "NAVIC Week number"};
    fields[field_count++] = (FieldInfo){70, 77, "NAVIC TOW"};
    fields[field_count++] = (FieldInfo){78, 114, "Type 22 data"};
    fields[field_count++] = (FieldInfo){115, 130, "SV status of each Channel"};
    fields[field_count++] = (FieldInfo){131, 146, "SV ID Note2"};
    fields[field_count++] = (FieldInfo){147, 162, "C/N0 Note2"};
    fields[field_count++] = (FieldInfo){163, 290, "Pseudorange"};
    
    // Measurements
    fields[field_count++] = (FieldInfo){291, 418, "Deltarange"};
    fields[field_count++] = (FieldInfo){419, 546, "Carrier Phase Measurements"};
    fields[field_count++] = (FieldInfo){547, 554, "Clock bias"};
    fields[field_count++] = (FieldInfo){555, 562, "Clock drift"};
    fields[field_count++] = (FieldInfo){563, 633, "Ephemeris Note2"};
    fields[field_count++] = (FieldInfo){634, 635, "Message reception counter"};
    
    // Debug info
    fields[field_count++] = (FieldInfo){636, 657, "AST debug info for all channel"};
    fields[field_count++] = (FieldInfo){658, 671, "SV IDNote2"};
    fields[field_count++] = (FieldInfo){672, 685, "C/N0 Note2"};
    fields[field_count++] = (FieldInfo){686, 693, "ODP Position X"};
    fields[field_count++] = (FieldInfo){694, 701, "ODP Position Y"};
    fields[field_count++] = (FieldInfo){702, 709, "ODP Position Z"};
    fields[field_count++] = (FieldInfo){710, 713, "Velocity X"};
    fields[field_count++] = (FieldInfo){714, 717, "Velocity Y"};
    fields[field_count++] = (FieldInfo){718, 721, "Velocity Z"};
    
    // Counters and status
    fields[field_count++] = (FieldInfo){722, 729, "Estimated Clock Bias"};
    fields[field_count++] = (FieldInfo){730, 743, "Validation flag"};
    fields[field_count++] = (FieldInfo){744, 745, "SPS Update Counter"};
    fields[field_count++] = (FieldInfo){746, 747, "UART transmission counter"};
    fields[field_count++] = (FieldInfo){748, 749, "UART reception counter"};
    fields[field_count++] = (FieldInfo){750, 750, "UART checksum failure counter"};
    fields[field_count++] = (FieldInfo){751, 751, "I2C IC Rec counter"};
    fields[field_count++] = (FieldInfo){752, 752, "IC counter"};
    fields[field_count++] = (FieldInfo){753, 753, "IC update counter"};
    fields[field_count++] = (FieldInfo){754, 754, "AST reset Counter"};
    fields[field_count++] = (FieldInfo){755, 755, "SMAV71 Reset Counter"};
    fields[field_count++] = (FieldInfo){756, 756, "SPS Reset Counter"};
    fields[field_count++] = (FieldInfo){757, 757, "Reset ID"};
    fields[field_count++] = (FieldInfo){758, 765, "SMAV71 health debug info"};
    fields[field_count++] = (FieldInfo){766, 767, "Checksum"};
}

// Convert hex string to byte
unsigned char hex_to_byte(const char *hex) {
    unsigned char byte = 0;
    sscanf(hex, "%2hhx", &byte);
    return byte;
}

void display_data(const unsigned char *data) {
    printf("\033[2J\033[H");  // Clear screen and move cursor to top
    printf("=== Real-Time Data Display ===\n\n");
    
    for (int i = 0; i < field_count; i++) {
        int start = fields[i].start;
        int end = fields[i].end;
        
        printf("%s: ", fields[i].name);
        
        // Display raw bytes
        for (int j = start; j <= end; j++) {
            printf("%02X ", data[j]);
        }
        printf("\n");
    }
    printf("\nLast updated: %s", ctime(&(time_t){time(NULL)}));
    fflush(stdout);
}

int main() {
    FILE *file;
    unsigned char data[DATA_SIZE];
    char line[LINE_BUFFER_SIZE];
    char *token;
    int row_count = 0;
    int byte_index;
    
    // Initialize field information
    initialize_fields();
    
    // Open data file
    file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open data file.\n");
        return 1;
    }
    
    // Read and display data
    while (fgets(line, LINE_BUFFER_SIZE, file) != NULL) {
        byte_index = 0;
        token = strtok(line, " \t\n");
        
        while (token != NULL && byte_index < DATA_SIZE) {
            if (strlen(token) == 2) { // Expecting 2-character hex values
                data[byte_index++] = hex_to_byte(token);
            }
            token = strtok(NULL, " \t\n");
        }
        
        if (byte_index != DATA_SIZE) {
            printf("Warning: Row %d has only %d bytes (expected %d)\n", 
                   row_count + 1, byte_index, DATA_SIZE);
        }
        
        row_count++;
        printf("Displaying row %d:\n", row_count);
        display_data(data);
        
        sleep(1);  // Wait for 1 second before showing next row
    }
    
    fclose(file);
    printf("End of data file reached. Total rows: %d\n", row_count);
    
    return 0;
}
```

How to use this program:

1. Save this code to a file named data_display.c
2. Compile it with: gcc data_display.c -o data_display
3. Create a data file named data.txt with your hexadecimal data in the format:
   ```
   aa 55 a6 89 ... (up to 767 bytes)
   ```
4. Run the program: ./data_display

Key features:

1. Reads hexadecimal byte data directly from the input file
2. Extracts and displays the raw bytes for each field without any conversion
3. Clears the screen between updates for better readability
4. Shows a timestamp of the last update
5. Processes each row of data with a 1-second delay between rows

The program will parse each line of hexadecimal values, extract the fields according to your specification, and display the raw byte data in hexadecimal format.





-+-+-+-+++++++--------+++-+----+----------+++

complete byte with tables


I'll update the code to display position/velocity data in one table and SV data in another table. Here's the modified code:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define DATA_SIZE 768
#define MAX_FIELD_NAME 50
#define MAX_FIELDS 100
#define LINE_BUFFER_SIZE 5000
#define MAX_SV_CHANNELS 16

typedef struct {
    int start;
    int end;
    char name[MAX_FIELD_NAME];
} FieldInfo;

typedef struct {
    unsigned char sv_id;
    unsigned char cno;
    unsigned char pseudorange[8];  // 8 bytes for pseudorange
    unsigned char deltarange[8];   // 8 bytes for deltarange
} SVData;

FieldInfo fields[MAX_FIELDS];
int field_count = 0;

void initialize_fields() {
    // Header
    fields[field_count++] = (FieldInfo){0, 1, "Header"};
    fields[field_count++] = (FieldInfo){2, 2, "Measurement counter"};
    fields[field_count++] = (FieldInfo){3, 3, "Navigation State"};
    
    // Antenna Status
    fields[field_count++] = (FieldInfo){4, 4, "PVT status | Availability"};
    fields[field_count++] = (FieldInfo){5, 5, "No.of satellites GPS used"};
    fields[field_count++] = (FieldInfo){6, 6, "No.of satellites NAVICused"};
    fields[field_count++] = (FieldInfo){7, 7, "GPS Week number"};
    
    // GPS data
    fields[field_count++] = (FieldInfo){10, 17, "GPS TOW"};
    fields[field_count++] = (FieldInfo){18, 25, "Position X"};
    fields[field_count++] = (FieldInfo){26, 33, "Position Y"};
    fields[field_count++] = (FieldInfo){34, 41, "Position Z"};
    fields[field_count++] = (FieldInfo){42, 45, "Velocity X"};
    fields[field_count++] = (FieldInfo){46, 49, "Velocity Y"};
    fields[field_count++] = (FieldInfo){50, 53, "Velocity Z"};
    fields[field_count++] = (FieldInfo){54, 57, "GDOP"};
    fields[field_count++] = (FieldInfo){58, 61, "PDOP"};
    fields[field_count++] = (FieldInfo){62, 65, "Delta Time"};
    
    // NAVIC data
    fields[field_count++] = (FieldInfo){66, 67, "Checksum"};
    fields[field_count++] = (FieldInfo){68, 69, "NAVIC Week number"};
    fields[field_count++] = (FieldInfo){70, 77, "NAVIC TOW"};
    fields[field_count++] = (FieldInfo){78, 114, "Type 22 data"};
    fields[field_count++] = (FieldInfo){115, 130, "SV status of each Channel"};
    fields[field_count++] = (FieldInfo){131, 146, "SV ID Note2"};
    fields[field_count++] = (FieldInfo){147, 162, "C/N0 Note2"};
    fields[field_count++] = (FieldInfo){163, 290, "Pseudorange"};
    
    // Measurements
    fields[field_count++] = (FieldInfo){291, 418, "Deltarange"};
    fields[field_count++] = (FieldInfo){419, 546, "Carrier Phase Measurements"};
    fields[field_count++] = (FieldInfo){547, 554, "Clock bias"};
    fields[field_count++] = (FieldInfo){555, 562, "Clock drift"};
    fields[field_count++] = (FieldInfo){563, 633, "Ephemeris Note2"};
    fields[field_count++] = (FieldInfo){634, 635, "Message reception counter"};
    
    // Debug info
    fields[field_count++] = (FieldInfo){636, 657, "AST debug info for all channel"};
    fields[field_count++] = (FieldInfo){658, 671, "SV IDNote2"};
    fields[field_count++] = (FieldInfo){672, 685, "C/N0 Note2"};
    fields[field_count++] = (FieldInfo){686, 693, "ODP Position X"};
    fields[field_count++] = (FieldInfo){694, 701, "ODP Position Y"};
    fields[field_count++] = (FieldInfo){702, 709, "ODP Position Z"};
    fields[field_count++] = (FieldInfo){710, 713, "Velocity X"};
    fields[field_count++] = (FieldInfo){714, 717, "Velocity Y"};
    fields[field_count++] = (FieldInfo){718, 721, "Velocity Z"};
    
    // Counters and status
    fields[field_count++] = (FieldInfo){722, 729, "Estimated Clock Bias"};
    fields[field_count++] = (FieldInfo){730, 743, "Validation flag"};
    fields[field_count++] = (FieldInfo){744, 745, "SPS Update Counter"};
    fields[field_count++] = (FieldInfo){746, 747, "UART transmission counter"};
    fields[field_count++] = (FieldInfo){748, 749, "UART reception counter"};
    fields[field_count++] = (FieldInfo){750, 750, "UART checksum failure counter"};
    fields[field_count++] = (FieldInfo){751, 751, "I2C IC Rec counter"};
    fields[field_count++] = (FieldInfo){752, 752, "IC counter"};
    fields[field_count++] = (FieldInfo){753, 753, "IC update counter"};
    fields[field_count++] = (FieldInfo){754, 754, "AST reset Counter"};
    fields[field_count++] = (FieldInfo){755, 755, "SMAV71 Reset Counter"};
    fields[field_count++] = (FieldInfo){756, 756, "SPS Reset Counter"};
    fields[field_count++] = (FieldInfo){757, 757, "Reset ID"};
    fields[field_count++] = (FieldInfo){758, 765, "SMAV71 health debug info"};
    fields[field_count++] = (FieldInfo){766, 767, "Checksum"};
}

// Convert hex string to byte
unsigned char hex_to_byte(const char *hex) {
    unsigned char byte = 0;
    sscanf(hex, "%2hhx", &byte);
    return byte;
}

// Extract SV data from the raw bytes
void extract_sv_data(const unsigned char *data, SVData sv_data[MAX_SV_CHANNELS]) {
    // Extract SV IDs (bytes 131-146)
    for (int i = 0; i < MAX_SV_CHANNELS; i++) {
        sv_data[i].sv_id = data[131 + i];
    }
    
    // Extract C/N0 (bytes 147-162)
    for (int i = 0; i < MAX_SV_CHANNELS; i++) {
        sv_data[i].cno = data[147 + i];
    }
    
    // Extract Pseudorange (bytes 163-290, 8 bytes per SV)
    for (int i = 0; i < MAX_SV_CHANNELS; i++) {
        for (int j = 0; j < 8; j++) {
            sv_data[i].pseudorange[j] = data[163 + i * 8 + j];
        }
    }
    
    // Extract Deltarange (bytes 291-418, 8 bytes per SV)
    for (int i = 0; i < MAX_SV_CHANNELS; i++) {
        for (int j = 0; j < 8; j++) {
            sv_data[i].deltarange[j] = data[291 + i * 8 + j];
        }
    }
}

// Display position and velocity table
void display_position_velocity_table(const unsigned char *data) {
    printf("=== Position & Velocity Table ===\n");
    printf("+----------------+----------------------------------+\n");
    printf("| Parameter      | Value (Hex)                      |\n");
    printf("+----------------+----------------------------------+\n");
    
    // Position X (bytes 18-25)
    printf("| Position X     | ");
    for (int i = 18; i <= 25; i++) {
        printf("%02X ", data[i]);
    }
    printf(" |\n");
    
    // Position Y (bytes 26-33)
    printf("| Position Y     | ");
    for (int i = 26; i <= 33; i++) {
        printf("%02X ", data[i]);
    }
    printf(" |\n");
    
    // Position Z (bytes 34-41)
    printf("| Position Z     | ");
    for (int i = 34; i <= 41; i++) {
        printf("%02X ", data[i]);
    }
    printf(" |\n");
    
    // Velocity X (bytes 42-45)
    printf("| Velocity X     | ");
    for (int i = 42; i <= 45; i++) {
        printf("%02X ", data[i]);
    }
    printf("           |\n");
    
    // Velocity Y (bytes 46-49)
    printf("| Velocity Y     | ");
    for (int i = 46; i <= 49; i++) {
        printf("%02X ", data[i]);
    }
    printf("           |\n");
    
    // Velocity Z (bytes 50-53)
    printf("| Velocity Z     | ");
    for (int i = 50; i <= 53; i++) {
        printf("%02X ", data[i]);
    }
    printf("           |\n");
    
    printf("+----------------+----------------------------------+\n\n");
}

// Display SV data table
void display_sv_table(const SVData sv_data[MAX_SV_CHANNELS]) {
    printf("=== SV Data Table ===\n");
    printf("+----+------+-------+----------------------------------+----------------------------------+\n");
    printf("| Ch | SVID | C/N0  | Pseudorange (Hex)                | Deltarange (Hex)                 |\n");
    printf("+----+------+-------+----------------------------------+----------------------------------+\n");
    
    for (int i = 0; i < MAX_SV_CHANNELS; i++) {
        printf("| %2d |  %02X  |  %02X   | ", i + 1, sv_data[i].sv_id, sv_data[i].cno);
        
        // Display pseudorange (8 bytes)
        for (int j = 0; j < 8; j++) {
            printf("%02X ", sv_data[i].pseudorange[j]);
        }
        printf("| ");
        
        // Display deltarange (8 bytes)
        for (int j = 0; j < 8; j++) {
            printf("%02X ", sv_data[i].deltarange[j]);
        }
        printf("|\n");
    }
    
    printf("+----+------+-------+----------------------------------+----------------------------------+\n\n");
}

void display_data(const unsigned char *data) {
    printf("\033[2J\033[H");  // Clear screen and move cursor to top
    printf("=== Real-Time Data Display ===\n\n");
    
    // Display position and velocity in table format
    display_position_velocity_table(data);
    
    // Extract and display SV data in table format
    SVData sv_data[MAX_SV_CHANNELS];
    extract_sv_data(data, sv_data);
    display_sv_table(sv_data);
    
    // Display other fields
    for (int i = 0; i < field_count; i++) {
        int start = fields[i].start;
        int end = fields[i].end;
        
        // Skip fields that are already displayed in tables
        if ((start >= 18 && end <= 53) || // Position/Velocity
            (start >= 131 && end <= 162) || // SV ID and C/N0
            (start >= 163 && end <= 290) || // Pseudorange
            (start >= 291 && end <= 418)) { // Deltarange
            continue;
        }
        
        printf("%s: ", fields[i].name);
        
        // Display raw bytes
        for (int j = start; j <= end; j++) {
            printf("%02X ", data[j]);
        }
        printf("\n");
    }
    printf("\nLast updated: %s", ctime(&(time_t){time(NULL)}));
    fflush(stdout);
}

int main() {
    FILE *file;
    unsigned char data[DATA_SIZE];
    char line[LINE_BUFFER_SIZE];
    char *token;
    int row_count = 0;
    int byte_index;
    
    // Initialize field information
    initialize_fields();
    
    // Open data file
    file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open data file.\n");
        return 1;
    }
    
    // Read and display data
    while (fgets(line, LINE_BUFFER_SIZE, file) != NULL) {
        byte_index = 0;
        token = strtok(line, " \t\n");
        
        while (token != NULL && byte_index < DATA_SIZE) {
            if (strlen(token) == 2) { // Expecting 2-character hex values
                data[byte_index++] = hex_to_byte(token);
            }
            token = strtok(NULL, " \t\n");
        }
        
        if (byte_index != DATA_SIZE) {
            printf("Warning: Row %d has only %d bytes (expected %d)\n", 
                   row_count + 1, byte_index, DATA_SIZE);
        }
        
        row_count++;
        printf("Displaying row %d:\n", row_count);
        display_data(data);
        
        sleep(1);  // Wait for 1 second before showing next row
    }
    
    fclose(file);
    printf("End of data file reached. Total rows: %d\n", row_count);
    
    return 0;
}
```

How to create the input file:

Create a file named data.txt with your hexadecimal data. Each line should contain 768 space-separated hexadecimal bytes. For example:

```
AA 55 A6 89 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 ... (continue for 768 bytes)
```

Each line represents one complete set of data (one row), and the program will display each row with a 1-second delay between them.

Table formats:

1. Position & Velocity Table: Shows Position X, Y, Z and Velocity X, Y, Z in a formatted table
2. SV Data Table: Shows SV ID, C/N0, Pseudorange, and Deltarange for all 16 channels in a formatted table

The rest of the data fields are displayed in the standard format below the tables.


