#include "main.h"

#define EPSILON 0.0001

//원점에서 벡터 b가 벡터 a의 반시계 방향이면 양수, 시계방향이면 음수, 평행이면 0을 반환 한다.
double ccw(vector2 a, vector2 b) {
	return a.cross(b);
}
//점 p를 기준으로 벡터 b가 벡터 a의 반시계 방향이면 양수, 시계방향이면 음수, 평행이면 0을 반환 한다.
double ccw(vector2 p, vector2 a, vector2 b) {
	return ccw(a - p, b - p);
}

void swap(vector2 &a, vector2 &b)
{
	vector2 temp = b;
	b = a;
	a = temp;
}

//점 a, b와 점 c, d가 평행한 두 선분 일 때 이들이 한 점에서 겹치는지 확인한다.
bool paralleSegments(vector2 a, vector2 b, vector2 c, vector2 d, vector2& p) {
	if (b < a) swap(a, b);
	if (d < c) swap(c, d);

	//한 직선위에 없거나 두 선분이 겹치지 않는 경우를 우선 걸러낸다. 본문의 1번 관계인 경우이다.
	if (ccw(a, b, c) != 0 || b < c || d < a) return false;

	//두 선분이 확실히 겹친다면 교차점 하나를 찾는다.
	if (a<c) p = c;
	else p = a;
	return true;
}


// - p가 두 점 a, b를 감싸면서 각 변이 x, y축에 평행한 최소사각형 내부에 있는지 확인한다.
// a, b, p는 일직선 상에 있다고 가정한다.
bool inBoundingRectangle(vector2 p, vector2 a, vector2 b) {
	if (b < a) swap(a, b);
	return p == a || p == b || (a <p && p < b);
}

// - 점 a, b를 지나는 직선과 점 c, d를 지나는 직선의 교점을 x에 반환한다.
// - 두 직선이 평행이면(겹치는 경우 포함) 거짓을, 아니면 참을 반환한다.
bool lineIntersection(vector2 a, vector2 b, vector2 c, vector2 d, vector2& x) {
	double det = (b - a).cross(d - c);
	//두선이 평행인 경우
	if (fabs(det) < EPSILON) return false;
	x = a + (b - a)*((c - a).cross(d - c) / det);
	return true;
}

// - 두 점 a, b를 지나는 선분과 두 점 c, b를 지나는 선분을 p에 반환한다.
// - 교짐이 여러개일 경우 아무점이나 반환한다.
bool segmentIntersection(vector2 a, vector2 b, vector2 c, vector2 d, vector2& p) {
	//printf("segmentIntersection\n");
	//두 직선이 평행인 경우를 우선 예외로 처리한다.
	if (!lineIntersection(a, b, c, d, p))
	{
		//printf("lineIntersection : false\n");
		return paralleSegments(a, b, c, d, p); 
	}
	//p가 두 선분에 포함되어 있는 경우에만 참을 반환한다.
	return inBoundingRectangle(p, a, b) && inBoundingRectangle(p, c, d);
}


//두 선분이 서로 접촉하는지 여부를 반환한다.
bool sementIntersects(vector2 a, vector2 b, vector2 c, vector2 d) {
	//printf("sementIntersects\n");
	//printf("받은값 a:(%lf,%lf) b:(%lf,%lf) \nc:(%lf,%lf) d:(%lf,%lf)\n",a.x,a.y, b.x, b.y, c.x, c.y, d.x, d.y);
	double ab = ccw(a, b, c)*ccw(a, b, d);
	double cd = ccw(c, d, a)*ccw(c, d, b);
	//printf("%lf, %lf\n",ab,cd);
	//두 선분이 한 직선에 위에 있거나 끝점이 겹치는 경우
	if (ab == 0 && cd == 0) {
		if (b < a) swap(a, b);
		if (d < c) swap(c, d);
		return !(b< c || d <a);
	}
	return ab <= 0 && cd <= 0;
}

double PlaneCollision(Box *box, const vector3 *Pos, const vector3 *Dir)
{
	bool XY[4] = { false,false,false,false };
	bool YZ[4] = { false,false,false,false };
	bool ZX[4] = { false,false,false,false };
	bool XYcheck = false, YZcheck = false, ZXcheck = false;
	double Dist = 10000;
	vector2 BoxA, BoxB, Cross2D, RayA, RayB;
	//vector3 Cross3D;

	RayA.x = Pos->x;
	RayA.y = Pos->y;
	RayB.x = Dir->x;
	RayB.y = Dir->y;

	BoxA.x = (double)box->x - (double)(box->size / 2);
	BoxA.y = (double)box->y + (double)box->size;
	BoxB.x = (double)box->x + (double)(box->size / 2);
	BoxB.y = (double)box->y + (double)box->size;
	XY[0] = sementIntersects(RayA, RayB, BoxA, BoxB);
	//segmentIntersection(RayA, RayB, BoxA, BoxB, Cross2D);
	//if (XY[0])
	//{
	//	Cross3D.x = Cross2D.y;
	//	Cross3D.y = Cross2D.x;
	//}

	BoxA.x = (double)box->x + (double)(box->size / 2);
	BoxA.y = (double)box->y + (double)box->size;
	BoxB.x = (double)box->x + (double)(box->size / 2);
	BoxB.y = (double)box->y;
	XY[1] = sementIntersects(RayA, RayB, BoxA, BoxB);
	//segmentIntersection(RayA, RayB, BoxA, BoxB, Cross2D);
	//if (XY[1])
	//{
	//	Cross3D.x = Cross2D.y;
	//	Cross3D.y = Cross2D.x;
	//}

	BoxA.x = (double)box->x + (double)(box->size / 2);
	BoxA.y = (double)box->y;
	BoxB.x = (double)box->x - (double)(box->size / 2);
	BoxB.y = (double)box->y;
	XY[2] = sementIntersects(RayA, RayB, BoxA, BoxB);
	//segmentIntersection(RayA, RayB, BoxA, BoxB, Cross2D);
	//if (XY[2])
	//{
	//	Cross3D.x = Cross2D.y;
	//	Cross3D.y = Cross2D.x;
	//}

	BoxA.x = (double)box->x - (double)(box->size / 2);
	BoxA.y = (double)box->y;
	BoxB.x = (double)box->x - (double)(box->size / 2);
	BoxB.y = (double)box->y + (double)box->size;
	XY[3] = sementIntersects(RayA, RayB, BoxA, BoxB);
	//segmentIntersection(RayA, RayB, BoxA, BoxB, Cross2D);
	//if (XY[3])
	//{
	//	Cross3D.x = Cross2D.y;
	//	Cross3D.y = Cross2D.x;
	//}

	if (XY[0] || XY[1] || XY[2] || XY[3])
	{
		XYcheck = true;
		//printf("XYCheck\n");
	}


	RayA.x = Pos->y;
	RayA.y = Pos->z;
	RayB.x = Dir->y;
	RayB.y = Dir->z;

	BoxA.x = (double)box->y + (double)box->size;
	BoxA.y = (double)box->z + (double)(box->size / 2);
	BoxB.x = (double)box->y + (double)box->size;
	BoxB.y = (double)box->z - (double)(box->size / 2);
	YZ[0] = sementIntersects(RayA, RayB, BoxA, BoxB);
	//segmentIntersection(RayA, RayB, BoxA, BoxB, Cross2D);
	//if (YZ[0])
	//{
	//	Cross3D.y = Cross2D.x;
	//	Cross3D.z = Cross2D.y;
	//}

	BoxA.x = (double)box->y + (double)box->size;
	BoxA.y = (double)box->z - (double)(box->size / 2);
	BoxB.x = (double)box->y;
	BoxB.y = (double)box->z - (double)(box->size / 2);
	YZ[1] = sementIntersects(RayA, RayB, BoxA, BoxB);
	//segmentIntersection(RayA, RayB, BoxA, BoxB, Cross2D);
	//if (YZ[1])
	//{
	//	Cross3D.y = Cross2D.x;
	//	Cross3D.z = Cross2D.y;
	//}

	BoxA.x = (double)box->y;
	BoxA.y = (double)box->z - (double)(box->size / 2);
	BoxB.x = (double)box->y;
	BoxB.y = (double)box->z + (double)(box->size / 2);
	YZ[2] = sementIntersects(RayA, RayB, BoxA, BoxB);
	//segmentIntersection(RayA, RayB, BoxA, BoxB, Cross2D);
	//if (YZ[2])
	//{
	//	Cross3D.y = Cross2D.x;
	//	Cross3D.z = Cross2D.y;
	//}

	BoxB.x = (double)box->y;
	BoxB.y = (double)box->z + (double)(box->size / 2);
	BoxB.x = (double)box->y + (double)box->size;
	BoxB.y = (double)box->z + (double)(box->size / 2);
	YZ[3] = sementIntersects(RayA, RayB, BoxA, BoxB);
	//segmentIntersection(RayA, RayB, BoxA, BoxB, Cross2D);
	//if (YZ[3])
	//{
	//	Cross3D.y = Cross2D.x;
	//	Cross3D.z = Cross2D.y;
	//}
	if (YZ[0] || YZ[1] || YZ[2] || YZ[3])
	{
		YZcheck = true;
		//printf("YZCheck\n");
	}


	RayA.x = Pos->z;
	RayA.y = Pos->x;
	RayB.x = Dir->z;
	RayB.y = Dir->x;

	BoxA.x = (double)box->z - (double)(box->size / 2);
	BoxA.y = (double)box->x - (double)(box->size / 2);
	BoxB.x = (double)box->z - (double)(box->size / 2);
	BoxB.y = (double)box->x + (double)(box->size / 2);
	ZX[0] = sementIntersects(RayA, RayB, BoxA, BoxB);
	//segmentIntersection(RayA, RayB, BoxA, BoxB, Cross2D);
	//if (ZX[0])
	//{
	//	Cross3D.z = Cross2D.x;
	//	Cross3D.x = Cross2D.y;
	//}

	BoxA.x = (double)box->z - (double)(box->size / 2);
	BoxA.y = (double)box->x + (double)(box->size / 2);
	BoxB.x = (double)box->z + (double)(box->size / 2);
	BoxB.y = (double)box->x + (double)(box->size / 2);
	ZX[1] = sementIntersects(RayA, RayB, BoxA, BoxB);
	//segmentIntersection(RayA, RayB, BoxA, BoxB, Cross2D);
	//if (ZX[1])
	//{
	//	Cross3D.z = Cross2D.x;
	//	Cross3D.x = Cross2D.y;
	//}

	BoxA.x = (double)box->z + (double)(box->size / 2);
	BoxA.y = (double)box->x + (double)(box->size / 2);
	BoxB.x = (double)box->z + (double)(box->size / 2);
	BoxB.y = (double)box->x - (double)(box->size / 2);
	ZX[2] = sementIntersects(RayA, RayB, BoxA, BoxB);
	//segmentIntersection(RayA, RayB, BoxA, BoxB, Cross2D);
	//if (ZX[2])
	//{
	//	Cross3D.z = Cross2D.x;
	//	Cross3D.x = Cross2D.y;
	//}
	BoxA.x = (double)box->z + (double)(box->size / 2);
	BoxA.y = (double)box->x - (double)(box->size / 2);
	BoxB.x = (double)box->z - (double)(box->size / 2);
	BoxB.y = (double)box->x - (double)(box->size / 2);
	ZX[3] = sementIntersects(RayA, RayB, BoxA, BoxB);
	/*segmentIntersection(RayA, RayB, BoxA, BoxB, Cross2D);
	if (ZX[3])
	{
	Cross3D.z = Cross2D.x;
	Cross3D.x = Cross2D.y;
	}*/
	if (ZX[0] || ZX[1] || ZX[2] || ZX[3])
	{
		ZXcheck = true;
		//printf("ZXCheck\n");
	}

	if (XYcheck && YZcheck && ZXcheck)
	{
		//printf("충돌!\n");
		return 1.0;
		//Dist = sqrt(pow(player_client.Camx - Cross3D.x, 2.0) + pow(player_client.Camy - Cross3D.y, 2.0) + pow(player_client.Camz - Cross3D.z, 2.0));
		//printf("x : %lf\ny : %lf\nz :%lf\n", Cross3D.x, Cross3D.y, Cross3D.z);
		//return Dist;
		
	}
	else
	{
		return 0.0;
	}
}