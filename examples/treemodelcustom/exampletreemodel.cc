//$Id$ -*- c++ -*-

/* gtkmm example Copyright (C) 2002 gtkmm development team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <iostream>
#include "exampletreemodel.h"


ExampleTreeModel::ExampleTreeModel()
: Glib::ObjectBase( typeid(ExampleTreeModel) ), //register a custom GType.
  Glib::Object(), //The custom GType is actually registered here.
  m_stamp(1) //When the model's stamp != the iterator's stamp then that iterator is invalid and should be ignored. Also, 0=invalid
{
  
  //We need to specify a particular get_type() from one of the virtual base classes, but they should
  //both return the same piece of data.
  Gtk::TreeModel::add_interface( Glib::Object::get_type() );
   
  //Initialize our underlying data:
  const typeListOfRows::size_type rows_count = 100;
  const typeRow::size_type columns_count = 10;
  m_rows.resize(rows_count); //100 rows.
  for(unsigned int row_number = 0; row_number < rows_count; ++row_number)
  {
    //Create the row:
    m_rows[row_number].resize(columns_count); // 10 cells (columns) for each row.

    for(unsigned int column_number = 0; column_number < columns_count; ++column_number)
    {
      // Set the data in the row cells:
      // It is more likely that you would be reusing existing data from some other data structure,
      // instead of generating the data here.
      
      char buffer[20]; //You could use a std::stringstream instead.
      g_snprintf(buffer, sizeof(buffer), "%d, %d", row_number, column_number);

      (m_rows[row_number])[column_number] = buffer; //Note that all 10 columns here are of the same type.
    }
  }

  //The Column information that can be used with TreeView::append(), TreeModel::iterator[], etc.
  m_listModelColumns.resize(columns_count);
  for(unsigned int column_number = 0; column_number < columns_count; ++column_number)
  {
    m_column_record.add( m_listModelColumns[column_number] );
  }
}

ExampleTreeModel::~ExampleTreeModel()
{
}

//static:
Glib::RefPtr<ExampleTreeModel> ExampleTreeModel::create()
{
  return Glib::RefPtr<ExampleTreeModel>( new ExampleTreeModel );
}

Gtk::TreeModelFlags ExampleTreeModel::get_flags_vfunc()
{
   return Gtk::TreeModelFlags(0);
}

int ExampleTreeModel::get_n_columns_vfunc()
{
   return m_rows[0].size(); //The number of columns in the first (same as every one) row.
}

GType ExampleTreeModel::get_column_type_vfunc(int index)
{
  if(index <= (int)m_listModelColumns.size())
    return m_listModelColumns[index].type();
  else
    return 0;
}

void ExampleTreeModel::get_value_vfunc(const TreeModel::iterator& iter, int column, Glib::ValueBase& value)
{
  if(check_treeiter_validity(iter))
  {  
    if(column <= (int)m_listModelColumns.size())
    {
      //Get the correct ValueType from the Gtk::TreeModel::Column's type, so we don't have to repeat it here:
      typeModelColumn::ValueType value_specific;
      value_specific.init( typeModelColumn::ValueType::value_type() );  //TODO: Is there any way to avoid this step?

      //Or, instead of asking the compiler for the TreeModelColumn's ValueType:
      //Glib::Value< Glib::ustring > value_specific;
      //value_specific.init( Glib::Value< Glib::ustring >::value_type() ); //TODO: Is there any way to avoid this step?

      typeListOfRows::iterator dataRowIter = get_data_row_iter_from_tree_row_iter(iter);
      if(dataRowIter != m_rows.end())
      {
        typeRow& dataRow = *dataRowIter;

        Glib::ustring result = dataRow[column];

        value_specific.set(result); //The compiler would complain if the type was wrong.
        value.init( Glib::Value< Glib::ustring >::value_type() ); //TODO: Is there any way to avoid this step? Can't it copy the type as well as the value?
        value = value_specific;
      }
    }
  }
}

bool ExampleTreeModel::iter_next_vfunc(const iterator& iter, iterator& iter_next) const
{
  if( check_treeiter_validity(iter) )
  {
    //Start with an iter to the same row:
    iter_next = iter;
    
    //Make the GtkTreeIter represent the next row:
    typeListOfRows::size_type row_index = (typeListOfRows::size_type)iter_next.gobj()->user_data;
    row_index++;
    if( row_index < m_rows.size() )
    {
      //Put the index of the next row in the iter, replacing the previous row index:
      //TODO: Discover how to associate more complex data with the iterator. Memory management seems difficult. murrayc
      iter_next.gobj()->user_data = (void*)row_index;
      return true; //success
    }
  }
  else
    iter_next = iterator(); //Set is as invalid, as the TreeModel documentation says that it should be.

  return false; //There is no next row.
}

bool ExampleTreeModel::iter_children_vfunc(const iterator& parent, iterator& iter)
{
  return iter_nth_child_vfunc(parent, 0, iter);
}

bool ExampleTreeModel::iter_has_child_vfunc(const iterator& iter) const
{
  return (iter_n_children_vfunc(iter) > 0);
}

int ExampleTreeModel::iter_n_children_vfunc(const iterator& iter) const
{
  if(!check_treeiter_validity(iter))
    return 0;
    
  return 0; //There are no children
}

int ExampleTreeModel::iter_n_root_children_vfunc() const
{
  return 0; //There are no children
}

bool ExampleTreeModel::iter_nth_child_vfunc(const iterator& parent, int /* n */, iterator& iter) const
{
  if(!check_treeiter_validity(parent))
  {
    iter = iterator(); //Set is as invalid, as the TreeModel documentation says that it should be.
    return false;
  }

  iter = iterator(); //Set is as invalid, as the TreeModel documentation says that it should be.  
  return false; //There are no children.
}

bool ExampleTreeModel::iter_nth_root_child_vfunc(int /* n */, iterator& iter) const
{
  iter = iterator(); //Set is as invalid, as the TreeModel documentation says that it should be.
  return false; //There are no children.  
}
  

bool ExampleTreeModel::iter_parent_vfunc(const iterator& child, iterator& iter) const
{
  if(!check_treeiter_validity(child))
  {
    iter = iterator(); //Set is as invalid, as the TreeModel documentation says that it should be.
    return false;
  }

  iter = iterator(); //Set is as invalid, as the TreeModel documentation says that it should be.
  return false; //There are no children, so no parents.
}

Gtk::TreeModel::Path ExampleTreeModel::get_path_vfunc(const iterator& /* iter */)
{
   //TODO:
   return Path();
}

bool ExampleTreeModel::get_iter_vfunc(const Path& path, iterator& iter) const
{
   unsigned sz = path.size();
   if(!sz)
   {
     iter = iterator(); //Set is as invalid, as the TreeModel documentation says that it should be.
     return false;
   }

   if(sz > 1) //There are no children.
   {
     iter = iterator(); //Set is as invalid, as the TreeModel documentation says that it should be.
     return false; 
   }

   //This is a new GtkTreeIter, so it needs the current stamp value.
   //See the comment in the constructor.
   iter = iterator(); //clear the input parameter.
   iter.set_stamp(m_stamp);
   
   //Store the row_index in the GtkTreeIter:
   //See also iter_next_vfunc()
   //TODO: Store a pointer to some more complex data type such as a typeListOfRows::iterator.
   
   unsigned row_index = path[0];
   iter.gobj()->user_data = (void*)row_index;
   
   return true;
}

Gtk::TreeModelColumn< Glib::ustring >& ExampleTreeModel::get_model_column(int column)
{
  return m_listModelColumns[column];
}

ExampleTreeModel::typeListOfRows::iterator ExampleTreeModel::get_data_row_iter_from_tree_row_iter(const iterator& iter)
{
  typeListOfRows::size_type row_index = (typeListOfRows::size_type)iter.gobj()->user_data;
  if( row_index > m_rows.size() )
    return m_rows.end();
  else
    return m_rows.begin() + row_index; //TODO: Performance.
}

bool ExampleTreeModel::check_treeiter_validity(const iterator& iter) const
{
  // Anything that modifies the model's structure should change the model's stamp,
  // so that old iters are ignored.
  return m_stamp == iter.get_stamp();
}

bool ExampleTreeModel::iter_is_valid(const iterator& iter) const
{
  if(!check_treeiter_validity(iter))
    return false;

  return Gtk::TreeModel::iter_is_valid(iter);
}

