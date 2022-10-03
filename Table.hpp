#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Table.H>
#include <FL/fl_draw.H>
#include <Fl/Fl_Input.H>
#include <memory>
#include <string>
#include <list>
#include "Parser.hpp"


#define MAX_ROWS 30
#define MAX_COLS 26             // A-Z



namespace window {	
	using namespace std;
	using namespace parser;
	struct Cell 
	{
		string expr;
		shared_ptr<Fl_Input> the_cell;
		Cell(string expr_, int x, int y, int w, int h) : expr(expr_) {
			the_cell.reset(new Fl_Input(x, y, w, h));
			the_cell->box(FL_BORDER_BOX);
			//the_cell->color(FL_BACKGROUND_COLOR);
			the_cell->value(expr.c_str());
		}
		Cell(Cell&& c) {
			expr = move(c.expr);
			the_cell = move(c.the_cell);
		}
		Cell() {}
	};
	
	struct TestCell : public Fl_Input
	{
		string expr;
		TestCell(string expr_, int x, int y, int w, int h) : Fl_Input(x, y, w, h), expr(expr_) {
			this->box(FL_BORDER_BOX);
			//the_cell->color(FL_BACKGROUND_COLOR);
			this->value(expr.c_str());
		}
		// TestCell(TestCell&& c) {
		// 	expr = move(c.expr);
		// 	this
		// }
		TestCell() : Fl_Input(0, 0, 0, 0) {}
	};
	
	class Table {
		list<list<Cell>> the_table;
		list<Cell> row;
		list<Cell> col;
		shared_ptr<Fl_Group> table_gr;
		int cell_w;
		int cell_h;
		Tockenizer tock{};
		list<Cell>::iterator at(pair<int, int> coord) {
			auto iter = the_table.begin();
			for(int i = 0; i < coord.first; i++)  { iter++; }
			auto iter2 = iter->begin();
			for(int i = 0; i < coord.second; i++) { iter2++; }
			return iter2;
		}
		Parser par{tock, [&](string a) -> double {
			auto coords = get_coord(a);
			auto iter = at(coords);
			return get_expression(*iter);
		}};
		pair<int, int> get_coord(string coord) {
			pair<int, int> coords;
			coords.first = coord[0] - 'A';
			coords.second = coord[1] - '1';
			return coords;
		}

		std::pair<Fl_Input*, Fl_Input*> inputs;
	public:
		Table() {}
		void operator=(Table&& tb) {
			the_table = move(tb.the_table);
			row = move(tb.row);
			col = move(tb.col);
			table_gr = move(tb.table_gr);
			cell_w = tb.cell_w;
			cell_h = tb.cell_h;
		}
		Table(int w, int h, int x, int y, int rows, int cols) :
			cell_w(w), cell_h(h)
		{
			table_gr.reset(new Fl_Group(x, y,  (cols+2)*w, (rows+2)*h));
			auto invis = new Cell("asdf", x + (cols+1)*w, y + (rows+1)*h, w, h);
			invis->the_cell->hide();
			table_gr->add(invis->the_cell.get());
			table_gr->resizable(invis->the_cell.get());
			int rowx = x;
			int rowy = y + h;
			int colx = x + w;
			int coly = y;
			string tmp;

			for(int i = 0; i < rows; i++) {
				tmp = 'A';
				tmp[0] += i;
				Cell c(tmp, rowx, rowy + i*h, w, h);
				c.the_cell->readonly(true);
				row.push_back(move(c));
			}
			for(int i = 0; i < cols; i++) {
				Cell c(to_string(i+1) , colx + i*w, coly, w, h);
				c.the_cell->readonly(true);
				row.push_back(move(c));
			}

			int cellx = x + w;
			int celly = y + h;
			for(int i = 0; i < rows; i++) {
				the_table.push_back(list<Cell>());
				for(int j = 0; j < cols; j++) {
					the_table.back().push_back(Cell("", cellx + j*w, celly + i*h, w, h));
					table_gr->add(the_table.back().back().the_cell.get());
				}
			}
			table_gr->end();
		}

		double get_expression(Cell& the_cell) {
			if(the_cell.expr[0] == '=')
				return par.parse(string(the_cell.expr).erase(0, 1));
			else
				return par.parse(the_cell.expr);
		}

		void setExpr(string coord, string expr) {
			auto coords = get_coord(coord);
			auto iter = at(coords);
			iter->expr = expr;
			renew();
		}
		void set_inputs(Fl_Input* one, Fl_Input* two) {
			inputs.first = one;
			inputs.second = two;
		}
		void renew() {
			auto iter = the_table.begin();
			for(auto& vec : the_table) {
				for(auto& elem : vec) {
					double res = get_expression(elem);
					elem.the_cell->value(to_string(res).c_str());
				}
			}
		}

		shared_ptr<Fl_Group> as_group() { return table_gr; }
	};
}