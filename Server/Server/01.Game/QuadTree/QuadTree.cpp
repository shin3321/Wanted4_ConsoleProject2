#include "pch.h"
#include "QuadTree.h"

void QuadTree::queryCircle(int cx, int cy, int radius, std::vector<Unit*>& found) const
{
    Rect range(cx, cy, radius, radius);

    //range가 boundary안에 있는지 확인
    if (!_boundary.intersects(range)) return;
    for (const auto& p : _points) 
    {
        double dx = p.x - cx, dy = p.y - cy;
        // 실제 원형 범위 체크
        if (dx * dx + dy * dy <= radius * radius && p.unit->_isAlive)
            found.push_back(p.unit);
    }
    if (_isDivided)
    {
        nw->queryCircle(cx, cy, radius, found);
        ne->queryCircle(cx, cy, radius, found);
        sw->queryCircle(cx, cy, radius, found);
        se->queryCircle(cx, cy, radius, found);
    }
}

void QuadTree::subdivide()
{
    double hw = _boundary._w / 2, hh = _boundary._h / 2;
    double x = _boundary._x, y = _boundary._y;

    //중심 기분으로 4방향에 자식 노드 생성
    nw = std::make_unique<QuadTree>(Rect(x - hw, y - hh, hw, hh), _capacity);
    ne = std::make_unique<QuadTree>(Rect(x + hw, y - hh, hw, hh), _capacity);
    sw = std::make_unique<QuadTree>(Rect(x - hw, y + hh, hw, hh), _capacity);
    se = std::make_unique<QuadTree>(Rect(x + hw, y + hh, hw, hh), _capacity);
    _isDivided = true;
    for (auto& p : _points)   
        nw->insert(p) || ne->insert(p) || 
        sw->insert(p) || se->insert(p);
    _points.clear();
}

bool QuadTree::insert(const Point& p)
{
    //이미 있으면 추가 안 함
    if (!_boundary.contains(p)) return false;
    if ((int)_points.size() < _capacity && !_isDivided)
    {
        _points.push_back(p);
        return true;
    }
    if (!_isDivided) subdivide();
    return nw->insert(p) || ne->insert(p) ||
        sw->insert(p) || se->insert(p);
}