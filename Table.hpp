#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Table.H>
#include <FL/fl_draw.H>
#include <Fl/Fl_Input.H>
#include <memory>
#include <string>
#include <list>
#include <sstream>
#include <exception>
#include "Parser.hpp"
#include "Callbacks.hpp"
#include <iomanip>



#define MAX_ROWS 30
#define MAX_COLS 26             // A-Z



namespace window {	
	using namespace std;
	using namespace parser;
	struct Cell : public Fl_Input
	{
		string expr;
		string coords;
		Cell(string expr_, int x, int y, int w, int h) : Fl_Input(x, y, w, h), expr(expr_) {
			this->box(FL_BORDER_BOX);
			//the_cell->color(FL_BACKGROUND_COLOR);
			this->value(expr.c_str());
		}
		// TestCell(TestCell&& c) {
		// 	expr = move(c.expr);
		// 	this
		// }
		Cell() : Fl_Input(0, 0, 0, 0) {}
	};
	
	class Table {
		list<list<Cell*>> the_table;
		list<Cell*> row;
		list<Cell*> col;
		shared_ptr<Fl_Group> table_gr;
		int cell_w;
		int cell_h;
		Tockenizer tock{};
		list<Cell*>::iterator at(pair<int, int> coord) {
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
			coords.first = toupper(coord[0]) - 'A';
			coords.second = coord[1] - '1';
			return coords;
		}

		std::pair<Fl_Input*, Fl_Input*>* inputs;
	public:
		Table() {}
		void operator=(Table&& tb) {
			the_table = move(tb.the_table);
			row = move(tb.row);
			col = move(tb.col);
			table_gr = move(tb.table_gr);
			cell_w = tb.cell_w;
			cell_h = tb.cell_h;
			inputs = tb.inputs;
		}
		Table(int w, int h, int x, int y, int rows, int cols) :
			cell_w(w), cell_h(h)
		{
			table_gr.reset(new Fl_Group(x, y,  (cols+2)*w, (rows+2)*h));
			auto invis = new Cell("asdf", x + (cols+1)*w, y + (rows+1)*h, w, h);
			invis->hide();
			table_gr->add(invis);
			table_gr->resizable(invis);
			int rowx = x;
			int rowy = y + h;
			int colx = x + w;
			int coly = y;
			string tmp;

			for(int i = 0; i < rows; i++) {
				tmp = 'A';
				tmp[0] += i;
				Cell* c = new Cell(tmp, rowx, rowy + i*h, w, h);
				c->readonly(true);
				row.push_back(move(c));
			}
			for(int i = 0; i < cols; i++) {
				Cell* c = new Cell(to_string(i+1) , colx + i*w, coly, w, h);
				c->readonly(true);
				row.push_back(move(c));
			}

			int cellx = x + w;
			int celly = y + h;
			for(int i = 0; i < rows; i++) {
				the_table.push_back(list<Cell*>());
				for(int j = 0; j < cols; j++) {
					the_table.back().push_back(new Cell("", cellx + j*w, celly + i*h, w, h));
					auto the_cell = the_table.back().back();
					the_cell->coords = getCoordName(i, j);
					table_gr->add(the_table.back().back());
				}
			}
			table_gr->end();
		}
		double string_to_double(string str) {
			size_t a = 0;
			double tmp = stod(str, &a);
			if(a != str.size())
				throw runtime_error("not full number");
			return tmp;
		}
		double get_expression(Cell* the_cell) {
			if(the_cell->expr[0] == '=')
				return par.parse(string(the_cell->expr).erase(0, 1));
			else {
				return string_to_double(the_cell->expr);
			}
				
		}

		void setExpr(string coord, string expr) {
			auto coords = get_coord(coord);
			auto iter = at(coords);
			(*iter)->expr = expr;
			renew();
		}
		void set_inputs(Fl_Input* one, Fl_Input* two) {
			inputs = new pair<Fl_Input*, Fl_Input*>;
			inputs->first = one;
			inputs->second = two;
			for(auto& vec : the_table)
			for(auto& elem : vec) {
					elem->callback(press_cell_call, (void*)inputs);
					elem->when(FL_FOCUS);
			}
		}
		void renew() {
			for(auto& vec : the_table) {
				for(auto& elem : vec) {
					double res;
					if(!elem->expr.empty()) {
						try {
							res = get_expression(elem);
							elem->value(tostring(res).c_str());
						} 
						catch(exception& ex) {
							cout << "Error: " << ex.what() << endl;
							elem->value(elem->expr.c_str());
						}
					} else {
						elem->value("");
					}
				}
			}
		}
		string tostring(double num) {
			stringstream strg;
			strg << fixed;
			strg << setprecision(2);
			strg << num;
			return strg.str();
		}
		string getCoordName(int i, int j) {
			string s;
			s += char('A' + i);
			s += to_string(1+j);
			return s;
		}
		shared_ptr<Fl_Group> as_group() { return table_gr; }
	};
}