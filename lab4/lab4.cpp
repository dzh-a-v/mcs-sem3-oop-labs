// Контейнеры STL: 
//deque, stack, queue, priority_queue
//set, multiset, map, multimap
//Итераторы. Стандартные алгоритмы. Предикаты.

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include "helpers.hpp"
#include "MyString.h"
#include "Point.h"
#include "Rect.h"

#define NN std::cout << "\n\n";

//using namespace std;
//#define	  //stop __asm nop



int main()
{

	//Очередь с двумя концами - контейнер deque

	//Создайте пустой deque с элементами типа Point. С помощью
	//assign заполните deque копиями элементов вектора. С помощью
	//разработанного Вами в предыдущем задании универсального шаблона
	//выведите значения элементов на печать

	std::vector<Point> vec = { Point(1, 2), Point(3, 4), Point(5, 6) };
	std::deque<Point> d;
	d.assign(vec.begin(), vec.end());
	pr(d, "Deque of Points");


	//Создайте deque с элементами типа MyString. Заполните его значениями
	//с помощью push_back(), push_front(), insert()
	//С помощью erase удалите из deque все элементы, в которых строчки
	//начинаются с 'A' или 'a'

	std::deque<MyString> ds;
	ds.push_back(MyString("Apple"));
	ds.push_front(MyString("banana"));
	ds.push_back(MyString("Apricot"));
	ds.insert(ds.begin() + 1, MyString("cherry"));

	pr(ds, "Deque of MyString before erase");

	for (auto it = ds.begin(); it != ds.end(); )
	{
		char* s = it->GetString();
		if (s[0] == 'A' || s[0] == 'a')
			it = ds.erase(it);
		else
			++it;
	}

	pr(ds, "Deque of MyString after erase");
	NN


	////////////////////////////////////////////////////////////////////////////////////


	//Напишите шаблон функции для вывода значений stack, queue, priority_queue
	//Подумайте, как "получать" данное "с верхушки"?
	//Что происходит с контейнерами после вывода значений?

	/*
	Nothing happens to the containers after printing the values using the provided template function.
	They remain unchanged because the function operates on a copy of the container.
	*/

	//testing
	{
		std::stack<int> s_int;
		s_int.push(1);
		s_int.push(2);
		s_int.push(3);
		std::cout << "Stack of int: ";
		printContainer(s_int);
		std::queue<double> q_double;
		q_double.push(1.1);
		q_double.push(2.2);
		q_double.push(3.3);
		std::cout << "Queue of double: ";
		printContainer(q_double);
		std::priority_queue<Point> pq_point;
		pq_point.push(Point(1, 2));
		pq_point.push(Point(3, 4));
		pq_point.push(Point(5, 6));
		std::cout << "Priority Queue of Point: ";
		printContainer(pq_point);
		std::stack<MyString> s_mystring;
		s_mystring.push(MyString("Hello"));
		s_mystring.push(MyString("World"));
		s_mystring.push(MyString("C++"));
		std::cout << "Stack of MyString: ";
		printContainer(s_mystring);
	}




	////////////////////////////////////////////////////////////////////////////////////
	//stack

	//Создайте стек таким образом, чтобы
	//а) элементы стека стали копиями элементов вектора
	//б) при выводе значений как вектора, так и стека порядок значений был одинаковым 

	{
		std::vector<int> v = { 1, 2, 3, 4, 5 };
		std::stack<int> s;
		for (auto it = v.rbegin(); it != v.rend(); ++it)
			s.push(*it);
		pr(v, "Vector: ");
		std::cout << "Stack: ";
		printContainer(s);
		NN
	}



	//Сравнение и копирование стеков
	//а) создайте стек и любым способом задайте значения элементов
	//б) создайте новый стек таким образом, чтобы он стал копией первого
	//в) сравните стеки на равенство
	//г) модифицируйте любой из стеком любым образом (push, pop, top)
	//д) проверьте, какой из стеков больше (подумайте, какой смысл вкладывается в такое сравнение)
	{
		std::stack<int> s1;
		s1.push(1);
		s1.push(2);
		s1.push(3);
		std::stack<int> s2 = s1;
		std::cout << "Stack s1: ";
		printContainer(s1);
		std::cout << "Stack s2 (copy of s1): ";
		printContainer(s2);
		if (s1 == s2)
			std::cout << "Stacks are equal.\n";
		else
			std::cout << "Stacks are not equal.\n";
		s1.push(4);
		if (s1 > s2)
			std::cout << "Stack s1 is greater than stack s2.\n";
		else if (s1 < s2)
			std::cout << "Stack s1 is less than stack s2.\n";
		else
			std::cout << "Stacks are equal.\n";
		s2.push(5);
		if (s1 > s2)
			std::cout << "Stack s1 is greater than stack s2.\n";
		else if (s1 < s2)
			std::cout << "Stack s1 is less than stack s2.\n";
		else
			std::cout << "Stacks are equal.\n";
		s1.push(7);
		s2.push(6);
		if (s1 > s2)
			std::cout << "Stack s1 is greater than stack s2.\n";
		else if (s1 < s2)
			std::cout << "Stack s1 is less than stack s2.\n";
		else
			std::cout << "Stacks are equal.\n";
		s1.push(8);
		s2.push(8);
		if (s1 > s2)
			std::cout << "Stack s1 is greater than stack s2.\n";
		else if (s1 < s2)
			std::cout << "Stack s1 is less than stack s2.\n";
		else
			std::cout << "Stacks are equal.\n";
		s2.pop(); s2.pop();
		s2.push(9);
		if (s1 > s2)
			std::cout << "Stack s1 is greater than stack s2.\n";
		else if (s1 < s2)
			std::cout << "Stack s1 is less than stack s2.\n";
		else
			std::cout << "Stacks are equal.\n";
		NN
	}


	////////////////////////////////////////////////////////////////////////////////////
	//queue

	//Создайте очередь, которая содержит указатели на объекты типа Point,
	//при этом явно задайте базовый контейнер.
	//Измените значения первого и последнего элементов посредством front() и back()
	//Подумайте, что требуется сделать при уничтожении такой очереди?
	{
		std::queue<Point*, std::deque<Point*>> q;
		q.push(new Point(1, 2));
		q.push(new Point(3, 4));
		q.push(new Point(5, 6));
		std::cout << "First element: " << *(q.front()) << std::endl;
		std::cout << "Last element: " << *(q.back()) << std::endl;
		*(q.front()) = Point(7, 8);
		*(q.back()) = Point(9, 10);
		std::cout << "After modification:" << std::endl;
		std::cout << "First element: " << *(q.front()) << std::endl;
		std::cout << "Last element: " << *(q.back()) << std::endl;
		while (!q.empty()) { // Answer: this is necessary.
			delete q.front();
			q.pop();
		}
	}
	////////////////////////////////////////////////////////////////////////////////////
	//priority_queue
	//а) создайте очередь с приоритетами, которая будет хранить адреса строковых литералов - const char*
	//б) проинициализируйте очередь при создании с помощью вспомогательного массива с элементами const char*
	//в) проверьте "упорядоченность" значений (с помощью pop() ) - если они оказываются не упорядоченными, подумайте:
	//		что сравнивается при вставке?


	{
		std::vector<const char*> arr = { "banana", "apple", "cherry", "date" };
		std::priority_queue<const char*> pq(arr.begin(), arr.end());
		std::cout << "Priority Queue of const char*:" << std::endl;
		printContainer(pq);
		// They are not sorted, because the comparison is done based on pointer values, not string content.
		// To correct this, we can use a custom comparator.
		std::priority_queue<const char*, std::deque<const char*>, CompareString> pq2(arr.begin(), arr.end());
		std::cout << "Priority Queue of const char* with custom comparator:" << std::endl;
		printContainer(pq2);
	}


	////////////////////////////////////////////////////////////////////////////////////
	//set
	//a) создайте множество с элементами типа Point - подумайте, что необходимо определить
	//		в классе Point (и каким образом)
	//б) распечатайте значения элементов с помощью шаблона, реализованного в предыдущей лаб. работе
	//в) попробуйте изменить любое значение...
	//г) Создайте два множества, которые будут содержать одинаковые значения
	//		типа int, но занесенные в разном порядке
	//д) Вставьте в любое множество диапазон элементов из любого другого
	//	контейнера, например, элементов массива	(что происходит, если в массиве имеются дубли?)

	{
		std::set<Point> set1;
		set1.insert(Point(3, 4));
		set1.insert(Point(1, 2));
		set1.insert(Point(5, 6));
		pr(set1, "Set of Points");
		std::set<int> setA = { 5, 3, 1, 4, 2 };
		std::set<int> setB = { 1, 2, 3, 4, 5 };
		std::cout << (setA == setB) ? "Sets A and B are equal.\n" : "Sets A and B are not equal.\n";
		int arr[] = { 3, 4, 5, 6, 7, 3, 4 };
		setA.insert(arr, arr + sizeof(arr) / sizeof(arr[0]));
		pr(setA, "Set A after inserting array elements");
		NN
	}



	////////////////////////////////////////////////////////////////////////////////////
	//multiset




	////////////////////////////////////////////////////////////////////////////////////
	//map	
	//а) создайте map, который хранит пары "фамилия, зарплата" - pair<const char*, int>,
	//	при этом строки задаются строковыми литералами
	std::map <const char*, int> salaryMap;
	//б) заполните контейнер значениями посредством operator[] и insert()
	salaryMap["Ivanova"] = 50000;
	salaryMap["Petrov"] = 60000;
	salaryMap.insert(std::make_pair("Sidorov", 55000));
	//в) распечатайте содержимое
	pr(salaryMap, "Salary Map before key change");
	//е) замените один из КЛЮЧЕЙ на новый (была "Иванова", вышла замуж => стала "Петрова")
	int salary = salaryMap["Ivanova"];
	salaryMap.erase("Ivanova");
	salaryMap["Petrova"] = salary;
	pr(salaryMap, "Salary Map after key change");

		////////////////////////////////////////////////////////////////////////////////////
		//multimap
		//а) создайте "англо-русский" словарь, где одному и тому же ключу будут соответствовать
		//		несколько русских значений - pair<string,string>, например: strange: чужой, странный...
	std::multimap<std::string, std::string> dict = {
	{"strange", "chuzhoy"},
	{"apple", "yabloko"},
	{"banana", "banan"}
	};
		//б) Заполните словарь парами с помощью метода insert или проинициализируйте с помощью 
		//		вспомогательного массива пара (пары можно конструировать или создавать с помощью шаблона make_pair)
	dict.insert(std::make_pair("strange", "strannyy"));
	const char* en = "funny";
	const char* ru[] = { "smeshnoy", "strannyy" };
	for (const auto& r : ru)
		dict.insert(std::make_pair(en, r));
	dict.emplace(en, "zabavnyy");
		//в) Выведите все содержимое словаря на экран
	pr(dict, "Dictionary contents");
		//г) Выведите на экран только варианты "переводов" для заданного ключа. Подсказка: для нахождения диапазона
		//		итераторов можно использовать методы lower_bound() и upper_bound()
	// with equal_range
	std::string key = "strange";
	auto range = dict.equal_range(key);
	std::cout << "Translations for key \"" << key << "\" (using equal_range):\n";
	for (auto it = range.first; it != range.second; ++it)
		std::cout << it->second << std::endl;
	std::cout << "\n";
	// with lower_bound and upper_bound
	auto lb = dict.lower_bound(key);
	auto ub = dict.upper_bound(key);
	std::cout << "Translations for key \"" << key << "\ (using lower_bound and upper_bound):\n";
	for (auto it = lb; it != ub; ++it)
		std::cout << it->second << std::endl;
	NN


	///////////////////////////////////////////////////////////////////

		//Итераторы

		//Реверсивные итераторы. Сформируйте set<Point>. Подумайте, что
		//нужно перегрузить в классе Point. Создайте вектор, элементы которого 
		//являются копиями элементов set, но упорядочены по убыванию

	std::set<Point> pointSet = { Point(1, 2), Point(3, 4), Point(5, 6) };
	std::vector<Point> pointVec(pointSet.rbegin(), pointSet.rend());
	pr(pointSet, "Set of Points");
	pr(pointVec, "Vector of Points in descending order");
	NN


		//Потоковые итераторы. С помощью ostream_iterator выведите содержимое
		//vector и set из предыдущего задания на экран.

	std::cout << "\nSet via ostream_iterator: ";
	std::copy(pointSet.begin(), pointSet.end(),
		std::ostream_iterator<Point>(std::cout, " "));

	std::cout << "\nVector via ostream_iterator: ";
	std::copy(pointVec.begin(), pointVec.end(),
		std::ostream_iterator<Point>(std::cout, " "));

	NN


		//Итераторы вставки. С помощью возвращаемых функциями:
		//back_inserter()
		//front_inserter()
		//inserter()
		//итераторов вставки добавьте элементы в любой из созданных контейнеров. Подумайте:
		//какие из итераторов вставки можно использовать с каждым контейнером.
	std::vector<Point> vec2 = { Point(4,3), Point(1,1) };
	std::set<Point> set2 = { Point(2,2), Point(3,3) };

	// Append vec2 to pointVec
	std::copy(vec2.begin(), vec2.end(), std::back_inserter(pointVec));
	pr(pointVec, "pointVec after back_inserter with vec2");

	// If you want to prepend to vec2, use inserter to the last position instead of front_inserter
	std::copy(pointVec.begin(), pointVec.end(), std::inserter(vec2, vec2.begin()));
	pr(vec2, "vec2 after inserter with pointVec at beginning");

	// Insert set2 into pointSet
	pr(pointSet, "pointSet before inserter");
	std::copy(set2.begin(), set2.end(), std::inserter(pointSet, pointSet.end()));
	pr(pointSet, "pointSet after inserter");

	// And now front_inserter with deque
	std::deque<Point> deq;
	std::copy(vec2.begin(), vec2.end(), std::front_inserter(deq));
	pr(deq, "deque after initializing with front_inserter with vec2");

	// Adding single element using front_inserter
	Point p(0, 0);
	std::copy(&p, &p + 1, std::front_inserter(deq));
	pr(deq, "deque after adding single element with front_inserter");

	// all of them can be used for such containers which support insertion of elements
	// with push_back, push_front or insert methods respectively.
	NN



	///////////////////////////////////////////////////////////////////

		//Обобщенные алгоритмы (заголовочный файл <algorithm>). Предикаты.

		// алгоритм for_each() - вызов заданной функции для каждого элемента любой последовательности
		//(массив, vector, list...)
		//С помощью алгоритма for_each в любой последовательности с элементами любого типа
		//распечатайте значения элементов
		//Подсказка : неплохо вызываемую функцию определить как шаблон

	std::vector<Point> pVec = { Point(1, 1), Point(2, 2), Point(3, 0) };
	std::cout << "Vector of int elements: ";
	std::for_each(pVec.begin(), pVec.end(), printElement<decltype(pVec[0])>);
	NN


		//stop

		//С помощью алгоритма for_each в любой последовательности с элементами типа Point
		//измените "координаты" на указанное значение (такой предикат тоже стоит реализовать 
		//как шаблон) и выведите результат с помощью предыдущего предиката

	float n = 3.2;
	std::cout << "Adding " << n << " to each Point in vector:\n";
	std::for_each(pVec.begin(), pVec.end(), [n](Point& p) {addToElement(p, n); });
	std::for_each(pVec.begin(), pVec.end(), printElement<decltype(pVec[0])>);
	NN




		//С помощью алгоритма find() найдите в любой последовательности элементов Point
		//все итераторы на элемент Point с указанным значением.
	pVec.push_back(Point(2, 2));
	pVec.push_back(Point(2, 2));
	pVec[0] = Point(2, 2);
	Point target(2, 2);
	std::cout << "\nFinding all occurrences of " << target << " in vector:\n";
	auto it = pVec.begin();
	while (it != pVec.end()) {
		it = std::find(it, pVec.end(), target);
		if (it != pVec.end()) {
			std::cout << "Found at position: " << std::distance(pVec.begin(), it) << std::endl;
			++it;
		}
	}
	NN


		//С помощью алгоритма sort() отсортируйте любую последовательность элементов Point. 
		////По умолчанию алгоритм сортирует последовательность по возрастанию.
		//Что должно быть определено в классе Point?
		// Замечание: обобщенный алгоритм sort не работает со списком, так как
		//это было бы не эффективно => для списка сортировка реализована методом класса!!!

	std::sort(pVec.begin(), pVec.end());
	std::list<Point> pList;
	std::copy(pVec.begin(), pVec.end(), std::back_inserter(pList));
	pr(pVec, "Sorted vector of Points");
	pList.sort();
	pr(pList, "Sorted list of Points");


		//Создайте глобальную функцию вида: bool Pred1_1(const Point& ), которая будет вызываться
		//алгоритмом find_if(), передавая в качестве параметра очередной элемент последовательности.
		//С помощью алгоритма find_if() найдите в любой последовательности элементов Point
		//итератор на элемент Point, удовлетворяющий условию: координаты x и y лежат в промежутке
		//[-n, +m].
	pVec[0] = Point(11, 22);
	auto it2 = std::find_if(pVec.begin(), pVec.end(), Pred1_1);
	if (it2 != pVec.end())
		std::cout << "Found Point satisfying Pred1_1: " << *it2 << std::endl;
	else
		std::cout << "No Point found satisfying Pred1_1.\n";
	NN


		//С помощью алгоритма sort() отсортируйте любую последовательность элементов Rect,
		//располагая прямоугольники по удалению центра от начала координат.
	
	std::vector<Rect> rectVec = { Rect(2,2,3,3),  Rect(1,1,2,2), Rect(0,0,1,1), Rect(-1,-1,0,0), Rect(19, 20, -21, 23)};
	rectVec.push_back(Rect(0, 0, 0, 0));
	rectVec[3] = Rect(0.1, 0.2, 0.3, 0.4);
	pr(rectVec, "Vector of Rect before sorting");
	std::sort(rectVec.begin(), rectVec.end(), CompareRectCenters());
	pr(rectVec, "Vector of Rect after sorting by center distance from origin");
	NN


	{//transform
		//Напишите функцию, которая с помощью алгоритма transform переводит 
		//содержимое объекта string в нижний регистр.
		//Подсказка: класс string - это "почти" контейнер, поэтому для него
		// определены методы begin() и end()

		std::string helloWorld = "HELLo, WOrlD!";
		std::string helloworld = " ";
		lowerCase(helloWorld, helloworld);
		std::cout << "Original string: " << helloWorld << std::endl;
		std::cout << "Lowercase string: " << helloworld << std::endl;
		NN

		//Заполните list объектами string. С помощью алгоритма transform сформируте
		//значения "пустого" set, конвертируя строки в нижний регистр

		std::list<std::string> strList = { "One", "Two", "Three", "FOUR", "Five" };
		pr(strList, "List of strings");
		std::set<std::string> strSet;
		std::transform(strList.begin(), strList.end(),
			std::inserter(strSet, strSet.begin()),
			[](const std::string& s) {
				std::string lower;
				lowerCase(s, lower);
				return lower;
			});
		pr(strSet, "Set of lowercase strings");

		//stop
	}
	{// map

		//Сформируйте любым способом вектор с элементами типа string.
		//Создайте (и распечатайте для проверки) map<string, int>, который будет
		//содержать упорядоченные по алфавиту строки и
		//количество повторений каждой строки в векторе
		
		std::vector<std::string> words = { "apple", "banana", "Apple", "orange", "banana", "apple", "kiwi", "Orange" };
		std::map<std::string, int> wordCount;
		for (const auto& word : words) {
			++wordCount[word];
		}
		pr(wordCount, "Map of word counts");
	}

	return 0;
}