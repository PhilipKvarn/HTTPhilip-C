#ifndef DATABASE_H
#define DATABASE_H

typedef struct {
    int id;
    char poster_name[256];
    char title[128];
    char description[2048];
    char created_date[32];      // ISO 8601 string
    char event_date[32];        // ISO 8601 string
} Event;

typedef struct {
    Event *events;
    int count;
} EventList;

// Initialize database connection, create table if not exists
int init_database(void);

// Insert new event, return 1 on success, 0 on failure
int insert_event(const char *poster_name, const char *title, 
                 const char *description, const char *event_date);

// Fetch all events sorted by created_date DESC (most recent first)
EventList get_all_events(void);

// Free event list memory
void free_events(EventList list);

// Close database connection
void close_database(void);

#endif
