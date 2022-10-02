#include <FL/Fl.H>
#include <FL/Fl_Table.H>
#include <FL/fl_draw.H>
#include <string>


#define MAX_ROWS 30
#define MAX_COLS 26             // A-Z



namespace window {	
	using namespace std;
	struct Cell 
	{
		string data;
		string expr;
		Cell(string expr_, string& data_) : data(data_), expr(expr_) {}
		Cell() {}
	};

	// Derive a class from Fl_Table
	class MyTable : public Fl_Table {

		Cell data[MAX_ROWS][MAX_COLS];         // data array for cells

		// Draw the row/col headings
		//    Make this a dark thin upbox with the text inside.
		//
		void DrawHeader(const char *s, int X, int Y, int W, int H) {
			fl_push_clip(X,Y,W,H);
			fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, row_header_color());
			fl_color(FL_BLACK);
			fl_draw(s, X,Y,W,H, FL_ALIGN_CENTER);
			fl_pop_clip();
		}
		// Draw the cell data
		//    Dark gray text on white background with subtle border
		//
		void DrawData(const char *s, int X, int Y, int W, int H) {
			fl_push_clip(X,Y,W,H);
			// Draw cell bg
			fl_color(FL_WHITE); fl_rectf(X,Y,W,H);
			// Draw cell data
			fl_color(FL_GRAY0); fl_draw(s, X,Y,W,H, FL_ALIGN_CENTER);
			// Draw box border
			fl_color(color()); fl_rect(X,Y,W,H);
			fl_pop_clip();
		}
		// Handle drawing table's cells
		//     Fl_Table calls this function to draw each visible cell in the table.
		//     It's up to us to use FLTK's drawing functions to draw the cells the way we want.
		//
		void draw_cell(TableContext context, int ROW=0, int COL=0, int X=0, int Y=0, int W=0, int H=0) {
			static char s[40];
			const char *cs;
			switch ( context ) {
			case CONTEXT_STARTPAGE:                   // before page is drawn..
				fl_font(FL_HELVETICA, 16);              // set the font for our drawing operations
				return;
			case CONTEXT_COL_HEADER:                  // Draw column headers
				sprintf(s,"%c", COL+1);                // "A", "B", "C", etc.
				DrawHeader(s,X,Y,W,H);
				return;
			case CONTEXT_ROW_HEADER:                  // Draw row headers
				sprintf(s,"%03d:", 'A' + ROW);                 // "001:", "002:", etc
				DrawHeader(s,X,Y,W,H);
				return;
			case CONTEXT_CELL:                        // Draw data in cells
				cs = data[ROW][COL].data.c_str();
				DrawData(cs,X,Y,W,H);
				return;
			default:
				return;
			}
		}
		public:
		// Constructor
		//     Make our data array, and initialize the table options.
		//
		MyTable(int X, int Y, int W, int H, const char *L=0) : Fl_Table(X,Y,W,H,L) {
			// Rows
			rows(MAX_ROWS);             // how many rows
			row_header(1);              // enable row headers (along left)
			row_height_all(20);         // default height of rows
			row_resize(0);              // disable row resizing
			// Cols
			cols(MAX_COLS);             // how many columns
			col_header(1);              // enable column headers (along top)
			col_width_all(80);          // default width of columns
			col_resize(1);              // enable column resizing
			end();                      // end the Fl_Table group
		}
		~MyTable() { }
	};
}