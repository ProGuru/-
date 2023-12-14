//�������� �� ����������
//========== ���� shape.cpp (���������� ���������) ==========
// ���������� � ������������� ���������� �����
#include <iostream>
#include "screen.h"
#include "shape.h"

// ������ �������: �������������� �������� - ��������������
class h_circle : public rectangle, public reflectable
{
	bool reflected;
public:
	h_circle(point a, point b, bool r = true) : rectangle(a, b), reflected(r) { };
	void draw();
	void flip_horisontally() { }; // �������� ������������� (������ �������)
	void flip_vertically()
	{
		reflected = !reflected;
	};	// �������� �����������
};

void h_circle::draw() //�������� ���������� ��� �����������
{   //(�������� ��� �������, ����������� ��������� reflected)
	int x0 = (sw.x + ne.x) / 2, y0 = reflected ? sw.y : ne.y;
	int radius = (ne.x - sw.x) / 2;
	int x = 0, y = radius, delta = 2 - 2 * radius, error = 0;

	while (y >= 0)
	{ // ���� ���������
		if (reflected) { put_point(x0 + x, y0 + y*0.7); put_point(x0 - x, y0 + y*0.7); }
		else { put_point(x0 + x, y0 - y*0.7); put_point(x0 - x, y0 - y*0.7); }
		error = 2 * (delta + y) - 1;

		if (delta < 0 && error <= 0) { ++x; delta += 2 * x + 1; continue; }
		error = 2 * (delta - x) - 1;
		if (delta > 0 && error > 0) { --y; delta += 1 - 2 * y; continue; }
		++x; delta += 2 * (x - y);  --y;
	}
}
// ������ �������: �������������� ������� ��������������

void down(shape &p, const shape &q)
{
	point n = q.south();
	point s = p.north();
	p.move(n.x - s.x, n.y - s.y - 1);
}

void up(shape &p, const shape &q) // ��������� p ��� q
{	//��� ������� �������, �� ���� ������! ������������ ����������!!
	point n = q.north();
	point s = p.south();
	p.move(n.x - s.x, n.y - s.y + 1);
}

void upLeft�orner(shape &p, const shape &q) // ��������� p ��� q
{	//��� ������� �������, �� ���� ������! ������������ ����������!!
	point n = q.west();
	point s = p.south();
	p.move(n.x - s.x, n.y - s.y + 1);
}

void upRight�orner(shape &p, const shape &q) // ��������� p ��� q
{	//��� ������� �������, �� ���� ������! ������������ ����������!!
	point n = q.east();
	point s = p.south();
	p.move(n.x - s.x, n.y - s.y + 1);
}

void upLeftDown�orner(shape &p, const shape &q) // ��������� p ��� q
{	//��� ������� �������, �� ���� ������! ������������ ����������!!
	point n = q.swest();
	point s = p.neast();
	p.move(n.x - s.x, n.y - s.y + 1);
}

void upRightDown�orner(shape &p, const shape &q) // ��������� p ��� q
{	//��� ������� �������, �� ���� ������! ������������ ����������!!
	point n = q.seast();
	point s = p.nwest();
	p.move(n.x - s.x, n.y - s.y + 1);
}

// C������ ���������������� ������ - ����������
class myshape : public rectangle
{ // ��� ������ ��������
	int w, h;			             //        ���������������
	line l_eye; // ����� ���� � ��� ������ �������� �����
	line r_eye; // ������ ����
	line mouth; // ���

public:
	myshape(point, point);
	void draw();
	void move(int, int);
	void resize(int) { }
};

myshape::myshape(point a, point b) : 
	rectangle(a, b),	//������������� �������� ������
	w(neast().x - swest().x + 1), // ������������� ������
	h(neast().y - swest().y + 1), // - ������ � ������� ����������!
	l_eye(point(swest().x + 2, swest().y + h * 3 / 4), 2),
	r_eye(point(swest().x + w - 4, swest().y + h * 3 / 4), 2),
	mouth(point(swest().x + 2, swest().y + h / 4), w - 4)
{ }

void myshape::draw()
{
	rectangle::draw(); //������ ���� (����� � ��� �������� ����!) 
	int a = (swest().x + neast().x) / 2;
	int b = (swest().y + neast().y) / 2;
	put_point(point(a, b)); // ��� � ���������� ������ �� �������!
}

void myshape::move(int a, int b)
{
	rectangle::move(a, b);
	l_eye.move(a, b);
	r_eye.move(a, b);
	mouth.move(a, b);
}


int main()
{
	screen_init();
	//== 1.���������� ������ ����� ==
	rectangle hat(point(0, 0), point(14, 5));
	line brim(point(0, 15), 17);
	myshape face(point(15, 10), point(27, 18));
	h_circle beard(point(40, 10), point(50, 20));
	parallelogram horn1(point(12, 32), point(20, 38));
	parallelogram horn2(point(70, 19), point(86, 31));
	parallelogram horn3(point(66, 4), point(74, 10));
	parallelogram horn4(point(60, 36), point(68, 42));
	shape_refresh();
	std::cout << "=== Generated... ===\n";
	std::cin.get(); //�������� �������� �����

					//== 2.���������� � ������ ==
	// ��������������� ����������� �����
	horn1.resize(0.5);
	horn2.resize(0.25);
	horn3.resize(0.5);
	horn4.resize(0.5);

	// ������� ����������� �����
	horn1.flip_horisontally();
	horn1.rotate_right(); // ��� 1
	horn2.rotate_left(); // ��� 2
	horn4.flip_vertically();  // ��������� 2

	hat.rotate_right();
	brim.resize(2);
	face.resize(2);
	beard.flip_vertically();
	shape_refresh();
	std::cout << "=== Prepared... ===\n";
	std::cin.get(); //�������� ��������� ���������/���������
					//== 3.������ ����������� ==
					//	face.move(0, -10); // ���� - � �������� ���������
	up(brim, face);
	up(hat, brim);
	down(beard, face);

	// ���������� �����
	upLeft�orner(horn2, brim);
	upRight�orner(horn1, brim);

	// ���������� �����������
	upLeftDown�orner(horn4, face);
	upRightDown�orner(horn3, face);

	shape_refresh();
	std::cout << "=== Ready! ===\n";
	std::cin.get(); //�������� ���������
	screen_destroy();
	return 0;
}