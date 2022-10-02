
#include <utility>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Menu_Bar.H>
#include <memory>

#include "Table.hpp"

namespace window
{
	using namespace std;
	class Window
	{
	public:
		shared_ptr<Fl_Window> win;
		shared_ptr<Fl_Menu_Bar> menu_bar;
		shared_ptr<Fl_Group> input_group;
		shared_ptr<Fl_Input> pos_input;
		shared_ptr<Fl_Input> expr_input;
	public:
		Window()
		{
			int x = 800;
			int y = 500;
			int sp = 10;
			int menu_height = 30;
			int input_height = 25;
			int pos_width = 50;
			int input_width = 300;
			
			win.reset(new Fl_Window(x, y, "MyExel"));
			win->size_range(400, 200);


			Fl_Group* gr = new Fl_Group(0, y, x, 30);
			win->resizable(gr);
			gr->end();

			menu_bar.reset(new Fl_Menu_Bar(0, 0, x, menu_height));
			menu_bar->add("Save|Open|Clear|@<|@>|@2>|@2<");

			input_group.reset(new Fl_Group(0, menu_height, x, 2*sp + input_height));
			pos_input.reset(new Fl_Input(sp, input_group->y() + sp, pos_width, input_height));
			pos_input->box(FL_BORDER_BOX);
			expr_input.reset(new Fl_Input(2*sp + pos_width, input_group->y() + sp, x - 3*sp - pos_width, input_height));
			input_group->resizable(expr_input.get());
			input_group->end();

			MyTable* tb (new MyTable(0, input_group->y() + input_group->h(), x, 500));

			win->show();
		}
		~Window() {}
	};
	
} // namespace window
