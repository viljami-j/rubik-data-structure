#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <chrono>

class Rubik {

	/*
	* Assumptions:
	     _
	   _|_|_ _        U
      |_|_|_|_| =   L F RB
	    |_|           D

		Left  = Orange
		Front = Green
		Right = Red
		Back  = Blue
		Up    = White
		Down  = Yellow
	*/


public:
	enum RotAxis { // Rotation Axis
		Undefined,
		Xpos,
		Xneg,
		Ypos,
		Yneg,
	};

	enum Face {
		L, // Left
		F, // Front
		R, // Right


		U, // Up
		B, // Back
		D  // Down
	};

	struct RotOp { // Rotation Operation
		RotAxis rot_axis = RotAxis::Undefined;
		int line = -1; // between 1-3

		RotOp(RotAxis axis, int line) : rot_axis(axis), line(line) {}

	public:
		void Apply() {
			if (rot_axis == RotAxis::Undefined) {
				std::cerr << "RotOp: rot_axis must be defined! returning..";
				return;
			}
			
			if (line > 3 && line < 1) {
				std::cerr << "RotOp: line must be between 1-3! returning..";
				return;
			}

			bool is_x_rot = false;
			bool is_y_rot = false;

			// Apply rots
			switch (rot_axis)
			{
			case Rubik::Undefined:
				// This should never happen
				break;
			case Rubik::Xpos:
				is_x_rot = true;
				if (line == 1) positive_rotate(x1);
				if (line == 2) positive_rotate(x2);
				if (line == 3) positive_rotate(x3);
				break;
			case Rubik::Xneg:
				is_x_rot = true;
				if (line == 1) negative_rotate(x1);
				if (line == 2) negative_rotate(x2);
				if (line == 3) negative_rotate(x3);
				break;
			case Rubik::Ypos:
				is_y_rot = true;
				if (line == 1) positive_rotate(y1);
				if (line == 2) positive_rotate(y2);
				if (line == 3) positive_rotate(y3);
				break;
			case Rubik::Yneg:
				is_y_rot = true;
				if (line == 1) negative_rotate(y1);
				if (line == 2) negative_rotate(y2);
				if (line == 3) negative_rotate(y3);
				break;
			default:
				break;
			}

			// If we did an X operation, rewrite Y perspective to make Front 'F' match in both xA & yA perspectives/interpretations.
			// If we did an Y operation, vice versa.
			if (is_x_rot) {
				y1[3] = x1[3];
				y2[3] = x1[4];
				y3[3] = x1[5];

				y1[4] = x2[3];
				y2[4] = x2[4];
				y3[4] = x2[5];

				y1[5] = x3[3];
				y2[5] = x3[4];
				y3[5] = x3[5];
			}

			if (is_y_rot) {
				x1[3] = y1[3];
				x1[4] = y2[3];
				x1[5] = y3[3];

				x2[3] = y1[4];
				x2[4] = y2[4];
				x2[5] = y3[4];

				x3[3] = y1[5];
				x3[4] = y2[5];
				x3[5] = y3[5];
			}
		}
	};

private:
	static inline std::deque<std::string> x1; // LFRB
	static inline std::deque<std::string> x2; // LFRB
	static inline std::deque<std::string> x3; // LFRB

	static inline std::deque<std::string> y1; // UFD
	static inline std::deque<std::string> y2; // UFD
	static inline std::deque<std::string> y3; // UFD

	std::vector<RotOp> operations;


	static void positive_rotate(std::deque<std::string>& deque) {
		std::string popped = deque.back();   // Positive Rotate (x+: right rotate, y+: down rotate)
		deque.pop_back();
		deque.push_front(popped);

		popped = deque.back();               // Positive Rotate (x+: right rotate, y+: down rotate)
		deque.pop_back();
		deque.push_front(popped);

		popped = deque.back();               // Positive Rotate (x+: right rotate, y+: down rotate)
		deque.pop_back();
		deque.push_front(popped);
	}
	static void negative_rotate(std::deque<std::string>& deque) {
		std::string popped2 = deque.front(); // Negative Rotate (x-: left rotate, y-: up rotate)
		deque.pop_front();
		deque.push_back(popped2);

		popped2 = deque.front();             // Negative Rotate (x-: left rotate, y-: up rotate)
		deque.pop_front();
		deque.push_back(popped2);

		popped2 = deque.front();             // Negative Rotate (x-: left rotate, y-: up rotate)
		deque.pop_front();
		deque.push_back(popped2);
	}

	std::vector<std::string> get_face(Face face) {
		std::vector<std::string> face_construct; // constructed from x1-3, y1-3
		switch (face)
		{
		case Rubik::L:
			face_construct.push_back(x1[0]);
			face_construct.push_back(x1[1]);
			face_construct.push_back(x1[2]);

			face_construct.push_back(x2[0]);
			face_construct.push_back(x2[1]);
			face_construct.push_back(x2[2]);

			face_construct.push_back(x3[0]);
			face_construct.push_back(x3[1]);
			face_construct.push_back(x3[2]);
			return face_construct;
			break;
		case Rubik::F:
			face_construct.push_back(x1[3]);
			face_construct.push_back(x1[4]);
			face_construct.push_back(x1[5]);

			face_construct.push_back(x2[3]);
			face_construct.push_back(x2[4]);
			face_construct.push_back(x2[5]);

			face_construct.push_back(x3[3]);
			face_construct.push_back(x3[4]);
			face_construct.push_back(x3[5]);
			return face_construct;
			break;
		case Rubik::R:
			face_construct.push_back(x1[6]);
			face_construct.push_back(x1[7]);
			face_construct.push_back(x1[8]);

			face_construct.push_back(x2[6]);
			face_construct.push_back(x2[7]);
			face_construct.push_back(x2[8]);

			face_construct.push_back(x3[6]);
			face_construct.push_back(x3[7]);
			face_construct.push_back(x3[8]);
			return face_construct;
			break;
		case Rubik::U:
			face_construct.push_back(y1[0]);
			face_construct.push_back(y2[0]);
			face_construct.push_back(y3[0]);

			face_construct.push_back(y1[1]);
			face_construct.push_back(y2[1]);
			face_construct.push_back(y3[1]);

			face_construct.push_back(y1[2]);
			face_construct.push_back(y2[2]);
			face_construct.push_back(y3[2]);
			return face_construct;
			break;
		case Rubik::B:
			face_construct.push_back(x1[9]);
			face_construct.push_back(x1[10]);
			face_construct.push_back(x1[11]);

			face_construct.push_back(x2[9]);
			face_construct.push_back(x2[10]);
			face_construct.push_back(x2[11]);

			face_construct.push_back(x3[9]);
			face_construct.push_back(x3[10]);
			face_construct.push_back(x3[11]);
			return face_construct;
			break;
		case Rubik::D:
			face_construct.push_back(y1[6]);
			face_construct.push_back(y2[6]);
			face_construct.push_back(y3[6]);

			face_construct.push_back(y1[7]);
			face_construct.push_back(y2[7]);
			face_construct.push_back(y3[7]);

			face_construct.push_back(y1[8]);
			face_construct.push_back(y2[8]);
			face_construct.push_back(y3[8]);
			return face_construct;
			break;
		default:
			break;
		}
	}

public:

	Rubik() {
		// Horizontal Line X1 (verified 100% correct)
		x1.push_back("l0"); x1.push_back("l1"); x1.push_back("l2"); x1.push_back("f0"); x1.push_back("f1"); x1.push_back("f2"); x1.push_back("r0"); x1.push_back("r1"); x1.push_back("r2"); x1.push_back("b0"); x1.push_back("b1"); x1.push_back("b2");

		// Horizontal Line X2 (verified 100% correct)
		x2.push_back("l3"); x2.push_back("l4"); x2.push_back("l5"); x2.push_back("f3"); x2.push_back("f4"); x2.push_back("f5"); x2.push_back("r3"); x2.push_back("r4"); x2.push_back("r5"); x2.push_back("b3"); x2.push_back("b4"); x2.push_back("b5");

		// Horizontal Line X3 (verified 100% correct)
		x3.push_back("l6"); x3.push_back("l7"); x3.push_back("l8"); x3.push_back("f6"); x3.push_back("f7"); x3.push_back("f8"); x3.push_back("r6"); x3.push_back("r7"); x3.push_back("r8"); x3.push_back("b6"); x3.push_back("b7"); x3.push_back("b8");

		// Vertical Line Y1 (verified 100% correct)
		y1.push_back("u0"); 
		y1.push_back("u3"); 
		y1.push_back("u6"); 
		y1.push_back("f0"); 
		y1.push_back("f3"); 
		y1.push_back("f6"); 
		y1.push_back("d0"); 
		y1.push_back("d3"); 
		y1.push_back("d6");

		// Vertical Line Y2 (verified 100% correct)
		y2.push_back("u1"); 
		y2.push_back("u4"); 
		y2.push_back("u7");
		y2.push_back("f1"); 
		y2.push_back("f4"); 
		y2.push_back("f7");
		y2.push_back("d1"); 
		y2.push_back("d4"); 
		y2.push_back("d7");

		// Vertical Line Y3 (verified 100% correct)
		y3.push_back("u2"); 
		y3.push_back("u5"); 
		y3.push_back("u8");
		y3.push_back("f2"); 
		y3.push_back("f5"); 
		y3.push_back("f8");
		y3.push_back("d2"); 
		y3.push_back("d5"); 
		y3.push_back("d8");
	}

	void rotate(RotAxis axis, int line) {
		RotOp(axis, line).Apply();
	}

	void calc_face(Face face) {
		// Make copies of state
		std::deque<std::string> x1 = x1; // LFRB
		std::deque<std::string> x2 = x2; // LFRB
		std::deque<std::string> x3 = x3; // LFRB

		std::deque<std::string> y1 = y1; // UFD
		std::deque<std::string> y2 = y2; // UFD
		std::deque<std::string> y3 = y3; // UFD

		// Undo all operations

	}

	void print_state() {
		auto l = get_face(Face::L);
		auto f = get_face(Face::F);
		auto r = get_face(Face::R);
		auto b = get_face(Face::B);

		auto u = get_face(Face::U);
		auto d = get_face(Face::D);

		std::cout << "           " << u[0] << " " << u[1] << " " << u[2] << std::endl;
		std::cout << "           " << u[3] << " " << u[4] << " " << u[5] << std::endl;
		std::cout << "           " << u[6] << " " << u[7] << " " << u[8] << std::endl;
		std::cout << "           " << "________" << std::endl;
		std::cout << l[0] << " " << l[1] << " " << l[2] << " | ";
		std::cout << f[0] << " " << f[1] << " " << f[2] << " | ";
		std::cout << r[0] << " " << r[1] << " " << r[2] << " | ";
		std::cout << b[0] << " " << b[1] << " " << b[2] << std::endl;
		std::cout << l[3] << " " << l[4] << " " << l[5] << " | ";
		std::cout << f[3] << " " << f[4] << " " << f[5] << " | ";
		std::cout << r[3] << " " << r[4] << " " << r[5] << " | ";
		std::cout << b[3] << " " << b[4] << " " << b[5] << std::endl;
		std::cout << l[6] << " " << l[7] << " " << l[8] << " | ";
		std::cout << f[6] << " " << f[7] << " " << f[8] << " | ";
		std::cout << r[6] << " " << r[7] << " " << r[8] << " | ";
		std::cout << b[6] << " " << b[7] << " " << b[8] << std::endl;
		std::cout << "           " << "--------" << std::endl;
		std::cout << "           " << d[0] << " " << d[1] << " " << d[2] << std::endl;
		std::cout << "           " << d[3] << " " << d[4] << " " << d[5] << std::endl;
		std::cout << "           " << d[6] << " " << d[7] << " " << d[8] << std::endl;
	}
};


int main()
{
	Rubik rubik;
	rubik.print_state();

	rubik.rotate(Rubik::RotAxis::Ypos, 1); // positiivinen -> vastap‰iv‰‰n
	rubik.rotate(Rubik::RotAxis::Ypos, 2);
	rubik.rotate(Rubik::RotAxis::Ypos, 3);

	std::cout << std::endl;
	rubik.print_state();
}