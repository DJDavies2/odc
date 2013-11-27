/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/log/Log.h"
#include "eckit/filesystem/PathName.h"
#include "eckit/config/Resource.h"
#include "eckit/web/Html.h"
#include "eckit/web/HtmlObject.h"
#include "eckit/web/HtmlResource.h"
#include "eckit/web/HttpBuf.h"
#include "eckit/web/Url.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

ostream& operator<<(ostream& s,const Html::Tag& tag)
{
	s << HttpBuf::dontEncode;	
	tag.print(s);
	s << HttpBuf::doEncode;	
	return s;
}

string Html::addHex(const string& s)
{
	string t;
	int index  = 0;
	int length = s.length();

	while(index < length)
	{
		char c = s[index];
		bool ok = false;

		switch(c)
		{
			case '+':
			case '&':
			case '/':
			case '=':
			case '?':
				ok = true;
				break;

			default:
				ok = isalnum(c);
				break;
		}

		if(ok)
			t += c;
		else {
			t += '%';

			unsigned int h = ((unsigned char)c) / 16;
			unsigned int l = ((unsigned char)c) % 16;

			if(h>=10) c = h - 10 + 'A'; else c = h + '0';
			t += c;

			if(l>=10) c = l - 10 + 'A'; else c = l + '0';
			t += c;
		}
		index++;
	}

	return t;
}

string Html::removeHex(const string& s)
{
	string t;
	int index  = 0;
	int length = s.length();

	while(index < length)
	{
		if(s[index] == '+')
		{
			t += ' ';	
		} else if(s[index] == '%')
		{
			char h = s[index+1];
			char l = s[index+2];
			index += 2;

			unsigned int a = (h>='A')?(h-'A'+10):(h-'0');
			unsigned int b = (l>='A')?(l-'A'+10):(l-'0');

			t += char(a * 16 + b);
		}
		else t += s[index];

		index++;
	}
	return t;
}

//=======================================================

Html::Include::Include(const string& name,HtmlObject* sub):
	sub_(sub),
	name_(name)
{
}

Html::Include::Include(const string& name,HtmlObject& sub):
	sub_(&sub),
	name_(name)
{
}

//=======================================================

void Html::Include::print(ostream& s) const
{
	eckit::PathName path = eckit::Resource<PathName>("htmlPath","~/html");

	path = path + '/' + name_;

	ifstream in(path.localPath());

	if(!in) 
	{
		s << path << ": " << Log::syserr << endl;
		return ;
	}


	char   c;
	string p;
	bool word = false;


	s << HttpBuf::dontEncode;

	while(in.get(c))
	{
		if(c == '%')
		{
			if(word)
			{
				if(sub_)
					sub_->substitute(s,p);
				else
					s << '%' << p << '%';
				p = "";
				word = false;
			}
			else word = true;
			in.get(c);
		}

		if(word)
			p += c;
		else
			s << c;
	}

	s << HttpBuf::doEncode;

}

//=======================================================

Html::Image::Image(const string& name):
	name_(name)
{
}

void Html::Image::print(ostream& s) const
{
	s << "<IMG SRC=\"" << resource() + '/' + name_ << "\">";
}

string Html::Image::resource()
{
	return "/image";
}


//=======================================================

Html::Link::Link(Url& url):
	url_(addHex(url.str()))
{
}

void Html::Link::print(ostream& s) const
{
	if(url_.length())
		s << "<A HREF=\"" <<  url_ << "\">" ;
	else
		s << "</A>" ;
}

//=======================================================

void Html::Substitute::substitute(ostream& s,const string& p)
{
	map<string,string,less<string> >::iterator i = map_.find(p);
	if(i ==  map_.end())
		s << '%' << p << '%';
	else
	 	s << HttpBuf::doEncode << (*i).second << HttpBuf::dontEncode;
}

Html::Substitute::Substitute()
{
}

Html::Substitute::~Substitute()
{
}


string& Html::Substitute::operator[](const string& p) 
{
	return map_[p];
}


void Html::Class::print(ostream& s) const
{
	string p;
	long len = str_.length();
	string base = "http://wwwec.ecmwf.int/dhs/classfinder?file=";

	for(int i = 0; i < len; i++)
	{
		char c = str_[i];
		if(isalnum(c) || c == '_')
			p += c;
		else if(p.length())
		{
			s << Link(base+p) << p << Link();
			s << c;
			p = "";
		}
		else s << c;
	}
	if(p.length()) s << Link(base+p) << p << Link();
}

void Html::BeginForm::print(ostream& s) const  
{ 
	s << "<FORM METHOD=\"POST\""; 

	if(str_.length())
		s << " ACTION=\"" << str_ << "\"";

	s << ">";
}

void Html::EndForm::print(ostream& s) const
{ 
	s << "</FORM>"; 
}

void Html::TextField::print(ostream& s) const
{
	s << title_ << HttpBuf::dontEncode;	
	s << "<INPUT NAME=\"" << name_ << "\" VALUE=\"" << value_ << "\">";
	s << HttpBuf::doEncode;	
}

void Html::HiddenField::print(ostream& s) const
{
	s << "<INPUT TYPE=\"hidden\" NAME=\"" << name_ << "\" VALUE=\"" << value_ << "\">";
}

void Html::CheckBox::print(ostream& s) const
{
	s << "<INPUT TYPE=\"checkbox\" ";
	if(on_) s << "checked ";
	s << "NAME=\"" << name_ << "\" VALUE=\"" << value_ << "\">";
}

void Html::Button::print(ostream& s) const
{
	s << "<INPUT TYPE=\"" << type_ << "\" VALUE=\"" << title_ << "\">";
}

void Html::BeginTextArea::print(ostream& s) const
{
	s << HttpBuf::dontEncode;
	s << "<TEXTAREA NAME=\"" << name_ 
	   << "\" ROWS=" << row_ << " COLS=" << col_ << ">";
}

void Html::EndTextArea::print(ostream& s) const
{
	s << "</TEXTAREA>" << endl << HttpBuf::doEncode;
}

//-----------------------------------------------------------------------------

class ImageProvider : public HtmlResource {
public:
    ImageProvider(): HtmlResource(Html::Image::resource()) { }
    void html(ostream& , Url&);
};

static ImageProvider imageProvider;

void ImageProvider::html(ostream& out, Url& url)
{
	eckit::PathName path = eckit::Resource<PathName>("imagePath","~/html/image");
	char c;

	for(int i = 1; i < url.size() ; i++)
		path = path + "/" + url[i] ;

	ifstream in(path.localPath());
	if(!in)	
	{
		(url.headerOut()).status(404);  // Not Found
		out << path << ": " << Log::syserr << endl;
	}
	else
	{
		(url.headerOut()).type("image/gif");

		out << HttpBuf::dontEncode;
		while(in.get(c))
			out << c;
		out << HttpBuf::doEncode;
	}
}

//-----------------------------------------------------------------------------

class HtmlProvider : public HtmlResource {
public:
	HtmlProvider(): HtmlResource("/html") { }
	void html(ostream& , Url&);
};

static HtmlProvider htmlProvider;

void HtmlProvider::html(ostream& s, Url& url)
{
	string path; 

	for(int i = 1; i < url.size() ; i++)
		path += "/" + url[i] ;

	Html::Substitute empty;
	Html::Include include(path,empty); 
	s << include;
}


//-----------------------------------------------------------------------------

void Html::BeginTable::print(ostream& s) const
{
	s << "<TABLE";
	if(border_)  s << " BORDER";
	if(border_>1) s << "=" << border_;
	if(padding_) s << " CELLPADDING=" << padding_;
	if(spacing_) s << " CELLSPACING=" << spacing_;
	if(width_)   s << " WIDTH=" << '"' << width_ << '%' << '"' ;
	s << ">";
}

void Html::TableTag::print(ostream& s) const
{
	s << '<' << tag_;

	if(align_) {
		
		if( (align_&Center) ) s << " ALIGN=center";
		if( (align_&Left  ) ) s << " ALIGN=left";
		if( (align_&Right ) ) s << " ALIGN=right";
		if( (align_&Top   ) ) s << " VALIGN=top";
		if( (align_&Bottom) ) s << " VALIGN=bottom";
	}

	if(colspan_) s << " COLSPAN=" << colspan_;
	if(rowspan_) s << " ROWSPAN=" << rowspan_;

	s << '>';
}

//-----------------------------------------------------------------------------

} // namespace eckit

