# Sqlite3 SPIFFS ESP-IDF 5 Example

Creates two Sqlite3 databases on SPIFFS, inserts and retrieves data from them.

For more information, please see README.md at https://github.com/siara-cc/esp32_arduino_sqlite3_lib/ 

## How to use example

### Hardware Required

You need a ESP32 basic Devboard, this was tested with a NodeMCU.

### ESP-IDF Compatibility

For this code to compile with `ESP-IDF v5.X.X`you must:

* In the file `esp/esp-idf/components/esp32-idf-sqlite3/CMakeLists.txt` add `spi_flash` to `REQUIRES`.

For this code to compile with `ESP-IDF v4.X.X`you must copy [esp32-idf-sqlite3 repository](https://github.com/siara-cc/esp32-idf-sqlite3) to your components folder. In this repository are more step by step instructions.

## Example Output
Note that the output, in particular the order of the output, may vary depending on the environment. Also, the first time you test it the SPIFFS will be formated, showing in the log something like:

    I (444) sqlite3_spiffs: Initializing SPIFFS
    W (454) SPIFFS: mount failed, -10025. formatting...
    I (38084) sqlite3_spiffs: Partition size: total: 2824001, used: 0


Console output in general:

    I (444) main_task: Calling app_main()
    I (444) sqlite3_spiffs: Initializing SPIFFS
    I (684) sqlite3_spiffs: Partition size: total: 2824001, used: 0
    I (894) sqlite3_spiffs: Reading table test1
    Opened database successfully
    I (1224) sqlite3_spiffs: Reading table test2
    Opened database successfully
    I (1544) sqlite3_spiffs: Creating table test1
    CREATE TABLE test1 (id INTEGER, content);
    Operation done successfully
    Time taken: 290846
    I (1834) sqlite3_spiffs: Creating table test2
    CREATE TABLE test2 (id INTEGER, content);
    Operation done successfully
    Time taken: 288158
    I (2124) sqlite3_spiffs: Tables created succesfully
    I (2124) sqlite3_spiffs: Inserting data in table test1
    INSERT INTO test1 VALUES (1, 'Hello, World from test1, ESP-IDF 5.1.1');
    Operation done successfully
    Time taken: 51256
    I (2184) sqlite3_spiffs: Inserting data in table test2
    INSERT INTO test2 VALUES (1, 'Hello, World from test2, ESP-IDF 5.1.1');
    Operation done successfully
    Time taken: 54877
    I (2244) sqlite3_spiffs: Selecting data from test1
    SELECT * FROM test1
    Callback function called: 
    id = 1
    content = Hello, World from test1, ESP-IDF 5.1.1

    Operation done successfully
    Time taken: 13564
    I (2254) sqlite3_spiffs: Selecting data from test2
    SELECT * FROM test2
    Callback function called: 
    id = 1
    content = Hello, World from test2, ESP-IDF 5.1.1

    Operation done successfully
    Time taken: 13080
    I (2494) sqlite3_spiffs: SPIFFS unmounted
    I (2494) main_task: Returned from app_main()

## Log

* Last compile: November 03t 2023.
* Last test: November 03t 2023.
* Last compile espidf version: `v 5.1.1`.
* VSCode ESP-IDF Visual Studio Code Extension `v 1.6.5`.

## License

Apache License, Version 2.0, January 2004.

## Version

`v 1.0.0`