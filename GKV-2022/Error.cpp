#include "Error.h"
#include <iostream>
namespace Error {
	ERROR errors[ERROR_MAX_ENTRY] = {
		ERROR_ENTRY(0,"Недопустимый код ошибки"),
		ERROR_ENTRY(1,"Системный сбой"),
		ERROR_ENTRY_NODEF(2),ERROR_ENTRY_NODEF(3),ERROR_ENTRY_NODEF(4),ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6),ERROR_ENTRY_NODEF(7),ERROR_ENTRY_NODEF(8),ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10),ERROR_ENTRY_NODEF10(20),ERROR_ENTRY_NODEF10(30),ERROR_ENTRY_NODEF10(40),ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60),ERROR_ENTRY_NODEF10(70),ERROR_ENTRY_NODEF10(80),ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100,"Параметр -in должен быть задан"),
		ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104, "Превышена длина входного параметра"),
		ERROR_ENTRY(105, "Ошибка слишком большой размер таблицы лексем/идентификаторов"), ERROR_ENTRY(106, "Размер таблицы  лексем/идентификаторов привысел максимальный возможный"), ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111, "Не доступный символв исходном файле (-in)"),
		ERROR_ENTRY(112, "Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY(113, "Ошибка лексического анализа"),
		ERROR_ENTRY(114, "Ошибка при создании файла выходного файла (-out)"), ERROR_ENTRY_NODEF(115),
		ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY(200, "Лексический анализатор: Неинициализированная переменная"),
		ERROR_ENTRY(201, "Лексический анализатор: Неверное имя идентификатора"),
		ERROR_ENTRY(202, "Лексический анализатор: Превышен допустимый размер литерала"),
		ERROR_ENTRY_NODEF(203), ERROR_ENTRY_NODEF(204),
		ERROR_ENTRY_NODEF(205), ERROR_ENTRY_NODEF(205), ERROR_ENTRY_NODEF(205), ERROR_ENTRY_NODEF(208),
		ERROR_ENTRY_NODEF(209), ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230),
		ERROR_ENTRY_NODEF10(240), ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260),
		ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),
		ERROR_ENTRY_NODEF(304),
		ERROR_ENTRY(301, "Семантическая ошибка: Отсутствует точка входа main"),
		ERROR_ENTRY(302, "Семантическая ошибка: Обнаружено несколько точек входа main"),
		ERROR_ENTRY_NODEF(303),
		ERROR_ENTRY_NODEF(304),
		ERROR_ENTRY(305, "Семантическая ошибка: Попытка переопределения идентификатора"),
		ERROR_ENTRY_NODEF(306),
		ERROR_ENTRY_NODEF(307),
		ERROR_ENTRY(308, "Семантическая ошибка: Кол-во ожидаемых и переданных функция и параметров не совпадают"),
		ERROR_ENTRY(309, "Семантическая ошибка: Несовпадение типов передаваемых параметров"),
		ERROR_ENTRY(310, "Семантическая ошибка: Использование пустого строкового литерала недопустимо"),
		ERROR_ENTRY(311, "Семантическая ошибка: Не закрыт строковый литерал"),
		ERROR_ENTRY_NODEF(312),
		ERROR_ENTRY(313, "Семантическая ошибка: Недопустимый размер целочисленного литерал"),
		ERROR_ENTRY(314, "Семантическая ошибка: Типы данных в выражении не совпадают"),
		ERROR_ENTRY(315, "Семантическая ошибка: Тип функции и возвращаемого значения не совпадают"),
		ERROR_ENTRY(316, "Семантическая ошибка: Недопустимое строковое выражение справа от знака \'=\'"),
		ERROR_ENTRY(317, "Семантическая ошибка: Неверное условное выражение"),
		ERROR_ENTRY_NODEF(318),
		ERROR_ENTRY_NODEF(319),
		ERROR_ENTRY_NODEF10(320),ERROR_ENTRY_NODEF10(330),ERROR_ENTRY_NODEF10(340),ERROR_ENTRY_NODEF10(350),
		ERROR_ENTRY_NODEF10(360),ERROR_ENTRY_NODEF10(370),ERROR_ENTRY_NODEF10(380),ERROR_ENTRY_NODEF10(390), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "Синтаксическая ошибка: Неверная структура программы"),
		ERROR_ENTRY(601, "Синтаксическая ошибка: Ошибка в теле функции"),
		ERROR_ENTRY(602, "Синтаксическая ошибка: Ошибка в выражении"),
		ERROR_ENTRY(603, "Синтаксическая ошибка: Ошибка в параметрах функции"),
		ERROR_ENTRY(604, "Синтаксическая ошибка: Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY_NODEF(605),
		ERROR_ENTRY(606, "Синтаксическая ошибка: Неверная структура условия"),
		ERROR_ENTRY(607, "Синтаксическая ошибка: Неверная конструкия цикла"), ERROR_ENTRY_NODEF(608),
		ERROR_ENTRY_NODEF(609), ERROR_ENTRY_NODEF10(610), ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630),
		ERROR_ENTRY_NODEF10(640), ERROR_ENTRY_NODEF10(650),ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670),
		ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};

	ERROR geterror(int id) {
		if (id > 0 && id < ERROR_MAX_ENTRY) {
			std::cout << "\nОшибка " << id << ": " << errors[id].message << std::endl;

			return errors[id];

		}
		else {
			std::cout << "\nОшибка " << 0 << ": " << errors[0].message << std::endl;
			return errors[0];
		}
	}
	ERROR geterrorin(int id, int line = -1, int col = -1) {
		ERROR e;
		if (id > 0 && id < ERROR_MAX_ENTRY) {
			e = errors[id];
			e.inext.col = col;
			e.inext.line = line;
			std::cout << "\nОшибка " << e.id << ": " << e.message << " cтрока " << e.inext.line << " позиция " << e.inext.col << std::endl;

			return e;
		}
		else{
			std::cout << "\nОшибка " << 0 << ": " << errors[0].message << std::endl;
			return errors[0];
		}

	}
}
