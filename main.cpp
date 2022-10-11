#include <gtkmm.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Parser.hpp"

using namespace std;
using namespace Gtk;

namespace interface {
	const int margin = 7;
	const int menu_height = 10;
	class MenuButton : public Button 
	{
	public:
		MenuButton(string name) 
		{
			set_margin(7);
			set_label(name);
			set_size_request(50, 10);
			set_expand(false);
		}
	};

	class MenuBox : public Box 
	{
	public:
		MenuButton* but_save;
		MenuButton* but_load;
		MenuButton* but_clear;
		MenuButton* but_add_row;
		MenuButton* but_rem_row;
		MenuButton* but_add_col;
		MenuButton* but_rem_col;
		MenuBox() 
		{
			this->set_orientation(Orientation::HORIZONTAL);
			but_save = make_managed<MenuButton>("Save");
			but_load = make_managed<MenuButton>("Load");
			but_clear = make_managed<MenuButton>("Clear");
			but_add_row = make_managed<MenuButton>("+");
			auto but_row = make_managed<MenuButton>("row");
			but_rem_row = make_managed<MenuButton>("-");
			but_add_col = make_managed<MenuButton>("+");
			auto but_col = make_managed<MenuButton>("col");
			but_rem_col = make_managed<MenuButton>("-");

			but_row->set_sensitive(false);
			but_col->set_sensitive(false);

			append(*but_save);
			append(*but_load);
			append(*but_clear);
			append(*but_add_row);
			append(*but_row);
			append(*but_rem_row);
			append(*but_add_col);
			append(*but_col);
			append(*but_rem_col);
		}
	};

	class Cell : public Entry 
	{
		string coords;
		string expr;
		static Entry* inp_coords;
		static Entry* inp_expr;
		static function<void(string, string)> set_shit_expr;
	public:
		Cell(string coords_, int w, int h, bool header = false) : coords(coords_)
		{
			if(header)
				set_text(coords);
			set_width_chars(6);
			set_max_width_chars(6);
			set_expand(false);
			set_size_request(-1, 35);
			auto provider = CssProvider::create();
			provider->load_from_data(".button {font-size: 20px; border-radius: 0; border-width: 1px; outline-color: yellow; border-color: grey; border-style: solid;}");
			add_css_class("button");
			get_style_context()->add_provider(provider, GTK_STYLE_PROVIDER_PRIORITY_USER);

			set_on_get_focus([&] {
				inp_coords->set_text(coords);
				inp_expr->set_text(expr);	
			});
			set_on_press_enter();
			set_on_focus_leave();
		}
		string get_coords() const { return coords; }
		void set_expr(const string& expr_) { expr = expr_; }

		static void set_func(function<void(string,string)> func) { set_shit_expr = func; }
		static void set_entries(Entry* coords_, Entry* expr_)  {
			inp_coords = coords_;
			inp_expr = expr_;
		}
		void set_on_get_focus(function<void()> func) 
		{
			auto controller = Gtk::EventControllerFocus::create();
			controller->signal_enter().connect([func, this]{
				func();
				this->set_text(expr);
			}, false);
			add_controller(controller);
		}
		void set_on_press_enter() 
		{
			auto key_press = EventControllerKey::create();
			
			key_press->signal_key_released().connect([&](guint keyval, guint, Gdk::ModifierType state){
				if(keyval == 65293) {
					expr = this->get_text();
					set_shit_expr(coords, this->get_text());
					inp_expr->set_text(expr);
					this->set_text(expr);
				}
			}, false);
			
			add_controller(key_press);
		}
		void set_on_focus_leave()
		{
			auto controller = Gtk::EventControllerFocus::create();
			controller->signal_leave().connect([&]{
				expr = this->get_text();
				set_shit_expr(coords, this->get_text());
				inp_expr->set_text(expr);
			}, false);
			add_controller(controller);
		}
		string get_expr() { return expr; }
	};
	Entry* Cell::inp_coords = nullptr;
	Entry* Cell::inp_expr = nullptr;
	function<void(string, string)> Cell::set_shit_expr = nullptr;
	class InputBox : public Box 
	{
		Entry* inp_expr;
		Entry* inp_coord;
	public:
		InputBox() 
		{
			this->set_orientation(Orientation::HORIZONTAL);
			inp_coord = make_managed<Entry>();
			inp_coord->set_width_chars(6);
			inp_coord->set_max_width_chars(6);
			inp_coord->set_margin(margin);
			inp_coord->set_hexpand(false);

			inp_expr = make_managed<Entry>();
			inp_expr->set_size_request(60, menu_height);
			inp_expr->set_margin(margin);
			inp_expr->set_hexpand(true);
			
			auto act_entr = Gio::SimpleActionGroup::create();

			append(*inp_coord);
			append(*inp_expr);
			
			Cell::set_entries(inp_coord, inp_expr);	
		}
		void set_enter_event(function<void(const string&, const string&)> set_expr) {
			auto key_press = EventControllerKey::create();
			
			key_press->signal_key_released().connect([=](guint keyval, guint, Gdk::ModifierType state){
				if(keyval == 65293) {
					set_expr(inp_coord->get_text(), inp_expr->get_text());
				}
			}, false);
			
			inp_expr->add_controller(key_press);
		}
	};
	
	class Spreadsheet : public Grid 
	{
		const int w = 100;
		const int h = 40;
		int cols, rows;
		parser::Tockenizer tock;
		parser::Parser pars{tock, [this](const string& str) -> double { return this->calc_cell_expr(str);}};
	public:
		Spreadsheet(int rows_ = 5, int cols_ = 5) : cols(cols_), rows(rows_)
		{
			Cell::set_func([this](const string& str1, const string& str2) { this->set_expression(str1, str2);});
			set_margin(margin);
			cols++;
			rows++;
			for(int i = 1; i < rows; i++) {
				auto cell = make_managed<Cell>(getCoords(pair{i, 0}), w, h, true);
				cell->set_sensitive(false);
				attach(*cell, 0, i);
			}
			for(int j = 1; j < cols; j++) {
				auto cell = make_managed<Cell>(to_string(j), w, h, true);
				cell->set_sensitive(false);
				attach(*cell, j, 0);
			}
			for(int i = 1; i < rows; i++) {
				for(int j = 1; j < cols; j++) {
					auto cell = make_managed<Cell>(getCoords(pair{i, j}), w, h);
					attach(*cell, j, i);
				}
			}
		}
		void add_col() 
		{
			auto cell = make_managed<Cell>(to_string(cols), w, h, true);
			cell->set_sensitive(false);
			attach(*cell, cols, 0);
			for(int i = 1; i < rows; i++) {
				auto cell = make_managed<Cell>(getCoords(pair{i, cols}), w, h);
				attach(*cell, cols, i);
			}
			cols++;
		}
		void add_row() 
		{
			auto cell = make_managed<Cell>(getCoords(pair{rows, 0}), w, h, true);
			cell->set_sensitive(false);
			attach(*cell, 0, rows);
			for(int i = 1; i < cols; i++) {
				auto cell = make_managed<Cell>(getCoords(pair{rows, i}), w, h);
				attach(*cell, i, rows);
			}
			rows++;
		}

		void rem_col()
		{
			if(cols <= 2)
				return ;
			cols--;
			remove_column(cols);
			renew();
		}

		void rem_row() 
		{
			if(rows <= 2)
				return ;
			rows--;
			remove_row(rows);
			renew();
		}

		// pair.fist = row, pair.second = col
		string getCoords(pair<int, int> coords) {
			string res;
   		 	char tmp;
			int n = coords.first;
			while(n){
				n -= 1;
				tmp = 'A' + n % 26;
				res = tmp + res;
				n /= 26;
			}
			if(coords.second)
				res += to_string(coords.second);
			return res;
		}

		// pair.fist = row, pair.second = col
		pair<int, int> getCoords(string coords) {
			int result = 0;
			string columnTitle;
			int i = 0;
			for(; isalpha(coords[i]); i++)
				columnTitle += toupper(coords[i]);
			for(char c : columnTitle)
			{
				int d = c - 'A' + 1;
				result = result * 26 + d;
			}
			pair<int, int> res;
			res.first = result;
			res.second = stoi(coords.erase(0, i));
			return res;			
		}

		double string_to_double(string str) {
			size_t a = 0;
			double tmp = stod(str, &a);
			if(a != str.size())
				throw runtime_error("not full number");
			return tmp;
		}

		void set_expression(const string& coords, const string& expres)
		{
			if(!tock.validateCoords(coords)) 
				return ;
			auto int_coords = getCoords(coords);
			auto cell_widget = get_child_at(int_coords.second, int_coords.first);
			if(!cell_widget)
				return;
			auto cell = static_cast<Cell*>(cell_widget);
			cell->set_expr(expres);
			renew();
		}

		void renew() {
			for(int i = 1; i < rows; i++) {
				for(int j = 1; j < cols; j++) {
					auto cell_widget = get_child_at(j, i);
					auto cell = static_cast<Cell*>(cell_widget);
					if(!cell->get_expr().size())
						continue;
					try {
						double double_expr = calc_cell_expr(pair{i, j});
						cell->set_text(tostring(double_expr));
					} catch(exception& ex) {
						cell->set_text(cell->get_expr());
						cout << ex.what() << endl;
					}
				}
			}
		}

		double calc_cell_expr(const string& coords)
		{
			return calc_cell_expr(getCoords(coords));
		}

		// pair.first = row, pair.second = col
		double calc_cell_expr(const pair<int, int>& coords)
		{
			auto cell_widget = get_child_at(coords.second, coords.first);
			if(!cell_widget) {
				throw runtime_error("Bad coordinates");
			}
			auto cell = static_cast<Cell*>(cell_widget);
			if(cell->get_expr()[0] == '=') 
				return pars.parse(cell->get_expr().erase(0, 1));
			else
				return string_to_double(cell->get_expr());
		}

		string tostring(double num) {
			stringstream strg;
			strg << fixed;
			strg << setprecision(2);
			strg << num;
			return strg.str();
		}

		void clear()
		{
			for(int i = 1; i < rows; i++)
			for (int j = 1; j < cols; j++)
			{
				auto cell_widget = get_child_at(j, i);
				auto cell = static_cast<Cell*>(cell_widget);
				cell->set_expr("");
				cell->set_text("");
			}
		}
	};

	class ScrollableTable : public ScrolledWindow
	{
	public:
		Spreadsheet* table;
		ScrollableTable() 
		{
			table = make_managed<Spreadsheet>(5, 10);
			set_propagate_natural_width(false);
			set_size_request(100, 100);
			set_child(*table);
			set_expand(true);
		}
	};

}

class ExitDialog : public MessageDialog
{
public:
	ExitDialog(Window& parent) : MessageDialog(parent, "Do you really wanna exit?", false, 
		MessageType::QUESTION, ButtonsType::OK_CANCEL, true)
	{
		set_secondary_text("You may need to save your spreadsheet first");
		set_hide_on_close(true);
		signal_response().connect([&](int response_id){
			if(response_id == ResponseType::CANCEL) {
				this->hide();
			} else {
				this->hide();
				parent.hide();
			}
		});
	}
};

class FileSaver : public FileChooserNative
{
public:
	FileSaver(Window& parent, function<void(const Glib::ustring&)> manage_saving) : FileChooserNative("Please choose a file to save",
		Gtk::FileChooser::Action::SAVE, "Save", "Cancel")
	{
		set_transient_for(parent);
		set_modal(true);
		signal_response().connect([this, manage_saving](int response){
			if(response == ResponseType::ACCEPT) {
				manage_saving(get_file()->get_path());
			}
		});
	}
};

class FileLoader : public FileChooserNative
{
public:
	FileLoader(Window& parent, function<void(const Glib::ustring&)> manage_load) : FileChooserNative("Plese choose a file to load",
		FileChooser::Action::OPEN, "Load", "Cancel")
	{
		set_transient_for(parent);
		set_modal(true);
		signal_response().connect([this, manage_load] (int response) {
			if(response == ResponseType::ACCEPT) {
				cout << "Ok" ;
			}
		});
	}
};

class MyWindow : public Window 
{
	Box outer_box;
	unique_ptr<MessageDialog> ex_dialog;
	unique_ptr<FileSaver> save_dialog;
	unique_ptr<FileLoader> load_dialog;
public:
	MyWindow() : Window() {
		outer_box.set_orientation(Orientation::VERTICAL);
		outer_box.set_homogeneous(false);
		auto menu_box = make_managed<interface::MenuBox>();
		outer_box.append(*menu_box);
		auto inp_box = make_managed<interface::InputBox>();
		outer_box.append(*inp_box);
		auto scroll_table = make_managed<interface::ScrollableTable>();
		outer_box.append(*scroll_table);

		inp_box->set_enter_event(sigc::mem_fun(*scroll_table->table, &interface::Spreadsheet::set_expression));

		menu_box->but_add_row->signal_clicked().connect([scroll_table]{scroll_table->table->add_row();});
		menu_box->but_add_col->signal_clicked().connect([scroll_table]{scroll_table->table->add_col();});
		menu_box->but_rem_col->signal_clicked().connect([scroll_table]{scroll_table->table->rem_col();});
		menu_box->but_rem_row->signal_clicked().connect([scroll_table]{scroll_table->table->rem_row();});
		menu_box->but_clear->signal_clicked().connect([scroll_table]{scroll_table->table->clear();});

		ex_dialog.reset(new ExitDialog(*this));
		signal_close_request().connect([this]{ ex_dialog->show(); return true; }, true);

	
		save_dialog.reset(new FileSaver(*this, [](Glib::ustring) {}));
		menu_box->but_save->signal_clicked().connect([this]{ save_dialog->show(); });

		load_dialog.reset(new FileLoader(*this, [](string){}));
		menu_box->but_load->signal_clicked().connect([this]{ load_dialog->show(); });

		set_child(outer_box);
	}
private:
	void on_button_clicked() {
		std::cout << "something" << std::endl;
		cout << get_width() << endl;
	}

};

int main(int argc, char* argv[])
{
	auto app = Gtk::Application::create("org.gtkmm.examples.base");
  	return app->make_window_and_run<MyWindow>(argc, argv);
}