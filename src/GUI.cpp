

/* 
IMPLEMENT_APP(MyApp)
bool MyApp::OnInit()
{
	//dp->Show(true);
	return true;
}


DrawFrame::DrawFrame(const wxString& title, const wxPoint & pos, int high, int wide) 
	: wxFrame(NULL, wxID_ANY, title,pos, wxSize(high,wide))
{
	
}

DrawPanel::DrawPanel(wxFrame* parent):wxPanel(parent) {
	Bind(wxEVT_PAINT, &DrawPanel::OnPaint, this);
}




void DrawPanel::OnPaint(wxPaintEvent & event) {
	wxPaintDC dc(this);
	dc.Clear();

	for (auto & i : pix) {
		dc.SetPen(wxPen(wxColor(i->r, i->g, i->b)));
		dc.DrawPoint(i->x, i->y);
		delete i;
	}
}

void DrawPanel::addPixel(RenderPixel *pixel) {
	pix.push_back(pixel);
}
*/