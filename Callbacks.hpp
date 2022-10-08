#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Table.H>
#include <FL/fl_draw.H>
#include <Fl/Fl_Input.H>

void enter_expression_call(Fl_Widget* widget, void* data);

void press_cell_call(Fl_Widget* widget, void* data);

void add_row_call(Fl_Widget* widget, void* data);

void add_col_call(Fl_Widget* widget, void* data);