#include <gtkmm.h>
#include <iostream>

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
		MenuBox() 
		{
			this->set_orientation(Orientation::HORIZONTAL);
			auto but_save = make_managed<MenuButton>("Save");
			auto but_load = make_managed<MenuButton>("Load");
			auto but_clear = make_managed<MenuButton>("Clear");
			auto but_add_row = make_managed<MenuButton>("+");
			auto but_row = make_managed<MenuButton>("row");
			auto but_rem_row = make_managed<MenuButton>("-");
			auto but_add_col = make_managed<MenuButton>("+");
			auto but_col = make_managed<MenuButton>("col");
			auto but_rem_col = make_managed<MenuButton>("-");

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

	class InputBox : public Box 
	{
	public:
		InputBox() 
		{
			this->set_orientation(Orientation::HORIZONTAL);
			auto inp_coord = make_managed<Entry>();
			inp_coord->set_width_chars(6);
			inp_coord->set_max_width_chars(6);
			inp_coord->set_margin(margin);
			inp_coord->set_hexpand(false);

			auto inp_expr = make_managed<Entry>();
			inp_expr->set_size_request(60, menu_height);
			inp_expr->set_margin(margin);
			inp_expr->set_hexpand(true);

			append(*inp_coord);
			append(*inp_expr);
		}
	};

	class Cell : public Entry 
	{
		Label* label;
	public:
		Cell(string data, int w, int h)
		{
			set_width_chars(6);
			set_max_width_chars(6);
			set_expand(false);
			set_size_request(-1, 35);
			auto provider = CssProvider::create();
			provider->load_from_data(".button {font-size: 20px; border-radius: 0; border-width: 1px; outline-color: yellow; border-color: grey; border-style: solid;}");
			add_css_class("button");
			get_style_context()->add_provider(provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
			//this->set_css_classes(Glib::OptionGroup::vecustrings{"font-size: 30px", "height: 90px"});
		}
	};
	
	class Spreadsheet : public Grid 
	{
		const int w = 100;
		const int h = 40;
	public:
		Spreadsheet(int rows = 5, int cols = 5) 
		{
			set_margin(margin);
			for(int i = 0; i < rows; i++) {
				for(int j = 0; j < cols; j++) {
					if(!(i+j))
						continue;
					auto cell = make_managed<Cell>("das", w, h);
					attach(*cell, j, i);
				}
			}
		}
		
	};

	class ScrollableTable : public ScrolledWindow
	{
	public:
		ScrollableTable() 
		{
			auto table = make_managed<Spreadsheet>(5, 10);
			set_propagate_natural_width(false);
			set_child(*table);
			set_expand(true);
		}
	};

}

class MyWindow : public Window 
{
	Box outer_box;
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

		set_child(outer_box);
	}
private:
	void on_button_clicked() {
		std::cout << "something" << std::endl;
		cout << get_width() << endl;
	}

};

class TestWin : public Window
{
public:
	TestWin() {
		auto scroll = make_managed<ScrolledWindow>();
		scroll->set_hadjustment(scroll->get_vadjustment());
		auto grid = make_managed<Grid>();
		grid->set_size_request(1000, 1000);
		//scroll->set_size_request(100, 100);
		for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
		{
			auto entr = make_managed<Entry>();
			entr->set_text("alsdfkl");
			entr->set_size_request(40, 20);
			grid->attach(*entr, i, j);
		}
		scroll->set_child(*grid);
		set_child(*scroll);
	}
};

int main(int argc, char* argv[])
{
	auto app = Gtk::Application::create("org.gtkmm.examples.base");
  	return app->make_window_and_run<MyWindow>(argc, argv);
}