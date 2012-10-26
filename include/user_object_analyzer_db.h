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

    void insert(const osm_api_data_types::osm_node * const p_node);
    void insert(const osm_api_data_types::osm_way * const p_way);
    void insert(const osm_api_data_types::osm_relation * const p_relation);

    inline void insert_node(const osm_api_data_types::osm_object::t_osm_id p_id);
    inline void insert_way(const osm_api_data_types::osm_object::t_osm_id p_id);
    inline void insert_relation(const osm_api_data_types::osm_object::t_osm_id p_id);

    bool contains(const osm_api_data_types::osm_core_element * const p_element);
  private:
    void insert(sqlite3_stmt * p_stmt,osm_api_data_types::osm_object::t_osm_id p_id,const std::string & p_type);
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


  //----------------------------------------------------------------------------
  void user_object_analyzer_db::insert_node(const osm_api_data_types::osm_object::t_osm_id p_id)
  {
    insert(m_insert_node_id_stmt,p_id,osm_api_data_types::osm_node::get_type_str());
  }

  //----------------------------------------------------------------------------
  void user_object_analyzer_db::insert_way(const osm_api_data_types::osm_object::t_osm_id p_id)
  {
    insert(m_insert_way_id_stmt,p_id,osm_api_data_types::osm_way::get_type_str());
  }

  //----------------------------------------------------------------------------
  void user_object_analyzer_db::insert_relation(const osm_api_data_types::osm_object::t_osm_id p_id)
  {
    insert(m_insert_relation_id_stmt,p_id,osm_api_data_types::osm_relation::get_type_str());
  }

}

#endif // _USER_OBJECT_ANALYZER_DB_H_
//EOF
