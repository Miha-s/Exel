#include "Table.hpp"
#include "Callbacks.hpp"

void enter_expression_call(Fl_Widget* widget, void* data)
{
	auto table_coord = (std::pair<window::Table*, Fl_Input*>*)data;
	Fl_Input* inp = (Fl_Input*)widget;
	table_coord->first->setExpr(table_coord->second->value(), inp->value());
}


void press_cell_call(Fl_Widget* widget, void* data)
{
	auto inputs = (std::pair<Fl_Input*, Fl_Input*>*)data;
	
}