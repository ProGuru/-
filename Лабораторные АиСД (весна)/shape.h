#pragma once
/*
	�������: ���������� ������ shape.cpp, ������� � ���������
������ ��������������. 
	��� ���� ������ ����� ���������� ���������� ����� � �������� ������� 
� �������� ����������� �������-�����.
	����������� ����� � ������ ������������ ������������ �������-�����, 
������������� ������� �� ��������������, ������������� ������� ������������.
	
	��� ���������� ����� ������ �������������� �� ���������� �����.  
���������� �������� ������� ������� up (��������� p ��� q), �������������� 
���������� ��������� ������ p � ������ ������� �� ������-��� � ��� ����������� q. 
��� ������ ���� ������� �������, � �� �������-����� ������, 
����� �� ����� ���� ��������� ��� ����� ���-�����.

	��� �������������� ������ ������, ����������� ������� �/��� ��-�������, 
������������� �������� ����������, ����������� ��� ������-��� ������ � ������ � ����������, 
� ���������� ��� ���������� ����-����� ����� ��������, �� ��������� �� ����������.
*/

//�������� �� ����������
//======== ���� shape.h � ���������� ����� =========
#include <list>
#include <iostream>
#include "screen.h"
#include <cmath>

#define PI 3.14159265 
//==1. ��������� ������ � ����� ������� �������� ==
char screen[YMAX][XMAX];
enum color
{
	black = '*',
	white = '.'
};

// ������ � �������
void screen_init() // ���������� ������ �������
{
	for (auto y = 0; y < YMAX; ++y)
		for (auto &x : screen[y])  x = white;
}

void screen_destroy() // ���������� ������ ���������
{
	for (auto y = 0; y < YMAX; ++y)
		for (auto &x : screen[y])  x = black;
}

void screen_clear()
{
	screen_init();
} //������� ������

void screen_refresh() // ����� ������� �� �����. ���������� ������
{
	for (int y = YMAX - 1; 0 <= y; --y) { // � ������� ������ �� ������
		for (auto x : screen[y])    // �� ������ ������� �� �������
			std::cout << x;
		std::cout << '\n';
	}
}

bool is_on_screen(int a, int b) // �������� ��������� ����� �� �����
{
	return 0 <= a && a < XMAX && 0 <= b && b < YMAX;
}

void put_point(int a, int b)
{
	if (is_on_screen(a, b)) screen[b][a] = black;
}

void put_line(int x0, int y0, int x1, int y1)
/* �������� ���������� ��� ������:
��������� ������� ������ �� (x0,y0) �� (x1,y1).
��������� ������: b(x-x0) + a(y-y0) = 0.
�������������� �������� abs(eps), ��� eps = 2*(b(x-x0)) + a(y-y0).  */
{
	int dx = 1;
	int a = x1 - x0;   if (a < 0) dx = -1, a = -a;
	int dy = 1;
	int b = y1 - y0;   if (b < 0) dy = -1, b = -b;
	int two_a = 2 * a;
	int two_b = 2 * b;
	int xcrit = -b + two_a;
	int eps = 0;
	for (;;) { //������������ ������ ����� �� ������. ����������� ����
		put_point(x0, y0);
		if (x0 == x1 && y0 == y1) break;
		if (eps <= xcrit) x0 += dx, eps += two_b;
		if (eps >= a || a < b) y0 += dy, eps -= two_a;
	}
}

//== 2. ���������� ����� ==
struct shape { // ����������� ����������� ������� ����� "������"
	// ! ������ ��������� ������� �� ������������ ������
	static std::list<shape*> shapes;// ������ ����� (���� �� ��� ������!)
	shape() { shapes.push_back(this); } //������ �������������� � ������
	// ����������� ������ (������ ����������� �������). 
	// "= 0" �������� ��� � ������������ ������ ��� ���� �������
	virtual point north() const = 0;	//����� ��� ��������
	virtual point south() const = 0;
	virtual point east() const = 0;
	virtual point west() const = 0;
	virtual point neast() const = 0;
	virtual point seast() const = 0;
	virtual point nwest() const = 0;
	virtual point swest() const = 0;
	virtual void draw() = 0;		//���������
	virtual void move(int, int) = 0;	//�����������
	virtual void resize(float) = 0;    	//��������� ������� ��������������� ����� int
};

std::list<shape*> shape::shapes;   // ���������� ������ �����

void shape_refresh() // ����������� ���� ����� �� ������
{
	screen_clear(); // �������� �����. ������� ����������� �������
	for (auto p : shape::shapes) p->draw(); // ���������� ������ �� ������. ��������� � �������. ������������ ����������!!!
	screen_refresh(); // ����� ���������� �� �����
}

class rotatable : virtual public shape // ! ������ ��������� ������� �� ������������ ������
{ //������, ��������� � �������� 
public:
	virtual void rotate_left() = 0;	//��������� �����
	virtual void rotate_right() = 0;	//��������� ������
};

class reflectable : virtual public shape // ! ������ ��������� ������� �� ������������ ������
{ // ������, ���������
public:					     // � ����������� ���������
	virtual void flip_horisontally() = 0;	// �������� �������������
	virtual void flip_vertically() = 0;	          // �������� �����������
};

class line : public shape
{
	/* ������� ������ ["w", "e"].
	north( ) ���������� ����� "���� ������ ������� � ��� ������
	�� �����, ��� ����� ��� �������� �����", � �. �. */
protected:
	point w, e;
public:
	line(point a, point b) : w(a), e(b) { };
	line(point a, int L) : w(point(a.x + L - 1, a.y)), e(a) {  };
	point north() const { return point((w.x + e.x) / 2, e.y<w.y ? w.y : e.y); }
	point south() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
	point east() const { return point(e.x<w.x ? w.x : e.x, (w.y + e.y) / 2); }
	point west() const { return point(e.x<w.x ? e.x : w.x, (w.y + e.y) / 2); }
	point neast() const { return point(w.x<e.x ? e.x : w.x, e.y<w.y ? w.y : e.y); }
	point seast() const { return point(w.x<e.x ? e.x : w.x, e.y<w.y ? e.y : w.y); }
	point nwest() const { return point(w.x<e.x ? w.x : e.x, e.y<w.y ? w.y : e.y); }
	point swest() const { return point(w.x<e.x ? w.x : e.x, e.y<w.y ? e.y : w.y); }
	void move(int a, int b) { 
			w.x += a; 
			w.y += b; 
			e.x += a; 
			e.y += b; 
	}
	void draw() { put_line(w, e); } // ����� ��������� � ������
	void resize(float d) // ���������� ����� ����� � (d) ���
	{
		e.x += (e.x - w.x) * (d - 1); 
		e.y += (e.y - w.y) * (d - 1);
	}
};

// �������������
class rectangle : public rotatable
{
	/* nw ------ n ------ ne
	|		       |
	|		       |
	w	   c            e
	|		       |
	|		       |
	sw ------- s ------ se */
protected:
	point sw, ne;
public:
	rectangle(point a, point b) : sw(a), ne(b) { }
	point north() const { return point((sw.x + ne.x) / 2, ne.y); }
	point south() const { return point((sw.x + ne.x) / 2, sw.y); }
	point east() const { return point(ne.x, (sw.y + ne.y) / 2); }
	point west() const { return point(sw.x, (sw.y + ne.y) / 2); }
	point neast() const { return ne; }
	point seast() const { return point(ne.x, sw.y); }
	point nwest() const { return point(sw.x, ne.y); }
	point swest() const { return sw; }
	void rotate_right() // ������� ������ ������������ se. ����������� �� rotatable
	{
		int w = ne.x - sw.x, h = ne.y - sw.y; //(����������� ������� �� ����)
		sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;
	}
	void rotate_left() // ������� ����� ������������ sw. ����������� �� rotatable
	{
		int w = ne.x - sw.x, h = ne.y - sw.y;
		ne.x = sw.x + h * 2; ne.y = sw.y + w / 2;
	}
	void move(int a, int b)
	{
		sw.x += a; sw.y += b; ne.x += a; ne.y += b;
	}
	void resize(float d)
	{
		ne.x += (ne.x - sw.x) * (d - 1); ne.y += (ne.y - sw.y) * (d - 1);
	}
	void draw()
	{
		put_line(nwest(), ne);   put_line(ne, seast());
		put_line(seast(), sw);   put_line(sw, nwest());
	}
};

// ��������������
class parallelogram : public rotatable, public reflectable
{
		/*	nw ---- n ----------- ne
		   /                     /
		  /                     /
		w		    c          e
	   /                      /
	  /                      /
	sw ------------ s ---- se */
protected:
	point sw, ne;
	point nw, se; // �������������� ����������. �������� sw � ne, ���� reflected = true
	float angle = -0.5; // ������ ���� � ��������������
	bool rotate;
public:
	parallelogram(point a, point b, bool r = true) : sw(a), ne(b), rotate(r), 
		nw((int)(sw.x + (ne.y - sw.y) / std::tan(-0.5)), ne.y), se((int)(ne.x - (ne.y - sw.y) / std::tan(-0.5)), sw.y) { }
	point north() const { return point((sw.x + ne.x) / 2, ne.y); }
	point south() const { return point((sw.x + ne.x) / 2, sw.y); }
	point east() const { return point((int)(ne.x - 0,5 * (ne.y - sw.y) / std::tan(angle)), (sw.y + ne.y) / 2); }
	point west() const { return point((int)(sw.x + 0,5 * (ne.y - sw.y) / std::tan(angle)), (sw.y + ne.y) / 2); }
	point neast() const { return ne; }
	point seast() const { return point((int)(ne.x - (ne.y - sw.y) / std::tan(angle)), sw.y); }
	point nwest() const { return point((int)(sw.x + (ne.y - sw.y) / std::tan(angle)), ne.y); }
	point swest() const { return sw; }
	void rotate_right() // ������� ������ ������������ se. ����������� �� rotatable
	{
		rotate = !rotate;
		
		// ������� �������� �����
		point centr;
		centr.x = sw.x + abs((int)((ne.x - sw.x) / 2));
		centr.y = sw.y + abs((int)((ne.y - sw.y) / 2));

		// ������������ ���������� �� ������� ������� ������������� ������
		rotateCoordinate(sw, centr, true);
		rotateCoordinate(ne, centr, true);
		rotateCoordinate(nw, centr, true);
		rotateCoordinate(se, centr, true);
		/*
		point templePoint = nw;
		nw = ne;
		ne = se;
		se = sw;
		sw = templePoint;
		*/
	}
	void rotate_left() // ������� ����� ������������ sw. ����������� �� rotatable
	{
		rotate = !rotate;

		// ������� �������� �����
		point centr;
		centr.x = sw.x + abs((int)((ne.x - sw.x) / 2));
		centr.y = sw.y + abs((int)((ne.y - sw.y) / 2));

		// ������������ ���������� �� ������� ������� ������������� ������
		rotateCoordinate(sw, centr, false);
		rotateCoordinate(ne, centr, false);
		rotateCoordinate(nw, centr, false);
		rotateCoordinate(se, centr, false);

		// �������� ����� ������������
		point templePoint = nw;
		nw = sw;
		sw = se;
		se = ne;
		ne = templePoint;
	}

	void flip_horisontally()
	{
			/*	nw ---- n ----------- ne
			   /                     /
			  /                     /
			w		    c          e
		   /                      /
		  /                      /
		sw ------------ s ---- se */

	/*	nw ---- n ------- ne
		 /                     /
		  /                      /
		   w		    c          e
		    /                      /
		     /                      /
		      sw ------------ s ---- se */
		point templeSW = sw, templeNE = ne, templeNW = nw, templeSE = se;
		if (rotate) {
			sw.x = templeNW.x;
			ne.x = templeSE.x;
			nw.x = templeSW.x;
			se.x = templeNE.x;
		}
		else
		{
			sw.y = templeSE.y;
			ne.y = templeNW.y;
			nw.y = templeNE.y;
			se.y = templeSW.y;
		}

	}

	void flip_vertically()
	{
		flip_horisontally(); // ��� ��������������� �� ����� ��������, �������������� ��� ������������ ���������
	}

	void move(int a, int b)
	{
		sw.x += a; sw.y += b; ne.x += a; ne.y += b;
		nw.x += a; nw.y += b; se.x += a; se.y += b;
	}
	void resize(float d)
	{
		ne.x += (ne.x - sw.x) * (d - 1); ne.y += (ne.y - sw.y) * (d - 1);

		nw.x = (int)(sw.x + (ne.y - sw.y) / tan(angle)); 
		nw.y = ne.y;
		se.x = (int)(ne.x - (ne.y - sw.y) / tan(angle)); 
		se.y = sw.y;
	}
	void draw()
	{
		put_line(sw, nw);   put_line(nw, ne);
		put_line(ne, se);   put_line(se, sw);
	}

	void rotateCoordinate(point& p, const point& c, bool clockWise) // �������������� ������� ��� ���������� ���������
	{
		int deltaX = p.x - c.x;
		int deltaY = p.y - c.y;
		if (clockWise) {
			p.x = c.x - deltaY;
			p.y = c.y + deltaX;
		}
		else
		{
			p.x = c.x + deltaY;
			p.y = c.y - deltaX;
		}
		
	}
};