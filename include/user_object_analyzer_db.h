/*
    This file is part of osm_diff_analyzer_user_object, Openstreetmap
    diff analyzer based on CPP diff representation. It's aim is to survey
    objects edited by parametered used and to generate an alert in case of 
    edition on these objects
    Copyright (C) 2012  Julien Thevenon ( julien_thevenon at yahoo.fr )

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#ifndef _USER_OBJECT_ANALYZER_DB_H_
#define _USER_OBJECT_ANALYZER_DB_H_

#include "osm_api_data_types.h"
#include <string>
#include <set>

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

    inline void get_all_node_ids(std::set<osm_api_data_types::osm_object::t_osm_id> & p_id);
    inline void get_all_way_ids(std::set<osm_api_data_types::osm_object::t_osm_id> & p_id);
    inline void get_all_relation_ids(std::set<osm_api_data_types::osm_object::t_osm_id> & p_id);

    bool contains(const osm_api_data_types::osm_core_element * const p_element);
  private:
    bool contains(sqlite3_stmt * p_stmt,const osm_api_data_types::osm_object::t_osm_id & p_id);
    void insert(sqlite3_stmt * p_stmt,osm_api_data_types::osm_object::t_osm_id p_id,const std::string & p_type);
    void get_all_ids(sqlite3_stmt * p_stmt,std::set<osm_api_data_types::osm_object::t_osm_id> & p_id,const std::string & p_type);
    void create_id_table(const std::string & p_name);
    void prepare_insert_id_stmt(const std::string & p_name,sqlite3_stmt * & p_stmt_ptr);
    void prepare_contains_id_stmt(const std::string & p_name,sqlite3_stmt * & p_stmt_ptr);
    void prepare_get_all_stmt(const std::string & p_name,sqlite3_stmt * & p_stmt_ptr);
    sqlite3 *m_db;
    sqlite3_stmt * m_insert_node_id_stmt;                       
    sqlite3_stmt * m_insert_way_id_stmt;                       
    sqlite3_stmt * m_insert_relation_id_stmt;                       
    sqlite3_stmt * m_contains_node_id_stmt;                       
    sqlite3_stmt * m_contains_way_id_stmt;                       
    sqlite3_stmt * m_contains_relation_id_stmt;     
    sqlite3_stmt * m_get_all_node_id_stmt;                       
    sqlite3_stmt * m_get_all_way_id_stmt;                       
    sqlite3_stmt * m_get_all_relation_id_stmt;     
    bool m_transaction_opened;
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
  //----------------------------------------------------------------------------
  void user_object_analyzer_db::get_all_node_ids(std::set<osm_api_data_types::osm_object::t_osm_id> & p_ids)
  {
    get_all_ids(m_get_all_node_id_stmt,p_ids,osm_api_data_types::osm_node::get_type_str());
  }
  
  //----------------------------------------------------------------------------
  void user_object_analyzer_db::get_all_way_ids(std::set<osm_api_data_types::osm_object::t_osm_id> & p_ids)
  {
    //    get_all_ids(m_get_all_way_id_stmt,p_ids,osm_api_data_types::osm_way::get_type_str());
  }

  //----------------------------------------------------------------------------
  void user_object_analyzer_db::get_all_relation_ids(std::set<osm_api_data_types::osm_object::t_osm_id> & p_ids)
  {
    get_all_ids(m_get_all_relation_id_stmt,p_ids,osm_api_data_types::osm_relation::get_type_str());
  }

}

#endif // _USER_OBJECT_ANALYZER_DB_H_
//EOF
