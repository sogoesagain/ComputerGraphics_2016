﻿#include <gl/glut.h>
#include <math.h>

// 점을 나타내는 구조체 Point, GLint형의 x와 y를 멤버변수로 갖는다.
struct Point {
	GLint x;
	GLint y;
};

// 픽셀을 칠한다.
void drawPixel(Point p) {
	glBegin(GL_QUADS); {
		glVertex3f(p.x - 0.5, p.y + 0.5, 0);
		glVertex3f(p.x + 0.5, p.y + 0.5, 0);
		glVertex3f(p.x + 0.5, p.y - 0.5, 0);
		glVertex3f(p.x - 0.5, p.y - 0.5, 0);
	} glEnd();
}

// 브래스넘 알고리즘을 이용하여 선분을 그린다.
void drawLine(Point p1, Point p2) {
	Point pixel;				// 색을 칠하게 될 픽셀의 좌표값
	int dx = abs(p1.x - p2.x);	// 선분 전체의 x축 변화량
	int dy = abs(p1.y - p2.y);	// 선분 전체의 y축 변화량

	// 기울기가 1보다 작거나 -1보다 클때, (dx, dy가 절댓값이므로)
	// 즉 선분의 각도가 45도 이하, -45도 이상일 경우
	if (dx >= dy) {
		// p(1)의 값, 초기 결정변수의 값이다.
		// 브래스넘 알고리즘은 P(i+1)과 p(i)의 점화식으로 이루어져있다.
		int p = 2 * dy - dx;
		int const1 = 2 * dy;		// P(i)가 음수일때 P에 더해 P(i+1)을 구한다.
		int const2 = 2 * (dy - dx);	// P(i)가 0또는 양수일때 P에 더해 P(i+1)을 구한다.

		// 시작점의 좌표가 끝점보다 왼쪽에 있으면,
		// 현재 시작점을 최초 그려지는 점으로 설정한다.
		if (p1.x < p2.x) {
			pixel.x = p1.x;
			pixel.y = p1.y;
		}
		else {	// 시작점의 좌표가 끝점보다 오른쪽에 있으면 서로 순서를 바꿔
				// 끝점을 최초 그려지는 점으로 설정하고, 시작점 좌표를 끝점으로 설정한다.
			pixel.x = p2.x;
			pixel.y = p2.y;
			p2.x = p1.x;
			p2.y = p1.y;
		}

		// 시작 픽셀을 칠한다.
		drawPixel(pixel);

		// 현재 색칠한 픽셀의 좌표의 x값이 끝점의 x값보다 작으면 다음 칠할 픽셀을 결정한다.
		while (pixel.x < p2.x) {
			// x값을 하나 늘려 어디를 칠할 것인지 결정한다.
			pixel.x = pixel.x + 1;

			// p가 0보다 작다는 것은 동쪽 픽셀이 선분과 더 가깝다는 것이다.
			// p에 const1을 더하고 그려질 픽셀의 y좌표는 이전과 같으므로 그대로 둔다.
			if (p < 0) {
				p += const1;
			}

			// p값이 0보다 크다는 것은 북동쪽 픽셀이 선분과 더 가깝다는 것이다.
			// 일단 p에 const2를 더한다.
			// 그런데 현재 선분의 기울기가 1과 0사이에 있을 경우와
			// -1과 0 사이에 있을 경우, 총 2가지 경우가 있다.
			// 기울기가 1과 0사이에 있을 경우는 y값을 더해야 하며,
			// 기울기가 -1과 0 사이에 있을 경우는 y값을 빼야한다.
			// 즉, 그려져야할 픽셀 y의 절대값이 1 늘어나야 한다.
			else {
				// 현재 선분이 왼쪽에서 오른쪽으로 진행하고 있는데,
				// 현재 그리는 점보다 끝점이 위에 있으면 기울기가 1과 0사이에 있는 것이다.
				// 현재 그리는 점보다 끝점이 아래에 있으면 기울기가 -1과 0사이에 있는 것이다.
				if (pixel.y < p2.y) {
					pixel.y++;
					p += const2;
				}
				else {
					pixel.y--;;
					p += const2;
				}
			}

			// 좌표가 계산 되었으므로 픽셀을 칠한다.
			drawPixel(pixel);

		}
	}

	// 선분의 기울기가 1보다 크거나, -1보다 작을때, (dx, dy가 절댓값이므로)
	// 즉 선분의 각도가 45도 이상, -45도 이하일 경우
	// 브레스넘 알고리즘은 원래 선분의 기울기가 0도 이상, 45도 이하일 경우에 적합한 알고리즘이다.
	// 지금의 경우는 그 와 반대기 때문에 먼저 선분을 y=x에 대해 대칭해서 그린다고 생각하면 된다.
	// 즉 일반적인 경우(기울기가 0과 45사이)에는 x값을 증가시키면서 픽셀을 그렸다면
	// 지금의 경우는 y값을 증가시키면서 픽셀을 그린다.
	else {
		int p = 2 * dx - dy;
		int const1 = 2 * dx;
		int const2 = 2 * (dx - dy);

		// 시작점의 y좌표가 끝점의 y좌표보다 작다면
		// 즉, 시작점이 아래에 있다면 이대로 시작한다.
		if (p1.y < p2.y) {
			pixel.x = p1.x;
			pixel.y = p1.y;
		}
		// 시작점보다 끝점이 아래에 있다면 두 점을 서로 바꿔 시작한다.
		else {
			pixel.x = p2.x;
			pixel.y = p2.y;
			p2.x = p1.x;
			p2.y = p1.y;
		}

		// 시작 픽셀을 칠한다.
		drawPixel(pixel);

		// 끝점의 y좌표를 칠할때 까지 y값을 하나씩 늘리면서 픽셀을 칠해나간다.
		while (pixel.y < p2.y) {
			pixel.y = pixel.y + 1;

			// p가 0보다 작다면 선분이 왼쪽픽셀과 더 가까우므로
			// 이전에 그린 x값을 그대로 취한다.
			if (p < 0) {
				p += const1;
			}

			// 선분이 오른쪽 픽셀과 더 가깝다면 x값을 증가시켜야 한다.
			// 단, 기울기가 1보다 큰 경우 x값을 그대로 증가시키면 되지만,
			// 기울기가 -1보다 작은 경우는 왼쪽으로 가야하므로 x값을 감소시킨다.
			// 즉, 그려져야할 픽셀 x의 절댓값이 1늘어나야 한다.
			else {
				if (pixel.x > p2.x) {
					pixel.x--;
					p += const2;
				}
				else {
					pixel.x++;
					p += const2;
				}
			}

			// 좌표가 계산되었으므로 픽셀을 칠한다.
			drawPixel(pixel);

		}
	}
}