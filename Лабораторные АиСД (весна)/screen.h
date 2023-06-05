#pragma once
//Проверка на сохранение
//=== Файл screen.h -- поддержка работы с экраном
#pragma once // защита от повторного включения файла
const int XMAX = 120; //Размер экрана
const int YMAX = 50;

class point
{	//Точка на экране
public:
	int x, y;
	point(int a = 0, int b = 0) : x(a), y(b) {  }
};
// Набор утилит для работы с экраном
void put_point(int a, int b); // Вывод точки (1-й вариант)

void put_point(point p) // 2-й вариант (ПЕРЕГРУЗКА ФУНКЦИИ)
{
	put_point(p.x, p.y); // 2-й вариант преобразуется в 1-й путём передачи в одноимённую функцию параметров int
} //

void put_line(int, int, int, int); // Вывод линии (1-й вариант)

void put_line(point a, point b) // 2-й вариант (ПЕРЕГРУЗКА ФУНКЦИИ)
{
	put_line(a.x, a.y, b.x, b.y); // 2-й вариант преобразуется в 1-й путём передачи в одноимённую функцию параметров int
}

void screen_init();		// Создание экрана
void screen_destroy();	// Удаление 
void screen_refresh();	// Обновление
void screen_clear();	  // Очистка