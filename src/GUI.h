
/* 
struct RenderPixel {
	int x;
	int y;
	int r;
	int g;
	int b;
};


class DrawFrame : public wxFrame {
public:
	DrawFrame(const wxString& title, const wxPoint &, int high, int wide);
};

class DrawPanel : public wxPanel
{

public:
	DrawPanel(wxFrame* parent);
	std::vector<RenderPixel*> pix;
	void OnPaint(wxPaintEvent & event);
	void addPixel(RenderPixel*);
	DECLARE_EVENT_TABLE()
};



class MyApp : public wxApp
{
public:
	wxFrame* df;
	DrawPanel* dp;
	virtual bool OnInit();
};
DECLARE_APP(MyApp)
*/


