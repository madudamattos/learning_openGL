// #include <iostream>
// #include "alglib.h"

// using namespace Alglib;
// using std::cout;
// using std::endl;

// /*
// comando para rodar o código: 
// g++ -std=c++17 utils/test_transforms.cpp utils/alglib.cpp -o test_transforms
// */


// void section(const char* title) {
//     cout << "\n===== " << title << " =====\n";
// }

// int main() {
//     // 2D tests
//     section("2D: basic tuple + scale");
//     Tuple2 p(1.0f, 2.0f);
//     cout << "initial Tuple2: "; p.Print();

//     Mat2 m2 = Mat2::Identity();
//     cout << "Identity Mat2:\n"; m2.Print();

//     m2.Scale(2.0f, 3.0f);
//     cout << "After Scale(2,3):\n"; m2.Print();

//     Tuple2 p2 = p; // copy
//     p2.Transform(m2);
//     cout << "Transformed Tuple2 (scale): "; p2.Print();

//     section("2D: rotate + translate");
//     Mat2 r2 = Mat2::Identity();
//     r2.Rotate(90.0f); // degrees
//     cout << "Rotate 90 Mat2:\n"; r2.Print();

//     Mat2 t2 = Mat2::Identity();
//     t2.Translate(5.0f, -1.0f);
//     cout << "Translate(5,-1) Mat2:\n"; t2.Print();

//     Mat2 composed2 = r2.MatrixMultiply(t2);
//     cout << "Composed Mat2 (Rotate * Translate):\n"; composed2.Print();

//     Tuple2 p3(1.0f, 0.0f);
//     cout << "Before composed transform: "; p3.Print();
//     p3.Transform(composed2);
//     cout << "After composed transform:  "; p3.Print();

//     section("2D: add/subtract");
//     Tuple2 a2(3.0f, 4.0f);
//     Tuple2 b2(1.0f, 1.5f);
//     cout << "a2: "; a2.Print();
//     cout << "b2: "; b2.Print();
//     Tuple2 s2 = Add(a2, b2);
//     Tuple2 d2 = Subtract(a2, b2);
//     cout << "Add(a2,b2): "; s2.Print();
//     cout << "Subtract(a2,b2): "; d2.Print();

//     // 3D tests
//     section("3D: basic tuple + scale");
//     Tuple3 v(1.0f, 2.0f, 3.0f);
//     cout << "initial Tuple3: "; v.Print();

//     Mat3 m3 = Mat3::Identity();
//     cout << "Identity Mat3:\n"; m3.Print();

//     m3.Scale(2.0f, 0.5f, 1.5f);
//     cout << "After Scale(2,0.5,1.5):\n"; m3.Print();

//     Tuple3 v2 = v;
//     v2.Transform(m3);
//     cout << "Transformed Tuple3 (scale): "; v2.Print();

//     section("3D: rotateX, rotateY, rotateZ (applied in sequence)");
//     Mat3 rx = Mat3::Identity(); rx.RotateX(30.0f);
//     Mat3 ry = Mat3::Identity(); ry.RotateY(45.0f);
//     Mat3 rz = Mat3::Identity(); rz.RotateZ(60.0f);

//     cout << "RotateX(30):\n"; rx.Print();
//     cout << "RotateY(45):\n"; ry.Print();
//     cout << "RotateZ(60):\n"; rz.Print();

//     Mat3 combo = Mat3::Identity();
//     combo.Rotate(30.0f, 45.0f, 60.0f); // convenience rotate (X then Y then Z)
//     cout << "Combo Rotate (30,45,60) via Rotate():\n"; combo.Print();

//     Tuple3 v3(1.0f, 0.0f, 0.0f);
//     cout << "Before combo rotate: "; v3.Print();
//     v3.Transform(combo);
//     cout << "After combo rotate:  "; v3.Print();

//     section("3D: translate + compose with multiply");
//     Mat3 transl = Mat3::Identity();
//     transl.Translate(5.0f, -2.0f, 3.5f);
//     cout << "Translate Mat3:\n"; transl.Print();

//     Mat3 composed3 = combo.MatrixMultiply(transl);
//     cout << "Composed Mat3 (Rotate * Translate):\n"; composed3.Print();

//     Tuple3 v4(1.0f, 1.0f, 1.0f);
//     cout << "Before composed3: "; v4.Print();
//     v4.Transform(composed3);
//     cout << "After composed3:  "; v4.Print();

//     section("3D: add/subtract");
//     Tuple3 a3(3.0f, 4.0f, 5.0f);
//     Tuple3 b3(1.0f, 1.5f, -2.0f);
//     cout << "a3: "; a3.Print();
//     cout << "b3: "; b3.Print();
//     Tuple3 s3 = Add(a3, b3);
//     Tuple3 d3 = Subtract(a3, b3);
//     cout << "Add(a3,b3): "; s3.Print();
//     cout << "Subtract(a3,b3): "; d3.Print();

//     section("Done: All tests executed");
//     return 0;
// }
