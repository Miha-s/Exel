#include "Table.hpp"
#include "Callbacks.hpp"

void enter_expression_call(Fl_Widget* widget, void* data)
{
	auto table_coord = (std::pair<Fl_Input*, window::Table*>*)data;
	Fl_Input* inp = (Fl_Input*)widget;
	table_coord->second->setExpr(table_coord->first->value(), inp->value());
}


void press_cell_call(Fl_Widget* widget, void* data)
{
	auto cell = (window::Cell*)widget;
	auto inputs = (std::pair<Fl_Input*, Fl_Input*>*)data;
	inputs->first->value(cell->coords.c_str());
	inputs->second->value(cell->expr.c_str());
	inputs->second->take_focus();
}