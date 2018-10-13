/*************************Hello World Program*********************/

import io globally
from stdlib

proc([][]uchar args) -> int main {
	print("Hello, world.\n");
	return 0;
}

/*****************************************************************/

import pointer as p
from stdlib

// Some functions and procedures

proc([](T as Ord) a) -> void quicksort {			// T is a class that implements the Ord interface
	if (a.length < 2) {
		return;
	}
	unsigned end = a.length - 1;
	unsigned j = 0;
	T pivot = a[a.length - 1];
	for (unsigned i = 0; i < end; i++) {
		if(a[i] < pivot) {
			p.swap(a + i, a + j);
			j++;
		}
	}
	p.swap(a + j, &pivot);
	quicksort( CAST( [j]T, a ) );
	quicksort( CAST( [end-j]T, a + j + 1 ) );
}


proc(int a, int b) -> int add {
	return a + b;
}


proc([][]uchar args) -> int main {
	proc(int, int) -> int x;		// Procedure pointer
	x = add;
	x = proc(int a, int b) -> int {		// Anonymous procedure
			return a + b;
		}
	x = (\a, b -> a + b);			// Anonymous function
									// Procedure pointers and function pointers are inter-castable
}


// Function composition looks ugly, but it can be done
func(func(int)->int f, func(int)->int g) -> (func(int x)->int) compose = f(g(x));


// Procedure composition looks very similar to function composition
proc(proc(int)->int f, proc(int)->int g) -> (proc(int x)->int) compose {
	return f(g(x));
}


func(int a, int b) -> int addFunction = a + b;

func(bool a) -> int pointlessFunction = if(a) {7}
										else {15};

proc([][]uchar args) -> int main {
	int x = stoi(args[0]);
}

interface A {
	proc(impl) -> int x;				// impl is the class implementing the interface
	proc(T as A) -> impl y;				// T can be any class which has implemented the interface, but doesn't need to work for all implementing classes
	proc(A, impl&) -> void z;			// A needs to work for any class which implements the interface
	proc(T as A thing) -> int yx {
		return x(y(thing));
	}
}

class B {
	int data;
	int moreData;
}

// B implements A
B : A {
	proc(B thing) -> int x {
		return thing.data;
	}
	proc(B thing) -> B y {
		thing.data++;
		thing.moreData += 17;
		return thing;
	}
	proc(T as A param1, B &param2) -> void z {
		param2.data = x(param1);
		return;
	}
}

class C {
	int data;
}

// C implements A
C : A {
	proc(C thing) -> int x {
		return thing.data;
	}
	proc(B thing) -> C a {
		C ret;
		ret.data = thing.data;
		return ret;
	}
	proc(T as A thing1, C thing2) -> void b{
		thing2.data = x(thing1);
		return;
	}
}

proc() -> void forEachDemo {

	[10] int arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	// For each loop by index
	for (var i ind arr) {		//	for (unsigned i = 0; i < 10; i++) {
		print(arr[i]);			//		print(arr[i]);
	}							//	}

	// For each loop by value
	for (var v val arr) {		//	for (unsigned i = 0; i < 10; i++) {
								//		int v = arr[i];
		print(v);				//		print(v)
	}							//	}

	// For each loop by reference
	for (var r ref arr) {		// 	for (int& r = arr; r < arr + 10; r++) {
		print(*r);				//		print(*r);
	}							//	}
}


// I have no idea what I'm doing here
// Until I remember what its purpose is, I'm just not going to use this piece of syntax
func(a,,,b)
