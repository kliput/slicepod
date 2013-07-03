#include "db_model.hpp"

class QSqlError;

// TODO
// #if LOG_LEVEL >= 2
#define LOG_DEBUG(STREAM) std::cout << "DEBUG: " << STREAM << std::endl;
// #else
// #define LOG_DEBUG(STREAM)
// #endif

// TODO
// #if LOG_LEVEL >= 1
#define LOG_INFO(STREAM) std::cout << "INFO: " << STREAM << std::endl;
// #else
// #define LOG_INFO(STREAM)
// #endif

void db_connect(const char* db_name);
void check(const QSqlError& error);
directory_ptr add_scan_dir(const char* dir_path);
