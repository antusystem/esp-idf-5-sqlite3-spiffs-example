/* ESP-IDF 5 SPIFFS Example
  * 
  * This example initializes SPIFFS creates two SQLite databases on SPIFFS,
  * inserts and retrieves data from them and finally unmounts SPIFFS.
  * 
  * This is adaptation from siara-cc examples (https://github.com/siara-cc/esp32-idf-sqlite3-examples)
  * for it to work on ESP-IDF v5.X.X
*/
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "esp_timer.h"
#include "sqlite3.h"

static const char *TAG = "sqlite3_spiffs";

const char* data = "Callback function called";
char *zErrMsg = 0;

// Create database pointers
sqlite3 *db1;
sqlite3 *db2;
int rc;

/**
  * @brief  SQLite Callback Function
  * 
  * This function is a callback used with SQLite queries to process the results. It prints the
  * values of the result set columns, including their names and data, and an optional custom message.
  * 
  * @param data - A pointer to optional custom data (can be NULL).
  * @param argc - The number of columns in the result set.
  * @param argv - An array of strings containing the values of the result set's columns.
  * @param azColName - An array of strings containing the names of the columns in the result set.
  *
  * @return
  * - 0 to indicate success.
  *
  * @note
  * - The `data` parameter can be used to pass an optional custom message or data.
  * - This function is typically used as a callback for SQLite query execution.
*/
static int callback(void *data, int argc, char **argv, char **azColName) {
    int i;

    // Print the custom message
    printf("%s: \n", (const char*)data);

    // Loop through the result set columns and print their names and values
    for (i = 0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    // Print a newline character to separate rows
    printf("\n");
    return 0;
}

/**
 * @brief Open a SQLite database.
 *  
 * This function opens a SQLite database specified by the filename and provides an SQLite
 * database connection object for further database operations.
 *  
 * @param filename - The name of the database file to open.
 * @param db - A pointer to a pointer to an SQLite database connection object. Upon success,
 *             this pointer will store the reference to the opened database.
 *  
 * @return
 *  - 0 on success, indicating the database was opened successfully.
 *  - A non-zero error code if there was an issue opening the database.
 *
 * @note
 * It is the responsibility of the caller to handle errors appropriately based on the
 * return value.
 *  
 * @see sqlite3_open
 */
int db_open(const char *filename, sqlite3 **db) {
    int rc = sqlite3_open(filename, db);
    if (rc) {
        printf("Can't open database: %s\n", sqlite3_errmsg(*db));
        return rc;
    } else {
        printf("Opened database successfully\n");
    }
    return rc;
}

/**
 * @brief Execute an SQL statement on an SQLite database.
 *
 * This function executes the provided SQL statement on the specified SQLite database
 * and optionally calls a callback function to process the results. It also measures
 * the execution time and provides error handling.
 *
 * @param db - A pointer to the SQLite database connection.
 * @param sql - The SQL statement to be executed.
 *
 * @return
 *  - SQLITE_OK (0) on success, indicating the operation was completed successfully.
 *  - An SQLite error code on failure.
 *
 * @note
 * - The function provides timing information to measure the execution time of the SQL statement.
 * - Error handling is performed, and any SQL errors are printed along with timing information.
 * - The provided `sql` parameter should be a well-formed SQL statement.
 * - The `callback` function, if specified, processes the results of the SQL query.
 */
int db_exec(sqlite3 *db, const char *sql) {
    // Print the SQL statement
    printf("%s\n", sql);
    // Start measuring time
    int64_t start = esp_timer_get_time();
    int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK) {
        // Print SQL error message
        printf("SQL error: %s\n", zErrMsg);
        // Free the error message string
        sqlite3_free(zErrMsg);
    } else {
        printf("Operation done successfully\n");
    }
    // Print execution time
    printf("Time taken: %lld\n", esp_timer_get_time()-start);
    return rc;
}

/**
 * @brief Create Database Tables
 *
 * This function is responsible for creating database tables in two separate SQLite
 * database connections. It performs SQL operations to create tables and handles any
 * errors that may occur during the process.
 *
 * @note
 * - The function creates two tables, "test1" and "test2," in the respective databases.
 * - If an error occurs during table creation, both database connections are closed, and
 *   the function returns without creating the second table.
 */
void create_db(){
    ESP_LOGI(TAG, "Creating table test1");
    rc = db_exec(db1, "CREATE TABLE test1 (id INTEGER, content);");
    if (rc != SQLITE_OK) {
        sqlite3_close(db1);
        sqlite3_close(db2);
        return;
    }
    ESP_LOGI(TAG, "Creating table test2");
    rc = db_exec(db2, "CREATE TABLE test2 (id INTEGER, content);");
    if (rc != SQLITE_OK) {
        sqlite3_close(db1);
        sqlite3_close(db2);
        return;
    }
    ESP_LOGI(TAG, "Tables created succesfully");
}

/**
 * @brief Insert Data into Database Tables
 *
 * This function inserts sample data into two separate SQLite database tables. It performs
 * SQL operations to insert data and handles any errors that may occur during the process.
 *
 * @note
 * - The function inserts sample data into the "test1" and "test2" tables of the respective
 *   databases.
 * - If an error occurs during data insertion, both database connections are closed, and
 *   the function returns without inserting data into the second table.
 */
void insert_data(){
    ESP_LOGI(TAG, "Inserting data in table test1");
    rc = db_exec(db1, "INSERT INTO test1 VALUES (1, 'Hello, World from test1, ESP-IDF 5.1.1');");
    if (rc != SQLITE_OK) {
        sqlite3_close(db1);
        sqlite3_close(db2);
        return;
    }
    ESP_LOGI(TAG, "Inserting data in table test2");
    rc = db_exec(db2, "INSERT INTO test2 VALUES (1, 'Hello, World from test2, ESP-IDF 5.1.1');");
    if (rc != SQLITE_OK) {
        sqlite3_close(db1);
        sqlite3_close(db2);
        return;
    }
}

/**
 * @brief Select Data from Database Tables
 *
 * This function performs SQL SELECT operations to retrieve data from two separate SQLite
 * database tables. It handles any errors that may occur during the retrieval process.
 *
 * @note
 * - The function retrieves data from the "test1" and "test2" tables of the respective
 *   databases using SQL SELECT queries.
 * - If an error occurs during data retrieval, both database connections are closed, and
 *   the function returns without completing the second SELECT operation.
 */
void select_data(){
    ESP_LOGI(TAG, "Selecting data from test1");
    rc = db_exec(db1, "SELECT * FROM test1");
    if (rc != SQLITE_OK) {
        sqlite3_close(db1);
        sqlite3_close(db2);
        return;
    }
    ESP_LOGI(TAG, "Selecting data from test2");
    rc = db_exec(db2, "SELECT * FROM test2");
    if (rc != SQLITE_OK) {
        sqlite3_close(db1);
        sqlite3_close(db2);
        return;
    }
}

void app_main()
{
    // Initialize and mount SPIFFS filesystem.
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        //.partition_label = "storage",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };
    
    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    // Retrieve and log partition information.
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    // Comment this lines in case you want to remove the DB
    unlink("/spiffs/test1.db");
    unlink("/spiffs/test2.db");

    // Initialize SQLite library.
    sqlite3_initialize();

    // Open SQLite databases.
    ESP_LOGI(TAG, "Opening table test1");
    if (db_open("/spiffs/test1.db", &db1))
        return;
    ESP_LOGI(TAG, "Opening table test2");
    if (db_open("/spiffs/test2.db", &db2))
        return;


    // Creating DBs
    create_db();

    // Perform database operations (e.g., create tables, insert data, select data).

    // Inserting data
    insert_data();

    // Selecting data
    select_data();

    // Close SQLite databases.
    sqlite3_close(db1);
    sqlite3_close(db2);

    // Unmount partition and disable SPIFFS
    esp_vfs_spiffs_unregister(NULL);
    ESP_LOGI(TAG, "SPIFFS unmounted");

    //while(1);
}
