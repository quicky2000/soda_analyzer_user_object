#include "user_object_analyzer_db.h"
#include "osm_api_data_types.h"
#include <sqlite3.h>
#include <iostream>
#include <cstdlib>

#if SQLITE_VERSION_NUMBER < 3006000
#define sqlite3_prepare_v2 sqlite3_prepare
#endif

namespace osm_diff_analyzer_user_object
{
  //----------------------------------------------------------------------------
  user_object_analyzer_db::user_object_analyzer_db(const std::string &p_name):
    m_db(NULL),
    m_insert_node_id_stmt(NULL),
    m_insert_way_id_stmt(NULL),              
    m_insert_relation_id_stmt(NULL),                       
    m_contains_node_id_stmt(NULL),
    m_contains_way_id_stmt(NULL),              
    m_contains_relation_id_stmt(NULL)                       

  {
    // Opening the database
    int l_status = sqlite3_open(p_name.c_str(), &m_db);
    if(l_status == SQLITE_OK)
      {
        create_id_table(osm_api_data_types::osm_node::get_type_str());
        create_id_table(osm_api_data_types::osm_way::get_type_str());
        create_id_table(osm_api_data_types::osm_relation::get_type_str());
        prepare_insert_id_stmt(osm_api_data_types::osm_node::get_type_str(),m_insert_node_id_stmt);
        prepare_insert_id_stmt(osm_api_data_types::osm_way::get_type_str(),m_insert_way_id_stmt);
        prepare_insert_id_stmt(osm_api_data_types::osm_relation::get_type_str(),m_insert_relation_id_stmt);
        prepare_contains_id_stmt(osm_api_data_types::osm_node::get_type_str(),m_contains_node_id_stmt);
        prepare_contains_id_stmt(osm_api_data_types::osm_way::get_type_str(),m_contains_way_id_stmt);
        prepare_contains_id_stmt(osm_api_data_types::osm_relation::get_type_str(),m_contains_relation_id_stmt);
      }
    else
      {
        std::cerr << "Can't open database \"" << p_name << "\" : " << sqlite3_errmsg(m_db) << std::endl ;
      }
    std::cout << "Database \"" << p_name << "\" successfully opened" << std::endl ; 
  }
   
  //----------------------------------------------------------------------------
  void user_object_analyzer_db::prepare_insert_id_stmt(const std::string & p_name,sqlite3_stmt * & p_stmt_ptr)
  {
    std::string l_table_name = p_name + "_ids";
    // Prepare insert node id statement
    //-------------------------------------
    int l_status = sqlite3_prepare_v2(m_db,("INSERT INTO " + l_table_name + " (Id ) VALUES ($id)").c_str(),-1,&p_stmt_ptr,NULL);
    if(l_status != SQLITE_OK)
      {
        std::cout << "ERROR during preparation of statement to create id in table \"" << l_table_name << "\" : " << sqlite3_errmsg(m_db) << std::endl ;     
        exit(-1);
      }
  }

  //----------------------------------------------------------------------------
  void user_object_analyzer_db::prepare_contains_id_stmt(const std::string & p_name,sqlite3_stmt * & p_stmt_ptr)
  {
    std::string l_table_name = p_name + "_ids";
    // Preparing named_item get_by_id statements
    //--------------------------------------------
    int l_status = sqlite3_prepare_v2(m_db,("SELECT Id FROM " + l_table_name + " WHERE Id == $id").c_str(),-1,&p_stmt_ptr,NULL);
    if(l_status != SQLITE_OK)
      {
        std::cout << "ERROR during preparation of statement to get " << p_name << " item by id: " << sqlite3_errmsg(m_db) << std::endl ;     
        exit(-1);
      }

    
  }

  //----------------------------------------------------------------------------
  void user_object_analyzer_db::create_id_table(const std::string & p_name)
  {
    sqlite3_stmt *l_stmt = NULL;
    std::string l_table_name = p_name + "_ids";
    // Creation of table
    //--------------------
    int l_status = sqlite3_prepare_v2(m_db,("CREATE TABLE IF NOT EXISTS " + l_table_name +" ( Id INTEGER PRIMARY KEY);").c_str(),-1,&l_stmt,NULL);
    if(l_status != SQLITE_OK)
      {
        std::cout << "ERROR during preparation of statement to create table \"" << l_table_name << "\" : " << sqlite3_errmsg(m_db) << std::endl ;
        exit(-1);
      }
    
    l_status = sqlite3_step(l_stmt);
    if(!l_status == SQLITE_DONE)
      {
        std::cout << "ERROR during creation of \"" << l_table_name << "\" table : " << sqlite3_errmsg(m_db) << std::endl ;
        exit(-1);
      }
    sqlite3_finalize(l_stmt);  
  }

  //----------------------------------------------------------------------------
  void user_object_analyzer_db::insert(const osm_api_data_types::osm_core_element * const p_element)
  {
    sqlite3_stmt * l_stmt = NULL;
    switch(p_element->get_core_type())
      {
      case osm_api_data_types::osm_core_element::NODE :
        l_stmt = m_insert_node_id_stmt;
        break;
      case osm_api_data_types::osm_core_element::WAY :
        {
          l_stmt = m_insert_way_id_stmt;
          //          const osm_api_data_types::osm_way * const l_way = dynamic_cast<const osm_api_data_types::osm_way * const>(p_element);
          //          if(l_way==NULL)
          //            {
          //              std::cout << "ERROR : invalid way cast for object id " << p_element->get_id() << std::endl ;
          //              exit(-1);
          //            }
          //          const std::vector<osm_core_element::t_osm_id>::
        }
        break;
      case osm_api_data_types::osm_core_element::RELATION :
        l_stmt = m_insert_relation_id_stmt;
        break;
      case osm_api_data_types::osm_core_element::INTERNAL_INVALID:
        std::cout << "ERROR : unexpected core type value \"" << osm_api_data_types::osm_core_element::get_osm_type_str(p_element->get_core_type()) << "\"" << std::endl ;
        exit(-1);
        break;
     }
    
    assert(l_stmt);

    // Binding values to statement
    //----------------------------
    int l_status = sqlite3_bind_int(l_stmt,sqlite3_bind_parameter_index(l_stmt,"$id"),p_element->get_id());
    if(l_status != SQLITE_OK)
      {
        std::cout << "ERROR during binding of Id parameter for insert id statement of " << p_element-> get_core_type_str() << " : " << sqlite3_errmsg(m_db) << std::endl ;     
        exit(-1);
      }
  
    // Executing statement
    //---------------------
    l_status = sqlite3_step(l_stmt);
    if( l_status == SQLITE_DONE)
      {
#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
        std::cout << p_element-> get_core_type_str() << " successfully inserted" << std::endl ;
#endif
      }
    else
      {
        std::cout << "ERROR during insertion of " << p_element->get_core_type_str() << " id " << p_element->get_id() <<" : " << sqlite3_errmsg(m_db) << std::endl ;
        exit(-1);
      }

    // Reset the statement for the next use
    //--------------------------------------
    l_status = sqlite3_reset(l_stmt);  
    if(l_status != SQLITE_OK)
      {
        std::cout << "ERROR during reset of " << p_element->get_core_type_str() << " insert statement : " << sqlite3_errmsg(m_db) << std::endl ;     
        exit(-1);
      }

    // Reset bindings because they are now useless
    //--------------------------------------------
#if SQLITE_VERSION_NUMBER > 3006000
    l_status = sqlite3_clear_bindings(l_stmt);
    if(l_status != SQLITE_OK)
      {
        std::cout << "ERROR during reset of bindings of " << p_element->get_core_type_str() << " insert statement : " << sqlite3_errmsg(m_db) << std::endl ;     
        exit(-1);
      }
#endif
  }

  //----------------------------------------------------------------------------
  bool user_object_analyzer_db::contains(const osm_api_data_types::osm_core_element * const p_element)
  {
    bool l_result = false;

    sqlite3_stmt * l_stmt = NULL;
    switch(p_element->get_core_type())
      {
      case osm_api_data_types::osm_core_element::NODE :
        l_stmt = m_contains_node_id_stmt;
        break;
      case osm_api_data_types::osm_core_element::WAY :
        l_stmt = m_contains_way_id_stmt;
        break;
      case osm_api_data_types::osm_core_element::RELATION :
        l_stmt = m_contains_relation_id_stmt;
        break;
      case osm_api_data_types::osm_core_element::INTERNAL_INVALID:
        std::cout << "ERROR : unexpected core type value \"" << osm_api_data_types::osm_core_element::get_osm_type_str(p_element->get_core_type()) << "\"" << std::endl ;
        exit(-1);
        break;
     }
    
    // Binding values to statement
    //----------------------------
    int l_status = sqlite3_bind_int(l_stmt,sqlite3_bind_parameter_index(l_stmt,"$id"),p_element->get_id());
    if(l_status != SQLITE_OK)
      {
        std::cout << "ERROR during binding of Id parameter for contains statement of " << p_element->get_core_type_str() << " : " << sqlite3_errmsg(m_db) << std::endl ;     
        exit(-1);
      }
    
  // Executing statement
  //---------------------
  l_status = sqlite3_step(l_stmt);
  if( l_status == SQLITE_ROW)
    {
#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
      std::cout << p_element->get_core_type_str() << " successfully selected" << std::endl ;
#endif
      // Ensure that ID is unique
      l_status = sqlite3_step(l_stmt);
      if( l_status == SQLITE_DONE)
	{
#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
	  std::cout << p_element->get_core_type_str() << " successfully selected done" << std::endl ;
#endif
	  l_result = true;
	}
      else
	{
	  std::cout << "ERROR during selection of " << p_element->get_core_type_str() << " : Id " << p_element->get_id() << " is not unique " << sqlite3_errmsg(m_db) << std::endl ;
	  exit(-1);
	}
    }
  else if(l_status != SQLITE_DONE)
    {
      std::cout << "ERROR during selection of " << p_element->get_core_type_str() << " : " << sqlite3_errmsg(m_db) << std::endl ;
      exit(-1);
    }



  // Reset the statement for the next use
  //--------------------------------------
  l_status = sqlite3_reset(l_stmt);  
  if(l_status != SQLITE_OK)
    {
      std::cout << "ERROR during reset of " << p_element->get_core_type_str() << " contains statement : " << sqlite3_errmsg(m_db) << std::endl ;     
      exit(-1);
    }

  // Reset bindings because they are now useless
  //--------------------------------------------
#if SQLITE_VERSION_NUMBER > 3006000
  l_status = sqlite3_clear_bindings(l_stmt);
  if(l_status != SQLITE_OK)
    {
      std::cout << "ERROR during reset of bindings of " << p_element->get_core_type_str() << " contains statement : " << sqlite3_errmsg(m_db) << std::endl ;     
      exit(-1);
    }
#endif
  return l_result;
  }

  //----------------------------------------------------------------------------
  user_object_analyzer_db::~user_object_analyzer_db(void)
  {
    sqlite3_finalize(m_contains_relation_id_stmt);
    sqlite3_finalize(m_contains_way_id_stmt);
    sqlite3_finalize(m_contains_node_id_stmt);
    sqlite3_finalize(m_insert_relation_id_stmt);
    sqlite3_finalize(m_insert_way_id_stmt);
    sqlite3_finalize(m_insert_node_id_stmt);
    sqlite3_close(m_db);     
  }
}
//EOF
