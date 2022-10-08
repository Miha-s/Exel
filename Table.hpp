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
		Cell() : Fl_Input(0, 0, 0, 0) {}
	};
	
	class Table {
		list<list<Cell*>> the_table;
		list<Cell*> row;
		list<Cell*> col;
		Cell* invis;
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
			invis = tb.invis;
		}

		Table(int w, int h, int x, int y, int rows, int cols) :
			cell_w(w), cell_h(h)
		{
			table_gr.reset(new Fl_Group(x, y,  (cols+2)*w, (rows+2)*h));
			invis = new Cell("asdf", x + (cols+1)*w , y + (rows+1)*h , w, h);
			//invis->hide();
			table_gr->add(invis);
			table_gr->resizable(invis);
			int rowx = x;
			int rowy = y + h;
			int colx = x + w;
			int coly = y;
			string tmp;


			for(int i = 0; i < rows; i++) {
				tmp = getCoords(pair<int, int>(i, 0));
				Cell* c = new Cell(tmp, rowx, rowy + i*h, w, h);
				c->readonly(true);
				row.push_back(move(c));
			}
			for(int i = 0; i < cols; i++) {
				Cell* c = new Cell(to_string(i+1) , colx + i*w, coly, w, h);
				c->readonly(true);
				col.push_back(move(c));
			}

			int cellx = x + w;
			int celly = y + h;
			for(int i = 0; i < rows; i++) {
				the_table.push_back(list<Cell*>());
				for(int j = 0; j < cols; j++) {
					the_table.back().push_back(new Cell("", cellx + j*w, celly + i*h, w, h));
					auto the_cell = the_table.back().back();
					the_cell->coords = getCoords(pair<int, int>(i, j+1));
					table_gr->add(the_table.back().back());
				}
			}
			table_gr->end();
		}

		void add_row() {
			string row_pos; 
			row_pos = getCoords(pair<int, int>(the_table.size(), 0));
			the_table.push_back(list<Cell*>());
			int x = row.back()->x();
			int y = row.back()->y();
			y += cell_h;
			row.push_back(new Cell(row_pos, x, y, cell_w, cell_h));
			table_gr->add(row.back());
			x += cell_w;
			auto& last = the_table.back();
			for(unsigned int i = 0; i < the_table.front().size(); i++) {
				last.push_back(new Cell("", x + i*cell_w, y, cell_w, cell_h));
				last.back()->coords = getCoords(pair<int, int> (the_table.size(), i+1));
				table_gr->add(last.back());
			}

			invis->resize(last.back()->x() + cell_w, last.back()->y() + cell_h, invis->w(), invis->h()-cell_h);
			table_gr->redraw();
		}
		void add_col() {
			string col_pos;
			col_pos = to_string(the_table.front().size() + 1);
			int x = col.back()->x();
			int y = col.back()->y();
			x += cell_w;
			col.push_back(new Cell(col_pos, x, y, cell_w, cell_h));
			table_gr->add(col.back());
			y += cell_h;
			int i = 0;
			string tmp;
			for(auto &elem : the_table) {
				elem.push_back(new Cell("", x, y + i*cell_h, cell_w, cell_h));
				elem.back()->coords = getCoords(pair<int, int>(i, elem.size()));
				table_gr->add(elem.back());
				++i;
			}
			invis->resize(the_table.back().back()->x() + cell_w, the_table.back().back()->y() + cell_h, invis->w()-cell_w, invis->h());
			table_gr->redraw();

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

		shared_ptr<Fl_Group> as_group() { return table_gr; }

		string getCoords(pair<int, int> coords) {
			string res;
			res += char('A' + coords.first);
			if(coords.second)
				res += to_string(coords.second);
			return res;
		}
		pair<int, int> getCoords(string coords) {
			pair<int, int> res;
			res.first = tolower(coords[0]) - 'a';
			res.second = string_to_double(coords.erase(0, 1));
			return res;			
		}
	};
}