#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/database.h"

static sqlite3 *db = NULL;

int init_database(void) {
    int rc;
    char *err_msg = 0;
    
    // Open database connection (creates file if it doesn't exist)
    rc = sqlite3_open("./events.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        db = NULL;
        return 0;
    }
    
    // Create table if it doesn't exist
    const char *sql = "CREATE TABLE IF NOT EXISTS events("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "poster_name TEXT NOT NULL,"
        "title TEXT NOT NULL,"
        "description TEXT NOT NULL,"
        "created_date DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "event_date TEXT NOT NULL);";
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        db = NULL;
        return 0;
    }
    
    printf("Database initialized successfully\n");
    return 1;
}

int insert_event(const char *poster_name, const char *title, 
                 const char *description, const char *event_date) {
    if (db == NULL) {
        fprintf(stderr, "Database not initialized\n");
        return 0;
    }
    
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO events (poster_name, title, description, event_date) VALUES (?, ?, ?, ?);";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL prepare error: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    // Bind parameters
    sqlite3_bind_text(stmt, 1, poster_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, title, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, description, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, event_date, -1, SQLITE_STATIC);
    
    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL step error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }
    
    sqlite3_finalize(stmt);
    printf("Event inserted successfully\n");
    return 1;
}

EventList get_all_events(void) {
    EventList list = {NULL, 0};
    
    if (db == NULL) {
        fprintf(stderr, "Database not initialized\n");
        return list;
    }
    
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, poster_name, title, description, created_date, event_date FROM events ORDER BY created_date DESC;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL prepare error: %s\n", sqlite3_errmsg(db));
        return list;
    }
    
    // First, count rows
    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        count++;
    }
    
    if (count == 0) {
        sqlite3_finalize(stmt);
        return list;
    }
    
    // Allocate memory for events
    list.events = (Event *)malloc(count * sizeof(Event));
    if (list.events == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        sqlite3_finalize(stmt);
        return list;
    }
    
    // Reset to beginning and fetch data
    sqlite3_reset(stmt);
    list.count = 0;
    
    while (sqlite3_step(stmt) == SQLITE_ROW && list.count < count) {
        Event *event = &list.events[list.count];
        
        event->id = sqlite3_column_int(stmt, 0);
        
        const unsigned char *poster_name = sqlite3_column_text(stmt, 1);
        strncpy(event->poster_name, (const char *)poster_name, 255);
        event->poster_name[255] = '\0';
        
        const unsigned char *title = sqlite3_column_text(stmt, 2);
        strncpy(event->title, (const char *)title, 127);
        event->title[127] = '\0';
        
        const unsigned char *description = sqlite3_column_text(stmt, 3);
        strncpy(event->description, (const char *)description, 2047);
        event->description[2047] = '\0';
        
        const unsigned char *created_date = sqlite3_column_text(stmt, 4);
        strncpy(event->created_date, (const char *)created_date, 31);
        event->created_date[31] = '\0';
        
        const unsigned char *event_date = sqlite3_column_text(stmt, 5);
        strncpy(event->event_date, (const char *)event_date, 31);
        event->event_date[31] = '\0';
        
        list.count++;
    }
    
    sqlite3_finalize(stmt);
    return list;
}

void free_events(EventList list) {
    if (list.events != NULL) {
        free(list.events);
    }
}

void close_database(void) {
    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;
        printf("Database closed\n");
    }
}
