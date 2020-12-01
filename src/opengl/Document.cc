#include "opengl/GLWinFonts.hh"
#include "opengl/Document.hh"
#include "opengl/DocView.hh"
#include <fstream>
#include <sstream>
#include "util/DynArray.hh"
using namespace std;

Page::Page(const unsigned char *text,
					 const PageLayout *layout, uint32_t offset)
		: text(text), layout(layout)
{
	// TODO: what to do with offset, store as local variable?
}

void Document::appendFile(const PageLayout &newLayout, const char filename[])
{
	ifstream t(filename);
	t.seekg(0, ios::end);
	uint32_t len = t.tellg();
	text = new unsigned char[len];
	t.seekg(0, std::ios::beg);
	t.read((char *)text, len);
	const Font *f = layout.f;
	pages.push_back(Page(text, &layout, 0));
	Page *page = &pages.back();
	float x = layout.x0, y = layout.y0;
	float lineSize = f->getHeight();
	page->addLine(0);
	for (uint32_t i = 0; i < len; i++)
	{
		if (text[i] == '\n')
		{
			page->addLine(++i);
		}
		else if (text[i] < ' ')
		{
			cerr << int(text[i]) << ' ';
			i++;
		}

		const Font::Glyph *glyph = f->getGlyph(text[i]);
		x += glyph->advance;
		if (x > layout.x1)
		{
			x = layout.x0;
			y += lineSize;
			if (y >= layout.y1)
			{
				pages.push_back(Page(text, &layout, i));
				page = &pages.back();
				page->addLine(i);
				x = layout.x0 + glyph->advance;
				y = layout.y0;
			}
			else
			{
				page->addLine(i);
			}
		}
	}
	page->addLine(len);
}
