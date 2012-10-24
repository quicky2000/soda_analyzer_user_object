#ifndef _USER_OBJECT_ANALYZER_DB_H_
#define _USER_OBJECT_ANALYZER_DB_H_

#include "osm_api_data_types.h"
#include <string>

class sqlite3;
class sqlite3_stmt;

namespace osm_diff_analyzer_user_object
{
  class user_object_analyzer_db
  {
  public:
    user_object_analyzer_db(const std::string &p_name="user_object.sqlite3");
    ~user_object_analyzer_db(void);
    void insert(const osm_api_data_types::osm_core_element * const p_element);
    bool contains(const osm_api_data_types::osm_core_element * const p_element);
  private:
    void create_id_table(const std::string & p_name);
    void prepare_insert_id_stmt(const std::string & p_name,sqlite3_stmt * & p_stmt_ptr);
    void prepare_contains_id_stmt(const std::string & p_name,sqlite3_stmt * & p_stmt_ptr);
    sqlite3 *m_db;
    sqlite3_stmt * m_insert_node_id_stmt;                       
    sqlite3_stmt * m_insert_way_id_stmt;                       
    sqlite3_stmt * m_insert_relation_id_stmt;                       
    sqlite3_stmt * m_contains_node_id_stmt;                       
    sqlite3_stmt * m_contains_way_id_stmt;                       
    sqlite3_stmt * m_contains_relation_id_stmt;                       
  };
}

#endif // _USER_OBJECT_ANALYZER_DB_H_
//EOF
